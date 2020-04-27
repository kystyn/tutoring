#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// size - in bytes
// wish_idx - index of array (by type, not by bytes)
void* Expand( void* str, int *size, int wish_idx, int sizeOfType ) {
    char *strcheck;
    // Malloc
    if (str == NULL) {
        *size = ARRAY_START_SIZE * sizeOfType;
        return realloc(str, *size);
    }
    // Realloc case
    if (wish_idx * sizeOfType >= *size) {
      *size = (wish_idx + ARRAY_INCR) * sizeOfType; /* can do smth like size *= 1.3 */
      strcheck = realloc(str, *size);
      if (strcheck == NULL) {
        free(str);
        return NULL;
      }
      return strcheck;
    }
    return str;
}

void Reverse(void *arr, int elemCount, int sizeOfType)
{
    int idx;
    char *tmp = malloc(sizeOfType);
    if (tmp == NULL)
    {
        printf("ERROR: no mem");
        return;
    }

    for (idx = 0; idx < elemCount / 2; idx++) {
        // t = a, a = b, b = t
        memcpy(tmp, (char*)arr + sizeOfType * idx, sizeOfType);
        memcpy((char*)arr + sizeOfType * idx,
               (char*)arr + sizeOfType * (elemCount - 1 - idx), sizeOfType);
        memcpy((char*)arr + sizeOfType * (elemCount - 1 - idx), tmp, sizeOfType);
    }

    free(tmp);
}
