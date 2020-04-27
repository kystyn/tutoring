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
            if (top.value.op < FIRST_UNAR) {
                Pop(num_stack, &numStackLength, sizeof(token_t), &operand1);

                switch (top.value.op)
                {
                case PLUS:
                    res.value.num = operand1.value.num + operand2.value.num;
                    break;
                case MINUS:
                    res.value.num = operand1.value.num - operand2.value.num;
                    break;
                case DIV:
                    res.value.num = operand1.value.num / operand2.value.num;
                    break;
                case MUL:
                    res.value.num = operand1.value.num * operand2.value.num;
                    break;
                case MOD:
                    // maybe only for int
                    res.value.num = fmod(operand1.value.num, operand2.value.num);
                    break;
                case POW:
                    res.value.num = pow(operand1.value.num, operand2.value.num);
                    break;
                }
            }
            else {
                switch (top.value.op)
                {
                case UNAR_MINUS:
                    res.value.num = -operand2.value.num;
                    break;
                case SIN:
                    res.value.num = sin(operand2.value.num);
                    break;
                case COS:
                    res.value.num = cos(operand2.value.num);
                    break;
                case TG:
                    res.value.num = tan(operand2.value.num);
                    break;
                case CTG:
                    res.value.num = 1.0 / tan(operand2.value.num);
                    break;
                case ASIN:
                    res.value.num = asin(operand2.value.num);
                    break;
                case ACOS:
                    res.value.num = acos(operand2.value.num);
                    break;
                case ATG:
                    res.value.num = atan(operand2.value.num);
                    break;
                case ACTG:
                    res.value.num = atan(1.0 / operand2.value.num);
                    //atg(y)==x <=> tg(x) == y <=>ctg(x) = 1/y <=> x = actg(1/y)
                    break;
                case LN:
                    res.value.num = log(operand2.value.num);
                    break;
                case SQRT:
                    res.value.num = sqrt(operand2.value.num);
                    break;
                case FLOOR:
                    res.value.num = floor(operand2.value.num);
                    break;
                case CEIL:
                    res.value.num = ceil(operand2.value.num);
                    break;
                }
            }

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

    Pop(num_stack, &numStackLength, sizeof(token_t), &top);
    free(num_stack);

    return top.value.num;
}
