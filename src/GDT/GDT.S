    .intel_syntax noprefix
    .global set_GDTR
set_GDTR:
    # The Sys-V ABI requires the first parameter to be passed through rdi
    # So it is safe to assume the GDT pointer is in `rdi`
    lgdt    [rdi]
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    ret