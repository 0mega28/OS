#include "function.h"

extern void panic(const char *message, const char *file, uint32_t line)
{
    /* Encountered problem, hlt cpu */
    asm volatile("cli"); /* Disable interrupt */
    kprint("PANIC(");
    kprint(message);
    kprint(") at ");
    kprint(file);
    kprint(":");
    char buffer[15];
    int_to_ascii(line, buffer);
    kprint(buffer);
    kprint("\n");

    while(1);
}

extern void panic_assert(const char *file, uint32_t line, const char *description)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    kprint("ASSERTION-FAILED(");
    kprint(description);
    kprint(") at ");
    kprint(file);
    kprint(":");
    char buffer[15];
    int_to_ascii(line, buffer);
    kprint(buffer);
    kprint("\n");
    
    while(1);
}
