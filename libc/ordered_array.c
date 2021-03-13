#include "ordered_array.h"
#include "../memory/kheap.h"

int8_t standard_lessthan_assert(void *a, void *b)
{
	return (a < b) ? 1 : 0;
}

ordered_array_t create_ordered_array(uint32_t max_size, lessthan_assert_t less_than)
{
	ordered_array_t ordered_array;
	ordered_array.array = (void **)kmalloc(max_size * sizeof(void *));
	memory_set((uint8_t *)ordered_array.array, 0, max_size * sizeof(void *));
	ordered_array.size = 0;
	ordered_array.max_size = max_size;
	ordered_array.less_than = less_than;

	return ordered_array;
}

ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lessthan_assert_t lessthan)
{
	ordered_array_t ordered_array;
	ordered_array.array = (void **)addr;
	memory_set((uint8_t *)ordered_array.array, 0, max_size * sizeof(void *));
	ordered_array.size = 0;
	ordered_array.max_size = max_size;
	ordered_array.less_than = lessthan;

	return ordered_array;
}

void insert_into_ordered_array(void *item, ordered_array_t *array)
{
	ASSERT(array->size <= array->max_size);

	uint32_t i = 0;
	while (i < array->max_size && array->less_than(array->array[i], item))
		i++;

	while (i < array->size)
	{
		void *temp = array->array[i];
		array->array[i++] = item;
		item = temp;
	}
	array->array[i] = item;
	array->size++;
}

void remove_from_ordered_array(uint32_t index, ordered_array_t *array)
{
	ASSERT(index < array->size);

	while (index < array->size)
	{
		array->array[index] = array->array[index+1];
		index++;
	}
	array->size--;
}

void *lookup_ordered_array(uint32_t index, ordered_array_t *array)
{
	ASSERT(index < array->size);
	return array->array[index];
}

void destroy_ordered_array(ordered_array_t *array)
{
	kfree((void *)array->array);
}
