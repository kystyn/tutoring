#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "sort.h"

//For qsort
int comp(const int* i, const int* j)
{
    return *i > *j;
  //return *i - *j;
}

//Some staff
int* CreateRandArr(int length, int sizeOfElem) {
  int i = 0;
  int l = INT8_MIN;
  int r = INT8_MAX;
  int* arr = malloc(length * sizeOfElem);
  if (arr == NULL)
    return NULL;

  for (i = 0; i < length; i++) {
    arr[i] = (int)(((double)rand() / RAND_MAX) * (r - l + 1) + r);
  }

  return arr;
}

ERR_STATUS CopyArr(int* arrCopy, int** arrPaste, int len) {
  int i = 0;

  if (*arrPaste != NULL)
    return ERROR;

  *arrPaste = malloc(len * sizeof(int));
  if (*arrPaste == NULL)
    return ERROR;

  for (i = 0; i < len; i++)
    (*arrPaste)[i] = arrCopy[i];

  return OK;
}

void PrintArr(int* arr, int len) {
  int i = 0;
  for (i = 0; i < len; i++)
    printf("%i ", arr[i]);
  return;
}

ERR_STATUS CheckSortedArr(int* arrR, int* arrToCheck, int len) {
  int i = 0;
  for (i = 0; i < len && arrR[i] == arrToCheck[i]; i++);
  if (i != len)
    return ERROR;
  return OK;
}

int main() {
  int* arrStand = NULL;
  int* arrRL = NULL;
  int* arrLR = NULL;
  int* arrRand = NULL;
  //int arrRand[5] = { 2, 3, 4, 1, 5 };
  int arrLen = 5;
  int i = 0;
  ERR_STATUS check = OK;

  arrRand = CreateRandArr(arrLen, sizeof(int));
  //InsertionSortBin(arrRand, arrLen);
  //PrintArr(arrRand, arrLen);
  if (CopyArr(arrRand, &arrStand, arrLen) == ERROR)
    printf("oh1\n");
  if (CopyArr(arrRand, &arrRL, arrLen) == ERROR)
    printf("oh1\n");
  if (CopyArr(arrRand, &arrLR, arrLen) == ERROR)
    printf("oh1\n");

  qsort(arrStand, arrLen, sizeof(int), (int(*) (const void*, const void*)) comp);
  InsertionSortRL(arrRL, arrLen);
  InsertionSortLR(arrLR, arrLen);

  if (CheckSortedArr(arrStand, arrRL, arrLen) == ERROR)
    printf("oh1\n");
  if (CheckSortedArr(arrStand, arrLR, arrLen) == ERROR)
    printf("oh2\n");
  printf("check done\n");


  free(arrLR);
  free(arrRL);
  free(arrStand);
  free(arrRand);
  return 0;
}

