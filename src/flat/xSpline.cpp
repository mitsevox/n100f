#include "xSpline.h"
#include "xMemMgr.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

struct xCoef
{
    F32 a[4];
};

struct xCoef3
{
    xCoef x;
    xCoef y;
    xCoef z;
};

static F32 sBasisBezier[4][4] = {
    { -1.0f, 3.0f, -3.0f, 1.0f },
    { 3.0f, -6.0f, 3.0f, 0.0f },
    { -3.0f, 3.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f, 0.0f },
};

static F32 sBasisHermite[4][4] = {
    { 2.0f, -3.0f, 0.0f, 1.0f },
    { 1.0f, -2.0f, 1.0f, 0.0f },
    { 1.0f, -1.0f, 0.0f, 0.0f },
    { -2.0f, 3.0f, 0.0f, 0.0f },
};

static void Tridiag_Solve(F32* a, F32* b, F32* c, xVec3* d, xVec3* x, S32 n);
static inline void Interpolate_Bspline(xVec3* data, xVec3* control, F32* knots, U32 nodata);
static F32 ArcLength3(xCoef3* coef, F64 ustart, F64 uend);
static inline void EvalCoef3(xCoef3* coef, F32 u, U32 deriv, xVec3* o);
static inline void BasisToCoef3(xCoef3* coef, F32 (*N)[4], xVec3* v1, xVec3* v2, xVec3* v3, xVec3* v4);
static inline void CoefToUnity3(xCoef3* dest, xCoef3* coef, F32 t1, F32 t2);
static inline void BasisBspline(F32 (*N)[4], F32* t);
static inline U32 SegBspline(xSpline3* spl, F32 u);
void xSpline3_Catmullize(xSpline3* spl);
void xSpline3_Update(xSpline3* spl);
static inline xSpline3* AllocSpline3(xVec3* points, F32* time, U32 numpoints, U32 numalloc, U32 flags,
                              U32 type);
xSpline3* xSpline3_Bezier(xVec3* points, F32* time, U32 numpoints, U32 numalloc, xVec3* p1, xVec3* p2);
static xCoef3* CoefSeg3(xSpline3* spl, U32 seg, xCoef3* tempCoef);
void xSpline3_ArcInit(xSpline3* spl, U32 sample);
static F32 ArcEvalIterate(xSpline3* spl, F32 s, U32 deriv, xVec3* o, U32 iterations);
F32 xSpline3_EvalArcApprox(xSpline3* spl, F32 s, U32 deriv, xVec3* o);
static void EvalBspline3(xSpline3* spl, F32 u, U32 deriv, xVec3* o);
void xSpline3_EvalSeg(xSpline3* spl, F32 u, U32 deriv, xVec3* o);

static void Tridiag_Solve(F32* a, F32* b, F32* c, xVec3* d, xVec3* x, S32 n)
{
    S32 j;
    S32 k;
    F32 beta;
    F32* gamma;
    xVec3* delta;

    gamma = (F32*)malloc(n * sizeof(F32));
    delta = (xVec3*)malloc(n * sizeof(xVec3));

    gamma[0] = c[0] / b[0];
    for (k = 0; k < 3; k++)
    {
        (&delta[0].x)[k] = (&d[0].x)[k] / b[0];
    }

    for (j = 1; j < n; j++)
    {
        beta = b[j] - a[j] * gamma[j - 1];
        gamma[j] = c[j] / beta;
        for (k = 0; k < 3; k++)
        {
            (&delta[j].x)[k] = ((&d[j].x)[k] - a[j] * (&delta[j - 1].x)[k]) / beta;
        }
    }

    for (k = 0; k < 3; k++)
    {
        (&x[n - 1].x)[k] = (&delta[n - 1].x)[k];
    }

    for (j = n - 2; j >= 0; j--)
    {
        for (k = 0; k < 3; k++)
        {
            (&x[j].x)[k] = (&delta[j].x)[k] - gamma[j] * (&x[j + 1].x)[k];
        }
    }

    free(gamma);
    free(delta);
}

