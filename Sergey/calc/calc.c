/* Course work Algorythms
Calculator of expressions
Polytech 2020 spring
1st course, AMI
Krotikov Sergei    */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#ifdef WIN32
#include <crtdbg.h>
#endif

#include "scanner.h"
#include "infix2polish.h"
#include "evaluator.h"
#include "utils.h"

/*POSSIBLE ERRORS:
-1 - not enough memory
10 - too many argments in main
11 - wrong file name */



void Print(char* str, double evalRes, FILE* output, int check) {
  /* Cases: 
  0 - string we should calculate
  1 - comment or empty sring
  2 - parsing error, only one case: Operator not exist 
  3 - infix2polish error, 3 cases of mistake
  4 - eval error, 3* cases of mistake 
  */
  switch (check) {
  case COMMENT:  //and empty string
    fprintf(output,"%s\n", str);
    break;
    // Parser
  case PARSER_ERR:
    fprintf(output, "%s == ERROR: parser error\n", str);
    break;
    //Inf2pol
  case I2P_NO_PREFIX:
    fprintf(output, "%s == ERROR: infix 2 polish error (wait for prefix but no)\n", str);
    break;
  case I2P_NO_SUFFIX:
    fprintf(output, "%s == ERROR: infix 2 polish error (wait for suffix but no)\n", str);
    break;
  case I2P_PROBLEM_BRACKETS:
    fprintf(output, "%s == ERROR: infix 2 polish error (problem with brackets)\n", str);
    break;
    //Evaluator 
  case EMPTY:
    fprintf(output, "%s == ERROR: eval error(a lack of operands, no data)\n", str);
    break;
  case DEFAREA:
    fprintf(output, "%s == ERROR: eval error(not defarea of this function)\n", str);
    break;
  case INCORRECT_OPERATION:
    fprintf(output, "%s == ERROR: eval error(too many operators, really not sure this works)\n", str);
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

#ifdef WIN32
#ifndef NDEBUG
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif
#endif


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
  ERR_STATUS checkStr;
  ERR_STATUS readingCondition;
  ERR_STATUS parseCondition;
  ERR_STATUS evalStatus;
  ERR_STATUS inf2polStatus;

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
    if (readingCondition == NO_MEM) {
      printf("ERROR: Not enough memory\n");
      continue;
    }
    else if (readingCondition == STREAM_END /* EMPTY_STR */)
        continue;

    checkStr = Checking(str, strLength);
    if (checkStr == OK) {
      strLength -= 1; // because of \0
      parseCondition = ParseInput(str, strLength, &infixTokens, &infixSize, &infixLength);
      if (parseCondition != OK) {
        if (parseCondition != NO_MEM)
            Print(str, 1, output, PARSER_ERR);
        free(infixTokens);
        free(str);
        continue;
      }

      if (Reverse(infixTokens, infixLength, sizeof(token_t)) == NO_MEM) {
          printf("ERROR: Not enough memory\n");
          free(infixTokens);
          free(str);
          continue;
      }

      inf2polStatus = Infix2Polish(infixTokens, &infixLength, &polishTokens, &polishSize, &polishLength);
      if (inf2polStatus != OK) {
          if (inf2polStatus == NO_MEM)
              printf("ERROR: Not enough memory\n");
          else
            Print(str, 1, output, inf2polStatus);
        free(str);
        continue;
      }

      assert(polishTokens != NULL);
      if (Reverse(polishTokens, polishLength, sizeof(token_t)) == NO_MEM) {
          printf("ERROR: Not enough memory\n");
          free(infixTokens);
          free(polishTokens);
          free(str);
          continue;
      }
      evalRes = Eval(polishTokens, polishSize, polishLength, &evalStatus);
      if (evalStatus != OK)
      {
          if (evalStatus == NO_MEM)
              printf("ERROR: Not enough memory\n");
          else
            Print(str, 1, output, evalStatus);
      }
      else
        Print(str, evalRes, output, checkStr);
      free(str);
    }
    else 
    {
      Print(str, 1, output, checkStr);
      free(str);
    }

  } while (readingCondition != STREAM_END);

  fclose(output);
  fclose(input);
  return 0;
}
