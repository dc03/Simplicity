#!/usr/bin/env sh
qemu-system-x86_64 -D /tmp/qemu.log -S -gdb tcp::1234 -no-reboot -no-shutdown -d int -M q35,smm=off image.iso&