# Copy the relevant files over.
cp -v build/simplicity.elf limine.cfg external/limine/limine.sys \
      external/limine/limine-cd.bin external/limine/limine-eltorito-efi.bin iso_root/
 
# Create the bootable ISO.
xorriso -as mkisofs -b limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-eltorito-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso_root -o image.iso
 
# Install Limine stage 1 and 2 for legacy BIOS boot.
./external/limine/limine-install image.iso