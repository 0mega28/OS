OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)
/* OUTPUT(kernel.bin) */

SECTIONS {
    begin = .; _begin = .; __begin = .;

    .text 0x1000 :
    {
        code = .; _code = .; __code = .;
        *(.text.entry) 
        *(.text)
		. = ALIGN(4096);
    }
    
	.data :
    {
        data = .; _data = .; __data = .;
        *(.rodata)
        *(.data)
		. = ALIGN(4096);
    }

    .bss :
    {
        bss = .; _bss = .; __bss = .;
        *(COMMON)
        *(.bss)
		. = ALIGN(4096);
    }

    end = .; _end = .; __end = .;
}