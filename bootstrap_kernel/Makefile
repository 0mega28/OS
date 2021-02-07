# $^ is substituted with all of the target's dependecy files
# $< is the first dependency and $@ is the target file

all: os.img

run: all
	qemu-system-x86_64 os.img

clean:
	rm -fr *.bin *.dis *.o os.img *.map

# Rather than previously linking kernel.o file to itself 
# we can link it with kernel_entry.o as follows:
# Linker respects order of file it is given
# therefore we are assured kernel_entry.o will precede the code in kernel.o
kernel.bin: kernel_entry.o kernel.o
	ld -o $@ -Ttext 0x1000 $^ --oformat binary

# -f elf tells the assemble to output object file,
# therefore preserving meta information about labels it must refer
kernel.o: kernel.c
	gcc -ffreestanding -c $^ -o $@

# -f elf tells the assemble to output object file,
# therefore preserving meta information about labels it must refer
kernel_entry.o: kernel_entry.asm
	nasm $< -f elf64 -o $@

boot_sect.bin: boot_sect.asm
	nasm -f bin $^ -o $@

os.img: boot_sect.bin kernel.bin
	cat $^ > $@

# Disassemble kernel
kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@