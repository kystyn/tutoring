#pragma once

typedef struct block_t {
  int size;
  struct block_t* next;
  struct block_t* prev;
} block_t;

int meminit(void* pMemory, int size);
void memdone();
void* memalloc(int size);
void memfree(void* p);
int memgetminimumsize();
int memgetblocksize();
