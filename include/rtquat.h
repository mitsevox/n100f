#ifndef RTQUAT_H
#define RTQUAT_H

#include "rwcore.h"

typedef struct RtQuat RtQuat;
struct RtQuat
{
    RwV3d imag;
    F32 real;
};

#define RtQuatModulusSquaredMacro(q)                                                               \
    ((q)->real * (q)->real + RwV3dDotProductMacro(&(q)->imag, &(q)->imag))

// Assumes q1 != result != q2.
#define RtQuatMultiplyMacro(result, q1, q2)                                                        \
    do                                                                                             \
    {                                                                                              \
        (result)->real = (q1)->real * (q2)->real - RwV3dDotProductMacro(&(q1)->imag, &(q2)->imag); \
        RwV3dCrossProductMacro(&(result)->imag, &(q1)->imag, &(q2)->imag);                         \
        RwV3dIncrementScaledMacro(&(result)->imag, &(q2)->imag, (q1)->real);                       \
        RwV3dIncrementScaledMacro(&(result)->imag, &(q1)->imag, (q2)->real);                       \
    } while (0)

// Assumes result != q.
#define RtQuatReciprocalMacro(result, q)                                                           \
    do                                                                                             \
    {                                                                                              \
        F32 val = RtQuatModulusSquaredMacro(q);                                                    \
                                                                                                   \
        if (val > (F32)0)                                                                          \
        {                                                                                          \
            val = ((F32)1) / val;                                                                  \
            (result)->real = (q)->real * val;                                                      \
            val = -val;                                                                            \
            RwV3dScaleMacro(&(result)->imag, &(q)->imag, val);                                     \
        }                                                                                          \
    } while (0)

#define RtQuatUnitConvertToMatrixMacro(qpQuat, mpMatrix)                                           \
    do                                                                                             \
    {                                                                                              \
        const F32 x = (qpQuat)->imag.x;                                                            \
        const F32 y = (qpQuat)->imag.y;                                                            \
        const F32 z = (qpQuat)->imag.z;                                                            \
        const F32 w = (qpQuat)->real;                                                              \
        RwV3d square;                                                                              \
        RwV3d cross;                                                                               \
        RwV3d wimag;                                                                               \
                                                                                                   \
        square.x = x * x;                                                                          \
        square.y = y * y;                                                                          \
        square.z = z * z;                                                                          \
                                                                                                   \
        cross.x = y * z;                                                                           \
        cross.y = z * x;                                                                           \
        cross.z = x * y;                                                                           \
                                                                                                   \
        wimag.x = w * x;                                                                           \
        wimag.y = w * y;                                                                           \
        wimag.z = w * z;                                                                           \
                                                                                                   \
        (mpMatrix)->right.x = 1 - 2 * (square.y + square.z);                                       \
        (mpMatrix)->right.y = 2 * (cross.z + wimag.z);                                             \
        (mpMatrix)->right.z = 2 * (cross.y - wimag.y);                                             \
                                                                                                   \
        (mpMatrix)->up.x = 2 * (cross.z - wimag.z);                                                \
        (mpMatrix)->up.y = 1 - 2 * (square.x + square.z);                                          \
        (mpMatrix)->up.z = 2 * (cross.x + wimag.x);                                                \
                                                                                                   \
        (mpMatrix)->at.x = 2 * (cross.y + wimag.y);                                                \
        (mpMatrix)->at.y = 2 * (cross.x - wimag.x);                                                \
        (mpMatrix)->at.z = (1 - 2 * (square.x + square.y));                                        \
                                                                                                   \
        (mpMatrix)->pos.x = ((F32)0);                                                              \
        (mpMatrix)->pos.y = ((F32)0);                                                              \
        (mpMatrix)->pos.z = ((F32)0);                                                              \
                                                                                                   \
        (mpMatrix)->flags = rwMATRIXTYPEORTHONORMAL & ~rwMATRIXINTERNALIDENTITY;                   \
    } while (0)

#endif
