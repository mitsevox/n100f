#include <rwcore.h>

typedef RwV3d* (*VectorMultFn)(RwV3d* pointsOut, const RwV3d* pointsIn, S32 numPoints,
                                 const RwMatrix* matrix);

typedef struct VectorModuleData VectorModuleData;
struct VectorModuleData
{
    U32* sqrtLookup;
    U32* invSqrtLookup;
    VectorMultFn pointsFn;
    VectorMultFn vectorsFn;
};

#define VECTOR_DATA(var) \
    (((VectorModuleData*)((U8*)RwEngineInstance + vectorModule.globalsOffset))->var)

static RwModuleInfo vectorModule;

RwV3d* VectorMultPoint(RwV3d* pointsOut, const RwV3d* pointsIn, S32 numPoints,
                       const RwMatrix* matrix);

static RwV3d* VectorMultVector(RwV3d* pointsOut, const RwV3d* pointsIn, S32 numPoints,
                               const RwMatrix* matrix)
{
    RwV3d* out = pointsOut;

    while (--numPoints >= 0)
    {
        F32 x = pointsIn->x;
        F32 y = pointsIn->y;
        F32 z = pointsIn->z;

        F32 outX = x * matrix->right.x + y * matrix->up.x;
        F32 outY = x * matrix->right.y + y * matrix->up.y;
        F32 outZ = x * matrix->right.z + y * matrix->up.z;

        pointsIn++;
        pointsOut->x = outX + z * matrix->at.x;
        pointsOut->y = outY + z * matrix->at.y;
        pointsOut->z = outZ + z * matrix->at.z;
        pointsOut++;
    }

    return out;
}

S32 _rwVectorSetMultFn(VectorMultFn multPoints, VectorMultFn multVectors)
{
    if (!multPoints)
    {
        multPoints = VectorMultPoint;
    }
    VECTOR_DATA(pointsFn) = multPoints;

    if (!multVectors)
    {
        multVectors = VectorMultVector;
    }
    VECTOR_DATA(vectorsFn) = multVectors;

    return 1;
}

void _rwV3dNormalize(RwV3d* out, const RwV3d* in)
{
    F32 recip = _rwInvSqrt(in->x * in->x + in->y * in->y + in->z * in->z);

    out->x = in->x * recip;
    out->y = in->y * recip;
    out->z = in->z * recip;
}

F32 RwV3dNormalize(RwV3d* out, const RwV3d* in)
{
    F32 length2 = in->x * in->x + in->y * in->y + in->z * in->z;
    F32 length = _rwSqrt(length2);
    F32 recip = _rwInvSqrt(length2);

    out->x = in->x * recip;
    out->y = in->y * recip;
    out->z = in->z * recip;

    if (length <= 0.0f)
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x19);
        RwErrorSet(&err);
    }

    return length;
}

F32 RwV3dLength(const RwV3d* in)
{
    return _rwSqrt(in->x * in->x + in->y * in->y + in->z * in->z);
}

F32 _rwSqrt(F32 num)
{
    U32* bits = (U32*)&num;

    if (*bits)
    {
        *bits += 0x800;
        *bits = ((*bits >> 1) & 0x3FC00000) + VECTOR_DATA(sqrtLookup)[(*bits >> 12) & 0xFFF];
    }

    return num;
}

F32 _rwInvSqrt(F32 num)
{
    U32* bits = (U32*)&num;

    if (*bits)
    {
        *bits += 0x800;
        *bits = ((~*bits >> 1) & 0x3FC00000) + VECTOR_DATA(invSqrtLookup)[(*bits >> 12) & 0xFFF];
    }

    return num;
}

RwV3d* RwV3dTransformPoints(RwV3d* pointsOut, const RwV3d* pointsIn, S32 numPoints,
                            const RwMatrix* matrix)
{
    return VECTOR_DATA(pointsFn)(pointsOut, pointsIn, numPoints, matrix);
}

RwV3d* RwV3dTransformVectors(RwV3d* vectorsOut, const RwV3d* vectorsIn, S32 numPoints,
                             const RwMatrix* matrix)
{
    return VECTOR_DATA(vectorsFn)(vectorsOut, vectorsIn, numPoints, matrix);
}

void* _rwVectorClose(void* instance)
{
    if (VECTOR_DATA(invSqrtLookup))
    {
        ((RwGlobals*)RwEngineInstance)->memoryFuncs.rwfree(VECTOR_DATA(invSqrtLookup));
        VECTOR_DATA(invSqrtLookup) = NULL;
    }

    if (VECTOR_DATA(sqrtLookup))
    {
        ((RwGlobals*)RwEngineInstance)->memoryFuncs.rwfree(VECTOR_DATA(sqrtLookup));
        VECTOR_DATA(sqrtLookup) = NULL;
    }

    vectorModule.numInstances--;

    return instance;
}
