#include <stdlib.h>
#include "evaluator.h"
#include "stack.h"
#include "error.h"
#define _USE_MATH_DEFINES
#include <math.h>

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
double Eval(token_t* tokens, int tokenSize, int tokensLength, ERR_STATUS* status) {
  token_t top;
  token_t* num_stack = NULL;
  int numStackSize = 0, numStackLength = 0;

  *status = OK;

  while (Top(tokens, tokensLength, sizeof(token_t), &top) != EMPTY) {
    Pop(tokens, &tokensLength, sizeof(token_t), &top);
    if (top.type == OPERATOR) {
      token_t operand1,
              operand2;
      token_t res;
      token_t tmpToTrans;
      res.type = NUMBER;

      if (Pop(num_stack, &numStackLength, sizeof(token_t), &operand2) != OK) {
        *status = EMPTY;
        break;
      }
      if (top.value.op < FIRST_UNAR) {
        if (Pop(num_stack, &numStackLength, sizeof(token_t), &operand1) != OK) {
          *status = EMPTY;
          break;
        }

        switch (top.value.op)
        {
        case PLUS:
          res.value.num = operand1.value.num + operand2.value.num;
          break;
        case MINUS:
          res.value.num = operand1.value.num - operand2.value.num;
          break;
        case DIV:
          if (fabs(operand2.value.num) < 1e-8)
            *status = DEFAREA;
          else
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
          if (operand1.value.num < 0 && fabs(operand2.value.num - ceil(operand2.value.num)) > 1e-8)
            *status = DEFAREA;
          else
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
          // remove pi / 2 + pi * k
          if (fabs((operand2.value.num - M_PI / 2) / M_PI -
            floor((operand2.value.num - M_PI / 2) / M_PI)) < 1e-8)
            *status = DEFAREA;
          else
            res.value.num = tan(operand2.value.num);
          break;
        case CTG:
          // remove pi * k
          if (fabs(operand2.value.num / M_PI -
            floor(operand2.value.num / M_PI)) < 1e-8)
            *status = DEFAREA;
          else
            res.value.num = 1.0 / tan(operand2.value.num);
          break;
        case ASIN:
          if (operand2.value.num < -1 || operand2.value.num > 1)
            *status = DEFAREA;
          else
            res.value.num = asin(operand2.value.num);
          break;
        case ACOS:
          if (operand2.value.num < -1 || operand2.value.num > 1)
            *status = DEFAREA;
          else
            res.value.num = acos(operand2.value.num);
          break;
        case ATG:
          res.value.num = atan(operand2.value.num);
          break;
        case ACTG:
          if (fabs(operand2.value.num) < 1e-8)
            res.value.num = M_PI / 2;
          else
            res.value.num = atan(1.0 / operand2.value.num);
          //atg(y)==x <=> tg(x) == y <=>ctg(x) = 1/y <=> x = actg(1/y)
          break;
        case LN:
          if (operand2.value.num <= 1e-8)
            *status = DEFAREA;
          else
            res.value.num = log(operand2.value.num);
          break;
        case SQRT:
          if (operand2.value.num < 0)
            *status = DEFAREA;
          else
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
      if (*status != OK)
        break;

      if (Push(&tokens, &tokenSize, &tokensLength, sizeof(token_t), &res) == NO_MEM)
      {
          if (tokens != NULL)
              free(tokens);

          if (num_stack != NULL)
              free(num_stack);

          *status = NO_MEM;
          return 0;
      }

      while (numStackLength != 0) {
        Pop(num_stack, &numStackLength, sizeof(token_t), &tmpToTrans);

        if (Push(&tokens, &tokenSize, &tokensLength, sizeof(token_t), &tmpToTrans) == NO_MEM)
        {
            if (tokens != NULL)
                free(tokens);

            if (num_stack != NULL)
                free(num_stack);

            *status = NO_MEM;
            return 0;
        }
      }
      free(num_stack);
      num_stack = NULL;
    }
    else
      if (Push(&num_stack, &numStackSize, &numStackLength, sizeof(token_t), &top) == NO_MEM)
      {
          if (tokens != NULL)
              free(tokens);

          if (num_stack != NULL)
              free(num_stack);

          *status = NO_MEM;
          return 0;
      }
  }

  free(tokens);

  Pop(num_stack, &numStackLength, sizeof(token_t), &top);
  if (numStackLength != 0)
    *status = INCORRECT_OPERATION;

  free(num_stack);
  return top.value.num;
}
