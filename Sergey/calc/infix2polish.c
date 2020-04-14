#include <stdlib.h>
#include "stack.h"
#include "infix2polish.h"

void Infix2Polish(token_t *infixTokens, int *infixTokenLength, token_t **polishTokens,
                  int *polishTokenSize, int *polishTokenLength) {
    token_t *operStack = NULL;
    token_t *numStack = NULL;

    int
            numStackSize = 0,
            numStackLen = 0;
    int
            operStackSize = 0,
            operStackLen = 0;

    token_t cur;

    while (*infixTokenLength != 0) {
        Pop(infixTokens, infixTokenLength, sizeof(token_t), &cur);
        if (cur.type == NUMBER)
            Push(&numStack, &numStackSize, &numStackLen, sizeof(token_t), &cur);
        else {
            if (cur.value.op == '(') {
                Push(&operStack, &operStackSize, &operStackLen, sizeof(token_t), &cur);
                continue;
            }
            DropOperators(operStack, operStackSize, &operStackLen,
                          &numStack, &numStackSize, &numStackLen, GetPriority(cur));
            if (cur.value.op != ')')
                Push(&operStack, &operStackSize, &operStackLen, sizeof(token_t), &cur);
        }
    }

    DropOperators(operStack, operStackSize, &operStackLen,
                  &numStack, &numStackSize, &numStackLen, 0);

    *polishTokens = numStack;
    *polishTokenSize = numStackSize;
    *polishTokenLength = numStackLen;

    free(infixTokens);
    free(operStack);
    *infixTokenLength = 0;
}

/* 1 + 2 * 3 % 5 */
int GetPriority(token_t token) {
    // check table
    switch (token.value.op) {
    case '(':
    case ')':
        return 1;
    case '+':
    case '-':
        return 2;
    case '*':
    case '/':
        return 3;
    case '%':
        return 4;
    case '$':
        return 5;
    case '^':
        return 6;
    }
}

void DropOperators(token_t *operStack, int operStackSize, int *operStackLen,
                   token_t **numStack, int *numStackSize, int *numStackLen, int priority) {
    token_t dropable;

    while (Top(operStack, *operStackLen, sizeof(token_t), &dropable) == OK)
        if (GetPriority(dropable) >= priority) {
            Pop(operStack, operStackLen, sizeof(token_t), &dropable);
            if (dropable.value.op != '(')
                 Push(numStack, numStackSize, numStackLen, sizeof(token_t), &dropable);
        }
        else
            break;
}
