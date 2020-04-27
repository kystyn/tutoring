#include <stdlib.h>
#include "stack.h"
#include "infix2polish.h"
#include "error.h"

// --2 -> 2 $ $

ERR_STATUS Infix2Polish(token_t *infixTokens, int *infixTokenLength, token_t **polishTokens,
                  int *polishTokenSize, int *polishTokenLength) {
  token_t *operStack = NULL;
  token_t *numStack = NULL;
  token_t cur;


  int numStackSize = 0,
      numStackLen = 0;

  int operStackSize = 0,
      operStackLen = 0;

  // if not ok not to forget about num (polish) stack and oper stack
  while (*infixTokenLength != 0) {
    Pop(infixTokens, infixTokenLength, sizeof(token_t), &cur);
    if (cur.type == NUMBER)
      Push(&numStack, &numStackSize, &numStackLen, sizeof(token_t), &cur);
    else {
      if (cur.value.op == LBRACE) {
        Push(&operStack, &operStackSize, &operStackLen, sizeof(token_t), &cur);
        continue;
      }
      else {
        DropOperators(operStack, &operStackLen, &numStack, &numStackSize, &numStackLen, GetPriority(cur));
        if (cur.value.op != RBRACE)
          Push(&operStack, &operStackSize, &operStackLen, sizeof(token_t), &cur);
      }
    }
  }

  DropOperators(operStack, &operStackLen, &numStack, &numStackSize, &numStackLen, 0);

  *polishTokens = numStack;
  *polishTokenSize = numStackSize;
  *polishTokenLength = numStackLen;

  free(infixTokens);
  free(operStack);
  *infixTokenLength = 0;
  return OK;
}

/* 1 + 2 * 3 % 5 */
//(1 + sin(2)) * 3
// sin ( 2 )
//1 2 sin + 3 *
int GetPriority(token_t token) {
  // check table
  switch (token.value.op) {
  case LBRACE:
  case RBRACE:
    return 1;
  case PLUS:
  case MINUS:
    return 2;
  case MUL:
  case DIV:
    return 3;
  case MOD:
    return 4;
  case SIN:
  case COS:
  case TG:
  case CTG:
  case ASIN:
  case ACOS:
  case ATG:
  case ACTG:
  case LN:
  case FLOOR:
  case CEIL:
  case SQRT:
  case UNAR_MINUS:
  case POW:
    return 5;    //If you will add new priority(correct Drop, ^case)
  default:
    return INCORRECT_OPERATION;
  }

}

void DropOperators(token_t* operStack, int* operStackLen, token_t** numStack, 
                    int* numStackSize, int* numStackLen, int priority) {
  token_t dropable;

  token_t power;
  power.type = OPERATOR;
  power.value.op = MUL;

  while (Top(operStack, *operStackLen, sizeof(token_t), &dropable) == OK) {
    if (GetPriority(dropable) >= priority &&
            priority != GetPriority(power)) {            // second expression for ^
      Pop(operStack, operStackLen, sizeof(token_t), &dropable);
      if (dropable.value.op != LBRACE)
        Push(numStack, numStackSize, numStackLen, sizeof(token_t), &dropable);
      else
        break;
    }
    else
      break;
  }
}
