#include "paging.h"

/* Frame Allocation 
 * Bitset for frame allocation more info here https://en.wikipedia.org/wiki/Bitset
 * 
 * There will be four function set, test, clear and find first frame
 * 
 * Two helper macros INDEX_FROM_BIT and OFFSET_FROM_BIT to control
 * by pointer frames
 * 
 * These will be internal to this file
 */

/* Frame pointer (used as bitset) to check if a frame 
 * is free or allocated
 */
uint32_t *frames;

/* Number of frames */
uint32_t no_of_frames;

/* Defined in kheap.c, this is end of kernel address(not now) */
extern uint32_t placement_address;

/* Macros used in bitset */
#define INDEX_BITSET(a) (a / (8 * 4))
#define OFFSET_BITSET(a) (a % (8 * 4))

/* Internal function declaration */

/* Function to set a bit in the frames bitset */
void set_frame(uint32_t frame_address);

/* Function to clear a bit in the frames bitset */
void clear_frame(uint32_t frame_address);

/* Function to test if a bit is set in frames bitset
 * returns 1 if frame is set, and 0 if not set */
uint32_t test_frame(uint32_t frame_address);

/* Function to find first free frame in bitset */
uint32_t first_frame();

/* Function to allocate a frame */
void alloc_frame(page_t *page, int is_kernel, int is_writeable);

/* Function to deallocate a frame */
void free_frame(page_t *page);

/* Exposed function */

/* Kernel's page directory */
page_directory_t *kernel_directory = 0;

/* Current page directory */
page_directory_t *current_directory = 0;

void initialise_paging()
{
	/* Size of physical memory. Assume 16M(For now) */
	uint32_t memory_end = 0x1000000;

	no_of_frames = memory_end / 0x1000;

	/* Allocating space to frame pointer */
	frames = (uint32_t *)kmalloc(INDEX_BITSET(no_of_frames));

	/* Setting bitmap */
	memory_set((uint8_t*)frames, 0, INDEX_BITSET(no_of_frames));

	/* Make page directory */
	kernel_directory = (page_directory_t *)kmalloc_a(sizeof(page_directory_t));
	current_directory = kernel_directory;

	for (uint32_t i = 0; i < placement_address; i += 0x1000)
		/* Kernel code is readable but not writeable from userspace */
		alloc_frame(get_page(i, 1, kernel_directory), 1, 0);

	/* Register page_fault handler */
	register_interrupt_handler(14, page_fault);

	/* Now enable paging */
	switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *dir)
{
	current_directory = dir;
	asm volatile("mov %0, %%cr3"
				 :
				 : "r"(&dir->tablePhysical));
	uint32_t cr0;
	asm volatile("mov %%cr0, %0"
				 : "=r"(cr0));
	cr0 |= 0x80000000; /* Enable paging */
	asm volatile("mov %0, %%cr0"
				 :
				 : "r"(cr0));
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
	/* Turn the address into an index */
	address /= 0x1000;

	/* Index in page directory */
	uint32_t table_idx = address / 1024;

	if (dir->tables[table_idx])
	{
		/* If table already assigned */
		return &dir->tables[table_idx]->pages[address % 1024];
	}
	else if (make)
	{
		uint32_t tmp;
		dir->tables[table_idx] = (page_table_t *)kmalloc_ap(sizeof(page_table_t), &tmp);
		dir->tablePhysical[table_idx] = tmp | 0x7; /* Present RW, US */
		memory_set((uint8_t *)dir->tables[table_idx], 0, 0x1000);
		return &dir->tables[table_idx]->pages[address % 1024];
	}

	return 0;
}

void page_fault(registers_t *regs)
{
	/* A page fault has occurred.
	The faulting address is stored in the CR2 register. */
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0"
				 : "=r"(faulting_address));

	/* 	The error code gives us details of what happened. */
	int present = !(regs->err_code & 0x1); // Page not present
	int rw = regs->err_code & 0x2;		  // Write operation?
	int us = regs->err_code & 0x4;		  // Processor was in user-mode?
	int reserved = regs->err_code & 0x8;	  // Overwritten CPU-reserved bits of page entry?
	int id = regs->err_code & 0x10;		  // Caused by an instruction fetch?
	UNUSED(id);

	/* Output an error message. */
	kprint("Page fault! ( ");
	if (present)
	{
		kprint("present ");
	}
	if (rw)
	{
		kprint("read-only ");
	}
	if (us)
	{
		kprint("user-mode ");
	}
	if (reserved)
	{
		kprint("reserved ");
	}
	kprint(") at 0x");
	char buffer[10];
	hex_to_ascii(faulting_address, buffer);
	kprint(buffer);
	kprint("\n");
	PANIC("Page fault");
}

/* Internal function definition */

void set_frame(uint32_t frame_address)
{
	uint32_t frame = frame_address / 0x1000;
	uint32_t idx = INDEX_BITSET(frame);
	uint32_t off = OFFSET_BITSET(frame);
	frames[idx] |= (0x1 << off);
}

void clear_frame(uint32_t frame_address)
{
	uint32_t frame = frame_address / 0x1000;
	uint32_t idx = INDEX_BITSET(frame);
	uint32_t off = OFFSET_BITSET(frame);
	frames[idx] &= ~(0x1 << off);
}

uint32_t test_frame(uint32_t frame_address)
{
	uint32_t frame = frame_address / 0x1000;
	uint32_t idx = INDEX_BITSET(frame);
	uint32_t off = OFFSET_BITSET(frame);
	return (frames[idx] & (0x1 << off));
}

uint32_t first_frame()
{
	uint32_t i, j;
	for (i = 0; i < INDEX_BITSET(no_of_frames); i++)
		if (frames[i] != 0xFFFFFFFF) /* At least one bit is free */
			for (j = 0; j < 32; j++)
			{
				uint32_t test_bit = 0x1 << j;
				if (!(frames[i] & test_bit)) /* frame is free */
					return i * 8 * 4 + j;
			}

	return UINT32_MAX - 1; /* If no frame available */
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
	if (page->frame != 0)
		return; /* It's already allocated */
	else
	{
		uint32_t idx = first_frame();

		if (idx == (UINT32_MAX - 1))
			PANIC("No free frames!!");

		set_frame(idx * 0x1000);
		page->present = 1;
		page->rw = (is_writeable) ? 1 : 0;
		page->user = (is_kernel) ? 0 : 1;
		page->frame = idx;
	}
}

void free_frame(page_t *page)
{
	uint32_t frame = page->frame;

	if (!frame)
		return; /* Already free */

	clear_frame(frame);
	page->frame = 0x0;
}