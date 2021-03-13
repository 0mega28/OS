/* 
	Ordered array data structure is used to maintain holes
	in heap.

	This file provide interface to ordered array and functions
	to create ordered array and insert and remove elements 
	from ordered array. 

	The array will be insertion sorted.
 */

#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

#include <stdint.h>
#include "../libc/function.h"
#include "../libc/mem.h"

/* Returns 1 if first argument is less than second argument
	else return 0
 */
typedef int8_t (*lessthan_assert_t)(void *, void *);

typedef struct
{
	void **array;
	uint32_t size;
	uint32_t max_size;
	lessthan_assert_t less_than;
} ordered_array_t;

/* A standard less than assert */
int8_t standard_lessthan_assert(void *a, void *b);

/* Create ordered array */
ordered_array_t create_ordered_array(uint32_t max_size, lessthan_assert_t less_than);

/* When we don't have kmalloc we place the ordered array (like create ordered array)*/
ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lessthan_assert_t lessthan);

/* Insert element into ordered array */
void insert_into_ordered_array(void *item, ordered_array_t *array);

/* Remove element from ordered array */
void remove_from_ordered_array(uint32_t index, ordered_array_t *array);

/* Lookup the item at index i */
void* lookup_ordered_array(uint32_t index, ordered_array_t *array);

/* Destroy ordered array */
void destroy_ordered_array(ordered_array_t *array);

#endif