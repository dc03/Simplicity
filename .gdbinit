add-symbol-file ./build/simplicity.elf
target remote | qemu -S -gdb -no-reboot -no-shutdown tcp::1234