#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "scanner.h"
#include "utils.h"
#include "token.h"

int IsOperator(char c) {
  return  c == '+' ||
    c == '-' ||
    c == '(' ||
    c == ')' ||
    c == '/' ||
    c == '*' ||
    c == '%' ||
    c == '^';
}


operator_t GetOperatorByStr( char* str ) {
    char *variants[] = { "+", "-", "*", "/", "^", "%", "(", ")",
                         "sin", "cos", "tg", "ctg", "arcsin", "arccos",
                         "arctg", "arcctg", "ln", "floor", "ceil", "sqrt" };
    operator_t operators[] = {PLUS, MINUS, MUL, DIV, POW, MOD, LBRACE, RBRACE,
                           SIN, COS, TG, CTG, ASIN, ACOS, ATG, ACTG, LN, FLOOR,
                           CEIL, SQRT};
    unsigned int i;
    for (i = 0; i < sizeof(variants) / sizeof(variants[0]); i++) {
        if (strcmp(variants[i], str) == 0)
            return operators[i];
    }
    return NOT_EXISTS;
}

// length - data size
// size - memory size
ERR_STATUS Read(char** str, int *strSize, int* strLength, FILE* input) {
  int symbol;
  int i = 0;

  *strSize = 0;

  /* Main body */
  while ((symbol = fgetc(input)) != EOF && (symbol != 26)) {  // 26 - means ctrl+Z
    // Expand (or create) dynamic array
    *str = Expand(*str, strSize, i + 1, sizeof(char));
    if (*str == NULL) {
      printf("ERROR: Not enough memory");
      return NO_MEM;
    }
    //Save character by character to an array
    (*str)[i] = (char)symbol; // S4: russian symbol! hazardous!!
    *strLength = i + 1;
    if (symbol == '\n'){
      (*str)[i] = '\0';
      return OK;
    }

    i++;
  }

  if (*str != NULL)
  {
      *str = Expand(*str, strSize, i + 1, sizeof(char));
      (*str)[i] = '\0';  // Cause i increses in last (cycle) iteration
      *strLength = i + 1;     //when we add \0 the length increase by one
  }
  if (i == 0)   //It means that if str is zero length do not write anything
    return EMPTY_STR;
  return STREAM_END;
}

double pow10( int exp ) {
    double res = 1;
    int i;
    int aexp = abs(exp);
    for (i = 0; i < aexp; i++)
        res *= 10;
    return exp < 0 ? 1.0 / res : res;
}

ERR_STATUS ParseInput(char* str, int strLength,
  token_t** tokens, int* tokenArraySize, int* tokenLength) {

  char symbol;
  int i = 0;
  *tokenArraySize = 0;
  while (i < strLength) {
    symbol = str[i];
    // parsing
    // if digit
    if (isdigit(symbol)) {
      double
        num = 0,
        intPart = 0,
        realPart = 0;
      int
        expPart = 0;
      int expSign = 1;
      double degree10 = 0.1;
      // here only integers
      for (; i < strLength && isdigit(symbol); symbol = str[++i])
        intPart = intPart * 10 + (symbol - '0');
      // here real
      if (symbol == '.') {
        for (symbol = str[++i]; i < strLength && isdigit(symbol);
          symbol = str[++i], degree10 /= 10.0)
          realPart += (symbol - '0') * degree10;
      }
      // scientific notation
      if (symbol == 'e') {
          if (i + 1 < strLength)
              if (str[i + 1] == '+') {
                  expSign = 1;
                  i += 2;
              }
              else if (str[i + 1] == '-') {
                  expSign = -1;
                  i += 2;
              }
              else if (isdigit(str[i + 1]))
                  i += 1;
          for (symbol = str[i]; i < strLength && isdigit(symbol); symbol = str[++i])
            expPart = expPart * 10 + (symbol - '0');
      }

      num = (intPart + realPart) * pow10(expSign * expPart);

      // write number to token array
      *tokens = Expand(*tokens, tokenArraySize, *tokenLength, sizeof(token_t));
      if (*tokens == NULL) {
        printf("ERROR: Not enough memory");
        return NO_MEM;
      }
      (*tokens)[*tokenLength].type = NUMBER;
      (*tokens)[*tokenLength].value.num = num;
      (*tokenLength)++;
      // check symbol after readed numbers
      // no i++!
      if (IsOperator(symbol))
          continue;
    }
    // skip white space
    else if (isspace(symbol)) {}
    // operators
    else {
      char opstr[30] = { 0 };
      unsigned int j;

      *tokens = Expand(*tokens, tokenArraySize, *tokenLength, sizeof(token_t));
      if (*tokens == NULL) {
        printf("ERROR: Not enough memory");
        return NO_MEM;
      }

      if (IsOperator(symbol)) {
          opstr[0] = symbol;
          i++;
      }
      else
          for (symbol = str[i], j = 0; i < strLength &&
               isalpha(symbol) &&
               j < sizeof(opstr); symbol = str[++i], j++)
            opstr[j] = symbol;

      (*tokens)[*tokenLength].type = OPERATOR;
      (*tokens)[*tokenLength].value.op = GetOperatorByStr(opstr);
      if ((*tokens)[*tokenLength].value.op == MINUS) {
        if (*tokenLength == 0)
          (*tokens)[*tokenLength].value.op = UNAR_MINUS;
        else if ((*tokens)[*tokenLength - 1].type == OPERATOR &&
          (*tokens)[*tokenLength - 1].value.op != RBRACE)
          (*tokens)[*tokenLength].value.op = UNAR_MINUS;
      }
      (*tokenLength)++;
      // if really scanned operator - no i++ in the loop end
      if (j != 0)
          continue;
    }
    i++;
  }
  return OK;
}
