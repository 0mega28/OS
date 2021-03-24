#ifndef FUNCTION_H
#define FUNCTION_H

#include "../drivers/screen.h"
#include "../libc/string.h"

/* Sometimes we want to keep parameters to a function for later use
 * and this is a solution to avoid the 'unused parameter' compiler warning */
#define UNUSED(x) (void)(x)

/* Macros wrapping debugging functions */
#define PANIC(msg) panic(msg, __FILE__, __LINE__)
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

/* Debugging functions */
extern void panic(char *message, char *file, uint32_t line);
extern void panic_assert(char *file, uint32_t line, char *description);

/* Align the address according to page if not already aligned */
void page_align(uint32_t *address);

/* Function which stops cpu execution for n seconds */
void wait_for_seconds(int n);

#endif