#ifndef INFIX2POLISH_H
#define INFIX2POLISH_H

#include "token.h"

void Infix2Polish(token_t *infixTokens, int *infixTokenLength,
                  token_t **polishTokens, int *polishTokenSize, int *polishTokenLength);

int GetPriority(token_t token);

void DropOperators(token_t *operStack, int *operStackLen,token_t **numStack,
                   int *numStackSize, int *numStackLen, int priority);

#endif /* INFIX2POLISH_H */
