# Automatically generate lists of source using wildcards.
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)

# Convert the *.c filenames to *.o to give a list of object files to build
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32
LDFLAGS = -Tlinker.ld

# Default build target
all: os.img

# Run
run: all
	qemu-system-i386 -drive format=raw,file=os.img -monitor stdio

# This is the actual disk image that our computer loads
# which is the combination of our compiled bootsector and kernel
os.img: boot/boot_sect.bin kernel.bin
	cat $^ > $@

# This builds the binary of our kernel from two object files:
#	- the kernel_entry, which jumps to main in our kernel
#	- the compile C kernel
kernel.bin: kernel/kernel_entry.o ${OBJ}
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
	rm os.img

# Disassemble our kernel (Useful for debugging)
kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@

# Used for debugging purposes
kernel.elf: kernel/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ ${LDFLAGS} $^

# Open the connection to QEmu and load our kernel-object file with symbols
# The -s option will make QEMU listen for an incoming connection from gdb on TCP port 1234
# -S will make QEMU not start the guest until you tell it to from gdb. 
# To setup breakpoint try b main:line_number or b kernel.c:line_number
debug: os.img kernel.elf
	qemu-system-i386 -s -S os.img &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf" -ex "b kmain" -ex "continue"
