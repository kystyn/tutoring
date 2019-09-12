#include <iostream>
#include <cmath>
#include "integration.h"

int main( void )
{
  std::function<double(double)> f = []( double x ) -> double {
      auto cosx = cos(x);
      auto sgn = []( double t ) {
          if (t > 0)
              return 1;
          if (t < 0)
              return -1;
          return 0;
      };
      return sgn(cosx) * pow(abs(cosx), 1 / 3.0) / (3 * x * x * x  + x / 4);
  };

  std::cout << "Trapezium: " << integrateTrapezium(1, 3, f, 1e-6) << "\n" <<
    "Monte Carlo: " << integrateMonteCarlo(1, 3, f, 1e-6) << "\n"; // 9

  return 0;
}