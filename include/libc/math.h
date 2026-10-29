#ifndef MATH_H
#define MATH_H

// MSL C library declarations, limited to what reconstructed units use (docs/libraries.md).

#ifdef __cplusplus
extern "C" {
#endif

extern int __float_nan[];
extern int __float_huge[];
extern int __double_huge[];

#define INFINITY (*(float*)__float_huge)
#define NAN (*(float*)__float_nan)
#define HUGE_VAL (*(double*)__double_huge)

#define __HI(x) (*(int*)&x)
#define __LO(x) (*(1 + (int*)&x))

#define FP_NAN 1
#define FP_INFINITE 2
#define FP_ZERO 3
#define FP_NORMAL 4
#define FP_SUBNORMAL 5

static inline int __fpclassifyf(float x)
{
    switch ((*(int*)&x) & 0x7f800000) {
    case 0x7f800000:
        if ((*(int*)&x) & 0x007fffff)
            return FP_NAN;
        else
            return FP_INFINITE;
        break;
    case 0:
        if ((*(int*)&x) & 0x007fffff)
            return FP_SUBNORMAL;
        else
            return FP_ZERO;
        break;
    }
    return FP_NORMAL;
}

static inline int __fpclassifyd(double x)
{
    switch (__HI(x) & 0x7ff00000) {
    case 0x7ff00000:
        if ((__HI(x) & 0x000fffff) || (__LO(x) & 0xffffffff))
            return FP_NAN;
        else
            return FP_INFINITE;
        break;
    case 0:
        if ((__HI(x) & 0x000fffff) || (__LO(x) & 0xffffffff))
            return FP_SUBNORMAL;
        else
            return FP_ZERO;
        break;
    }
    return FP_NORMAL;
}

#define fpclassify(x) \
    (sizeof(x) == sizeof(float) ? __fpclassifyf((float)(x)) : __fpclassifyd((double)(x)))
#define isnan(x) (fpclassify(x) == FP_NAN)
#define isinf(x) (fpclassify(x) == FP_INFINITE)
#define isfinite(x) (fpclassify(x) > FP_INFINITE)
#define signbit(x) ((int)(__HI(x) & 0x80000000))


double acos(double x);
double asin(double x);
double atan(double x);
double atan2(double y, double x);
double copysign(double x, double y);
double fmod(double x, double y);
double frexp(double x, int* exp);
double ldexp(double x, int exp);
double modf(double x, double* iptr);
double nextafter(double x, double y);
double pow(double x, double y);
float atanf(float x);
double cos(double x);
double sin(double x);
float cosf(float x);
float powf(float x, float y);
float sinf(float x);
double tan(double x);

#ifdef __cplusplus
}
#endif

// Game code (C++) inlines sqrtf; the C library units call it out of line.
#ifdef __cplusplus
extern inline float sqrtf(float x)
{
    static const double _half = .5;
    static const double _three = 3.0;
    volatile float y;
    if (x > 0.0f)
    {
        double guess = __frsqrte((double)x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        y = (float)(x * guess);
        return y;
    }
    return x;
}

extern inline double fabs(double x) { return __fabs(x); }
extern inline float fabsf(float x) { return ::fabs(x); }
#else
// The C units (the MSL math units and RenderWare) inline the double sqrt.
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
#endif

#endif
