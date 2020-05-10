#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "error.h"

// size - in bytes
// wish_idx - index of array (by type, not by bytes)
void* Expand(void* str, int* size, int wish_idx, int sizeOfType) {
  char* strcheck;
  // Malloc
  if (str == NULL) {
    *size = ARRAY_START_SIZE * sizeOfType;
    return malloc(*size);
  }
  // Realloc case
  if (wish_idx * sizeOfType >= *    size) {
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

ERR_STATUS Reverse(void* arr, int elemCount, int sizeOfType) {
  int idx;
  char* tmp = malloc(sizeOfType);

  if (tmp == NULL)
    return NO_MEM;

  for (idx = 0; idx < elemCount / 2; idx++) {
    // t = a, a = b, b = t
    memcpy(tmp, (char*)arr + sizeOfType * idx, sizeOfType);
    memcpy((char*)arr + sizeOfType * idx, (char*)arr + sizeOfType * (elemCount - 1 - idx), sizeOfType);
    memcpy((char*)arr + sizeOfType * (elemCount - 1 - idx), tmp, sizeOfType);
  }

  free(tmp);

  return OK;
}

ERR_STATUS Checking(char* str, int length) {
  /*There i tried to implement some sequence
  of checks to find comments and empty strings*/
  for (int i = 0; i < length; i++) {
    //str[i] is not a space 
    if (isspace(str[i]) == 0) {
      if (str[i] == '/') {
        if (str[i + 1] == '/')
          return COMMENT;   // case comment
        return OK;
      }
      if (str[i] == '\0')
        return COMMENT;  // case empty string
      return OK;
    }
  }
  return COMMENT; //I suppose this return will never work but compiler want it 
}
