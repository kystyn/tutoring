#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include "memallocator.h"

// pointer to the block list begin
block_t *memList;

int meminit(void *pMemory, int size)
{
    memList = pMemory;

    if (size < sizeof(block_t) || pMemory == NULL)
        return 0;

    memList->prev = NULL;
    memList->next = NULL;
    memList->size = size - sizeof(block_t);
    return 1;
}

void memdone()
{
    // ???
}

void * memalloc(int size)
{
    if (size <= 0)
        return NULL;

    block_t *block;
    block_t *best = NULL;
    int bestSize = INT_MAX;

    // find best block
    for (block = memList; block != NULL; block = block->next)
        if (block->size - size >= 0 && block->size - size < bestSize - size)
        {
            best = block;
            bestSize = best->size;
        }

    if (best == NULL)
        return NULL;

    // separate block
    block_t *returnToUser = best + 1;

    block_t *newBlock;
    newBlock = (char*)(best) + sizeof (block_t) + size;

    if (best->prev != NULL)
        best->prev->next = newBlock;
    newBlock->prev = best->prev;

    if (best->next != NULL)
        best->next->prev = newBlock;
    newBlock->next = best->next;

    newBlock->size = best->size - size - sizeof (block_t);

    return returnToUser;
}

void memfree(void *p)
{
    block_t *block;

    for (block = memList; (void*)block->next < p; block = block->next)
        ;
    block_t *oldNext = block->next;
    block_t *newBlock = (char *)p - sizeof (block_t);

    block->next = newBlock;
    newBlock->prev = block;

    newBlock->next = oldNext;
    oldNext->prev = newBlock;

    // NULL checks
    if (newBlock->prev + sizeof (block_t) + newBlock->prev->size == newBlock)
    {
        newBlock->prev->next = newBlock->next;
        newBlock->next->prev = newBlock->prev;
        newBlock->prev->size += sizeof (block_t) + newBlock->size;
        newBlock = newBlock->prev;
    }

    if (newBlock + sizeof(block_t) + newBlock->size == newBlock->next)
    {
        newBlock->next->next->prev = newBlock;
        newBlock->next = newBlock->next->next;
        newBlock->size += sizeof (block_t) + newBlock->next->size;
    }
}

int memgetminimumsize()
{
    return sizeof(block_t);
}

int memgetblocksize()
{
    return 0;
}
