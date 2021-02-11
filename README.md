# Create an OS from scratch

#### I have always wanted to boot OS from scratch.

<br/>

## Things which are done
-   Boot from scratch
-   Enter 32-bit Protected mode
-   Jump from assembly to C

<br/>

## Directory structure

```
├── boot
│   ├── boot_sect.asm
│   ├── disk_load.asm
│   ├── gdt.asm
│   ├── print_string.asm
│   ├── print_string_pm.asm
│   └── switch_to_pm.asm
├── drivers
│   ├── ports.c
│   ├── ports.h
│   ├── screen.c
│   └── screen.h
├── kernel
│   ├── kernel.c
│   └── kernel_entry.asm
├── Makefile
```

`/Practice` contains chunks of routines used to actually boot

<br/>

## Build

```bash
$ make 
$ make run
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

## PS
I should have made symbolic links to the routines from /Practice but I forgot :D 