#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "token.h"

/*
1 + 2 * 3 - 4 / 5
polish:
1 2 3 * + 4 5 / -
1 6 + 4 5 / -
*/
// tokens - polish notation
double eval(token_t *tokens, int tokensLength);

#endif /* EVALUATOR_H */
