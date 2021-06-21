#include "Kernel.hpp"

#include <cstdint>

Kernel::Kernel(stivale2_struct *stivale2_struct) : stivale2_header{stivale2_struct} {
    GDT.setup_stivale2();
    IDT.setup_IDT();
}

void *Kernel::stivale2_get_tag(struct stivale2_struct *stivale2_struct, std::uint64_t id) {
    auto current_tag = reinterpret_cast<stivale2_tag *>(stivale2_struct->tags);

    for (;;) {
        if (current_tag == nullptr) {
            return nullptr;
        }

        if (current_tag->identifier == id) {
            return current_tag;
        }

        current_tag = reinterpret_cast<stivale2_tag *>(current_tag->next);
    }
}

stivale2_struct_tag_terminal *Kernel::get_stivale2_terminal() {
    return reinterpret_cast<stivale2_struct_tag_terminal *>(
        stivale2_get_tag(stivale2_header, STIVALE2_STRUCT_TAG_TERMINAL_ID));
}