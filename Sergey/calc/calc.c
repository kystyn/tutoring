/* Course work Algorythms
Calculator of expressions
Polytech 2020 spring
1st course, AMI
Krotikov Sergei    */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "scanner.h"
#include "infix2polish.h"
#include "evaluator.h"
#include "utils.h"

/*POSSIBLE ERRORS:
-1 - not enough memory
10 - too many argments in main
11 - wrong file name */


int Checking(char* str, int length) {
  /*There i tried to implement some sequence
  of checks to find comments and empty strings*/
  for (int i = 0; i < length; i++) {
    //str[i] is not a space 
    if (isspace(str[i]) == 0) {
      if (str[i] == '/') {
        if (str[i + 1] == '/')
          return 1;   // case comment
        return 0;
      }
      if (str[i] == '\0')
        return 1;  // case empty string
      return 0;
    }
  }
  return 1; //I suppose this return will never work but compiler want it 
}


void Print(char* str, int length, double evalRes, FILE* output) {
  int check = 0;
  check = Checking(str, length);
  /* Cases: 0 - string we should calculate
  1 - comment or empty sring */
  switch (check) {
  case 1:
    fprintf(output,"%s\n", str);
    break;
  default:
    fprintf(output,"%s == %g\n", str, evalRes);
    break;
  }
}

int main(int argc, char* argv[]) {

  /* Reading arguments and stream redirection*/
  FILE* input = stdin;
  FILE* output = stdout;

  //Max num of arguments == 2 and argv[0] == main.exe
  if (argc > 3) {
    printf("ERROR: too many arguments");
    return 10;
  }

  if (argc >= 2) {
    if ((input = fopen(argv[1], "r")) == NULL) {
      printf("ERROR: wrong file name.");
      return 11;
    }
  }
  if (argc >= 3) {
    if ((output = fopen(argv[2], "w")) == NULL) {
      printf("ERROR: wrong file name.");
      return 11;
    }
  }

  /* Main body */
  int strLength = 0;
  int strSize = 0;
  char* str;
  ERR_STATUS readingCondition, parseCondition;

  do {
    token_t *infixTokens = NULL;
    int infixLength = 0;
    int infixSize = 0;

    token_t *polishTokens = NULL;
    int polishLength = 0;
    int polishSize = 0;

    double evalRes = 0;

    str = NULL;
    readingCondition = Read(&str, &strSize, &strLength, input);
    if (readingCondition == NO_MEM)
      return -1;
    if (readingCondition != OK && readingCondition != STREAM_END)
      break;

    strLength -= 1; // because of \0
    parseCondition = ParseInput(str, strLength, &infixTokens, &infixSize, &infixLength);
    Reverse(infixTokens, infixLength, sizeof(token_t));
    Infix2Polish(infixTokens, &infixLength, &polishTokens, &polishSize, &polishLength);
    Reverse(polishTokens, polishLength, sizeof(token_t));
    evalRes = Eval(polishTokens, polishSize, polishLength);

    if (parseCondition == NO_MEM)
      return -1;
    Print(str, strLength, evalRes, output);
    free(str);
  } while (readingCondition == OK);

  fclose(output);
  fclose(input);
  return 0;
}
