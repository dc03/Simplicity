    .intel_syntax noprefix

    .text
    .global set_IDTR
set_IDTR:
    lidt    [rdi]
    #sti - not yet
    ret

    .global exception_handler
exception_handler:
    cli
    hlt

# Using iretq in 64-bit mode
.macro ISR_error_stub n
ISR_stub_\n:
    call    exception_handler
    iretq
.endm

.macro ISR_no_error_stub n
ISR_stub_\n:
    call    exception_handler
    iretq
.endm

ISR_no_error_stub 0
ISR_no_error_stub 1
ISR_no_error_stub 2
ISR_no_error_stub 3
ISR_no_error_stub 4
ISR_no_error_stub 5
ISR_no_error_stub 6
ISR_no_error_stub 7
ISR_error_stub    8
ISR_no_error_stub 9
ISR_error_stub    10
ISR_error_stub    11
ISR_error_stub    12
ISR_error_stub    13
ISR_error_stub    14
ISR_no_error_stub 15
ISR_no_error_stub 16
ISR_error_stub    17
ISR_no_error_stub 18
ISR_no_error_stub 19
ISR_no_error_stub 20
ISR_no_error_stub 21
ISR_no_error_stub 22
ISR_no_error_stub 23
ISR_no_error_stub 24
ISR_no_error_stub 25
ISR_no_error_stub 26
ISR_no_error_stub 27
ISR_no_error_stub 28
ISR_no_error_stub 29
ISR_error_stub    30
ISR_no_error_stub 31

    .data
    .global ISR_stub_table
ISR_stub_table:
    .irpc a, 012345789
        .quad ISR_stub_\a
    .endr
    .irpc a, 12
    .irpc b, 0123456789
        .quad ISR_stub_\a\b
    .endr
    .endr
        .quad ISR_stub_30
        .quad ISR_stub_31