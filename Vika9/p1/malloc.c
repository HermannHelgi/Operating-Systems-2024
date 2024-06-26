#include "malloc.h"

#include <stdio.h>
#include <assert.h>
#include <pthread.h> 
// add debug output if set to 1
#define DEBUG 0


/*
 * This is the heap you should use.
 * The heap does not grow.
 */
uint8_t __attribute__ ((aligned(HEADER_SIZE))) _heapData[HEAP_SIZE];

/*
 * This should point to the first free block in memory.
 */
Block *_firstFreeBlock;
pthread_mutex_t my_lock = PTHREAD_MUTEX_INITIALIZER;

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
	pthread_mutex_lock(&my_lock);
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
	pthread_mutex_unlock(&my_lock);
	pthread_mutex_lock(&my_lock);
	/* Part b: free blocks, using next pointer, and starting at _firstFreeBlock */
	printf("Free block list:\n");
	current = _firstFreeBlock;
	while (current) {
		printf("  Free block starting at %" PRIuPTR ", size %" PRIu64 "\n",
			((uintptr_t)(void*)current - (uintptr_t)(void*)&_heapData[0]),
			current->size);

		current = current->next;
	}
	pthread_mutex_unlock(&my_lock);
}

/*
 * Round the integer up to the block header size (16 Bytes).
 */
uint64_t roundUp(uint64_t n)
{
	return (n+0xF) & (~0xF);
}

/* Helper function that allocates a block (last two bullets of Assignment description)
 * takes as first argument the address of the next pointer that needs to be updated (!)
 */
static void *allocate_block(Block **update_next, Block *block, uint64_t new_size) {
	assert(block->size >= new_size);  // something is wrong if this is not the case

	if(block->size == new_size) {
		// exactly the right size
		// remove the free block from the free-list
		*update_next = block->next;

		// Mark block as allocated
		block->magic = ALLOCATED_BLOCK_MAGIC;

		// return address of the data part
		pthread_mutex_unlock(&my_lock);
		return &(block->data[0]);
	}

	// Create new free block

	// Size of the new free block is the size of the current free block minutes the space needed for the new allocation
	uint64_t new_free_size = block->size - new_size;

	// Prepare the new allocated block
	// Don't update magic yet (we still need the next pointer)
	// block->magic = ALLOCATED_BLOCK_MAGIC;
	block->size = new_size;

	// Prepare the new free block
	Block *newfree = _getNextBlockBySize( block );
	newfree->size = new_free_size;
	newfree->next = block->next;
	block->magic = ALLOCATED_BLOCK_MAGIC;

	// update the previous free pointer to the new free block
	*update_next = newfree;
	pthread_mutex_unlock(&my_lock);
#if DEBUG
	dumpAllocator();
#endif
	return &(block->data[0]);
}

void *my_malloc(uint64_t size)
{
	pthread_mutex_lock(&my_lock);

	/* round the requested size up to the next larger multiple of 16, and add header */
	size = roundUp(size) + HEADER_SIZE;
	/* Search the free list to find the first free block of memory that is large enough */
	Block *block = _firstFreeBlock;
	Block **prevptr = &_firstFreeBlock;
	while(block != NULL) {
		if(block->size >= size) { // large enough
			break;
		}
	prevptr = &(block->next);
		block = block->next;
	}
	/* free list is empty or there is no block that is large enough */
	if(block == NULL) {
		pthread_mutex_unlock(&my_lock);
		return NULL;
	}
	return allocate_block(prevptr, block, size); ;
}


/* Helper function to merge two freelist blocks.
 * Assume: block1 is at a lower address than block2
 * Does nothing if blocks are not neighbors (i.e. if block1 address + block1 size is not block2 address)
 * Otherwise, merges block by merging block2 into block1 (updates block1's size and next pointer
 */
void merge_blocks(Block *block1, Block *block2)
{
	if( (char *)block1 + block1->size != (char *)block2 ) {
		return;
	}
	assert(block1->next == block2); // This really should be the case :)
		

	block1->size += block2->size;
	block1->next = block2->next;
}


void my_free(void *address)
{
	pthread_mutex_lock(&my_lock);
	// If address is NULL, do nothing and just return
	if(address==NULL)
	{
		pthread_mutex_unlock(&my_lock);
		return;
	}
	// Derive the allocation block from the address
	Block *block = (Block *)( (char *)address - HEADER_SIZE );

	// Insert block into freelist
	Block *freeblock = _firstFreeBlock;
	if(block < freeblock) { // insert at beginning
		block->next = _firstFreeBlock;
		_firstFreeBlock = block;	
		merge_blocks(block, block->next);
	} else {
		// blocks that are before our new free block
		while(freeblock->next < block && freeblock->next != NULL) {
			freeblock = freeblock->next;
		}
		// This if is not really necessary....
		if(freeblock->next == NULL) {  // append at the end
			block->next = NULL;
			freeblock->next = block;
		} else {
			// freeblock is before block, freeblock->next is after block, so insert in the middle
			block->next = freeblock->next;
			freeblock->next = block;
		}
		// merge with next block if neighbours
		// (block->next can be null, but then merge_blocks simply does nothing)
		merge_blocks(block, block->next);
		// merge with next block if neighbours
		merge_blocks(freeblock, block);
	}
	pthread_mutex_unlock(&my_lock);
}


