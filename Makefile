# Automatically generate lists of source using wildcards.
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

# Convert the *.c filenames to *.o to give a list of object files to build
OBJ = ${C_SOURCES:.c=.o}

# Default build target
all: os-image

# Run
run: all
	qemu-system-i386 os-image

# This is the actual disk image that our computer loads
# which is the combination of our compiled bootsector and kernel
os-image: boot/boot_sect.bin kernel.bin
	cat $^ > $@

# This builds the binary of our kernel from two object files:
#	- the kernel_entry, which jumps to main in our kernel
#	- the compile C kernel
kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Generic rule to compile C code to an object file
%.o: %.c ${HEADERS}
	gcc -ffreestanding -c $< -o $@

# Assemble kernel_entry
%.o: %.asm
	nasm $< -f elf64 -o $@

%.bin: %.asm
	nasm $< -f bin -I 'boot/' -o $@

clean:
	rm -fr *.bin *.dis *.o os-image
	rm -fr kernel/*.o boot/*.bin drivers/*.o

# Disassemble our kernel (Useful for debugging)
kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@