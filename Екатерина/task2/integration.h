#pragma once
#include <functional>

double integrateTrapezium( double a, double b, std::function<double(double)> func, double step = 1e-3 );
double integrateMonteCarlo( double a, double b,
                            int upperBound,
                            int dotsPerFragment,
                            std::function<double(double)> func);
