#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "../libc/function.h"
#include "kheap.h"
#include "../libc/mem.h"
#include "../cpu/isr.h"

typedef struct page
{
    uint32_t present : 1;  // Page present in memory
    uint32_t rw : 1;       // Read-only if clear, read-write if set
    uint32_t user : 1;     // Supervisor level only if clear
    uint32_t accessed : 1; // Page been accessed since last refresh
    uint32_t dirty : 1;    // Page been written to since last refresh
    uint32_t unused : 7;   // Amalgamation of unused and reserved bits
    uint32_t frame : 20;   // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    /* Array of pointers to pagetables */
    page_table_t *tables[1024];
    /*
     * Array of pointer to the pagetables but give their 
     * physical location, for loading into the CR3 register.
     */
    uint32_t tablePhysical[1024];
    /*
     * The physical address of tablePhysical.
     * Used when we get our kernel heap allocated and
     * the directory may be in a different location in 
     * virtual memory
     */
    uint32_t physicalAddr;
} page_directory_t;

/* 
 * Sets up the environment,
 * page directories etc and enables paging.
 */
void initialise_paging();

/* 
 * Causes the specified page directory to be 
 * loaded into the CR3 register.
 */
void switch_page_directory(page_directory_t *dir);

/*
    Retrieces a pointer to the page required.
    If make == 1, if the pagetable in which this
    page should reside isn't created, create it
*/
page_t *get_page(uint32_t address, int make, page_directory_t *dir);

/* Function to allocate a frame */
void alloc_frame(page_t *page, int is_kernel, int is_writeable);

/* Function to deallocate a frame */
void free_frame(page_t *page);

/* Handler for page faults. */
void page_fault(registers_t *regs);

#endif