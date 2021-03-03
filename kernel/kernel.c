#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../cpu/paging.h"
#include "../libc/function.h"

void kmain()
{
    isr_install();
    irq_install();
	initialise_paging();

	extern uint32_t end;
	char buffer[10];
	hex_to_ascii((uint32_t)&end, buffer);
	kprint(buffer);
	kprint("\n");
	
	uint32_t *ptr = (uint32_t*) 0xFFFF0000;
	uint32_t do_page_fault = *ptr;
	UNUSED(do_page_fault);

    kprint("Type END to halt the CPU\nType CLEAR to clear screen\n> ");
}

void user_input(char *input)
{
    if (strcmp(input, "END") == 0)
    {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    else if (strcmp(input, "CLEAR") == 0)
    {
        clear_screen();
    }
    kprint("You said: ");
    kprint(input);
    kprint("\n> ");
}
