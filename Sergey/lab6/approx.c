#include <math.h>
#include "approx.h"

double tolerance = 1e-10;

double min2( double a, double b ) {
    return a < b ? a : b;
}

double min4( double m1, double m2 , double m3, double m4 ) {
    return min2(m1, min2(m2, min2(m3, m4)));
}

double max2( double a, double b ) {
    return a > b ? a : b;
}

double max4( double m1, double m2 , double m3, double m4 ) {
    return max2(m1, max2(m2, max2(m3, m4)));
}


approx_t ApproxFromDouble( double center, double radius ) {
    approx_t num;

    num.lo = center - radius;
    num.hi = center + radius;

    return num;
}

approx_t ApproxFromRange( double lo, double hi ) {
    approx_t num = {lo, hi};
    return num;
}

double ApproxAsDouble( approx_t num ) {
    return (num.lo + num.hi) / 2;
}

double ApproxGetRadius( approx_t num ) {
    return (num.hi - num.lo) / 2;
}


approx_t ApproxParse( char const* str ) {
    /* <0.5; 1.25> */
    approx_t num;
    char dummy;
    sscanf(str + 1, "%lf%c%c%lf", &num.lo, &dummy, &dummy, &num.hi);

    return num;
}

void ApproxFormat( approx_t num, char *strToPrint ) {
    // think about string size
    sprintf(strToPrint, "<%lf; %lf>", num.lo, num.hi);
}

approx_t ApproxRead( FILE* istream ) {
    approx_t num;
    char dummy;
    fscanf(istream, "%c%lf%c%c%lf", &dummy, &num.lo, &dummy, &dummy, &num.hi);

    return num;
}

void ApproxWrite( FILE* ostream, approx_t num ) {
    fprintf(ostream, "<%lf; %lf>", num.lo, num.hi);
}

approx_t ApproxAdd( approx_t num1, approx_t num2 ) {
    approx_t sum = { num1.lo + num2.lo,
                     num1.hi + num2.hi
                   };
    return sum;
}

approx_t ApproxSub( approx_t num1, approx_t num2 ) {
    approx_t sub = { num1.lo - num2.hi,
                     num1.hi - num2.lo
                   };
    return sub;
}

approx_t ApproxMul( approx_t num1, approx_t num2 ) {
    approx_t mul;
    mul.lo = min4(num1.lo * num2.lo,
                  num1.lo * num2.hi,
                  num1.hi * num2.lo,
                  num1.hi * num2.hi);
    mul.hi = max4(num1.lo * num2.lo,
                  num1.lo * num2.hi,
                  num1.hi * num2.lo,
                  num1.hi * num2.hi);

    return mul;
}

approx_t ApproxDiv( approx_t num1, approx_t num2 ) {
    approx_t div;
    if (fabs(num2.lo) < tolerance || fabs(num2.hi) < tolerance) {
        div.lo = (double)INFINITY;
        div.hi = (double)INFINITY;
        return div;
    }

    div.lo = min4(num1.lo / num2.lo,
                  num1.lo / num2.hi,
                  num1.hi / num2.lo,
                  num1.hi / num2.hi);
    div.hi = max4(num1.lo / num2.lo,
                  num1.lo / num2.hi,
                  num1.hi / num2.lo,
                  num1.hi / num2.hi);

    return div;
}
approx_t ApproxNegate( approx_t num ) {
    approx_t negNum = {-num.hi, -num.lo};
    return negNum;
}

// -1 - less, 0 - equal, 1 - greater
int ApproxCompare( approx_t num1, approx_t num2 ) {
    if (num1.hi < num2.lo)
        return -1;
    if (num2.hi < num1.lo)
        return 1;
    return 0;
}

approx_t ApproxAbs( approx_t num ) {
    // think about -0.3 0.1 -> 0.1 0.3 ???
    double
            absLo = fabs(num.lo),
            absHi = fabs(num.hi);
    approx_t absNum = {min2(absLo, absHi), max2(absLo, absHi)};
    return absNum;
}

