#include "kheap.h"

/* extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

End is defined in the linker script
Currently not using this as placement address

Why we are taking address of "end"
read it here https://wiki.osdev.org/Using_Linker_Script_Values
 */

uint32_t placement_address = 0x100000;
heap_t *kheap = 0;
extern page_directory_t *kernel_directory;

/* Internal function declaration */

/* Alloc memory on heap of size "size" and it should be page aligned if page_allign is 1*/
void *alloc(uint32_t size, uint8_t align, heap_t *heap);

/* Free memory on heap from address addr*/
void free(void *addr, heap_t *heap);

/* Return index of smallest hole from ordered_array */
/* The argument size = size of block + size of footer + size of header */
uint32_t find_smallest_hole(uint32_t size, uint8_t page_allign, heap_t *heap);

/* Expand heap to new_size */
void expand(uint32_t new_size, heap_t *heap);

/* Contract heap to new_size and return new_size of heap*/
uint32_t contract(uint32_t new_size, heap_t *heap);

void modify_index_array_after_heap_expantion(uint32_t old_size, heap_t *heap);

uint32_t kmalloc_int(uint32_t size, int align, uint32_t *phys)
{
	if (kheap != 0)
	{
		void *addr = alloc(size, align, kheap);
		if (phys != 0)
		{
			page_t *page = get_page((uint32_t)addr, 0, kernel_directory);
			*phys = (page->frame * 0x1000) + ((uint32_t)addr & 0xFFF);
		}
		return (uint32_t)addr;
	}
	else
	{
		if (align == 1 && (placement_address & 0x00000FFF))
		{
			page_align(&placement_address);
		}
		if (phys)
		{
			*phys = placement_address;
		}
		uint32_t temp = placement_address;
		placement_address += size;
		return temp;
	}
}

uint32_t kmalloc_a(uint32_t size)
{
	return kmalloc_int(size, 1, 0);
}

uint32_t kmalloc_p(uint32_t size, uint32_t *phys)
{
	return kmalloc_int(size, 0, phys);
}

uint32_t kmalloc_ap(uint32_t size, uint32_t *phys)
{
	return kmalloc_int(size, 1, phys);
}

uint32_t kmalloc(uint32_t size)
{
	return kmalloc_int(size, 0, 0);
}

void kfree(void *addr)
{
	free(addr, kheap);
}

/* Function definition */

int8_t size_less_than_assert(void *a, void *b)
{
	return (((header_t *)a)->size < ((header_t *)b)->size) ? 1 : 0;
}

heap_t *create_heap(uint32_t start_address, uint32_t end_address, uint32_t max_address, uint8_t is_supervisor, uint8_t is_readonly)
{
	heap_t *heap = (heap_t *)kmalloc(sizeof(heap_t));

	/* Some assertion (Every thing should be page aligned) */
	ASSERT(start_address % 0x1000 == 0);
	ASSERT(end_address % 0x1000 == 0);
	ASSERT(max_address % 0x1000 == 0);

	/* Initialise index array */
	heap->index_array = place_ordered_array((void *)start_address, KHEAP_ARRAY_INDEX_SIZE, size_less_than_assert);

	/* Shift start address to after the index array (behaves like kmalloc) */
	start_address += sizeof(void *) * KHEAP_ARRAY_INDEX_SIZE;

	/* Make the start_address page aligned */
	page_align(&start_address);

	/* Initialise heap attributes */
	heap->start_address = start_address;
	heap->end_address = end_address;
	heap->max_address = max_address;
	heap->is_readonly = (is_readonly) ? 1 : 0;
	heap->is_supervisor = (is_supervisor) ? 1 : 0;

	/* Place a header in index array */
	header_t *hole = (header_t *)start_address;
	hole->magic_number = KHEAP_MAGIC;
	hole->is_hole = 1;
	hole->size = end_address - start_address;
	insert_into_ordered_array((void *)hole, &heap->index_array);

	return heap;
}

