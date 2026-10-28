#include <rwcore.h>
#include <math.h>

// Entries in each square root lookup table: one per mantissa step, for two exponents.
#define SQRTTABLE_SIZE 0x1000

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

static RwV3d* VectorMultPoint(RwV3d* pointsOut, const RwV3d* pointsIn, S32 numPoints,
                              const RwMatrix* matrix)
{
    RwV3d* out = pointsOut;

    while (--numPoints >= 0)
    {
        F32 z, y, x;
        F32 outX, outY, outZ;

        x = pointsIn->x;
        y = pointsIn->y;
        z = pointsIn->z;

        outX = x * matrix->right.x;
        outY = x * matrix->right.y;
        outZ = x * matrix->right.z;
        outX += y * matrix->up.x;
        outY += y * matrix->up.y;
        outZ += y * matrix->up.z;
        outX += z * matrix->at.x;
        outY += z * matrix->at.y;
        outZ += z * matrix->at.z;

        pointsIn++;
        pointsOut->x = outX + matrix->pos.x;
        pointsOut->y = outY + matrix->pos.y;
        pointsOut->z = outZ + matrix->pos.z;
        pointsOut++;
    }

    return out;
}

static RwV3d* VectorMultVector(RwV3d* pointsOut, const RwV3d* pointsIn, S32 numPoints,
                               const RwMatrix* matrix)
{
    RwV3d* out = pointsOut;

    while (--numPoints >= 0)
    {
        F32 z, y, x;
        F32 outX, outY, outZ;

        x = pointsIn->x;
        y = pointsIn->y;
        z = pointsIn->z;

        outX = x * matrix->right.x;
        outY = x * matrix->right.y;
        outZ = x * matrix->right.z;
        outX += y * matrix->up.x;
        outY += y * matrix->up.y;
        outZ += y * matrix->up.z;

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
    RwSplitBits bits;

    bits.nReal = num;

    if (bits.nUInt)
    {
        bits.nUInt += 0x800;
        bits.nUInt =
            ((bits.nUInt >> 1) & 0x3FC00000) + VECTOR_DATA(sqrtLookup)[(bits.nUInt >> 12) & 0xFFF];
    }

    return bits.nReal;
}

F32 _rwInvSqrt(F32 num)
{
    RwSplitBits bits;

    bits.nReal = num;

    if (bits.nUInt)
    {
        bits.nUInt += 0x800;
        bits.nUInt = ((~bits.nUInt >> 1) & 0x3FC00000) +
                     VECTOR_DATA(invSqrtLookup)[(bits.nUInt >> 12) & 0xFFF];
    }

    return bits.nReal;
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

static S32 SqrtTableCreate(void)
{
    U32* table = (U32*)RWSRCGLOBAL(memoryFuncs).rwmalloc(SQRTTABLE_SIZE * sizeof(U32));

    if (!table)
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, SQRTTABLE_SIZE * sizeof(U32));
        RwErrorSet(&err);
        return FALSE;
    }
    else
    {
        RwSplitBits num;
        U32* entry = table + SQRTTABLE_SIZE / 2;
        S32 i;

        num.nReal = 1.0f;

        for (i = 0; i < SQRTTABLE_SIZE / 2; i++)
        {
            RwSplitBits root;
            RwSplitBits value;

            root.nReal = (F32)sqrt(num.nReal);
            value.nUInt = root.nUInt - 0x1FC00000;
            *entry++ = value.nUInt;
            num.nUInt += 0x1000;
        }

        entry = table;

        for (i = 0; i < SQRTTABLE_SIZE / 2; i++)
        {
            RwSplitBits root;
            RwSplitBits value;

            root.nReal = (F32)sqrt(num.nReal);
            value.nUInt = root.nUInt - 0x20000000;
            *entry++ = value.nUInt;
            num.nUInt += 0x1000;
        }

        VECTOR_DATA(sqrtLookup) = table;
        return TRUE;
    }
}

static S32 InvSqrtTableCreate(void)
{
    U32* table = (U32*)RWSRCGLOBAL(memoryFuncs).rwmalloc(SQRTTABLE_SIZE * sizeof(U32));

    if (!table)
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, SQRTTABLE_SIZE * sizeof(U32));
        RwErrorSet(&err);
        return FALSE;
    }
    else
    {
        RwSplitBits num;
        U32* entry = table + SQRTTABLE_SIZE / 2;
        S32 i;

        num.nReal = 1.0f;

        for (i = 0; i < SQRTTABLE_SIZE / 2; i++)
        {
            RwSplitBits root;
            RwSplitBits value;

            root.nReal = (F32)(1.0 / sqrt(num.nReal));
            value.nUInt = root.nUInt - 0x20000000;
            *entry++ = value.nUInt;
            num.nUInt += 0x1000;
        }

        entry = table;

        for (i = 0; i < SQRTTABLE_SIZE / 2; i++)
        {
            RwSplitBits root;
            RwSplitBits value;

            root.nReal = (F32)(1.0 / sqrt(num.nReal));
            value.nUInt = root.nUInt - 0x1FC00000;
            *entry++ = value.nUInt;
            num.nUInt += 0x1000;
        }

        VECTOR_DATA(invSqrtLookup) = table;
        return TRUE;
    }
}

void* _rwVectorOpen(void* instance, S32 offset, S32 size)
{
    vectorModule.globalsOffset = offset;

    VECTOR_DATA(pointsFn) = VectorMultPoint;
    VECTOR_DATA(vectorsFn) = VectorMultVector;

    if (!SqrtTableCreate())
    {
        return NULL;
    }

    if (!InvSqrtTableCreate())
    {
        return NULL;
    }

    vectorModule.numInstances++;

    return instance;
}
