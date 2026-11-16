#include "xMath.h"
#include "xMath3.h"
#include "xMathInlines.h"

#include <math.h>

static inline F32 xpow(F32 x, F32 y)
{
    return pow(x, y);
}

static inline F32 xatan2(F32 y, F32 x)
{
    return atan2(y, x);
}

static inline F32 isin(F32 x)
{
    return sin(x);
}

static inline F32 icos(F32 x)
{
    return cos(x);
}

void xMathInit()
{
    iMathInit();
    xMath3Init();
}

void xMathExit()
{
    xMath3Exit();
    iMathExit();
}

U32 xMathSolveQuadratic(F32 a, F32 b, F32 c, F32* x1, F32* x2)
{
    F32 d;
    F32 dx;
    F32 p;

    if (a == 0.0f)
    {
        if (b == 0.0f)
        {
            return 0;
        }

        *x1 = -c / b;
        return 1;
    }

    d = b * b - (4.0f * a * c);

    if (d < 0.0f)
    {
        return 0;
    }

    dx = 1.0f / (2.0f * a);

    *x1 = -b * dx;

    if (d == 0.0f)
    {
        return 1;
    }

    *x2 = *x1;

    p = dx * xsqrt(d);

    if (a > 0.0f)
    {
        *x1 -= p;
        *x2 += p;
    }
    else
    {
        *x1 += p;
        *x2 -= p;
    }

    return 2;
}

// Cardano's method on the depressed cubic; a zero leading term falls back to the quadratic.
U32 xMathSolveCubic(F32 a, F32 b, F32 c, F32 d, F32* x1, F32* x2, F32* x3)
{
    F32 arecip;
    F32 fA;
    F32 fB;
    F32 fOffset;
    F32 fDiscr;
    F32 fHalfB;
    F32 fTemp;
    F32 fDist;
    F32 fAngle;
    F32 fCos;
    F32 fSin;

    if (a == 0.0f)
    {
        return xMathSolveQuadratic(b, c, d, x1, x2);
    }

    if (a != 1.0f)
    {
        arecip = 1.0f / a;
        b *= arecip;
        c *= arecip;
        d *= arecip;
    }

    fA = 0.33333334f * ((3.0f * c) - b * b);
    fB = 0.037037037f * ((27.0f * d) + ((2.0f * (b * b * b)) - (9.0f * b * c)));
    fOffset = 0.33333334f * b;
    fDiscr = (0.25f * (fB * fB)) + (0.037037037f * (fA * (fA * fA)));
    fHalfB = 0.5f * fB;

    if (fabsf(fDiscr) < 0.000001f)
    {
        fDiscr = 0.0f;
    }

    if (fDiscr > 0.0f)
    {
        fTemp = xsqrt(fDiscr);
        if (-fHalfB + fTemp >= 0.0f)
        {
            *x1 = xpow(-fHalfB + fTemp, 0.33333334f);
        }
        else
        {
            *x1 = -xpow(-(-fHalfB + fTemp), 0.33333334f);
        }

        if (-fHalfB - fTemp >= 0.0f)
        {
            *x1 += xpow(-fHalfB - fTemp, 0.33333334f);
        }
        else
        {
            *x1 -= xpow(-(-fHalfB - fTemp), 0.33333334f);
        }

        *x1 -= fOffset;
        return 1;
    }

    if (fDiscr < 0.0f)
    {
        fDist = xsqrt(-0.33333334f * fA);
        fTemp = xsqrt(-fDiscr);
        fAngle = 0.33333334f * xAngleClamp(xatan2(fTemp, -fHalfB));
        fCos = icos(fAngle);
        fSin = isin(fAngle);
        *x1 = (2.0f * fDist * fCos) - fOffset;
        *x2 = (-fDist * (fCos + 1.7320508f * fSin)) - fOffset;
        *x3 = (-fDist * (fCos - 1.7320508f * fSin)) - fOffset;
        return 3;
    }

    fTemp = (fHalfB >= 0.0f) ? -xpow(fHalfB, 0.33333334f) : xpow(-fHalfB, 0.33333334f);

    *x1 = (2.0f * fTemp) - fOffset;
    *x2 = -fTemp - fOffset;
    return 2;
}

// Wraps an angle into [0, 2pi).
F32 xAngleClamp(F32 a)
{
    F32 b = fmod(a, 2 * PI);

    if (b < 0.0f)
    {
        b += 2 * PI;
    }

    return b;
}

// Wraps an angle difference into [-pi, pi).
F32 xDangleClamp(F32 a)
{
    F32 b = fmod(a, 2 * PI);

    if (b >= PI)
    {
        b -= 2 * PI;
    }
    else if (b < -PI)
    {
        b += 2 * PI;
    }

    return b;
}
