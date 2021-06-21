#include "../Constants.hpp"

    .intel_syntax noprefix
    .text
    .global set_GDTR
set_GDTR:
    # The Sys-V ABI requires the first parameter to be passed through rdi
    # So it is safe to assume the GDT pointer is in `rdi`
    lgdt    [rdi]
    mov     ax, GDT_KERNEL_DS_OFFSET
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    pushq   GDT_KERNEL_CS_OFFSET
    lea     rdi, [rip+set_GDTR_end]
    push    rdi
    lretq
set_GDTR_end:
    ret