#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"
#include <stdint.h>

void memory_copy(char *source, char *dest, int nbytes);
void memory_set(uint8_t *dest, uint8_t val, uint32_t len);

#endif