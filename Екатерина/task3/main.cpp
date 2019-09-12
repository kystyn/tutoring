#include <iostream>
#include "data_analyzer.h"

int main( void )
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

    return 0;
}