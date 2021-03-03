# Create an OS from scratch

 - I have always wanted to boot OS from scratch.
 
 - In this project we are bootstraping kernel via a simple boot sector which loads the kernel into memory, then loadg gdt and switches to 32-bit protected mode from 16-bit real mode and then calls the kernel code.
 
 - Kernel sets up the interrupt descriptor table, initializes timer and keyboard interrupt.

<br/>

## Things which are done
-   Boot from scratch
-   Enter 32-bit Protected mode
-   Jump from assembly to C
-   Interrupt handling
-   Keyboard input and output
-   A basic libc
-	Virtual memory (Paging)

<br/>

## Directory structure

```
├── boot
│   ├── boot_sect.asm
│   ├── disk_load.asm
│   ├── gdt.asm
│   ├── print_hex.asm
│   ├── print_string.asm
│   ├── print_string_pm.asm
│   └── switch_to_pm.asm
├── cpu
│   ├── idt.c
│   ├── idt.h
│   ├── interrupt.asm
│   ├── isr.c
│   ├── isr.h
│   ├── paging.c
│   ├── paging.h
│   ├── ports.c
│   ├── ports.h
│   ├── timer.c
│   ├── timer.h
│   └── types.h
├── cross-compiler.sh
├── drivers
│   ├── keyboard.c
│   ├── keyboard.h
│   ├── screen.c
│   └── screen.h
├── get_kernel_size
├── kernel
│   ├── kernel.c
│   ├── kernel_entry.asm
│   └── kernel.h
├── libc
│   ├── function.h
│   ├── mem.c
│   ├── mem.h
│   ├── string.c
│   └── string.h
├── Makefile
```

`/Practice` contains chunks of routines used to actually boot

<br/>

## Build

```bash
$ sudo apt install nasm     # Netwide Assembler
$ sudo apt install qemu     # CPU emulator
$ sudo apt install make     # Build system
$ ./cross-compiler          # Builds the cross compiler 
$ make run                  # Build and Run
```

<br/>

## Resources 
-   [Writing a Simple Operating System —
from Scratch -by
Nick Blundell](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)
-   https://github.com/cfenollosa/os-tutorial
-   https://github.com/acmiitr/KSOS
-   http://www.brokenthorn.com/Resources/OSDevIndex.html
-   http://www.jamesmolloy.co.uk/tutorial_html/
-   https://wiki.osdev.org/Meaty_Skeleton

<br/>
