#include "function.h"

extern void panic(char *message, char *file, uint32_t line)
{
	/* Encountered problem, hlt cpu */
	asm volatile("cli"); /* Disable interrupt */
	kprint("PANIC(");
	kprint(message);
	kprint(") at ");
	kprint(file);
	kprint(":");
	char buffer[10];
	int_to_ascii(line, buffer);
	kprint(buffer);
	kprint("\n");

	while (1)
		;
}

extern void panic_assert(char *file, uint32_t line, char *description)
{
	// An assertion failed, and we have to panic.
	asm volatile("cli"); // Disable interrupts.

	kprint("ASSERTION-FAILED(");
	kprint(description);
	kprint(") at ");
	kprint(file);
	kprint(":");
	char buffer[10];
	int_to_ascii(line, buffer);
	kprint(buffer);
	kprint("\n");

	while (1)
		;
}

void page_align(uint32_t *address)
{
	if(*address & 0x00000FFF)
	{
		*address &= 0xFFFFF000;
		*address += 0x1000;
	}
}