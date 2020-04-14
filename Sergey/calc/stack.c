#include <string.h>
#include "utils.h"
#include "stack.h"

void Push(void **stack, int *size, int *stackLength, int sizeOfType, void *dataToPush) {
    *stack = Expand(*stack, size, *stackLength, sizeOfType);
    memcpy(((char *)(*stack)) + (*stackLength)++ * sizeOfType, dataToPush, sizeOfType);
}

ERR_STATUS Pop(void *stack, int *stackLength, int sizeOfType, void *dataToPop) {
    if (*stackLength == 0)
        return EMPTY;
    memmove(dataToPop, ((char *)stack) + ((*stackLength)-- - 1) * sizeOfType, sizeOfType);
    return OK;
}

ERR_STATUS Top(void *stack, int stackLength, int sizeOfType, void *dataToPop) {
    if (stackLength == 0)
        return EMPTY;
    memcpy(dataToPop, ((char *)stack) + (stackLength - 1) * sizeOfType, sizeOfType);
    return OK;
}
