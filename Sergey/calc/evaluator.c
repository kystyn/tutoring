#include <stdlib.h>
#include <math.h>
#include "evaluator.h"
#include "stack.h"

/*
1 + 2 * 3 - 4 / 5
polish:
1 2 3 * + 4 5 / -
* + 4 5 / -
1 6
1 6 + 4 5 / -

(1 * -2) / 10
1 2 $ * 10 /
*/
// top - left, bottom - right
double Eval(token_t *tokens, int tokenSize, int tokensLength) {
    token_t top;
    token_t *num_stack = NULL;
    int numStackSize = 0, numStackLength = 0;
    while (Top(tokens, tokensLength, sizeof(token_t), &top) != EMPTY) {
        Pop(tokens, &tokensLength, sizeof(token_t), &top);
        if (top.type == OPERATOR) {
            token_t
                    operand1,
                    operand2;
            token_t res;
            token_t tmpToTrans;
            res.type = NUMBER;

            Pop(num_stack, &numStackLength, sizeof(token_t), &operand2);
            if (top.value.op != '$') {
                Pop(num_stack, &numStackLength, sizeof(token_t), &operand1);

                switch (top.value.op)
                {
                case '+':
                    res.value.num = operand1.value.num + operand2.value.num;
                    break;
                case '-':
                    res.value.num = operand1.value.num - operand2.value.num;
                    break;
                case '/':
                    res.value.num = operand1.value.num / operand2.value.num;
                    break;
                case '*':
                    res.value.num = operand1.value.num * operand2.value.num;
                    break;
                case '%':
                    // maybe only for int
                    res.value.num = fmod(operand1.value.num, operand2.value.num);
                    break;
                case '^':
                    res.value.num = pow(operand1.value.num, operand2.value.num);
                    break;
                }
            }
            else
                res.value.num = -operand2.value.num;

            Push(&tokens, &tokenSize, &tokensLength, sizeof(token_t), &res);
            while (numStackLength != 0){
                Pop(num_stack, &numStackLength, sizeof(token_t), &tmpToTrans);
                Push(&tokens, &tokenSize, &tokensLength, sizeof(token_t), &tmpToTrans);
            }
            free(num_stack);
            num_stack = NULL;
        }
        else
            Push(&num_stack, &numStackSize, &numStackLength, sizeof(token_t), &top);
    }

    free(tokens);

    return top.value.num;
}
