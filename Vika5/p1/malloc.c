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
	return ((n + 15) & ~(15));
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
// static void * __attribute__ ((unused)) allocate_block(Block **update_next, Block *block, uint64_t new_size) {
// 	(void)update_next;
// 	(void)block;
// 	(void)new_size;
// 	// BTW, feel free to remove these lines starting (void)
// 	// Their purpose is just to avoid compiler warnings about unused variables
// 	//  as long as this function is unimplemented

// 	// TODO: Implement
// 	return NULL;
// }


void *my_malloc(uint64_t size)
{
	uint64_t totalSize = 16 + roundUp(size);
	Block *currentBlock = _firstFreeBlock;
	Block *previousBlock = NULL;
	while (currentBlock != NULL && currentBlock->size < totalSize )
	{
		previousBlock = currentBlock;
		currentBlock = currentBlock->next;
	}
	
	if (currentBlock != NULL)
	{
		if (currentBlock->size != totalSize)
		{
			if (previousBlock != NULL)
			{
				previousBlock->next = (Block*)((uintptr_t)currentBlock + totalSize);
				previousBlock->next->next = currentBlock->next;
				previousBlock->next->size = (currentBlock->size - totalSize);
			}
			else
			{
				_firstFreeBlock = (Block*)((uintptr_t)currentBlock + totalSize);
				_firstFreeBlock->next = currentBlock->next;
				_firstFreeBlock->size = (currentBlock->size - totalSize);
			}
		}
		else
		{
			if (previousBlock != NULL)
			{
				previousBlock->next = currentBlock->next;
			}
			else
			{
				_firstFreeBlock = currentBlock->next;
			}
		}
		currentBlock->magic = ALLOCATED_BLOCK_MAGIC;
		currentBlock->size = totalSize;
		return currentBlock->data;
	}

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
	// Only takes in free blocks.
	block1->size = block1->size + block2->size;
	block1->next = block2->next;
}


void my_free(void *address)
{
	if (address == NULL)
	{
		return;
	}

	Block *searchBlock = (Block*)&_heapData[0];
	Block *myBlock = (Block*)((uintptr_t)address - 16);
	Block *lastFreeBlock = NULL;

	while (searchBlock != myBlock && searchBlock < (Block*)&_heapData[HEAP_SIZE])
	{
		if (searchBlock->magic != ALLOCATED_BLOCK_MAGIC)
		{
			lastFreeBlock = searchBlock;
		}
		searchBlock = (Block*)(((uintptr_t)searchBlock) + searchBlock->size);
	} // Leaves as soon as currentblock == given address.

	if (searchBlock == myBlock)
	{
		if (lastFreeBlock != NULL) // Found a previous freeblock, it's next pointer has to be changed.
		{
			myBlock->next = lastFreeBlock->next;
			lastFreeBlock->next = myBlock;
		}
		else // No free block before the given address, _FF has to be updated.
		{
			myBlock->next = _firstFreeBlock;
			_firstFreeBlock = myBlock;
		}

		// // Merging Free Spaces
		// searchBlock = ((Block*)((uintptr_t)myBlock + myBlock->size));
		// if (searchBlock->magic != ALLOCATED_BLOCK_MAGIC) // Infront
		// {
		// 	merge_blocks(myBlock, searchBlock);
		// }
		// searchBlock = ((Block*)((uintptr_t)lastFreeBlock + lastFreeBlock->size));
		// if (searchBlock == myBlock) // Behind
		// {
		// 	merge_blocks(searchBlock, myBlock);
		// }
	}
	// If the program fails to find the address, it does nothing.
}


