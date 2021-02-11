# Automatically generate lists of source using wildcards.
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

# Convert the *.c filenames to *.o to give a list of object files to build
OBJ = ${C_SOURCES:.c=.o}

# Default build target
all: os-image

# Run
run: all
	qemu-system-i386 os-image 2> /dev/null

# This is the actual disk image that our computer loads
# which is the combination of our compiled bootsector and kernel
os-image: boot/boot_sect.bin kernel.bin
	cat $^ > $@

# This builds the binary of our kernel from two object files:
#	- the kernel_entry, which jumps to main in our kernel
#	- the compile C kernel
kernel.bin: kernel/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Generic rule to compile C code to an object file
%.o: %.c ${HEADERS}
	i686-elf-gcc -g -ffreestanding -c $< -o $@

# Assemble kernel_entry
%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -I 'boot/' -o $@

clean:
	rm -fr *.bin *.dis *.o os-image	*.elf
	rm -fr kernel/*.o boot/*.bin drivers/*.o boot/*.o

# Disassemble our kernel (Useful for debugging)
kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@

# Used for debugging purposes
kernel.elf: kernel/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ 

# Open the connection to QEmu and load our kernel-object file with symbols
# The -s option will make QEMU listen for an incoming connection from gdb on TCP port 1234
# -S will make QEMU not start the guest until you tell it to from gdb. 
# To setup breakpoint try b main:line_number or b kernel.c:line_number
debug: os-image kernel.elf
	qemu-system-i386 -s -S os-image &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf" -ex "b main" -ex "continue"
