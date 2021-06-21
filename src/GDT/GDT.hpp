#pragma once

#ifndef GDT_HPP
#define GDT_HPP

#include <cstddef>
#include <cstdint>

/**
 * @brief The Global Descriptor Table
 *
 * Defines the layout, type, and access privileges of the various segments present in memory.
 *
 * @tparam N The number of descriptors to store in the GDT
 */
template <std::size_t N>
class GlobalDescriptorTable {
    /**
     * @brief A GDT entry
     *
     * Packed attribute to prevent padding, so that the CPU does not read garbage data
     */
    struct GDTEntry {
        std::uint16_t limit_low{};
        std::uint16_t base_low{};
        std::uint8_t base_middle{};
        std::uint8_t access{};
        std::uint8_t granularity{};
        std::uint8_t base_high{};
    } __attribute__((packed));

    static_assert(offsetof(GDTEntry, limit_low) == 0);
    static_assert(offsetof(GDTEntry, base_low) == 2);
    static_assert(offsetof(GDTEntry, base_middle) == 4);
    static_assert(offsetof(GDTEntry, access) == 5);
    static_assert(offsetof(GDTEntry, granularity) == 6);
    static_assert(offsetof(GDTEntry, base_high) == 7);

    /**
     * @brief A GDT selector
     *
     * The first two bytes are the size of the GDT, the next four bytes are the base address
     */
    struct GDTPointer {
        std::uint16_t limit{};
        std::uint64_t base{};
    } __attribute__((packed));

    static_assert(offsetof(GDTPointer, limit) == 0);
    static_assert(offsetof(GDTPointer, base) == 2);

    GDTEntry table[N]{};
    GDTPointer pointer{};

  public:
    /**
     * @brief Sets a descriptor in the GDT
     *
     * @tparam num The index at which to add the descriptor
     * @param base The base address of the segment
     * @param limit The size of the segment
     * @param access The access flags
     * @param granularity The four upper limit bits and the four flag bits
     */
    template <std::size_t num>
    constexpr void set_descriptor(
        std::uint64_t base, std::uint64_t limit, std::uint8_t access, std::uint8_t granularity);

    /**
     * @brief Sets up the GDT
     *
     * Sets up the GDT as per the stivale2 spec
     */
    void setup_stivale2();
};

/**
 * @brief Sets the GDTR to point to the given GDTR
 *
 * Modifies the segment registers accordingly
 *
 * @param GDTR The GDT pointer used to set the GDTR
 */
extern "C" void set_GDTR(void *GDTR);

template <std::size_t N>
template <std::size_t num>
constexpr void GlobalDescriptorTable<N>::set_descriptor(
    std::uint64_t base, std::uint64_t limit, std::uint8_t access, std::uint8_t granularity) {
    static_assert(num < N, "Cannot access more descriptors than allotted.");

    table[num].base_low = (base & 0xFFFF);
    table[num].base_middle = (base >> 16) & 0xFF;
    table[num].base_high = (base >> 24) & 0xFF;

    table[num].limit_low = (limit & 0xFFFF);
    table[num].granularity = ((limit >> 16) & 0x0F);

    table[num].granularity |= (granularity << 4) & 0xF0;
    table[num].access = access;
}

template <std::size_t N>
void GlobalDescriptorTable<N>::setup_stivale2() {
    pointer.limit = sizeof(table) - 1;
    pointer.base = reinterpret_cast<std::uintptr_t>(&table);

    constexpr std::size_t code_segment =
        GDT_PRESENT | GDT_PRIVILEGE_DPL0 | GDT_CODE_SEGMENT | GDT_EXECUTABLE_SEGMENT | GDT_READ_WRITE_SEGMENT;
    constexpr std::size_t data_segment = GDT_PRESENT | GDT_PRIVILEGE_DPL0 | GDT_DATA_SEGMENT | GDT_READ_WRITE_SEGMENT;

    constexpr std::size_t bits_16 = GDT_1B_GRANULARITY | GDT_16_BIT_SEGMENT;
    constexpr std::size_t bits_32 = GDT_4K_GRANULARITY | GDT_32_BIT_SEGMENT;
    constexpr std::size_t bits_64 = GDT_4K_GRANULARITY | GDT_64_BIT_SEGMENT;

    set_descriptor<0>(0, 0, 0, 0);                             // null descriptor
    set_descriptor<1>(0, 0x000000FFFF, code_segment, bits_16); // 16-bit code
    set_descriptor<2>(0, 0x000000FFFF, data_segment, bits_16); // 16-bit data
    set_descriptor<3>(0, 0x00FFFFFFFF, code_segment, bits_32); // 32-bit code
    set_descriptor<4>(0, 0x00FFFFFFFF, data_segment, bits_32); // 32-bit data
    set_descriptor<5>(0, 0xFFFFFFFFFF, code_segment, bits_64); // 64-bit code
    set_descriptor<6>(0, 0xFFFFFFFFFF, data_segment, bits_64); // 64-bit data

    set_GDTR(reinterpret_cast<void *>(&pointer));
}

#endif