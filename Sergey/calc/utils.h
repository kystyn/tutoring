#ifndef UTILS_H
#define UTILS_H

#include "error.h"

#define ARRAY_START_SIZE 100
#define ARRAY_INCR 100

void* Expand( void* str, int *size, int wish_idx, int sizeOfType );

void Reverse( void *arr, int elemCount, int sizeOfType );

ERR_STATUS Checking(char* str, int length);

#endif /* UTILS_H */