void *alloc(uint32_t size, uint8_t align, heap_t *heap)
{
	/* Include size of header and footer */
	uint32_t required_size = size + sizeof(header_t) + sizeof(footer_t);

	/* Find smallest that will fit */
	int32_t iterator = find_smallest_hole(required_size, align, kheap);

	/* If we didn't find any hole, then expand the heap and check again */
	if (iterator == -1)
	{
		uint32_t old_size = heap->end_address - heap->start_address;
		expand(old_size + required_size, kheap);

		return alloc(size, align, kheap);
	}

	header_t *orig_hole_header = (header_t *)lookup_ordered_array(iterator, &heap->index_array);
	uint32_t orig_hole_pos = (uint32_t)orig_hole_header;
	uint32_t orig_hole_size = orig_hole_header->size;
	/* Here we work out if we should split the hole we found into two parts.
	Is the original hole size - requested hole size less than the overhead for adding a new hole? */
	if (orig_hole_size - required_size < sizeof(header_t) + sizeof(footer_t))
	{
		/* Then just increase the requested size to the size of the hole we found. */
		size += orig_hole_size - required_size;
		/* size = orig_hole_size - sizeof(H) - sizeof(F) */
		required_size = orig_hole_size;
	}

	/* 	If we need to page-align the data, do it now and make a new hole in front of our block.*/
	if (align && (orig_hole_pos & 0x00000FFF))
	{
		uint32_t new_location = orig_hole_pos + 0x1000 /* page size */ - (orig_hole_pos & 0xFFF) - sizeof(header_t);
		header_t *hole_header = (header_t *)orig_hole_pos;
		hole_header->size = 0x1000 /* page size */ - (orig_hole_pos & 0xFFF) - sizeof(header_t);
		hole_header->magic_number = KHEAP_MAGIC;
		hole_header->is_hole = 1;
		footer_t *hole_footer = (footer_t *)((uint32_t)new_location - sizeof(footer_t));
		hole_footer->magic_number = KHEAP_MAGIC;
		hole_footer->header = hole_header;
		orig_hole_pos = new_location;
		orig_hole_size = orig_hole_size - hole_header->size;
	}
	else
	{
		/* Else we don't need this hole any more, delete it from the index. */
		remove_from_ordered_array(iterator, &heap->index_array);
	}

	/* Overwrite the original header... */
	header_t *block_header = (header_t *)orig_hole_pos;
	block_header->magic_number = KHEAP_MAGIC;
	block_header->is_hole = 0;
	block_header->size = required_size;
	/* ...And the footer */
	footer_t *block_footer = (footer_t *)((uint32_t)block_header + block_header->size - sizeof(footer_t));
	block_footer->magic_number = KHEAP_MAGIC;
	block_footer->header = block_header;

	/* We may need to write a new hole after the allocated block.*/
	if (orig_hole_size - required_size > 0)
	{
		header_t *hole_header = (header_t *)(orig_hole_pos + required_size);
		hole_header->magic_number = KHEAP_MAGIC;
		hole_header->is_hole = 1;
		hole_header->size = orig_hole_size - required_size;
		footer_t *hole_footer = (footer_t *)((uint32_t)hole_header + hole_header->size - sizeof(footer_t));
		if (((uint32_t)hole_footer + sizeof(footer_t)) <= heap->end_address)
		{
			hole_footer->magic_number = KHEAP_MAGIC;
			hole_footer->header = hole_header;
		}
		/* Put the new hole in the index_array; */
		insert_into_ordered_array((void *)hole_header, &heap->index_array);
	}

	// ...And we're done!
	return (void *)((uint32_t)block_header + sizeof(header_t));
}

void free(void *addr, heap_t *heap)
{
	/* Exit for null pointers */
	if (addr == 0)
		return;

	/* Get header and footer */
	header_t *header = (header_t *)((uint32_t)addr - sizeof(header_t));
	footer_t *footer = (footer_t *)((uint32_t)header + header->size - sizeof(footer));

	/* Assert by checking magic number */
	ASSERT(header->magic_number == KHEAP_MAGIC);
	// ASSERT(footer->magic_number == KHEAP_MAGIC);

	/* Mark it as free */
	header->is_hole = 1;

	/* do_add to check if we have done unify left */
	uint8_t do_add = 1;

	/* Unify left */
	header_t *left_header = ((footer_t *)((uint32_t)(header) - sizeof(footer_t)))->header;

	/* If it is a hole then unify left */
	if ((left_header->magic_number == KHEAP_MAGIC) && left_header->is_hole)
	{
		left_header->size += header->size;
		footer->header = left_header;
		do_add = 0;
		header = left_header;
	}

	/* Unify Right */
	header_t *right_header = (header_t *)((uint32_t)header + header->size);
	footer_t *right_footer = (footer_t *)((uint32_t)right_header + right_header->size - sizeof(footer_t));

	/* If it is a hole then unify right */
	if ((right_header->magic_number == KHEAP_MAGIC) && right_header->is_hole)
	{
		header->size += right_header->size;
		right_footer->header = header;
		footer = right_footer;

		/* Find right header and delete it */
		uint32_t iterator = 0;
		while ((iterator < heap->index_array.size) && (lookup_ordered_array(iterator, &heap->index_array) != (void *)right_header))
			iterator++;

		ASSERT(iterator < heap->index_array.size);
		remove_from_ordered_array(iterator, &heap->index_array);
	}

	/* Check if we can contract the heap */
	if ((uint32_t)footer + sizeof(footer_t) == heap->end_address)
	{
		uint32_t old_size = heap->end_address - heap->start_address;
		uint32_t new_size = (uint32_t)header - heap->start_address;

		new_size = contract(new_size, heap);

		if (header->size > (old_size - new_size))
		{
			header->size -= (old_size - new_size);

			footer = (footer_t *)((uint32_t)header + header->size - sizeof(footer_t));
			footer->header = header;
			footer->magic_number = KHEAP_MAGIC;
		}
		else
		{
			/* We will no longer exist, Remove us from the index. */
			uint32_t iterator = 0;
			while ((iterator < heap->index_array.size) &&
				   (lookup_ordered_array(iterator, &heap->index_array) != (void *)header))
				iterator++;

			/* If we didn't find ourselves, we have nothing to remove. */
			if (iterator < heap->index_array.size)
				remove_from_ordered_array(iterator, &heap->index_array);
		}
	}

	if (do_add == 1)
		insert_into_ordered_array((void *)header, &heap->index_array);
}

