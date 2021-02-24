#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "kernel.h"
#include "../libc/string.h"

void kmain()
{
    isr_install();
    irq_install();

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
