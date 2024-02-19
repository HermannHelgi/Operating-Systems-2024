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
	return ((n + 15) & ~(15)); // Some shifting
}

/*
 * Removed internal helper function due to compiler errors :S
 */


void *my_malloc(uint64_t size)
{
	uint64_t totalSize = 16 + roundUp(size);
	Block *currentBlock = _firstFreeBlock;
	Block *previousBlock = NULL;
	while (currentBlock != NULL && currentBlock->size < totalSize ) // Searching for large enough block
	{
		previousBlock = currentBlock;
		currentBlock = currentBlock->next;
	}
	
	if (currentBlock != NULL) // Check to make sure a block was found
	{
		if (currentBlock->size != totalSize) // Check if i need to cut a block into two
		{
			if (previousBlock != NULL) // Checking if the first block check was the one used, if so _FF needs to be updated
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
		else // Block is perfect fit
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
		currentBlock->magic = ALLOCATED_BLOCK_MAGIC; // Setting values
		currentBlock->size = totalSize;
		return currentBlock->data;
	}
	
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
	// Setting base search variables
	Block *searchBlock = (Block*)&_heapData[0];
	Block *myBlock = (Block*)((uintptr_t)address - 16);
	Block *lastFreeBlock = NULL;

	while (searchBlock != myBlock && searchBlock < (Block*)&_heapData[HEAP_SIZE]) // Finding previous freeblock
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

		// Merging Free Spaces
		searchBlock = (Block*)(((uintptr_t)myBlock) + myBlock->size);
		if (searchBlock->magic != ALLOCATED_BLOCK_MAGIC) // Infront
		{
			merge_blocks(myBlock, searchBlock);
		}
		if (lastFreeBlock != NULL)
		{
			searchBlock = (Block*)(((uintptr_t)lastFreeBlock) + lastFreeBlock->size);
			if (searchBlock == myBlock) // Behind
			{
				merge_blocks(lastFreeBlock, myBlock);
			}
		}
	}
	// If the program fails to find the address, it does nothing.
}


