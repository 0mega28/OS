#!/usr/bin/bash

# boot_sect.asm is the file into which we saved the source code and
# boot_sect.bin is the assembled machine code that we can install as a boot sector
# on a disk.
# We have used -f bin option to tell nasm to produce raw maching code
# rather than a code package that has additional meta info

nasm boot_sect.asm -f bin -o boot_sect.bin

# Running the Emulator
qemu-system-x86_64 boot_sect.bin

# To see the content of boot_sect.bin run, 
# od -t x1 -A n boot sect.bin