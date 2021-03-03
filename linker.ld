OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)
/* OUTPUT(kernel.bin) */

SECTIONS {
    . = 0x1000;

    begin = .; _begin = .; __begin = .;

    .text : SUBALIGN(16)
    {
        code = .; _code = .; __code = .;
        *(.text.entry) 
        *(.text)
    }

    .data : SUBALIGN(16)
    {
        data = .; _data = .; __data = .;
        *(.rodata)
        *(.data)
    }

    .bss : SUBALIGN(16)
    {
        bss = .; _bss = .; __bss = .;
        *(COMMON)
        *(.bss)
    }
	
	. = ALIGN(4000);

    end = .; _end = .; __end = .;
}