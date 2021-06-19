#pragma once

#ifndef IDT_HPP
#define IDT_HPP

#include <cstddef>
#include <cstdint>

/**
 * @brief The Interrupt Descriptor Table
 *
 * Describes where the Interrupt Service Routines are present in 32- and 64-bit protected mode.
 * It is the protected mode counterpart of the Interrupt Vector Table, located at memory location 0x0000.
 */
class InterruptDescriptorTable {
    /**
     * @brief An entry into the IDT
     */
    struct IDTEntry {
        std::uint16_t ISR_base_low{};
        std::uint16_t kernel_cs{};
        std::uint8_t ist{};
        std::uint8_t attributes{};
        std::uint16_t ISR_base_middle{};
        std::uint32_t ISR_base_high{};
        std::uint32_t reserved{};
    } __attribute__((packed));

    static_assert(offsetof(IDTEntry, ISR_base_low) == 0);
    static_assert(offsetof(IDTEntry, kernel_cs) == 2);
    static_assert(offsetof(IDTEntry, ist) == 4);
    static_assert(offsetof(IDTEntry, attributes) == 5);
    static_assert(offsetof(IDTEntry, ISR_base_middle) == 6);
    static_assert(offsetof(IDTEntry, ISR_base_high) == 8);
    static_assert(offsetof(IDTEntry, reserved) == 12);


    /**
     * @brief Structure to load into IDTR
     *
     * Contains size of IDT + base address of it
     */
    struct IDTPointer {
        std::uint16_t limit{};
        std::uint64_t base{};
    } __attribute__((packed));

    static_assert(offsetof(IDTPointer, limit) == 0);
    static_assert(offsetof(IDTPointer, base) == 2);

    alignas(0x10) IDTEntry table[256]{};
    IDTPointer pointer{};

  public:
    /**
     * @brief Set value of an IDT descriptor
     *
     * @param vector Which descriptor to set the value of
     * @param ISR The pointer to the ISR
     * @param flags The value for the attributes byte in the descriptor
     */
    void set_descriptor(std::uint8_t vector, void *ISR, std::uint8_t flags);

    /**
     * @brief Sets up the IDT and loads it into the IDTR
     */
    void setup_IDT();
};

/**
 * @brief General exception handler
 *
 * Halts the computer
 */
extern "C" [[noreturn]] void exception_handler();

/**
 * @brief Sets the IDTR to point to the given IDT
 *
 * @param IDTR The IDT pointer used to set the IDTR
 */
extern "C" void set_IDTR(void *IDTR);

/**
 * @brief The ISR stub table defined in `IDT.S`
 */
extern void *ISR_stub_table[];

#endif