#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

#include "error.h"
#include "token.h"

ERR_STATUS Read(char** str, int* strLength, FILE* input);
ERR_STATUS ParseInput(char *str, int strLength, token_t **tokens, int *tokenLength);

#endif /* SCANNER_H */
