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

double atan2(double y, double x);
float atanf(float x);
double cos(double x);
float cosf(float x);
float powf(float x, float y);
float sinf(float x);

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
#endif

#endif
