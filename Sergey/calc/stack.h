#ifndef STACK_H
#define STACK_H

#include "error.h"

// stackLength -- elements count
// size - allocated bytes
ERR_STATUS Push( void **stack, int *size, int *stackLength, int sizeOfType, void *dataToPush );
ERR_STATUS Pop( void *stack, int *stackLength, int sizeOfType, void *dataToPop );
ERR_STATUS Top( void *stack, int stackLength, int sizeOfType, void *dataToPop );

#endif /* STACK_H*/
