#include <rwcore.h>
#include <math.h>

#define rwMATRIXTYPEMASK 0x00000003

// Degrees to radians.
#define MATRIX_DEGTORAD 0.017453292f

// The function that multiplies two matrices whose flags do not mark either as the identity.
typedef void (*MatrixMultiplyFunc)(RwMatrix* dstMatrix, const RwMatrix* matA, const RwMatrix* matB);

// The matrix module's data in the engine instance.
typedef struct MatrixModuleData MatrixModuleData;
struct MatrixModuleData
{
    RwFreeList* freeList;
    U32 optimizations;
    MatrixMultiplyFunc multiplyFunc;
    RwMatrixTolerance tolerance;
};

#define MATRIX_DATA(var) \
    (((MatrixModuleData*)((U8*)RwEngineInstance + matrixModule.globalsOffset))->var)

static RwModuleInfo matrixModule;

RwMatrix* MatrixOrthoNormalize(RwMatrix* dstMatrix, const RwMatrix* srcMatrix);

void* _rwMatrixClose(void* instance, S32 offset, S32 size)
{
    if (MATRIX_DATA(freeList))
    {
        RwFreeListDestroy(MATRIX_DATA(freeList));
        MATRIX_DATA(freeList) = NULL;
    }

    matrixModule.numInstances--;

    return instance;
}

S32 RwEngineSetMatrixTolerances(const RwMatrixTolerance* const tolerance)
{
    MATRIX_DATA(tolerance) = *tolerance;

    return 1;
}

RwMatrix* RwMatrixUpdate(RwMatrix* matrix)
{
    matrix->flags &= ~(rwMATRIXTYPEMASK | rwMATRIXINTERNALIDENTITY);

    return matrix;
}

RwMatrix* RwMatrixMultiply(RwMatrix* matrixOut, const RwMatrix* matrixIn1,
                           const RwMatrix* matrixIn2)
{
    U32 flags1 = matrixIn1->flags;
    U32 flags2 = matrixIn2->flags;
    U32 identity = MATRIX_DATA(optimizations) & rwMATRIXINTERNALIDENTITY;

    if (flags1 & identity)
    {
        *matrixOut = *matrixIn2;
    }
    else if (flags2 & identity)
    {
        *matrixOut = *matrixIn1;
    }
    else
    {
        MATRIX_DATA(multiplyFunc)(matrixOut, matrixIn1, matrixIn2);
        matrixOut->flags = flags1 & flags2;
    }

    return matrixOut;
}

RwMatrix* RwMatrixOrthoNormalize(RwMatrix* dstMatrix, const RwMatrix* srcMatrix)
{
    return MatrixOrthoNormalize(dstMatrix, srcMatrix);
}

RwMatrix* RwMatrixRotate(RwMatrix* matrix, const RwV3d* axis, F32 angle, RwOpCombineType combineOp)
{
    F32 sinValue;
    F32 cosValue;
    RwV3d unitAxis;
    F32 radians = angle * MATRIX_DEGTORAD;
    F32 recipLength = _rwInvSqrt(RwV3dDotProductMacro(axis, axis));

    unitAxis.x = axis->x * recipLength;
    unitAxis.y = axis->y * recipLength;
    unitAxis.z = axis->z * recipLength;

    sinValue = (F32)sin(radians);
    cosValue = (F32)cos(radians);

    RwMatrixRotateOneMinusCosineSine(matrix, &unitAxis, 1.0f - cosValue, sinValue, combineOp);

    return matrix;
}
