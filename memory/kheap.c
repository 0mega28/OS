#include "kheap.h"

/* extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

End is defined in the linker script
Currently not using this as placement address

Why we are taking address of "end"
read it here https://wiki.osdev.org/Using_Linker_Script_Values
 */

uint32_t placement_address = 0x100000;

uint32_t kmalloc_int(uint32_t size, int align, uint32_t *phys)
{
	if (align == 1 && (placement_address & 0x00000FFF))
	{
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}
	if (phys)
	{
		*phys = placement_address;
	}
	uint32_t temp = placement_address;
	placement_address += size;
	return temp;
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