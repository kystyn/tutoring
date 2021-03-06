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
  cur.type = NUMBER;
  cur.value.num = 1;   //initialized because of warning

  enum {
      WAIT_PREFIX,
      WAIT_SUFFIX,
      ERROR,
      DONE
  } state = WAIT_PREFIX;

  ERR_STATUS status = OK;

  int numStackSize = 0,
      numStackLen = 0;

  int operStackSize = 0,
      operStackLen = 0;

  int meetLBrace;

  // if not ok not to forget about num (polish) stack and oper stack
  while (state != DONE) {
    if (state != ERROR) {
      if (*infixTokenLength != 0)
        Pop(infixTokens, infixTokenLength, sizeof(token_t), &cur);
      else
        state = DONE;
    }

    switch (state) {
    case WAIT_PREFIX:
      if (cur.type == NUMBER) {
        if (Push(&numStack, &numStackSize, &numStackLen, sizeof(token_t), &cur) == NO_MEM)
        {
            if (numStack != NULL)
                free(numStack);

            if (operStack != NULL)
                free(operStack);

            if (infixTokens != NULL)
                free(infixTokens);

            return NO_MEM;
        }
        state = WAIT_SUFFIX;
      }
      else if (cur.type == OPERATOR && cur.value.op == LBRACE) {
        if (Push(&operStack, &operStackSize, &operStackLen, sizeof(token_t), &cur) == NO_MEM)
        {
            if (numStack != NULL)
                free(numStack);

            if (operStack != NULL)
                free(operStack);

            if (infixTokens != NULL)
                free(infixTokens);

            return NO_MEM;
        }
        state = WAIT_PREFIX;
      }
      else if (cur.type == OPERATOR && cur.value.op > FIRST_UNAR) {
        // need drop??
        if (DropOperators(
                    operStack, &operStackLen, &numStack, &numStackSize,
                    &numStackLen, GetPriority(cur), &meetLBrace) == NO_MEM)
        {
            if (numStack != NULL)
                free(numStack);

            if (operStack != NULL)
                free(operStack);

            if (infixTokens != NULL)
                free(infixTokens);

            return NO_MEM;
        }

        if (Push(&operStack, &operStackSize, &operStackLen, sizeof(token_t), &cur) == NO_MEM)
        {
            if (numStack != NULL)
                free(numStack);

            if (operStack != NULL)
                free(operStack);

            if (infixTokens != NULL)
                free(infixTokens);

            return NO_MEM;
        }
        state = WAIT_PREFIX;
      }
      else {
        state = ERROR;
        status = I2P_NO_PREFIX;
      }
      break;
    case WAIT_SUFFIX:
      if (cur.type == OPERATOR && cur.value.op < FIRST_UNAR) {
        if (DropOperators(operStack, &operStackLen, &numStack, &numStackSize, &numStackLen, GetPriority(cur), &meetLBrace) == NO_MEM)
        {
            if (numStack != NULL)
                free(numStack);

            if (operStack != NULL)
                free(operStack);

            if (infixTokens != NULL)
                free(infixTokens);

            return NO_MEM;
        }
        if (cur.value.op != RBRACE) {
          if (Push(&operStack, &operStackSize, &operStackLen, sizeof(token_t), &cur) == NO_MEM)
          {
              if (numStack != NULL)
                  free(numStack);

              if (operStack != NULL)
                  free(operStack);

              if (infixTokens != NULL)
                  free(infixTokens);

              return NO_MEM;
          }
          state = WAIT_PREFIX;
        }
        else {
          if (!meetLBrace) {
            state = ERROR;
            status = I2P_NO_SUFFIX;
          }
          else
            state = WAIT_SUFFIX;
        }
      }
      else {
        state = ERROR;
        status = I2P_NO_SUFFIX;
      }
      break;
    case ERROR:
      free(numStack);

      // kostyl' (actually shouldn't be called
      if (status == OK)
          status = I2P_NO_SUFFIX;

      numStack = NULL;
      *polishTokens = NULL;
      *polishTokenSize = 0;
      *polishTokenLength = 0;
      state = DONE;
      break;
    case DONE:
      cur.type = OPERATOR;
      cur.value.op = RBRACE;
      if (DropOperators(operStack, &operStackLen, &numStack, &numStackSize, &numStackLen, GetPriority(cur), &meetLBrace) == NO_MEM)
      {
          if (numStack != NULL)
              free(numStack);

          if (operStack != NULL)
              free(operStack);

          if (infixTokens != NULL)
              free(infixTokens);

          return NO_MEM;
      }
      if (operStackLen != 0 || meetLBrace == 1) {
        state = ERROR;
        status = I2P_PROBLEM_BRACKETS;
      }
      break;
    }
  }

  *polishTokens = numStack;
  *polishTokenSize = numStackSize;
  *polishTokenLength = numStackLen;

  free(infixTokens);
  free(operStack);
  *infixTokenLength = 0;

  return status;
}

/* 1 + 2 * 3 % 5 */
//(1 + sin(2)) * 3
// sin ( 2 )
//1 2 sin + 3 *
int GetPriority(token_t token) {
  // check table
  switch (token.value.op) {
  case LBRACE:
      return 0;
  case RBRACE:
    return -1;
  case PLUS:
  case MINUS:
    return 2;
  case MUL:
  case DIV:
    return 3;
  case MOD:
    return 4;
  case UNAR_MINUS:
  case POW:
    return 5;
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
      return 6;
    //If you will add new priority(correct Drop, ^case)
  default:
    return INCORRECT_OPERATION;
  }

}

ERR_STATUS DropOperators(token_t* operStack, int* operStackLen, token_t** numStack,
                    int* numStackSize, int* numStackLen, int priority, int *meetLBrace) {
  token_t dropable;

  token_t power;
  power.type = OPERATOR;
  power.value.op = POW;

  *meetLBrace = 0;

  while (Top(operStack, *operStackLen, sizeof(token_t), &dropable) == OK) {
    if ((GetPriority(dropable) >= priority && priority != GetPriority(power)) ||
            GetPriority(dropable) > priority) {            // second expression for ^
        Pop(operStack, operStackLen, sizeof(token_t), &dropable);
      if (dropable.value.op != LBRACE)
      {
        if (Push(numStack, numStackSize, numStackLen, sizeof(token_t), &dropable) == NO_MEM)
            return NO_MEM;
      }
      else {
        *meetLBrace = 1;
        break;
      }
    }
    else
      break;
  }

  return OK;
}
