#ifndef SORT_H
#define SORT_H

typedef enum {
  OK,
  ERROR
} ERR_STATUS;

typedef enum {
  LESS,
  EQUAL,
  BIGGER
} COMPARE_STATUS;


int* CreateRandArr(int, int);
void InsertionSortLR(int*, int);
void InsertionSortRL(int*, int);
void InsertionSortBin(int*, int);
int BinSearch(int*, int, int);
void PrintArr(int*, int);
ERR_STATUS CheckSortedArr(int*, int*, int);
ERR_STATUS CopyArr(int*, int**, int);

#endif /* SORT_H */
