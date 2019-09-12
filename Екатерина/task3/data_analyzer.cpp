#include <iostream>
#include <cmath>
#include "data_analyzer.h"

float * readData( int N )
{
    float *data = new float[N];

    for (int i = 0; i < N; i++)
      std::cin >> data[i];

    return data;
}

void clearData( float * data )
{
    delete []data;
}

int howManyHaveDeviation( float *data, int N, float deviation )
{
    float average = 0;
    int res = 0;

    for (int i = 0; i < N; i++)
        average += data[i] / N;

    for (int i = 0; i < N; i++)
        if (std::abs(data[i] - average) / average > deviation)
            res++;

    return res;
}

void evaluateMaxValue( float *data, int N, float &val, int &countMax )
{
    const float tollerance = float(1e-6);
    val = data[0];
    for (int i = 1; i < N; i++)
        if (data[i] > val)
          val = data[i];

    countMax = 0;
    for (int i = 0; i < N; i++)
      if (std::abs(data[i] - val) < 1e-6)
        countMax++;
}

void runDataAnalyze( void )
{
    int N;
    std::cout << "Input data size: ";
    std::cin >> N;

    auto data = readData(N);
    int
      dev10 = howManyHaveDeviation(data, N, 0.10f),
      dev15 = howManyHaveDeviation(data, N, 0.15f),
      dev20 = howManyHaveDeviation(data, N, 0.20f);

    int countMax;
    float maxVal;
    evaluateMaxValue(data, N, maxVal, countMax);

    std::cout <<
      "Deviation 10%: " << dev10 << '\n' <<
      "Deviation 15%: " << dev15 << '\n' <<
      "Deviation 20%: " << dev20 << '\n' <<
      "Maximal value: " << maxVal << '\n' << 
      "How many maximal values: " << countMax << '\n';

    clearData(data);
}
