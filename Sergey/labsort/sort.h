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

typedef enum
{
    SORT_QUICK, SORT_INSERION
} sort_family_t;

typedef enum
{
    N, N2
} sort_complexity_t;

typedef void (* sort_func_t)(int *array, int size);

typedef struct
{
    sort_func_t sort;
    sort_family_t family;
    sort_complexity_t complexity;
    char const *name;
} sort_info_t;

#ifdef _WIN32
__declspec(dllexport)
#endif
sort_info_t* GetSortList(int *size);


int* CreateRandArr(int, int);
void InsertionSortLR(int*, int);
void InsertionSortRL(int*, int);
void InsertionSortBin(int*, int);
int BinSearch(int*, int, int);
void PrintArr(int*, int);
ERR_STATUS CheckSortedArr(int*, int*, int);
ERR_STATUS CopyArr(int*, int**, int);

#endif /* SORT_H */
