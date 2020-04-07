#include <stdlib.h>
#include "utils.h"

// size - in bytes
// wish_idx - index of array (by type, not by bytes)
void* expand( void* str, int *size, int wish_idx, int sizeOfType ) {
    char *strcheck;
    // Malloc
    if (str == NULL) {
        *size = ARRAY_START_SIZE * sizeOfType;
        return malloc(ARRAY_START_SIZE * sizeOfType);
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
