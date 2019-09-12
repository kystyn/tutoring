#include "magic_square.h"

float ** fillMatrix( int N )
{
    float **data = new float *[N];

    for (int i = 0; i < N; i++)
        data[i] = new float[N];

    return data;
}

void clearMatrix( float **data, int N )
{
    for (int i = 0; i < N; i++)
        delete []data[i];

    delete data;
}

bool isMagicSquare( float *data, int N )
{
}