uint32_t find_smallest_hole(uint32_t size, uint8_t page_allign, heap_t *heap)
{
	uint32_t index = 0;

	while (index < heap->index_array.size)
	{
		header_t *header = (header_t *)lookup_ordered_array(index, &heap->index_array);

		uint32_t hole_size = header->size;

		/* If we want page aligned location */
		if (page_allign)
		{
			uint32_t location_of_header = (uint32_t)header;
			uint32_t new_size;

			/* We want the block address to be page aligned */
			if ((location_of_header + sizeof(header_t)) & 0x00000FFF)
			{
				uint32_t offset = 0x1000 - (location_of_header + sizeof(header_t)) % 0x1000;
				new_size = hole_size - offset;
			}

			if (new_size >= size)
				break;
		}
		else
		{
			if (hole_size >= size)
				break;
		}

		index++;
	}

	if (index == heap->index_array.size)
		return -1;

	return index;
}

void expand(uint32_t new_size, heap_t *heap)
{
	/* Check if new size is greater than old size */
	uint32_t old_size = heap->end_address - heap->start_address;

	ASSERT(new_size > old_size);

	/* Page align the new size */
	page_align(&new_size);

	ASSERT(new_size + heap->start_address <= heap->max_address);

	uint32_t i = old_size;

	while (i < new_size)
	{
		alloc_frame(get_page(i + heap->start_address, 1, kernel_directory), heap->is_supervisor, !heap->is_readonly);
		i += 0x1000;
	}

	heap->end_address = heap->start_address + new_size;

	modify_index_array_after_heap_expantion(old_size, heap);
}

uint32_t contract(uint32_t new_size, heap_t *heap)
{
	uint32_t old_size = heap->end_address - heap->start_address;
	ASSERT(new_size < old_size);

	page_align(&new_size);

	if (new_size < KHEAP_MINIMUM_SIZE)
		new_size = KHEAP_MINIMUM_SIZE;

	/* Free the pages */
	uint32_t i = new_size;
	while (i < old_size)
	{
		free_frame(get_page(heap->start_address + i, 0, kernel_directory));
		i += 0x1000;
	}

	heap->end_address = heap->start_address + new_size;

	return new_size;
}

void modify_index_array_after_heap_expantion(uint32_t old_size, heap_t *heap)
{
	int32_t index = -1;
	uint32_t address = 0;
	int32_t iterator = 0;
	/* Find the last header in terms of address */
	while (iterator < (int32_t)heap->index_array.size)
	{
		uint32_t temp_address = (uint32_t)lookup_ordered_array(iterator, &heap->index_array);

		if (temp_address > address)
		{
			index = iterator;
			address = temp_address;
		}
		iterator++;
	}

	uint32_t new_size = heap->end_address - heap->start_address;

	/* We check if found a header, it should be just adjacent to the new heap size we allocated */
	if (index != -1 && ((address + ((header_t *)address)->size) != heap->start_address + old_size))
	{
		((header_t *)address)->size += new_size - old_size;

		footer_t *hole_footer = (footer_t *)(heap->end_address - sizeof(footer_t));
		hole_footer->header = (header_t *)address;
		hole_footer->magic_number = KHEAP_MAGIC;
	}
	else
	{
		/* We didn't find any hole 
		or the header we found is not adjacent to this hole*/

		header_t *hole_header = (header_t *)(heap->start_address + old_size);
		hole_header->magic_number = KHEAP_MAGIC;
		hole_header->size = new_size - old_size;
		hole_header->is_hole = 1;

		footer_t *hole_footer = (footer_t *)(heap->end_address - sizeof(footer_t));
		hole_footer->header = hole_header;
		hole_footer->magic_number = KHEAP_MAGIC;

		insert_into_ordered_array((void *)hole_header, &heap->index_array);
	}
}