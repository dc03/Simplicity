#pragma once

#ifndef GDT_HPP
#define GDT_HPP

#include <cstdint>

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

/**
 * @brief A GDT selector
 * 
 * The first two bytes are the size of the GDT, the next four bytes are the base address
 */
struct GDTPointer {
    std::uint16_t limit{};
    std::uint64_t base{};
} __attribute__((packed));

extern "C" GDTEntry GDT[7];
extern "C" GDTPointer GDT_pointer;

extern "C" void gdt_flush();
void gdt_set_gate(std::size_t num, std::uint64_t base, std::uint64_t limit, std::uint8_t access, std::uint8_t granularity);
void gdt_install();

#endif