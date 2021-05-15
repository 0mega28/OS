#include "../drivers/screen.h"
#include "../cpu/gdt.h"
#include "../cpu/isr.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../memory/paging.h"
#include "../libc/function.h"
#include "bootanimation.h"
#include <stdbool.h>
#include "kshell.h"

bool is_busy = false;

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
