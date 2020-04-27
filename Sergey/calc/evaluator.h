#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "token.h"

// tokens - polish notation
// length - tokens count
// size - byte count
double Eval(token_t *tokens, int tokenSize, int tokensLength, ERR_STATUS *status);

#endif /* EVALUATOR_H */
