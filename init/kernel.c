#include "../drivers/screen.h"
#include "../cpu/gdt.h"
#include "../cpu/isr.h"
#include "../libc/string.h"
#include "../memory/paging.h"
#include "../libc/function.h"
#include "../kernel/bootanimation.h"
#include <stdbool.h>
#include "../kernel/kshell.h"

extern bool is_busy;

void kmain()
{
	init_gdt();
	isr_install();
	irq_install();

	is_busy = true;
	bootanimation();
	is_busy = false;

	initialise_paging();

	kshell();
}
