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
    F32 p;
    F32 q;
    F32 offset;
    F32 discr;
    F32 halfq;
    F32 sq;
    F32 u;
    F32 v;
    F32 r;
    F32 angle;
    F32 cs;
    F32 sn;
    F32 cr;

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

    p = 0.33333334f * ((3.0f * c) - b * b);
    q = 0.037037037f * ((27.0f * d) + ((2.0f * (b * b * b)) - (9.0f * b * c)));
    offset = 0.33333334f * b;
    discr = (0.25f * (q * q)) + (0.037037037f * (p * (p * p)));
    halfq = 0.5f * q;

    if (fabsf(discr) < 0.000001f)
    {
        discr = 0.0f;
    }

    if (discr > 0.0f)
    {
        sq = xsqrt(discr);
        u = -halfq + sq;
        if (u >= 0.0f)
        {
            *x1 = xpow(u, 0.33333334f);
        }
        else
        {
            *x1 = -xpow(-u, 0.33333334f);
        }

        v = -halfq - sq;
        if (v >= 0.0f)
        {
            *x1 += xpow(v, 0.33333334f);
        }
        else
        {
            *x1 -= xpow(-v, 0.33333334f);
        }

        *x1 -= offset;
        return 1;
    }

    if (discr < 0.0f)
    {
        r = xsqrt(-0.33333334f * p);
        sq = xsqrt(-discr);
        angle = 0.33333334f * xAngleClamp(xatan2(sq, -halfq));
        cs = icos(angle);
        sn = isin(angle);
        *x1 = (2.0f * r * cs) - offset;
        *x2 = (-r * (cs + 1.7320508f * sn)) - offset;
        *x3 = (-r * (cs - 1.7320508f * sn)) - offset;
        return 3;
    }

    cr = (halfq >= 0.0f) ? -xpow(halfq, 0.33333334f) : xpow(-halfq, 0.33333334f);

    *x1 = (2.0f * cr) - offset;
    *x2 = -cr - offset;
    return 2;
}

// Wraps an angle into [0, 2pi).
F32 xAngleClamp(F32 a)
{
    F32 rem = fmod(a, 2 * PI);

    if (rem < 0.0f)
    {
        rem += 2 * PI;
    }

    return rem;
}

// Wraps an angle difference into [-pi, pi).
F32 xDangleClamp(F32 a)
{
    F32 rem = fmod(a, 2 * PI);

    if (rem >= PI)
    {
        rem -= 2 * PI;
    }
    else if (rem < -PI)
    {
        rem += 2 * PI;
    }

    return rem;
}
