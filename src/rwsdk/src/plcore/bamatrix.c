#include <rwcore.h>
#include <math.h>

#define rwMATRIXTYPENORMAL 0x00000001
#define rwMATRIXTYPEORTHOGONAL 0x00000002
#define rwMATRIXTYPEMASK 0x00000003

// Degrees to radians.
#define MATRIX_DEGTORAD 0.017453292f

// The matrices the free list hands out are aligned to this.
#define MATRIX_ALIGNMENT 4

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

// The absolute value the way the orthonormalizer takes it.
#define MATRIX_ABS(_x) (((_x) >= 0.0f) ? (_x) : -(_x))

static RwModuleInfo matrixModule;

static const RwMatrixTolerance defaultTolerance = { 0.01f, 0.01f, 0.01f };

static void MatrixMultiply(RwMatrix* dstMatrix, const RwMatrix* matA, const RwMatrix* matB)
{
    dstMatrix->right.x = matA->right.x * matB->right.x + matA->right.y * matB->up.x +
                         matA->right.z * matB->at.x;
    dstMatrix->right.y = matA->right.x * matB->right.y + matA->right.y * matB->up.y +
                         matA->right.z * matB->at.y;
    dstMatrix->right.z = matA->right.x * matB->right.z + matA->right.y * matB->up.z +
                         matA->right.z * matB->at.z;

    dstMatrix->up.x = matA->up.x * matB->right.x + matA->up.y * matB->up.x + matA->up.z * matB->at.x;
    dstMatrix->up.y = matA->up.x * matB->right.y + matA->up.y * matB->up.y + matA->up.z * matB->at.y;
    dstMatrix->up.z = matA->up.x * matB->right.z + matA->up.y * matB->up.z + matA->up.z * matB->at.z;

    dstMatrix->at.x = matA->at.x * matB->right.x + matA->at.y * matB->up.x + matA->at.z * matB->at.x;
    dstMatrix->at.y = matA->at.x * matB->right.y + matA->at.y * matB->up.y + matA->at.z * matB->at.y;
    dstMatrix->at.z = matA->at.x * matB->right.z + matA->at.y * matB->up.z + matA->at.z * matB->at.z;

    dstMatrix->pos.x = matA->pos.x * matB->right.x + matA->pos.y * matB->up.x +
                       matA->pos.z * matB->at.x + matB->pos.x;
    dstMatrix->pos.y = matA->pos.x * matB->right.y + matA->pos.y * matB->up.y +
                       matA->pos.z * matB->at.y + matB->pos.y;
    dstMatrix->pos.z = matA->pos.x * matB->right.z + matA->pos.y * matB->up.z +
                       matA->pos.z * matB->at.z + matB->pos.z;
}

