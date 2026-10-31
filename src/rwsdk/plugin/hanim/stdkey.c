#include <rphanim.h>

// The bytes a standard keyframe takes in a stream: the time, the rotation and the translation,
// then the offset of the previous keyframe.
#define STDKEYFRAME_STREAMSIZE (sizeof(F32) * 8 + sizeof(S32))

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

void RpHAnimStdKeyFrameToMatrix(void* matrix, void* voidIFrame)
{
    RwMatrix* mpMatrix = (RwMatrix*)matrix;
    HAnimStdKey* iFrame = (HAnimStdKey*)voidIFrame;

    RtQuatUnitConvertToMatrixMacro(&iFrame->rotation, mpMatrix);
    mpMatrix->pos.x = iFrame->translation.x;
    mpMatrix->pos.y = iFrame->translation.y;
    mpMatrix->pos.z = iFrame->translation.z;
}

void RpHAnimStdKeyFrameInterpolate(void* voidOut, void* voidIn1, void* voidIn2, F32 time)
{
    HAnimStdKey* out = (HAnimStdKey*)voidOut;
    HAnimStdKey* in1 = (HAnimStdKey*)voidIn1;
    HAnimStdKey* in2 = (HAnimStdKey*)voidIn2;
    F32 fTime = (time - in1->time) / (in2->time - in1->time);
    F32 cosTheta;
    F32 fromScale;
    F32 toScale;

    cosTheta = RwV3dDotProductMacro(&in1->rotation.imag, &in2->rotation.imag) +
               in1->rotation.real * in2->rotation.real;
    out->translation.x = in1->translation.x + fTime * (in2->translation.x - in1->translation.x);
    out->translation.y = in1->translation.y + fTime * (in2->translation.y - in1->translation.y);
    out->translation.z = in1->translation.z + fTime * (in2->translation.z - in1->translation.z);
    out->time = time;

    if (cosTheta < 0.0f)
    {
        cosTheta = -cosTheta;
        in2->rotation.imag.x = -in2->rotation.imag.x;
        in2->rotation.imag.y = -in2->rotation.imag.y;
        in2->rotation.imag.z = -in2->rotation.imag.z;
        in2->rotation.real = -in2->rotation.real;
    }

    fromScale = 1.0f - fTime;
    toScale = fTime;

    if (cosTheta < 0.999f)
    {
        F32 theta;
        F32 sinTheta;
        F32 recipSinTheta;

        ACOSF(theta, cosTheta);
        RwSinMinusPiToPiMacro(sinTheta, theta);
        recipSinTheta = 1.0f / sinTheta;
        fromScale *= theta;
        RwSinMinusPiToPiMacro(fromScale, fromScale);
        fromScale *= recipSinTheta;
        toScale *= theta;
        RwSinMinusPiToPiMacro(toScale, toScale);
        toScale *= recipSinTheta;
    }

    out->rotation.imag.x = fromScale * in1->rotation.imag.x + toScale * in2->rotation.imag.x;
    out->rotation.imag.y = fromScale * in1->rotation.imag.y + toScale * in2->rotation.imag.y;
    out->rotation.imag.z = fromScale * in1->rotation.imag.z + toScale * in2->rotation.imag.z;
    out->rotation.real = fromScale * in1->rotation.real + toScale * in2->rotation.real;
}

