#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../memory/paging.h"
#include "../libc/function.h"
#include "bootanimation.h"

void kmain()
{
	isr_install();
	irq_install();
	
	bootanimation();

	initialise_paging();

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
