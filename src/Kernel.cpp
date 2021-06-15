#include "GDT.hpp"
#include "stivale2.h"

#include <cstddef>
#include <cstdint>

static uint8_t stack[4096];

// clang-format off
static stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = 0
    },
    .flags = 0
};

static stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = reinterpret_cast<std::uint64_t>(&terminal_hdr_tag)
    },

    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0
};

__attribute__((section(".stivale2hdr"), used)) static stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = reinterpret_cast<std::uintptr_t>(stack + sizeof(stack)),
    .flags = (1 << 1),
    .tags = reinterpret_cast<std::uintptr_t>(&framebuffer_hdr_tag)
};
// clang-format on

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    stivale2_tag *current_tag = reinterpret_cast<stivale2_tag *>(stivale2_struct->tags);

    for (;;) {
        if (current_tag == NULL) {
            return NULL;
        }

        if (current_tag->identifier == id) {
            return current_tag;
        }

        current_tag = reinterpret_cast<stivale2_tag *>(current_tag->next);
    }
}

extern "C" void _start(struct stivale2_struct *stivale2_struct) {
    gdt_install();

    stivale2_struct_tag_terminal *term_str_tag = reinterpret_cast<stivale2_struct_tag_terminal *>(
        stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID));

    if (term_str_tag == NULL) {
        for (;;) {
            asm("hlt");
        }
    }

    using term_write_t = void (*)(const char *string, size_t length);

    term_write_t term_write = reinterpret_cast<term_write_t>(term_str_tag->term_write);
    term_write("Hello world", 11);

    for (;;) {
        asm("hlt");
    }
}