static RwMatrix* MatrixOrthoNormalize(RwMatrix* dstMatrix, const RwMatrix* srcMatrix)
{
    RwV3d pos;
    RwV3d at;
    RwV3d up;
    RwV3d right;
    F32 recipRight;
    F32 recipUp;
    F32 recipAt;
    RwV3d* vect1;
    RwV3d* vect2;
    RwV3d* vect3;
    F32 recip;

    right = srcMatrix->right;
    up = srcMatrix->up;
    at = srcMatrix->at;
    pos = srcMatrix->pos;

    recipRight = _rwInvSqrt(RwV3dDotProductMacro(&right, &right));
    RwV3dScaleMacro(&right, &right, recipRight);
    recipUp = _rwInvSqrt(RwV3dDotProductMacro(&up, &up));
    RwV3dScaleMacro(&up, &up, recipUp);
    recipAt = _rwInvSqrt(RwV3dDotProductMacro(&at, &at));
    RwV3dScaleMacro(&at, &at, recipAt);

    if (recipRight > 0.0f)
    {
        if (recipUp > 0.0f)
        {
            if (recipAt > 0.0f)
            {
                // Keep the pair of axes nearest to perpendicular and rebuild the third.
                F32 upAt = RwV3dDotProductMacro(&up, &at);
                F32 atRight;
                F32 rightUp;

                upAt = MATRIX_ABS(upAt);
                atRight = RwV3dDotProductMacro(&at, &right);
                atRight = MATRIX_ABS(atRight);
                rightUp = RwV3dDotProductMacro(&right, &up);
                rightUp = MATRIX_ABS(rightUp);

                if (upAt < atRight)
                {
                    if (upAt < rightUp)
                    {
                        vect1 = &up;
                        vect2 = &at;
                        vect3 = &right;
                    }
                    else
                    {
                        vect1 = &right;
                        vect2 = &up;
                        vect3 = &at;
                    }
                }
                else if (atRight < rightUp)
                {
                    vect1 = &at;
                    vect2 = &right;
                    vect3 = &up;
                }
                else
                {
                    vect1 = &right;
                    vect2 = &up;
                    vect3 = &at;
                }
            }
            else
            {
                vect1 = &right;
                vect2 = &up;
                vect3 = &at;
            }
        }
        else
        {
            vect1 = &at;
            vect2 = &right;
            vect3 = &up;
        }
    }
    else
    {
        vect1 = &up;
        vect2 = &at;
        vect3 = &right;
    }

    RwV3dCrossProductMacro(vect3, vect1, vect2);
    recip = _rwInvSqrt(RwV3dDotProductMacro(vect3, vect3));
    RwV3dScaleMacro(vect3, vect3, recip);

    RwV3dCrossProductMacro(vect2, vect3, vect1);
    recip = _rwInvSqrt(RwV3dDotProductMacro(vect2, vect2));
    RwV3dScaleMacro(vect2, vect2, recip);

    dstMatrix->right = right;
    dstMatrix->up = up;
    dstMatrix->at = at;
    dstMatrix->pos = pos;
    dstMatrix->flags = (dstMatrix->flags | rwMATRIXTYPEORTHONORMAL) & ~rwMATRIXINTERNALIDENTITY;

    return dstMatrix;
}

S32 _rwMatrixSetMultFn(MatrixMultiplyFunc multiplyFunc)
{
    if (!multiplyFunc)
    {
        multiplyFunc = MatrixMultiply;
    }

    MATRIX_DATA(multiplyFunc) = multiplyFunc;

    return 1;
}

F32 _rwMatrixDeterminant(const RwMatrix* matrix)
{
    return matrix->right.x * (matrix->up.y * matrix->at.z - matrix->up.z * matrix->at.y) +
           matrix->right.y * (matrix->up.z * matrix->at.x - matrix->up.x * matrix->at.z) +
           matrix->right.z * (matrix->up.x * matrix->at.y - matrix->up.y * matrix->at.x);
}

F32 _rwMatrixOrthogonalError(const RwMatrix* matrix)
{
    F32 upAt = RwV3dDotProductMacro(&matrix->up, &matrix->at);
    F32 atRight = RwV3dDotProductMacro(&matrix->at, &matrix->right);
    F32 rightUp = RwV3dDotProductMacro(&matrix->right, &matrix->up);

    return upAt * upAt + atRight * atRight + rightUp * rightUp;
}

F32 _rwMatrixNormalError(const RwMatrix* matrix)
{
    F32 right = RwV3dDotProductMacro(&matrix->right, &matrix->right) - 1.0f;
    F32 up = RwV3dDotProductMacro(&matrix->up, &matrix->up) - 1.0f;
    F32 at = RwV3dDotProductMacro(&matrix->at, &matrix->at) - 1.0f;

    return right * right + up * up + at * at;
}

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


void* _rwMatrixOpen(void* instance, S32 offset, S32 size)
{
    matrixModule.globalsOffset = offset;
    MATRIX_DATA(freeList) = RwFreeListCreate(sizeof(RwMatrix), 50, MATRIX_ALIGNMENT);

    if (!MATRIX_DATA(freeList))
    {
        instance = NULL;
    }
    else
    {
        RwMatrixTolerance tolerance;

        MATRIX_DATA(optimizations) = rwMATRIXINTERNALIDENTITY;
        tolerance = defaultTolerance;
        MATRIX_DATA(multiplyFunc) = MatrixMultiply;
        RwEngineSetMatrixTolerances(&tolerance);
        matrixModule.numInstances++;
    }

    return instance;
}

