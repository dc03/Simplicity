#include "Kernel.hpp"

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

/**
 * @brief The entry point of the kernel
 *
 * All execution starts from here
 */
extern "C" [[noreturn]] void _start(struct stivale2_struct *stivale2_struct) {
    Kernel kernel{stivale2_struct};

    stivale2_struct_tag_terminal *term_str_tag = kernel.get_stivale2_terminal();

    if (term_str_tag == nullptr) {
        for (;;) {
            asm("hlt");
        }
    }

    auto term_write = reinterpret_cast<Kernel::Stivale2TermWriteType>(term_str_tag->term_write);
    term_write("Hello world", 11);

    for (;;) {
        asm("hlt");
    }
}