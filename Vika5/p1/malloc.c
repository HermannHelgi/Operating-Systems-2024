#include "malloc.h"

#include <stdio.h>
#include <assert.h>

/*
 * This is the heap you should use.
 * The heap does not grow.
 */
uint8_t __attribute__ ((aligned(HEADER_SIZE))) _heapData[HEAP_SIZE];

/*
 * This should point to the first free block in memory.
 */
Block *_firstFreeBlock;

/*
 * Initializes the memory block. You don't need to change this.
 */
void initAllocator()
{
	_firstFreeBlock = (Block*)&_heapData[0];
	_firstFreeBlock->next = NULL;
	_firstFreeBlock->size = HEAP_SIZE;
}

/*
 * Gets the next block that should start after the current one.
 */
static Block *_getNextBlockBySize(const Block *current)
{
	static const Block *end = (Block*)&_heapData[HEAP_SIZE];
	Block *next = (Block*)&current->data[current->size - HEADER_SIZE];

	assert(next <= end);
	return (next == end) ? NULL : next;
}

/*
 * Dumps the allocator. You should not need to modify this.
 */
void dumpAllocator()
{
	Block *current;
	/* Note: This sample code prints addresses relative to the beginning of the heap */

	/* Part a: all blocks, using size, and starting at the beginning of the heap */
	printf("All blocks:\n");
	current = (Block*)&_heapData[0];
	while (current) {
		printf("  Block starting at %" PRIuPTR ", size %" PRIu64 " (%s)\n",
			((uintptr_t)(void*)current - (uintptr_t)(void*)&_heapData[0]),
			current->size,
			current->magic == ALLOCATED_BLOCK_MAGIC ? "in use" : "free" );

		current = _getNextBlockBySize(current);
	}

	/* Part b: free blocks, using next pointer, and starting at _firstFreeBlock */
	printf("Free block list:\n");
	current = _firstFreeBlock;
	while (current) {
		printf("  Free block starting at %" PRIuPTR ", size %" PRIu64 "\n",
			((uintptr_t)(void*)current - (uintptr_t)(void*)&_heapData[0]),
			current->size);

		current = current->next;
	}
}

/*
 * Round the integer up to the block header size (16 Bytes).
 */
uint64_t roundUp(uint64_t n)
{
	(void) n;

	//TODO: Implement
	return 0;
}

/*
 * Internal helper function for making a new allocation
 * (not needed for any tests, but a suggested structure to simplify your life)
 * This function should basically do the last two items of the assignment description
 * of part b (my_malloc).
 * - block is the (currently free) block that you want to use for your new allocation
 * - new_size is the total size for the new allocation (size requested in the
 *   my_malloc call plus header size)
 * - update_next is a pointer to the next pointer pointing to block. This is
 *   the pointer you need to update for removing block from the free-list/
 *   replace block with a new free block starting somewhere within block
 */
static void * __attribute__ ((unused)) allocate_block(Block **update_next, Block *block, uint64_t new_size) {
	(void)update_next;
	(void)block;
	(void)new_size;
	// BTW, feel free to remove these lines starting (void)
	// Their purpose is just to avoid compiler warnings about unused variables
	//  as long as this function is unimplemented

	// TODO: Implement
	return NULL;
}


void *my_malloc(uint64_t size)
{
	(void) size;

	// TODO: Implement
	// Suggested approach: Search for a free block, then call allocate_block with that block
 	// (and suitable values for update_next and new_size)
	// This is not mandatory, what counts in the and is that my_malloc does the right thing.
	return NULL;
}

/*
 * Internal helper function to merge two blocks.
 * (not needed for any tests, but a suggested structure to simplify your life)
 * Suggested functionality (to be used in my_free):
 * if block2 follows directly after block1, then merge these two (free) blocks
 * i.e. update block1 such that it becomes one larger free block
 */
static void __attribute__ ((unused)) merge_blocks(Block *block1, Block *block2)
{
	(void)block1;
	(void)block2;
	// TODO: Implement
}


void my_free(void *address)
{
	(void) address;

	// TODO: Implement
}