S32 RwEngineSetMatrixTolerances(const RwMatrixTolerance* const tolerance)
{
    MATRIX_DATA(tolerance) = *tolerance;

    return 1;
}

RwMatrix* RwMatrixOptimize(RwMatrix* matrix, const RwMatrixTolerance* tolerance)
{
    S32 normal;
    S32 orthogonal;
    S32 identity;
    U32 flags;

    if (!tolerance)
    {
        tolerance = &MATRIX_DATA(tolerance);
    }

    normal = tolerance->Normal >= _rwMatrixNormalError(matrix);
    orthogonal = tolerance->Orthogonal >= _rwMatrixOrthogonalError(matrix);
    identity = 0;

    if (normal && orthogonal)
    {
        F32 rightX = matrix->right.x - 1.0f;
        F32 upY = matrix->up.y - 1.0f;
        F32 atZ = matrix->at.z - 1.0f;
        F32 error = rightX * rightX + matrix->right.y * matrix->right.y +
                    matrix->right.z * matrix->right.z + matrix->up.x * matrix->up.x +
                    upY * upY + matrix->up.z * matrix->up.z + matrix->at.x * matrix->at.x +
                    matrix->at.y * matrix->at.y + atZ * atZ + matrix->pos.x * matrix->pos.x +
                    matrix->pos.y * matrix->pos.y + matrix->pos.z * matrix->pos.z;

        if (tolerance->Identity >= error)
        {
            identity = 1;
        }
    }

    flags = matrix->flags;
    flags = normal ? (flags | rwMATRIXTYPENORMAL) : (flags & ~rwMATRIXTYPENORMAL);
    flags = orthogonal ? (flags | rwMATRIXTYPEORTHOGONAL) : (flags & ~rwMATRIXTYPEORTHOGONAL);
    flags = identity ? (flags | rwMATRIXINTERNALIDENTITY) : (flags & ~rwMATRIXINTERNALIDENTITY);
    matrix->flags = flags;

    return matrix;
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

RwMatrix* RwMatrixRotateOneMinusCosineSine(RwMatrix* matrix, const RwV3d* unitAxis,
                                           F32 oneMinusCosine, F32 sine,
                                           RwOpCombineType combineOp)
{
    RwMatrix rotate;
    RwMatrix result;
    F32 x = unitAxis->x;
    F32 y = unitAxis->y;
    F32 z = unitAxis->z;
    F32 xy;
    F32 yz;
    F32 zx;
    F32 xSine;
    F32 ySine;
    F32 zSine;

    rotate.flags = rwMATRIXTYPEORTHONORMAL;
    rotate.pos.x = 0.0f;
    rotate.pos.y = 0.0f;
    rotate.pos.z = 0.0f;

    yz = y * z * oneMinusCosine;
    xSine = x * sine;
    rotate.right.x = 1.0f - (1.0f - x * x) * oneMinusCosine;
    zx = z * x * oneMinusCosine;
    ySine = y * sine;
    rotate.up.y = 1.0f - (1.0f - y * y) * oneMinusCosine;
    rotate.at.z = 1.0f - (1.0f - z * z) * oneMinusCosine;
    rotate.up.z = yz + xSine;
    xy = x * y * oneMinusCosine;
    zSine = z * sine;
    rotate.right.z = zx - ySine;
    rotate.at.x = zx + ySine;
    rotate.at.y = yz - xSine;
    rotate.right.y = xy + zSine;
    rotate.up.x = xy - zSine;

    switch (combineOp)
    {
    case rwCOMBINEREPLACE:
        *matrix = rotate;
        break;
    case rwCOMBINEPRECONCAT:
        RwMatrixMultiply(&result, &rotate, matrix);
        *matrix = result;
        break;
    case rwCOMBINEPOSTCONCAT:
        RwMatrixMultiply(&result, matrix, &rotate);
        *matrix = result;
        break;
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000003, "Invalid combination type");
        RwErrorSet(&err);
        return NULL;
    }
    }

    matrix->flags &= ~rwMATRIXINTERNALIDENTITY;

    return matrix;
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

