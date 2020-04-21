#include <stdlib.h>
#include <ctype.h>

#include "scanner.h"
#include "utils.h"
#include "token.h"

int isOperator(char c) {
    return  c == '+' ||
            c == '-' ||
            c == '(' ||
            c == ')' ||
            c == '/' ||
            c == '*' ||
            c == '%' ||
            c == '^' ||
            c == '$'; // remove when test infix form
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
      (*str)[i] = '\0';  // Cause i increses in last (cycle) iteration
      *strLength = i + 1;     //when we add \0 the length increase by one
  }
  if (i)   //It means that if str is zero length do not write anything
    return EMPTY_STR;
  return STREAM_END;
}

ERR_STATUS ParseInput(char *str, int strLength,
                      token_t **tokens, int *tokenArraySize, int *tokenLength) {

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

            num = intPart + realPart;

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
            if (isOperator(symbol)) {
                *tokens = Expand(*tokens, tokenArraySize, *tokenLength, sizeof(token_t));
                if (*tokens == NULL) {
                    printf("ERROR: Not enough memory");
                    return NO_MEM;
                }
                (*tokens)[*tokenLength].type = OPERATOR;
                (*tokens)[*tokenLength].value.op = symbol;
                (*tokenLength)++;
            }
        }
        // operators
        else if (isOperator(symbol)) {
            *tokens = Expand(*tokens, tokenArraySize, *tokenLength, sizeof(token_t));
            if (*tokens == NULL) {
                printf("ERROR: Not enough memory");
                return NO_MEM;
            }
            (*tokens)[*tokenLength].type = OPERATOR;
            (*tokens)[*tokenLength].value.op = symbol;
            if ((*tokens)[*tokenLength].value.op == '-') {
              if(*tokenLength == 0)
                (*tokens)[*tokenLength].value.op = '$';
              else if((*tokens)[*tokenLength - 1].type == OPERATOR &&
                      (*tokens)[*tokenLength - 1].value.op != ')')
                (*tokens)[*tokenLength].value.op = '$';
             }
            (*tokenLength)++;
        }
        i++;
    }
    return OK;
}