// Solves for the B-spline control points that pass through the data points; the end control
// points repeat their neighbours.
static inline void Interpolate_Bspline(xVec3* data, xVec3* control, F32* knots, U32 nodata)
{
    F32* alpha;
    F32* beta;
    F32* gamma;
    U32 i;
    F32 t1;
    F32 t2;
    F32 t3;
    F32 t4;
    F32 t5;

    alpha = (F32*)malloc(nodata * sizeof(F32));
    beta = (F32*)malloc(nodata * sizeof(F32));
    gamma = (F32*)malloc(nodata * sizeof(F32));

    alpha[nodata - 1] = 0.0f;
    alpha[0] = 0.0f;
    beta[nodata - 1] = 1.0f;
    beta[0] = 1.0f;
    gamma[nodata - 1] = 0.0f;
    gamma[0] = 0.0f;

    for (i = 1; i < nodata - 1; i++)
    {
        t1 = knots[i + 1];
        t2 = knots[i + 2];
        t3 = knots[i + 3];
        t4 = knots[i + 4];
        t5 = knots[i + 5];

        alpha[i] = (t4 - t3) * (t4 - t3) / (t4 - t1);
        beta[i] = (t3 - t1) * (t4 - t3) / (t4 - t1) + (t5 - t3) * (t3 - t2) / (t5 - t2);
        gamma[i] = (t3 - t2) * (t3 - t2) / (t5 - t2);

        alpha[i] /= t4 - t2;
        beta[i] /= t4 - t2;
        gamma[i] /= t4 - t2;
    }

    Tridiag_Solve(alpha, beta, gamma, data, control + 1, nodata);

    control[0] = control[1];
    control[nodata + 1] = control[nodata];

    free(alpha);
    free(beta);
    free(gamma);
}

// Arc length of one cubic segment between two parameters, by Simpson's rule over 50 steps.
static F32 ArcLength3(xCoef3* coef, F64 ustart, F64 uend)
{
    U32 i;
    F64 u;
    F64 sum;
    F64 h;
    F64 E;
    F64 D;
    F64 C;
    F64 B;
    F64 A;
    F64 xa = coef->x.a[0];
    F64 xb = coef->x.a[1];
    F64 xc = coef->x.a[2];
    F64 ya = coef->y.a[0];
    F64 yb = coef->y.a[1];
    F64 yc = coef->y.a[2];
    F64 za = coef->z.a[0];
    F64 zb = coef->z.a[1];
    F64 zc = coef->z.a[2];

    E = 9.0 * (za * za + (xa * xa + ya * ya));
    D = 12.0 * (za * zb + (xa * xb + ya * yb));
    C = 6.0 * (za * zc + (xa * xc + ya * yc)) + 4.0 * (zb * zb + (xb * xb + yb * yb));
    B = 4.0 * (zb * zc + (xb * xc + yb * yc));
    A = zc * zc + (xc * xc + yc * yc);

    h = (uend - ustart) / 50.0;
    sum = 0.0;
    u = ustart + h;

    for (i = 2; i <= 50; i++)
    {
        if (!(i & 1))
        {
            sum += 4.0 * sqrt(u * (u * (u * (E * u + D) + C) + B) + A);
        }
        else
        {
            sum += 2.0 * sqrt(u * (u * (u * (E * u + D) + C) + B) + A);
        }
        u += h;
    }

    return h *
           (sum + sqrt(ustart * (ustart * (ustart * (E * ustart + D) + C) + B) + A) +
            sqrt(uend * (uend * (uend * (E * uend + D) + C) + B) + A)) /
           3.0;
}

static void EvalBspline3(xSpline3* spl, F32 u, U32 deriv, xVec3* o)
{
    F32 N[7][4];
    xCoef3 coef;
    U32 seg;

    if (u < 0.0f)
    {
        u = 0.0f;
    }
    if (u > spl->knot[spl->N + 3])
    {
        u = spl->knot[spl->N + 3];
    }

    seg = SegBspline(spl, u);
    BasisBspline(N, &spl->knot[seg]);
    BasisToCoef3(&coef, N, spl->bctrl + seg, spl->bctrl + seg + 1, spl->bctrl + seg + 2,
                 spl->bctrl + seg + 3);
    EvalCoef3(&coef, u, deriv, o);
}

