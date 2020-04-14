#ifndef UTILS_H
#define UTILS_H

#define ARRAY_START_SIZE 1
#define ARRAY_INCR 1

void* Expand( void* str, int *size, int wish_idx, int sizeOfType );

void Reverse( void *arr, int elemCount, int sizeOfType );

#endif /* UTILS_H */
