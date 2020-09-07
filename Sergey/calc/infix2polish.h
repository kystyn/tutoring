#ifndef INFIX2POLISH_H
#define INFIX2POLISH_H

#include "token.h"
#include "error.h"

ERR_STATUS Infix2Polish(token_t *infixTokens, int *infixTokenLength,
                  token_t **polishTokens, int *polishTokenSize, int *polishTokenLength);

int GetPriority(token_t token);

ERR_STATUS DropOperators(token_t *operStack, int *operStackLen,token_t **numStack,
                   int *numStackSize, int *numStackLen, int priority, int *meetLBrace);

#endif /* INFIX2POLISH_H */
