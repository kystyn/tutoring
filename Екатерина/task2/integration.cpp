#include "integration.h"

double integrateTrapezium( double a, double b, std::function<double(double)> func, double step )
{
    double res = (func(a) + func(b)) / 2 * step;

    for (double x = a + step; x <= b - step; x += step)
        res += func(x) * step;
    return res;
}

double integrateMonteCarlo( double a, double b, std::function<double(double)> func, double step )
{
    double res = 0;

    for (double x = a; x <= b; x += step)
        res += func(x) * step;
    return res;
}