#!/usr/bin/env sh
qemu-system-x86_64 -S -gdb tcp::1234 image.iso&