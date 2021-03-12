#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../memory/paging.h"
#include "../libc/function.h"

void kmain()
{
	isr_install();
	irq_install();

	uint32_t a = kmalloc(8);
	initialise_paging();
	uint32_t b = kmalloc(8);
	uint32_t c = kmalloc(8);
	/* monitor_write("a: ");
    monitor_write_hex(a);
    monitor_write(", b: ");
    monitor_write_hex(b);
    monitor_write("\nc: ");
    monitor_write_hex(c); */

	char buffer[10];
	buffer[0] = 0;
	hex_to_ascii(a, buffer);
	kprint(buffer);
	kprint("\n");
	buffer[0] = 0;
	hex_to_ascii(b, buffer);
	kprint(buffer);
	kprint("\n");

	buffer[0] = 0;
	hex_to_ascii(c, buffer);
	kprint(buffer);
	kprint("\n");

	kfree((void *)c);
	kfree((void *)b);
	uint32_t d = kmalloc(12);
	buffer[0] = 0;
	hex_to_ascii(d, buffer);
	kprint(buffer);

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
