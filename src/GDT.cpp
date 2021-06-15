#include "GDT.hpp"
#include <cstdint>

GDTEntry GDT[7];
GDTPointer GDT_pointer;

/**
 * @brief Adds a descriptor to the GDT
 * 
 * @param num The index at which to add the descriptor
 * @param base The base address of the segment
 * @param limit The size of the segment
 * @param access The access flags
 * @param granularity The four upper limit bits and the four flag bits
 */
void gdt_set_gate(std::size_t num, std::uint64_t base, std::uint64_t limit, std::uint8_t access, std::uint8_t granularity) {
    GDT[num].base_low = (base & 0xFFFF);
    GDT[num].base_middle = (base >> 16) & 0xFF;
    GDT[num].base_high = (base >> 24) & 0xFF;

    GDT[num].limit_low = (limit & 0xFFFF);
    GDT[num].granularity = ((limit >> 16) & 0x0F);

    GDT[num].granularity |= (granularity & 0xF0);
    GDT[num].access = access;
}

/**
 * @brief Sets up the GDT
 * 
 * Sets up the GDT as per the stivale2 spec
 */
void gdt_install() {
    GDT_pointer.limit = sizeof(GDT) - 1;
    GDT_pointer.base = reinterpret_cast<std::uintptr_t>(&GDT);

    gdt_set_gate(0, 0, 0, 0, 0); // null descriptor
    gdt_set_gate(1, 0, 0x000000FFFF, 0b10011010, 0b00000000); // 16-bit code
    gdt_set_gate(2, 0, 0x000000FFFF, 0b10010010, 0b00000000); // 16-bit data
    gdt_set_gate(3, 0, 0x00FFFFFFFF, 0b10011010, 0b11000000); // 32-bit code
    gdt_set_gate(4, 0, 0x00FFFFFFFF, 0b10010010, 0b11000000); // 32-bit data
    gdt_set_gate(5, 0, 0xFFFFFFFFFF, 0b10011010, 0b10100000); // 64-bit code
    gdt_set_gate(6, 0, 0xFFFFFFFFFF, 0b10010010, 0b10100000); // 64-bit data

    gdt_flush();
}

extern "C" void gdt_flush() {
    asm(
        "   .intel_syntax noprefix\n"
        "   .extern GDT_pointer\n"
        "   lgdt    [rip + GDT_pointer]\n"
        "   mov     ax, 0x10\n"
        "   mov     ds, ax\n"
        "   mov     es, ax\n"
        "   mov     fs, ax\n"
        "   mov     gs, ax\n"
        "   mov     ss, ax\n"
    );
}