static xCoef3* CoefSeg3(xSpline3* spl, U32 seg, xCoef3* tempCoef)
{
    switch (spl->type)
    {
    case 1:
        return &spl->coef[seg];
    case 2:
        BasisToCoef3(tempCoef, sBasisHermite, spl->points + seg, spl->p12 + seg * 2,
                     spl->p12 + seg * 2 + 1, spl->points + seg + 1);
        break;
    case 3:
        BasisToCoef3(tempCoef, sBasisBezier, spl->points + seg, spl->p12 + seg * 2,
                     spl->p12 + seg * 2 + 1, spl->points + seg + 1);
        break;
    case 4:
    {
        F32 N[7][4];
        BasisBspline(N, &spl->knot[seg]);
        BasisToCoef3(tempCoef, N, spl->bctrl + seg, spl->bctrl + seg + 1, spl->bctrl + seg + 2,
                     spl->bctrl + seg + 3);
        CoefToUnity3(tempCoef, tempCoef, spl->knot[seg + 3], spl->knot[seg + 4]);
        break;
    }
    }
    return tempCoef;
}

void xSpline3_EvalSeg(xSpline3* spl, F32 u, U32 deriv, xVec3* o)
{
    xCoef3 tempCoef;
    F32 flr;
    U32 seg;

    if (spl->type == 4)
    {
        EvalBspline3(spl, u, deriv, o);
        return;
    }

    if (u < 0.0f)
    {
        u = 0.0f;
    }
    flr = floor(u);
    seg = flr;
    if (seg >= spl->N)
    {
        u = 1.0f;
        seg = spl->N - 1;
    }
    else
    {
        u -= flr;
    }

    switch (spl->type)
    {
    case 1:
        EvalCoef3(&spl->coef[seg], u, deriv, o);
        break;
    case 2:
        BasisToCoef3(&tempCoef, sBasisHermite, spl->points + seg, spl->p12 + seg * 2,
                     spl->p12 + seg * 2 + 1, spl->points + seg + 1);
        EvalCoef3(&tempCoef, u, deriv, o);
        break;
    case 3:
        BasisToCoef3(&tempCoef, sBasisBezier, spl->points + seg, spl->p12 + seg * 2,
                     spl->p12 + seg * 2 + 1, spl->points + seg + 1);
        EvalCoef3(&tempCoef, u, deriv, o);
        break;
    }
}

// Maps arc length s to the spline parameter through the sampled arc-length table: bisects the
// sample interval `iterations` times, interpolates linearly, evaluates the point (or derivative)
// there and returns the parameter.
static F32 ArcEvalIterate(xSpline3* spl, F32 s, U32 deriv, xVec3* o, U32 iterations)
{
    xCoef3 tempCoef;
    xCoef3* coef;
    S32 segmul;
    S32 seg;
    S32 test;
    S32 max;
    S32 min;
    F32 arctest;
    F32 utest;
    F32 smax;
    F32 smin;
    F32 umax;
    F32 umin;
    F32 sbase;
    S32 sample = spl->arcSample;

    min = -1;
    max = sample * spl->N - 1;
    while (min + 1 != max)
    {
        test = (min + max) >> 1;
        if (s > spl->arcLength[test])
        {
            min = test;
        }
        else
        {
            max = test;
        }
    }

    seg = max / sample;
    segmul = seg * sample;
    umin = (F32)(max - segmul) / spl->arcSample;
    umax = (F32)(max + 1 - segmul) / spl->arcSample;
    smin = (max - 1 >= 0) ? spl->arcLength[max - 1] : 0.0f;
    smax = spl->arcLength[max];
    sbase = (segmul - 1 >= 0) ? spl->arcLength[segmul - 1] : 0.0f;

    coef = CoefSeg3(spl, seg, &tempCoef);

    if (s <= smin)
    {
        EvalCoef3(coef, umin, deriv, o);
        return (F32)seg + umin;
    }

    if (s >= smax)
    {
        EvalCoef3(coef, umax, deriv, o);
        return (F32)seg + umax;
    }

    s -= sbase;
    smin -= sbase;
    smax -= sbase;

    while (iterations != 0)
    {
        utest = 0.5f * (umax - umin) + umin;
        arctest = ArcLength3(coef, 0.0, utest);
        if (s > arctest)
        {
            umin = utest;
            smin = arctest;
        }
        else
        {
            umax = utest;
            smax = arctest;
        }
        iterations--;
    }

    if (smax - smin == 0.0f)
    {
        utest = umin;
    }
    else
    {
        utest = umin + (umax - umin) * (s - smin) / (smax - smin);
        if (utest < 0.0f)
        {
            utest = 0.0f;
        }
        else if (utest > 1.0f)
        {
            utest = 1.0f;
        }
    }

    EvalCoef3(coef, utest, deriv, o);
    return (F32)seg + utest;
}

