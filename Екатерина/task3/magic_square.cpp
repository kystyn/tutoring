#include <iostream>
#include "magic_square.h"

int ** fillMatrix( int N )
{
    int **data = new int *[N];

    for (int i = 0; i < N; i++)
        data[i] = new int[N];

    for (int y = 0; y < N; y++)
        for (int x = 0; x < N; x++)
          std::cin >> data[y][x];

    return data;
}

void clearMatrix( int **data, int N )
{
    for (int i = 0; i < N; i++)
        delete []data[i];

    delete []data;
}

bool isMagicSquare( int **data, int N )
{
    int value = 0;

    // first row
    for (int i = 0; i < N; i++)
    {
        value += data[0][i];
    }

    // rows
    for (int y = 0; y < N; y++)
    {
        int curval = 0;

        for (int x = 0; x < N; x++)
            curval += data[y][x];
        if (curval != value)
            return false;
    }

    // columns
    for (int x = 0; x < N; x++)
    {
        int curval = 0;

        for (int y = 0; y < N; y++)
            curval += data[y][x];
        if (curval != value)
            return false;
    }

    //diagonals
    int mainDiagVal = 0, subDiagVal = 0;
    for (int i = 0; i < N; i++)
    {
        mainDiagVal += data[i][i];
        subDiagVal += data[i][N - 1 - i];
    }

    if (mainDiagVal != value || subDiagVal != value)
      return false;

    return true;
}

void runMagicSquare( void )
{
    int N;
    std::cout << "Input matrix size: ";
    std::cin >> N;

    int **data = fillMatrix(N);
    bool magic = isMagicSquare(data, N);
    std::cout << (magic ? "Is magic\n" : "Not magic\n");

    clearMatrix(data, N);
}
