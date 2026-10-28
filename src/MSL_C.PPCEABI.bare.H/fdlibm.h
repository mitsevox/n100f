#ifndef MSL_FDLIBM_H
#define MSL_FDLIBM_H

#include <errno.h>
#include <math.h>

typedef int _INT32;
typedef unsigned int _UINT32;

#define __HIp(x) *(_INT32*)x
#define __LOp(x) *(1 + (_INT32*)x)

double floor(double x);

static inline double fabs(double x) { return __fabs(x); }

static inline double sqrt(double x)
{
    if (x > 0.0) {
        double guess = __frsqrte(x);
        guess = .5 * guess * (3.0 - guess * guess * x);
        guess = .5 * guess * (3.0 - guess * guess * x);
        guess = .5 * guess * (3.0 - guess * guess * x);
        guess = .5 * guess * (3.0 - guess * guess * x);
        return x * guess;
    } else if (x == 0.0) {
        return 0;
    } else if (x) {
        return NAN;
    }
    return INFINITY;
}

static inline double scalbn(double x, int n) { return ldexp(x, n); }

double __ieee754_acos(double x);
double __ieee754_asin(double x);
double __ieee754_atan2(double y, double x);
double __ieee754_exp(double x);
double __ieee754_fmod(double x, double y);
double __ieee754_pow(double x, double y);
int __ieee754_rem_pio2(double x, double* y);
double __kernel_cos(double x, double y);
double __kernel_sin(double x, double y, int iy);
double __kernel_tan(double x, double y, int iy);
int __kernel_rem_pio2(double* x, double* y, int e0, int nx, int prec, const int* ipio2);

#endif
