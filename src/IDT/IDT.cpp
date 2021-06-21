#include "IDT.hpp"

void InterruptDescriptorTable::setup_IDT() {
    pointer.limit = sizeof(table) - 1;
    pointer.base = reinterpret_cast<std::uintptr_t>(&table[0]);

    for (std::uint8_t vector = 0; vector < IDT_SIZE; vector++) {
        set_descriptor(vector, ISR_stub_table[vector], AttributeType::INTERRUPT_32_DPL0);
    }

    set_IDTR(reinterpret_cast<void *>(&pointer));
}

void InterruptDescriptorTable::set_descriptor(std::uint8_t vector, void *ISR, std::uint8_t attributes) {
    IDTEntry *descriptor = &table[vector];

    descriptor->ISR_base_low = reinterpret_cast<std::uint64_t>(ISR) & 0xFFFF;
    descriptor->kernel_cs = GDT_KERNEL_CS_OFFSET;
    descriptor->ist = 0;
    descriptor->attributes = attributes;
    descriptor->ISR_base_middle = (reinterpret_cast<std::uint64_t>(ISR) >> 16) & 0xFFFF;
    descriptor->ISR_base_high = (reinterpret_cast<std::uint64_t>(ISR) >> 32) & 0xFFFFFFFF;
    descriptor->reserved = 0;
}