# Automatically generate lists of source using wildcards.
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c memory/*.c init/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h memory/*.h init/*.h)

# Convert the *.c filenames to *.o to give a list of object files to build
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o cpu/gdt_flush.o}

CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32
LDFLAGS = -Tlinker.ld

# Default build target
all: kernel.elf

# Run
run: all
	qemu-system-i386 -kernel kernel.elf

# This builds the binary of our kernel from two object files:
#	- the kernel_entry, which jumps to main in our kernel
#	- the compile C kernel
kernel.bin: boot.o ${OBJ}
	i686-elf-ld -o $@ ${LDFLAGS} $^ --oformat binary

# Generic rule to compile C code to an object file
%.o: %.c ${HEADERS}
	i686-elf-gcc ${CFLAGS} -c $< -o $@

# Assemble kernel_entry
%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -I 'boot/' -o $@

clean:
	find . -name "*.o" -type f -delete
	find . -name "*.bin" -type f -delete
	find . -name "*.elf" -type f -delete
	find . -name "*.dis" -type f -delete

# Disassemble our kernel (Useful for debugging)
kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@

# Used for debugging purposes
kernel.elf: boot.o ${OBJ}
	i686-elf-ld -o $@ ${LDFLAGS} $^

# Open the connection to QEmu and load our kernel-object file with symbols
# The -s option will make QEMU listen for an incoming connection from gdb on TCP port 1234
# -S will make QEMU not start the guest until you tell it to from gdb. 
# To setup breakpoint try b main:line_number or b kernel.c:line_number
debug: kernel.elf
	qemu-system-i386 -s -S -kernel kernel.elf &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf" -ex "b kmain" -ex "continue"
