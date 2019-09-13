#include <ctime>
#include "integration.h"

double integrateTrapezium( double a, double b, std::function<double(double)> func, double step )
{
    double res = (func(a) + func(b)) / 2 * step;

    for (double x = a + step; x <= b - step; x += step)
        res += func(x) * step;
    return res;
}

double integrateMonteCarlo( double a, double b,
                            int upperBound,
                            int dotsPerFragment,
                            std::function<double(double)> func)
{

    srand((unsigned int)time(nullptr));

    double baseSquare = (b - a) * upperBound;
    int trueDots = 0;

    for (double x = a; x <= b; x += (b - a) / dotsPerFragment)
    {
        double dot = double(rand()) / RAND_MAX * upperBound;
        trueDots += dot <= func(x);
    }

    return baseSquare * trueDots / dotsPerFragment;
}
