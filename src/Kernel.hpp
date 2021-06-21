#pragma once

#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "Constants.hpp"
#include "GDT/GDT.hpp"
#include "IDT/IDT.hpp"
#include "stivale2.h"

/**
 * @brief The kernel itself
 *
 * GDT has a size of 7 because of the stivale2 spec requiring 7 descriptors:
 * - null descriptor
 * - 16-bit code
 * - 16-bit data
 * - 32-bit code
 * - 32-bit data
 * - 64-bit code
 * - 64-bit data
 */
class Kernel {
    GlobalDescriptorTable<GDT_SIZE> GDT{};
    InterruptDescriptorTable IDT{};
    stivale2_struct *stivale2_header{};

  public:
    using Stivale2TermWriteType = void (*)(const char *string, std::size_t length);

    /**
     * @brief Kernel class constructor
     *
     * Sets up the GDT and the IDT
     *
     * @param stivale2_struct The struct passed to _start by stivale2
     */
    Kernel(stivale2_struct *stivale2_struct);

    /**
     * @brief Function to try and find a particular stivale2 tag
     *
     * Traverses the linked list to find the required tag, otherwise returns
     * @c nullptr
     *
     * @param stivale2_struct The node of the linked list to start searching from
     * @param id The tag type to search for
     * @return The node containing the required tag
     */
    void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, std::uint64_t id);

    /**
     * @brief Special function to get the stivale2 terminal
     *
     * Just calls @c stivale2_get_tag
     *
     * @return The stivale2 terminal pointer
     */
    stivale2_struct_tag_terminal *get_stivale2_terminal();
};

#endif