void RpHAnimStdKeyFrameBlend(void* voidOut, void* voidIn1, void* voidIn2, F32 alpha)
{
    HAnimStdKey* out = (HAnimStdKey*)voidOut;
    HAnimStdKey* in1 = (HAnimStdKey*)voidIn1;
    HAnimStdKey* in2 = (HAnimStdKey*)voidIn2;
    F32 cosTheta;
    F32 fromScale;
    F32 toScale;

    cosTheta = RwV3dDotProductMacro(&in1->rotation.imag, &in2->rotation.imag) +
               in1->rotation.real * in2->rotation.real;
    out->translation.x = in1->translation.x + alpha * (in2->translation.x - in1->translation.x);
    out->translation.y = in1->translation.y + alpha * (in2->translation.y - in1->translation.y);
    out->translation.z = in1->translation.z + alpha * (in2->translation.z - in1->translation.z);

    if (cosTheta < 0.0f)
    {
        cosTheta = -cosTheta;
        in2->rotation.imag.x = -in2->rotation.imag.x;
        in2->rotation.imag.y = -in2->rotation.imag.y;
        in2->rotation.imag.z = -in2->rotation.imag.z;
        in2->rotation.real = -in2->rotation.real;
    }

    fromScale = 1.0f - alpha;
    toScale = alpha;

    if (cosTheta < 0.999f)
    {
        F32 theta;
        F32 sinTheta;
        F32 recipSinTheta;

        ACOSF(theta, cosTheta);
        RwSinMinusPiToPiMacro(sinTheta, theta);
        recipSinTheta = 1.0f / sinTheta;
        fromScale *= theta;
        RwSinMinusPiToPiMacro(fromScale, fromScale);
        fromScale *= recipSinTheta;
        toScale *= theta;
        RwSinMinusPiToPiMacro(toScale, toScale);
        toScale *= recipSinTheta;
    }

    out->rotation.imag.x = fromScale * in1->rotation.imag.x + toScale * in2->rotation.imag.x;
    out->rotation.imag.y = fromScale * in1->rotation.imag.y + toScale * in2->rotation.imag.y;
    out->rotation.imag.z = fromScale * in1->rotation.imag.z + toScale * in2->rotation.imag.z;
    out->rotation.real = fromScale * in1->rotation.real + toScale * in2->rotation.real;
}

RpHAnimAnimation* RpHAnimStdKeyFrameStreamRead(RwStream* stream, RpHAnimAnimation* animation)
{
    HAnimStdKey* frame;
    HAnimStdKey* frames;
    S32 i;

    animation->pFrames = RwMalloc(animation->numFrames * sizeof(HAnimStdKey));
    frames = (HAnimStdKey*)animation->pFrames;

    if (!frames)
    {
        return NULL;
    }

    frame = frames;

    for (i = 0; i < animation->numFrames; i++)
    {
        S32 prevOffset;

        if (!RwStreamReadReal(stream, &frame->time, sizeof(F32) * 8))
        {
            return NULL;
        }

        if (!RwStreamReadInt32(stream, &prevOffset, sizeof(S32)))
        {
            return NULL;
        }

        frame->prev = &frames[prevOffset / sizeof(HAnimStdKey)];
        frame++;
    }

    return animation;
}

S32 RpHAnimStdKeyFrameStreamWrite(RpHAnimAnimation* animation, RwStream* stream)
{
    HAnimStdKey* frame;
    S32 i;
    HAnimStdKey* frames = (HAnimStdKey*)animation->pFrames;

    frame = frames;

    for (i = 0; i < animation->numFrames; i++)
    {
        S32 prevOffset;

        if (!RwStreamWriteReal(stream, &frame->time, sizeof(F32) * 8))
        {
            return FALSE;
        }

        prevOffset = (U8*)frame->prev - (U8*)frames;

        if (!RwStreamWriteInt32(stream, &prevOffset, sizeof(S32)))
        {
            return FALSE;
        }

        frame++;
    }

    return TRUE;
}

S32 RpHAnimStdKeyFrameStreamGetSize(RpHAnimAnimation* animation)
{
    S32 size = 0;
    S32 i;

    for (i = 0; i < animation->numFrames; i++)
    {
        size += STDKEYFRAME_STREAMSIZE;
    }

    return size;
}

void RpHAnimStdKeyFrameMulRecip(void* voidFrame, void* voidStart)
{
    HAnimStdKey* frame = (HAnimStdKey*)voidFrame;
    HAnimStdKey* start = (HAnimStdKey*)voidStart;
    RtQuat qRecip;
    RtQuat qTemp;

    RtQuatReciprocalMacro(&qRecip, &start->rotation);
    qTemp = frame->rotation;
    RtQuatMultiplyMacro(&frame->rotation, &qRecip, &qTemp);
    RwV3dSubMacro(&frame->translation, &frame->translation, &start->translation);
}

void RpHAnimStdKeyFrameAdd(void* voidOut, void* voidIn1, void* voidIn2)
{
    HAnimStdKey* out = (HAnimStdKey*)voidOut;
    HAnimStdKey* in1 = (HAnimStdKey*)voidIn1;
    HAnimStdKey* in2 = (HAnimStdKey*)voidIn2;

    RtQuatMultiplyMacro(&out->rotation, &in1->rotation, &in2->rotation);
    RwV3dAddMacro(&out->translation, &in1->translation, &in2->translation);
}