RwMatrix* RwMatrixInvert(RwMatrix* dstMatrix, const RwMatrix* matrix)
{
    if (matrix->flags & (MATRIX_DATA(optimizations) & rwMATRIXINTERNALIDENTITY))
    {
        *dstMatrix = *matrix;
        return dstMatrix;
    }

    if ((matrix->flags & rwMATRIXTYPEMASK) == rwMATRIXTYPEORTHONORMAL)
    {
        // The inverse of a rotation is its transpose.
        dstMatrix->right.x = matrix->right.x;
        dstMatrix->right.y = matrix->up.x;
        dstMatrix->right.z = matrix->at.x;
        dstMatrix->up.x = matrix->right.y;
        dstMatrix->up.y = matrix->up.y;
        dstMatrix->up.z = matrix->at.y;
        dstMatrix->at.x = matrix->right.z;
        dstMatrix->at.y = matrix->up.z;
        dstMatrix->at.z = matrix->at.z;
        dstMatrix->pos.x = -RwV3dDotProductMacro(&matrix->pos, &matrix->right);
        dstMatrix->pos.y = -RwV3dDotProductMacro(&matrix->pos, &matrix->up);
        dstMatrix->pos.z = -RwV3dDotProductMacro(&matrix->pos, &matrix->at);
        dstMatrix->flags = (dstMatrix->flags | rwMATRIXTYPEORTHONORMAL) & ~rwMATRIXINTERNALIDENTITY;
        return dstMatrix;
    }
    else
    {
        RwSplitBits determinant;
        F32 recip;

        dstMatrix->right.x = matrix->up.y * matrix->at.z - matrix->up.z * matrix->at.y;
        dstMatrix->right.y = -(matrix->right.y * matrix->at.z - matrix->right.z * matrix->at.y);
        dstMatrix->right.z = matrix->right.y * matrix->up.z - matrix->right.z * matrix->up.y;

        determinant.nReal = dstMatrix->right.x * matrix->right.x +
                            dstMatrix->right.y * matrix->up.x + dstMatrix->right.z * matrix->at.x;
        recip = (determinant.nInt != 0) ? 1.0f / determinant.nReal : 1.0f;

        dstMatrix->right.x *= recip;
        dstMatrix->right.y *= recip;
        dstMatrix->right.z *= recip;
        dstMatrix->up.x = recip * -(matrix->up.x * matrix->at.z - matrix->up.z * matrix->at.x);
        dstMatrix->up.y = recip * (matrix->right.x * matrix->at.z - matrix->right.z * matrix->at.x);
        dstMatrix->up.z = recip * -(matrix->right.x * matrix->up.z - matrix->right.z * matrix->up.x);
        dstMatrix->at.x = recip * (matrix->up.x * matrix->at.y - matrix->up.y * matrix->at.x);
        dstMatrix->at.y = recip * -(matrix->right.x * matrix->at.y - matrix->right.y * matrix->at.x);
        dstMatrix->at.z = recip * (matrix->right.x * matrix->up.y - matrix->right.y * matrix->up.x);
        dstMatrix->pos.x = -(matrix->pos.x * dstMatrix->right.x + matrix->pos.y * dstMatrix->up.x +
                             matrix->pos.z * dstMatrix->at.x);
        dstMatrix->pos.y = -(matrix->pos.x * dstMatrix->right.y + matrix->pos.y * dstMatrix->up.y +
                             matrix->pos.z * dstMatrix->at.y);
        dstMatrix->pos.z = -(matrix->pos.x * dstMatrix->right.z + matrix->pos.y * dstMatrix->up.z +
                             matrix->pos.z * dstMatrix->at.z);
        dstMatrix->flags &= ~rwMATRIXINTERNALIDENTITY;
        return dstMatrix;
    }
}

