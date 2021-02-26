#include "mem.h"

void memory_copy(char *source, char *dest, int nbytes)
{
    for (int i = 0; i < nbytes; i++)
    {
        dest[i] = source[i];
    }
}

void memory_set(uint8_t *dest, uint8_t val, uint32_t len)
{
    uint8_t *temp = (uint8_t *)dest;
    for (; len != 0; len--)
        *temp++ = val;
}

/* Just a pointer to some free memory address which keeps growing */
uint32_t free_mem_addr = 0x10000;

uint32_t kmalloc(uint32_t size, int align, uint32_t *phys_addr)
{
    if (align == 1 && (free_mem_addr & 0xFFFFF000))
    {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }
    /* Save physical address */
    if (phys_addr)
        *phys_addr = free_mem_addr;

    uint32_t ret = free_mem_addr;
    free_mem_addr += size;

    return ret;
}