F32 xSpline3_EvalArcApprox(xSpline3* spl, F32 s, U32 deriv, xVec3* o)
{
    if (spl->arcLength != NULL)
    {
        return ArcEvalIterate(spl, s, deriv, o, 0);
    }
    xSpline3_EvalSeg(spl, s, deriv, o);
    return s;
}

void xSpline3_ArcInit(xSpline3* spl, U32 sample)
{
    xCoef3 tempCoef;
    F32 len;
    F32 arcsum;
    U32 i;
    U32 seg;

    len = 0.0f;
    if (sample < 1)
    {
        sample = 1;
    }

    spl->arcSample = sample;
    if ((spl->arcLength != NULL ? spl->allocN * spl->arcSample : 0) < spl->N * sample)
    {
        spl->arcLength = (F32*)xMemAlloc(gActiveHeap, spl->arcSample * spl->allocN * sizeof(F32));
    }

    arcsum = 0.0f;
    for (seg = 0; seg < spl->N; seg++)
    {
        xCoef3* coef = CoefSeg3(spl, seg, &tempCoef);
        for (i = 0; i < sample; i++)
        {
            len = ArcLength3(coef, 0.0, (F32)(i + 1) / (F32)sample);
            spl->arcLength[i + seg * sample] = arcsum + len;
        }
        arcsum += len;
    }
}

static inline xSpline3* AllocSpline3(xVec3* points, F32* time, U32 numpoints, U32 numalloc, U32 flags,
                              U32 type)
{
    xSpline3* spl = (xSpline3*)xMemAlloc(gActiveHeap, sizeof(xSpline3));

    if (numalloc < numpoints)
    {
        numalloc = numpoints;
    }

    spl->type = type;
    spl->flags = flags;
    spl->N = numpoints - 1;
    spl->allocN = numalloc - 1;
    spl->p12 = NULL;
    spl->bctrl = NULL;
    spl->knot = NULL;
    spl->coef = NULL;
    spl->arcSample = 0;
    spl->arcLength = NULL;

    spl->points = (xVec3*)xMemAlloc(gActiveHeap, (spl->allocN + 1) * sizeof(xVec3));
    memcpy(spl->points, points, (spl->N + 1) * sizeof(xVec3));

    if (time != NULL)
    {
        spl->time = (F32*)xMemAlloc(gActiveHeap, (spl->allocN + 1) * sizeof(F32));
        memcpy(spl->time, time, (spl->N + 1) * sizeof(F32));
    }
    else
    {
        spl->time = NULL;
    }

    return spl;
}

xSpline3* xSpline3_Bezier(xVec3* points, F32* time, U32 numpoints, U32 numalloc, xVec3* p1, xVec3* p2)
{
    U32 i;
    xSpline3* spl = AllocSpline3(points, time, numpoints, numalloc, 0, 3);

    spl->p12 = (xVec3*)xMemAlloc(gActiveHeap, spl->allocN * 2 * sizeof(xVec3));

    if (p1 == NULL || p2 == NULL)
    {
        xSpline3_Catmullize(spl);
    }
    else
    {
        for (i = 0; i < spl->N; i++)
        {
            spl->p12[2 * i] = p1[i];
            spl->p12[2 * i + 1] = p2[i];
        }
    }

    return spl;
}

void xSpline3_Update(xSpline3* spl)
{
    if (spl->type == 4)
    {
        Interpolate_Bspline(spl->points, spl->bctrl, spl->knot, spl->N + 1);
    }
    if (spl->arcLength != NULL)
    {
        xSpline3_ArcInit(spl, spl->arcSample);
    }
}