RwMatrix* RwMatrixScale(RwMatrix* matrix, const RwV3d* scale, RwOpCombineType combineOp)
{
    switch (combineOp)
    {
    case rwCOMBINEREPLACE:
        RwMatrixSetIdentityMacro(matrix);
        matrix->right.x = scale->x;
        matrix->up.y = scale->y;
        matrix->at.z = scale->z;
        break;
    case rwCOMBINEPRECONCAT:
        RwV3dScaleMacro(&matrix->right, &matrix->right, scale->x);
        RwV3dScaleMacro(&matrix->up, &matrix->up, scale->y);
        RwV3dScaleMacro(&matrix->at, &matrix->at, scale->z);
        break;
    case rwCOMBINEPOSTCONCAT:
        matrix->right.x *= scale->x;
        matrix->right.y *= scale->y;
        matrix->right.z *= scale->z;
        matrix->up.x *= scale->x;
        matrix->up.y *= scale->y;
        matrix->up.z *= scale->z;
        matrix->at.x *= scale->x;
        matrix->at.y *= scale->y;
        matrix->at.z *= scale->z;
        matrix->pos.x *= scale->x;
        matrix->pos.y *= scale->y;
        matrix->pos.z *= scale->z;
        break;
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000003, "Invalid combination type");
        RwErrorSet(&err);
        matrix = NULL;
        break;
    }
    }

    matrix->flags &= ~(rwMATRIXTYPEMASK | rwMATRIXINTERNALIDENTITY);

    return matrix;
}

RwMatrix* RwMatrixTranslate(RwMatrix* matrix, const RwV3d* translation, RwOpCombineType combineOp)
{
    switch (combineOp)
    {
    case rwCOMBINEREPLACE:
        RwMatrixSetIdentityMacro(matrix);
        matrix->pos.x = translation->x;
        matrix->pos.y = translation->y;
        matrix->pos.z = translation->z;
        break;
    case rwCOMBINEPRECONCAT:
        matrix->pos.x += translation->x * matrix->right.x + translation->y * matrix->up.x +
                         translation->z * matrix->at.x;
        matrix->pos.y += translation->x * matrix->right.y + translation->y * matrix->up.y +
                         translation->z * matrix->at.y;
        matrix->pos.z += translation->x * matrix->right.z + translation->y * matrix->up.z +
                         translation->z * matrix->at.z;
        break;
    case rwCOMBINEPOSTCONCAT:
        matrix->pos.x += translation->x;
        matrix->pos.y += translation->y;
        matrix->pos.z += translation->z;
        break;
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000003, "Invalid combination type");
        RwErrorSet(&err);
        matrix = NULL;
        break;
    }
    }

    matrix->flags &= ~rwMATRIXINTERNALIDENTITY;

    return matrix;
}

RwMatrix* RwMatrixTransform(RwMatrix* matrix, const RwMatrix* transform, RwOpCombineType combineOp)
{
    RwMatrix result;

    switch (combineOp)
    {
    case rwCOMBINEREPLACE:
        *matrix = *transform;
        matrix->flags = transform->flags;
        break;
    case rwCOMBINEPRECONCAT:
        RwMatrixMultiply(&result, transform, matrix);
        *matrix = result;
        matrix->flags &= ~(rwMATRIXTYPEMASK | rwMATRIXINTERNALIDENTITY);
        break;
    case rwCOMBINEPOSTCONCAT:
        RwMatrixMultiply(&result, matrix, transform);
        *matrix = result;
        matrix->flags &= ~(rwMATRIXTYPEMASK | rwMATRIXINTERNALIDENTITY);
        break;
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000003, "Invalid combination type");
        RwErrorSet(&err);
        matrix = NULL;
        break;
    }
    }

    return matrix;
}

S32 RwMatrixDestroy(RwMatrix* matrix)
{
    RWSRCGLOBAL(memoryFree)(MATRIX_DATA(freeList), matrix);

    return 1;
}

RwMatrix* RwMatrixCreate(void)
{
    RwMatrix* matrix = (RwMatrix*)RWSRCGLOBAL(memoryAlloc)(MATRIX_DATA(freeList));

    if (matrix)
    {
        matrix->flags = rwMATRIXTYPEORTHONORMAL;
        RwMatrixSetIdentityMacro(matrix);
    }

    return matrix;
}
