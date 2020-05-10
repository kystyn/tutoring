#include <string.h>
#include "utils.h"
#include "stack.h"

ERR_STATUS Push(void **stack, int *size, int *stackLength, int sizeOfType, void *dataToPush) {
    *stack = Expand(*stack, size, *stackLength, sizeOfType);
    if (*stack == NULL)
        return NO_MEM;
    memcpy(((char *)(*stack)) + (*stackLength)++ * sizeOfType, dataToPush, sizeOfType);
}

ERR_STATUS Pop(void *stack, int *stackLength, int sizeOfType, void *dataToPop) {
    if (stack == NULL) // shouldn't happen
        return NO_MEM;
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
