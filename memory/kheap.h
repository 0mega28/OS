#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>

/* 
 * kmalloc allocates chunks of memory
 * if align == 1, chunk must be page-aligned
 * if phy != 0, the physical address of the chunk
 * will be stored in phy 
 */

uint32_t kmalloc_int(uint32_t size, int align, uint32_t *phys);

/* Wrappers arround kmalloc_int below */

/* The chunk must be aligned */
uint32_t kmalloc_a(uint32_t size);

/* Not aligned but phy stores physical address */
uint32_t kmalloc_p(uint32_t size, uint32_t *phys);

/* Chunk must be aligned and phy stores physical address */
uint32_t kmalloc_ap(uint32_t size, uint32_t *phys);

/* General allocation function */
uint32_t kmalloc(uint32_t size);

#endif