static inline void EvalCoef3(xCoef3* coef, F32 u, U32 deriv, xVec3* o)
{
    switch (deriv)
    {
    case 0:
        o->x = u * (u * (coef->x.a[0] * u + coef->x.a[1]) + coef->x.a[2]) + coef->x.a[3];
        o->y = u * (u * (coef->y.a[0] * u + coef->y.a[1]) + coef->y.a[2]) + coef->y.a[3];
        o->z = u * (u * (coef->z.a[0] * u + coef->z.a[1]) + coef->z.a[2]) + coef->z.a[3];
        break;
    case 1:
        o->x = u * (2.0f * coef->x.a[1] + 3.0f * coef->x.a[0] * u) + coef->x.a[2];
        o->y = u * (2.0f * coef->y.a[1] + 3.0f * coef->y.a[0] * u) + coef->y.a[2];
        o->z = u * (2.0f * coef->z.a[1] + 3.0f * coef->z.a[0] * u) + coef->z.a[2];
        break;
    case 2:
        o->x = 2.0f * coef->x.a[1] + 6.0f * coef->x.a[0] * u;
        o->y = 2.0f * coef->y.a[1] + 6.0f * coef->y.a[0] * u;
        o->z = 2.0f * coef->z.a[1] + 6.0f * coef->z.a[0] * u;
        break;
    case 3:
        o->x = 6.0f * coef->x.a[0];
        o->y = 6.0f * coef->y.a[0];
        o->z = 6.0f * coef->z.a[0];
        break;
    default:
        o->x = 0.0f;
        o->y = 0.0f;
        o->z = 0.0f;
        break;
    }
}

static inline void BasisToCoef3(xCoef3* coef, F32 (*N)[4], xVec3* v1, xVec3* v2, xVec3* v3, xVec3* v4)
{
    U32 i;

    for (i = 0; i < 4; i++)
    {
        coef->x.a[i] = N[3][i] * v4->x + (N[2][i] * v3->x + (v1->x * N[0][i] + N[1][i] * v2->x));
        coef->y.a[i] = N[3][i] * v4->y + (N[2][i] * v3->y + (v1->y * N[0][i] + N[1][i] * v2->y));
        coef->z.a[i] = N[3][i] * v4->z + (N[2][i] * v3->z + (v1->z * N[0][i] + N[1][i] * v2->z));
    }
}

static inline void CoefToUnity3(xCoef3* dest, xCoef3* coef, F32 t1, F32 t2)
{
    F32 dt;
    F32* o;
    F32* p;
    F32 d;
    F32 c;
    F32 b;
    F32 a;
    U32 i;

    dt = t2 - t1;
    for (i = 0; i < 3; i++)
    {
        p = coef->x.a + i * 4;
        o = dest->x.a + i * 4;

        a = p[0];
        b = p[1];
        c = p[2];
        d = p[3];

        o[0] = dt * (dt * (a * dt));
        o[1] = t1 * (dt * (3.0f * a * dt)) + dt * (b * dt);
        o[2] = c * dt + (t1 * (t1 * (3.0f * a * dt)) + t1 * (2.0f * b * dt));
        o[3] = d + (c * t1 + (t1 * (t1 * (a * t1)) + t1 * (b * t1)));
    }
}

static inline void BasisBspline(F32 (*N)[4], F32* t)
{
    F32 d2;
    F32 d1;
    U32 c;
    U32 k;
    U32 i;

    N[0][3] = 0.0f;
    N[1][3] = 0.0f;
    N[2][3] = 0.0f;
    N[3][3] = 1.0f;
    N[4][3] = 0.0f;
    N[5][3] = 0.0f;
    N[6][3] = 0.0f;

    for (i = 2; i <= 4; i++)
    {
        for (k = 0; k < 8 - i; k++)
        {
            F32 Ntemp[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

            d2 = t[k + i - 1] - t[k];
            if (d2 != 0.0f)
            {
                d2 = 1.0f / d2;
            }
            d1 = t[k + i] - t[k + 1];
            if (d1 != 0.0f)
            {
                d1 = 1.0f / d1;
            }

            for (c = 5 - i; c <= 3; c++)
            {
                Ntemp[c - 1] += d2 * N[k][c] - d1 * N[k + 1][c];
                Ntemp[c] += d2 * (-t[k] * N[k][c]) + d1 * (t[k + i] * N[k + 1][c]);
            }

            N[k][0] = Ntemp[0];
            N[k][1] = Ntemp[1];
            N[k][2] = Ntemp[2];
            N[k][3] = Ntemp[3];
        }
    }
}

static inline U32 SegBspline(xSpline3* spl, F32 u)
{
    U32 mid;
    U32 max;
    U32 min;

    min = 3;
    max = spl->N + 3;
    while (min + 1 != max)
    {
        mid = (min + max) >> 1;
        if (spl->knot[mid] >= u)
        {
            max = mid;
        }
        else
        {
            min = mid;
        }
    }

    return min - 3;
}

void xSpline3_Catmullize(xSpline3* spl)
{
    xSpline3_Update(spl);
}
