#ifndef APPROX_H
#define APPROX_H

#include <stdio.h>

typedef struct {
    double lo;
    double hi;
} approx_t;

approx_t ApproxFromDouble( double center, double radius );
approx_t ApproxFromRange( double lo, double hi );
double ApproxAsDouble( approx_t num );
double ApproxGetRadius( approx_t num );


approx_t ApproxParse( char const* str );
void ApproxFormat( approx_t num, char *strToPrint );
approx_t ApproxRead( FILE* istream );
void ApproxWrite( FILE* ostream, approx_t num );

approx_t ApproxAdd( approx_t num1, approx_t num2 );
approx_t ApproxSub( approx_t num1, approx_t num2 );
approx_t ApproxMul( approx_t num1, approx_t num2 );
approx_t ApproxDiv( approx_t num1, approx_t num2 );
approx_t ApproxNegate( approx_t num );
int ApproxCompare( approx_t num1, approx_t num2 );
approx_t ApproxAbs( approx_t num );

#endif // APPROX_H
