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
float sinf(float x);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
inline double fabs(double x)
{
    return __fabs(x);
}
#endif

#endif
