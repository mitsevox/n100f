#include <rtslerp.h>

// The cosine of the angle between two rotations above which the angle counts as zero and RtQuatSlerp
// interpolates the quaternions linearly.
#define SLERP_NEARLYZEROCOS 0.99999f

// The rational approximation fdlibm's acos uses on z = x * x.
#define ACOSF_RATIO(z)                                                                             \
    (((z) * (_RW_pS0 +                                                                             \
             (z) * (_RW_pS1 + (z) * (_RW_pS2 + (z) * (_RW_pS3 + (z) * (_RW_pS4 + (z) * _RW_pS5)))))) / \
     (_RW_one + (z) * (_RW_qS1 + (z) * (_RW_qS2 + (z) * (_RW_qS3 + (z) * _RW_qS4)))))

// A single-precision acos in the manner of fdlibm, without the special cases for NaN.
#define ACOSF(_result, _x)                                                                         \
    do                                                                                             \
    {                                                                                              \
        RwSplitBits xBits;                                                                         \
        S32 hx;                                                                                    \
        S32 ix;                                                                                    \
                                                                                                   \
        xBits.nReal = (_x);                                                                        \
        hx = xBits.nInt;                                                                           \
        ix = hx & 0x7FFFFFFF;                                                                      \
                                                                                                   \
        if (ix >= 0x3F800000)                                                                      \
        {                                                                                          \
            /* |x| >= 1 */                                                                         \
            if (hx > 0)                                                                            \
            {                                                                                      \
                (_result) = 0.0f;                                                                  \
            }                                                                                      \
            else                                                                                   \
            {                                                                                      \
                (_result) = _RW_pi + 2.0f * _RW_pio2_lo;                                           \
            }                                                                                      \
        }                                                                                          \
        else if (ix < 0x3F000000)                                                                  \
        {                                                                                          \
            /* |x| < 0.5 */                                                                        \
            if (ix <= 0x23000000)                                                                  \
            {                                                                                      \
                (_result) = _RW_pio2_hi + _RW_pio2_lo;                                             \
            }                                                                                      \
            else                                                                                   \
            {                                                                                      \
                F32 z = (_x) * (_x);                                                               \
                                                                                                   \
                (_result) = _RW_pio2_hi - ((_x) - (_RW_pio2_lo - (_x) * ACOSF_RATIO(z)));          \
            }                                                                                      \
        }                                                                                          \
        else if (hx < 0)                                                                           \
        {                                                                                          \
            /* x < -0.5 */                                                                         \
            F32 z = 0.5f * (_RW_one + (_x));                                                       \
            F32 s = _rwSqrt(z);                                                                    \
                                                                                                   \
            (_result) = _RW_pi - 2.0f * (s + (ACOSF_RATIO(z) * s - _RW_pio2_lo));                  \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            /* x > 0.5 */                                                                          \
            F32 z = 0.5f * (_RW_one - (_x));                                                       \
            F32 s = _rwSqrt(z);                                                                    \
            RwSplitBits sBits;                                                                     \
            RwSplitBits dfBits;                                                                    \
            F32 df;                                                                                \
            F32 c;                                                                                 \
                                                                                                   \
            sBits.nReal = s;                                                                       \
            dfBits.nInt = sBits.nInt & 0xFFFFF000;                                                 \
            df = dfBits.nReal;                                                                     \
            c = (z - df * df) / (s + df);                                                          \
                                                                                                   \
            (_result) = 2.0f * (df + (c + ACOSF_RATIO(z) * s));                                    \
        }                                                                                          \
    } while (0)

// Fills a slerp cache for interpolating from one rotation to another: the two rotations, with the
// second negated where that takes the shorter way round, and the angle between them. Unless the
// angle is nearly zero, both rotations are divided by its sine, so that RtQuatSlerp only has to
// scale them by the sines of the two partial angles.
void RtQuatSetupSlerpCache(RtQuat* qpFrom, RtQuat* qpTo, RtQuatSlerpCache* sCache)
{
    F32 cosOm;

    sCache->raFrom = *qpFrom;

    cosOm = RwV3dDotProductMacro(&qpFrom->imag, &qpTo->imag) + qpFrom->real * qpTo->real;

    if (cosOm < 0.0f)
    {
        cosOm = (cosOm < -1.0f) ? 1.0f : -cosOm;

        sCache->raTo.real = -qpTo->real;
        sCache->raTo.imag.x = -qpTo->imag.x;
        sCache->raTo.imag.y = -qpTo->imag.y;
        sCache->raTo.imag.z = -qpTo->imag.z;
    }
    else
    {
        cosOm = (cosOm > 1.0f) ? 1.0f : cosOm;

        sCache->raTo = *qpTo;
    }

    ACOSF(sCache->omega, cosOm);

    sCache->nearlyZeroOm = (cosOm >= SLERP_NEARLYZEROCOS);

    if (!sCache->nearlyZeroOm)
    {
        F32 sinOm;
        F32 recipSinOm;

        RwSinMinusPiToPiMacro(sinOm, sCache->omega);
        recipSinOm = 1.0f / sinOm;

        sCache->raFrom.real *= recipSinOm;
        sCache->raFrom.imag.x *= recipSinOm;
        sCache->raFrom.imag.y *= recipSinOm;
        sCache->raFrom.imag.z *= recipSinOm;
        sCache->raTo.real *= recipSinOm;
        sCache->raTo.imag.x *= recipSinOm;
        sCache->raTo.imag.y *= recipSinOm;
        sCache->raTo.imag.z *= recipSinOm;
    }
}
