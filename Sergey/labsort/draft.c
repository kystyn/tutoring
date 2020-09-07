/*
Draft of SOR homework
realization of sort functions
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "sort.h"


//SORT part
void InsertionSortLR(int* arr, int len) {  //left to right
  int i;
  int loc;
  int new;
  for (i = 1; i < len; i++) {
    new = arr[i];
    for (loc = i - 1; loc >= 0 && arr[loc] >= new; loc--)
      arr[loc + 1] = arr[loc];
    arr[loc + 1] = new;
  }
}

void InsertionSortRL(int* arr, int len) { //right to left
  int i = 0;
  int loc;
  int new;
  for (i = len - 2; i >= 0; i--) {
    new = arr[i];
    for (loc = i + 1; loc < len && arr[loc] <= new; loc++)
      arr[loc - 1] = arr[loc];
    arr[loc - 1] = new;
  }
}

void InsertionSortBin(int* arr, int len) { //with bin search
  int i;
  int lenSort = 0;
  int new;
  int shift = 0;
  for (lenSort = 1; lenSort < len; ++lenSort) {
    new = arr[lenSort];
    shift = BinSearch(arr, lenSort, new);
    for (i = lenSort - 1; i > shift; --i)
      arr[i - 1] = arr[i];
    arr[i + 1] = new;
  }
}

int BinSearch(int* arr, int len, int target) {
  int start = 0;
  int end = len - 1;
  int mid = 0;
  while (start <= end) {
    mid = (start + end) / 2;
    if (arr[mid] < target)
      start = mid + 1;
    else if (arr[mid] == target)
      break;
    else
      end = mid - 1;
    }
  return mid;
}

sort_info_t* GetSortList(int *size)
{
    sort_info_t res[] =
    {
        {InsertionSortLR, 0, 0, "InsertionSortLR"},
        {InsertionSortRL, 0, 0, "InsertionSortRL"},
        {InsertionSortBin, 0, 0, "InsertionSortBin"}
    };

    *size = sizeof(res) / sizeof(sort_info_t);

    sort_info_t *arr = malloc(sizeof(res));
    if (arr == NULL)
        return NULL;

    // TODO: fill arr. do not need res actually

    memmove(arr, res, sizeof(res));
    return arr;
}
