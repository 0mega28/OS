# Create an OS from scratch

### I have always wanted to boot OS from scratch.

## Things which are done
-   Boot from scratch
-   Enter 32-bit Protected mode
-   Jump from assembly to C

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
│   └── placeholder
├── kernel
│   ├── kernel.c
│   └── kernel_entry.asm
├── Makefile
```

`/Practice` contains chunks of routines used to actually boot

## Build

```bash
$ make 
$ make run
```

# Resources 
[Writing a Simple Operating System —
from Scratch -by
Nick Blundell](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)