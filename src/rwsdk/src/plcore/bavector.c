#include <rwcore.h>
#include <math.h>

// Entries in each square root lookup table: one per mantissa step, for two exponents.
#define SQRTTABLE_SIZE 0x1000

typedef RwV3d* (*VectorMultFn)(RwV3d* pointsOut, const RwV3d* pointsIn, S32 numPoints,
                                 const RwMatrix* matrix);

typedef struct VectorModuleData VectorModuleData;
struct VectorModuleData
{
    // Lookup entries encode the result's IEEE bits for combination with its exponent.
    RwSplitBits* sqrtLookup;
    RwSplitBits* invSqrtLookup;
    VectorMultFn pointsFn;
    VectorMultFn vectorsFn;
};

#define VECTOR_DATA(var) \
    (((VectorModuleData*)((U8*)RwEngineInstance + vectorModule.globalsOffset))->var)

static RwModuleInfo vectorModule;

// FAKEMATCH: unreferenced stand-in at the table builders' place in the file, so their constants
// (1.0f, then sqrt's 0.0, 0.5 and 3.0, then 1.0) pool first in .sdata2; real form believed to be
// the SqrtTableCreate and InvSqrtTableCreate helpers that _rwVectorOpen inlines. The linker strips it.
static void SqrtTablePoolStandIn(RwSplitBits* num, RwSplitBits* root)
{
    num->nReal = 1.0f;
    root[0].nReal = (F32)sqrt(num->nReal);
    root[1].nReal = (F32)(1.0 / sqrt(num->nReal));
}

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
            ((bits.nUInt >> 1) & 0x3FC00000) + VECTOR_DATA(sqrtLookup)[(bits.nUInt >> 12) & 0xFFF].nUInt;
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
                     VECTOR_DATA(invSqrtLookup)[(bits.nUInt >> 12) & 0xFFF].nUInt;
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

// Opens the vector module: installs the default point and vector transform functions and builds
// the square root and inverse square root lookup tables. Returns NULL when a table cannot be allocated.
void* _rwVectorOpen(void* instance, S32 offset, S32 size)
{
    // FAKEMATCH: the two lookup-table builders are written out by hand, with every table local at
    // function scope in this order and an ok flag standing in for their return value; real form
    // believed to be calls to static SqrtTableCreate and InvSqrtTableCreate helpers that the
    // compiler inlines (the inlined form places the stack slots in a different order).
    RwSplitBits sqrtRoot1;
    RwSplitBits sqrtNum;
    RwSplitBits invRoot1;
    RwSplitBits invNum;
    RwSplitBits sqrtValue2;
    RwSplitBits sqrtRoot2;
    RwSplitBits sqrtValue1;
    RwSplitBits invValue2;
    RwSplitBits invRoot2;
    RwSplitBits invValue1;
    RwSplitBits* table;
    RwSplitBits* entry;
    S32 i;
    S32 ok;

    vectorModule.globalsOffset = offset;

    VECTOR_DATA(pointsFn) = VectorMultPoint;
    VECTOR_DATA(vectorsFn) = VectorMultVector;

    // Square root table.
    table = (RwSplitBits*)RWSRCGLOBAL(memoryFuncs).rwmalloc(SQRTTABLE_SIZE * sizeof(RwSplitBits));

    if (!table)
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, SQRTTABLE_SIZE * sizeof(RwSplitBits));
        RwErrorSet(&err);
        ok = FALSE;
    }
    else
    {
        entry = table + SQRTTABLE_SIZE / 2;
        sqrtNum.nReal = 1.0f;

        for (i = 0; i < SQRTTABLE_SIZE / 2; i++)
        {
            sqrtRoot1.nReal = (F32)sqrt(sqrtNum.nReal);
            sqrtValue1.nUInt = sqrtRoot1.nUInt - 0x1FC00000;
            (entry++)->nUInt = sqrtValue1.nUInt;
            sqrtNum.nUInt += 0x1000;
        }

        entry = table;

        for (i = 0; i < SQRTTABLE_SIZE / 2; i++)
        {
            sqrtRoot2.nReal = (F32)sqrt(sqrtNum.nReal);
            sqrtValue2.nUInt = sqrtRoot2.nUInt - 0x20000000;
            (entry++)->nUInt = sqrtValue2.nUInt;
            sqrtNum.nUInt += 0x1000;
        }

        VECTOR_DATA(sqrtLookup) = table;
        ok = TRUE;
    }

    if (!ok)
    {
        return NULL;
    }

    // Inverse square root table, in the same layout.
    table = (RwSplitBits*)RWSRCGLOBAL(memoryFuncs).rwmalloc(SQRTTABLE_SIZE * sizeof(RwSplitBits));

    if (!table)
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, SQRTTABLE_SIZE * sizeof(RwSplitBits));
        RwErrorSet(&err);
        ok = FALSE;
    }
    else
    {
        entry = table + SQRTTABLE_SIZE / 2;
        invNum.nReal = 1.0f;

        for (i = 0; i < SQRTTABLE_SIZE / 2; i++)
        {
            invRoot1.nReal = (F32)(1.0 / sqrt(invNum.nReal));
            invValue1.nUInt = invRoot1.nUInt - 0x20000000;
            (entry++)->nUInt = invValue1.nUInt;
            invNum.nUInt += 0x1000;
        }

        entry = table;

        for (i = 0; i < SQRTTABLE_SIZE / 2; i++)
        {
            invRoot2.nReal = (F32)(1.0 / sqrt(invNum.nReal));
            invValue2.nUInt = invRoot2.nUInt - 0x1FC00000;
            (entry++)->nUInt = invValue2.nUInt;
            invNum.nUInt += 0x1000;
        }

        VECTOR_DATA(invSqrtLookup) = table;
        ok = TRUE;
    }

    if (!ok)
    {
        return NULL;
    }

    vectorModule.numInstances++;

    return instance;
}
