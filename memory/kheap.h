#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>
#include "../libc/ordered_array.h"
#include "paging.h"

/* Start address of heap i.e. 3GB in memory */
#define KHEAP_START 0xC0000000

/* Intial size of heap */
#define KHEAP_INITIAL_SIZE 0x100000 /* 1 MB */

/* Index size of ordered array sorted by size of hole */
#define KHEAP_ARRAY_INDEX_SIZE 0x20000

/* Magic number to identify header and footer */
#define KHEAP_MAGIC 0xFF1729FF

/* This is minimum size of heap below which it will not contract
	same as initial size of heap */
#define KHEAP_MINIMUM_SIZE 0x100000

/* Header and footer definition for 
	blocks/holes */
typedef struct
{
	uint32_t magic_number; /* Used for identification */
	uint8_t is_hole;		   /* 1 if it is hole else 0 for block */
	uint32_t size;
} header_t;

typedef struct
{
	uint32_t magic_number; /* Used for identification */
	header_t *header;
} footer_t;

typedef struct
{
	ordered_array_t index_array;	/* Array to keap track of holes */
	uint32_t start_address; /* Start address of heap */
	uint32_t end_address;	/* End address of heap */
	uint32_t max_address;	/* Max address of heap i.e. 0xFFFFE000 */
	uint8_t is_supervisor;	/* 1 if it's ring 0, else 0 */
	uint8_t is_readonly;	/* 0 if it's writeable, else 1 */
} heap_t;

/* Heap initialsize function */
heap_t *create_heap(uint32_t start_address, uint32_t end_address, uint32_t max_address, uint8_t is_supervisor, uint8_t is_readonly);

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

/* kfree deallocates memory from heap from address "addr" */
void kfree(void *addr);

#endif