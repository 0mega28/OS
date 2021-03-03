#ifndef FUNCTION_H
#define FUNCTION_H

#include "../drivers/screen.h"
#include "../libc/string.h"

/* Sometimes we want to keep parameters to a function for later use
 * and this is a solution to avoid the 'unused parameter' compiler warning */
#define UNUSED(x) (void)(x)
#define PANIC(msg) panic(msg, __FILE__, __LINE__)
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(char *message, char *file, uint32_t line);
extern void panic_assert(char *file, uint32_t line, char *description);

#endif