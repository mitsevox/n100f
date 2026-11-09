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
    RwV3d right;
    RwV3d up;
    RwV3d at;
    RwV3d pos;
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
                F32 upAt;
                F32 atRight;
                F32 rightUp;

                // FAKEMATCH: the absolute value of upAt is an if with an inline-asm branch over the
                // negate. As MATRIX_ABS (the believed real form, like the two below) its value
                // takes a later register than the target's; as a plain if, the compiler drops the
                // branch the target keeps on the non-negative path.
                upAt = RwV3dDotProductMacro(&up, &at);
                if (upAt >= 0.0f)
                {
                    asm { b upAtDone }
                }
                upAt = -upAt;
upAtDone:
                atRight = MATRIX_ABS(RwV3dDotProductMacro(&at, &right));
                rightUp = MATRIX_ABS(RwV3dDotProductMacro(&right, &up));

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
    RwV3d cross;

    RwV3dCrossProductMacro(&cross, &matrix->up, &matrix->at);

    return RwV3dDotProductMacro(&cross, &matrix->right);
}

F32 _rwMatrixOrthogonalError(const RwMatrix* matrix)
{
    F32 atRight;
    F32 upAt;
    F32 rightUp;

    upAt = RwV3dDotProductMacro(&matrix->up, &matrix->at);
    atRight = RwV3dDotProductMacro(&matrix->at, &matrix->right);
    rightUp = RwV3dDotProductMacro(&matrix->right, &matrix->up);

    return upAt * upAt + atRight * atRight + rightUp * rightUp;
}

F32 _rwMatrixNormalError(const RwMatrix* matrix)
{
    F32 right = RwV3dDotProductMacro(&matrix->right, &matrix->right) - 1.0f;
    F32 up = RwV3dDotProductMacro(&matrix->up, &matrix->up) - 1.0f;
    F32 at = RwV3dDotProductMacro(&matrix->at, &matrix->at) - 1.0f;

    return right * right + up * up + at * at;
}

// How far the matrix is from the identity: the summed squared distance of each axis from its unit
// vector plus the squared length of pos. RwMatrixOptimize's identity test is its only user.
// FAKEMATCH: static inline, so that no out-of-line copy is emitted now that nothing calls it; real
// form believed to be the global F32 _rwMatrixIdentityError(const RwMatrix*) (GLOBAL in the PS2
// prototype symbol table; the target has no copy of it).
static inline F32 _rwMatrixIdentityError(const RwMatrix* matrix)
{
    const RwV3d* right = &matrix->right;
    RwV3d vTmp;
    F32 rightError, sum, atError, posError, result;

    // The at and pos terms are held in their own locals so each is added in front of the running
    // sum (the target's operand order), and the first sum is its own value.
    vTmp.x = right->x - 1.0f;
    vTmp.y = right->y;
    vTmp.z = right->z;
    rightError = RwV3dDotProductMacro(&vTmp, &vTmp);

    vTmp.x = matrix->up.x;
    vTmp.y = matrix->up.y - 1.0f;
    vTmp.z = matrix->up.z;
    sum = rightError + RwV3dDotProductMacro(&vTmp, &vTmp);

    vTmp.x = matrix->at.x;
    vTmp.y = matrix->at.y;
    vTmp.z = matrix->at.z - 1.0f;
    atError = RwV3dDotProductMacro(&vTmp, &vTmp);
    sum = atError + sum;

    posError = RwV3dDotProductMacro(&matrix->pos, &matrix->pos);
    result = posError + sum;

    return result;
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

// Opens the matrix module: creates the matrix free list and installs the default tolerances and
// multiply function.
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
        const RwMatrixTolerance tolerance = { 0.01f, 0.01f, 0.01f };

        MATRIX_DATA(optimizations) = rwMATRIXINTERNALIDENTITY;
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

// FAKEMATCH: RwMatrixOptimize's identity test, 1 when the matrix is normal and orthogonal and its
// _rwMatrixIdentityError is within the tolerance. The error sum is one-instruction inline asm and
// the declaration order of its locals is chosen for the register colors. Real form believed to be
// `identity = normal && orthogonal && tolerance->Orthogonal >= _rwMatrixIdentityError(matrix);`
// in RwMatrixOptimize's body. Called from an inline function, `identity` is a compiler temporary,
// which CW's late CSE pass needs to turn the && test's second `li 0` into the target's `mr`.
// ux to az are the up and at values RwMatrixOptimize already loaded.
static inline S32 MatrixIdentityTest(register RwMatrix* matrix, F32 tolerance, S32 normal,
                                     S32 orthogonal, register F32 ux, register F32 uy,
                                     register F32 uz, register F32 ax, register F32 ay,
                                     register F32 az)
{
    S32 identity;
    register F32 iry;
    register F32 px;
    register F32 irz;
    register F32 atError;
    register F32 py;
    register F32 irxx;
    register F32 pzz;
    register F32 rightError;
    register F32 ps1;
    register F32 irxm;
    register F32 sum;
    register F32 iryy;
    register F32 iuyy;
    register F32 iazm;
    register F32 upE;
    register F32 iuym;
    register F32 irzz;
    register F32 irx;
    register F32 ir1;
    register F32 iuxx;
    register F32 pz;
    register F32 iazz;
    register F32 posError;
    register F32 iuzz;
    register F32 iu1;
    register F32 iaxx;
    register F32 pxx;
    register F32 errI;
    register F32 sum2;
    register F32 iayy;
    register F32 one2;
    register F32 pyy;
    register F32 ia1;

    identity = 0;
    if ((normal && orthogonal) != 0)
    {
        one2 = 1.0f;
        asm { fmuls iuxx, ux, ux }
        asm { lfs irx, 0(matrix) }
        asm { fmuls iuzz, uz, uz }
        asm { fsubs iuym, uy, one2 }
        asm { lfs iry, 4(matrix) }
        asm { fsubs irxm, irx, one2 }
        asm { lfs irz, 8(matrix) }
        asm { fsubs iazm, az, one2 }
        asm { fmuls iuyy, iuym, iuym }
        asm { fmuls irxx, irxm, irxm }
        asm { lfs px, 0x30(matrix) }
        asm { fmuls iryy, iry, iry }
        asm { lfs py, 0x34(matrix) }
        asm { fadds iu1, iuxx, iuyy }
        asm { fmuls irzz, irz, irz }
        asm { fadds ir1, irxx, iryy }
        asm { lfs pz, 0x38(matrix) }
        asm { fmuls iaxx, ax, ax }
        asm { fmuls iayy, ay, ay }
        asm { fadds rightError, irzz, ir1 }
        asm { fadds upE, iuzz, iu1 }
        asm { fmuls iazz, iazm, iazm }
        asm { fadds ia1, iaxx, iayy }
        asm { fmuls pxx, px, px }
        asm { fmuls pyy, py, py }
        asm { fadds atError, iazz, ia1 }
        asm { fadds sum, rightError, upE }
        asm { fmuls pzz, pz, pz }
        asm { fadds ps1, pxx, pyy }
        asm { fadds sum2, atError, sum }
        asm { fadds posError, pzz, ps1 }
        asm { fadds errI, posError, sum2 }
        if (tolerance >= errI)
        {
            identity = 1;
        }
    }

    return identity;
}

// Sets the matrix's normal, orthogonal and identity flags by how far it is from each, under the
// given tolerances or the defaults. The identity test uses the orthogonal tolerance, not the
// identity one.
// FAKEMATCH: the normal and orthogonal errors are one-instruction inline asm in the target's
// schedule (C keeps the compares and flags), and the F32 declaration order is chosen for the
// register colors. Real form believed to be
// `normal = tolerance->Normal >= _rwMatrixNormalError(matrix);` and
// `orthogonal = tolerance->Orthogonal >= _rwMatrixOrthogonalError(matrix);` with both inlined;
// that C matches the instruction order but not the identity block's colors. matrix and
// tolerance are register-qualified so that the asm can address them.
RwMatrix* RwMatrixOptimize(register RwMatrix* matrix, register const RwMatrixTolerance* tolerance)
{
    U32 flags;
    S32 identity;
    S32 normal;
    S32 orthogonal;
    register F32 ary;
    register F32 as1;
    register F32 errN;
    register F32 ruy;
    register F32 arx;
    register F32 errO;
    register F32 arz;
    register F32 ayy;
    register F32 uee;
    register F32 axx;
    register F32 ree;
    register F32 q2;
    register F32 tolN;
    register F32 ua1;
    register F32 as;
    register F32 qs1;
    register F32 azz;
    register F32 us1;
    register F32 ar1;
    register F32 uzz;
    register F32 uyy;
    register F32 ue;
    register F32 ryy;
    register F32 uay;
    register F32 rxx;
    register F32 q1;
    register F32 uaz;
    register F32 one;
    register F32 s1;
    register F32 uxx;
    register F32 ru1;
    register F32 ry;
    register F32 rux;
    register F32 rightUp;
    register F32 rx;
    register F32 rz;
    register F32 us;
    register F32 uy;
    register F32 ux;
    register F32 re;
    register F32 ae;
    register F32 aee;
    register F32 uz;
    register F32 ay;
    register F32 ax;
    register F32 upAt;
    register F32 rs;
    register F32 az;
    register F32 uax;
    register F32 rs1;
    register F32 ruz;
    register F32 rzz;
    register F32 atRight;
    register F32 tolO;
    register F32 q3;

    if (!tolerance)
    {
        tolerance = &MATRIX_DATA(tolerance);
    }

    asm { lfs rx, 0(matrix) }
    asm { lfs ry, 4(matrix) }
    asm { lfs ux, 0x10(matrix) }
    asm { fmuls rxx, rx, rx }
    asm { lfs uy, 0x14(matrix) }
    asm { fmuls ryy, ry, ry }
    asm { lfs rz, 8(matrix) }
    asm { fmuls uxx, ux, ux }
    asm { fmuls uyy, uy, uy }
    asm { lfs uz, 0x18(matrix) }
    asm { fmuls rzz, rz, rz }
    asm { lfs ax, 0x20(matrix) }
    asm { fadds rs1, rxx, ryy }
    asm { lfs ay, 0x24(matrix) }
    asm { lfs az, 0x28(matrix) }
    asm { fmuls uzz, uz, uz }
    asm { fadds us1, uxx, uyy }
    one = 1.0f;
    asm { fadds rs, rzz, rs1 }
    asm { lfs tolN, 0(tolerance) }
    asm { fmuls axx, ax, ax }
    asm { fmuls ayy, ay, ay }
    asm { fadds us, uzz, us1 }
    asm { fsubs re, rs, one }
    asm { fmuls azz, az, az }
    asm { fadds as1, axx, ayy }
    asm { fsubs ue, us, one }
    asm { fmuls ree, re, re }
    asm { fadds as, azz, as1 }
    asm { fmuls uee, ue, ue }
    asm { fsubs ae, as, one }
    asm { fadds s1, ree, uee }
    asm { fmuls aee, ae, ae }
    asm { fadds errN, aee, s1 }
    normal = tolN >= errN;
    asm { fmuls uax, ux, ax }
    asm { fmuls uay, uy, ay }
    asm { lfs tolO, 4(tolerance) }
    asm { fmuls arx, ax, rx }
    asm { fmuls ary, ay, ry }
    asm { fmuls uaz, uz, az }
    asm { fadds ua1, uax, uay }
    asm { fmuls arz, az, rz }
    asm { fadds ar1, arx, ary }
    asm { fadds upAt, uaz, ua1 }
    asm { fmuls rux, rx, ux }
    asm { fmuls ruy, ry, uy }
    asm { fadds atRight, arz, ar1 }
    asm { fmuls ruz, rz, uz }
    asm { fadds ru1, rux, ruy }
    asm { fmuls q1, upAt, upAt }
    asm { fmuls q2, atRight, atRight }
    asm { fadds rightUp, ruz, ru1 }
    asm { fadds qs1, q1, q2 }
    asm { fmuls q3, rightUp, rightUp }
    asm { fadds errO, q3, qs1 }
    orthogonal = tolO >= errO;
    identity = MatrixIdentityTest(matrix, tolO, normal, orthogonal, ux, uy, uz, ax, ay, az);

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

// Builds the rotation about a unit axis from one minus the cosine and the sine of the angle, then
// replaces the matrix with it or multiplies it in before or after, as the combine operation says.
RwMatrix* RwMatrixRotateOneMinusCosineSine(RwMatrix* matrix, const RwV3d* unitAxis,
                                           F32 oneMinusCosine, F32 sine,
                                           RwOpCombineType combineOp)
{
    RwMatrix rotate;
    RwMatrix result;
    RwV3d leading;
    RwV3d crossed;
    RwV3d scaled;

    leading.x = 1.0f - unitAxis->x * unitAxis->x;
    leading.y = 1.0f - unitAxis->y * unitAxis->y;
    leading.z = 1.0f - unitAxis->z * unitAxis->z;
    RwV3dScaleMacro(&leading, &leading, oneMinusCosine);

    crossed.x = unitAxis->y * unitAxis->z;
    crossed.y = unitAxis->z * unitAxis->x;
    crossed.z = unitAxis->x * unitAxis->y;
    RwV3dScaleMacro(&crossed, &crossed, oneMinusCosine);

    RwV3dScaleMacro(&scaled, unitAxis, sine);

    rotate.right.x = 1.0f - leading.x;
    rotate.right.y = crossed.z + scaled.z;
    rotate.right.z = crossed.y - scaled.y;
    rotate.up.x = crossed.z - scaled.z;
    rotate.up.y = 1.0f - leading.y;
    rotate.up.z = crossed.x + scaled.x;
    rotate.at.x = crossed.y + scaled.y;
    rotate.at.y = crossed.x - scaled.x;
    rotate.at.z = 1.0f - leading.z;
    rotate.pos.x = 0.0f;
    rotate.pos.y = 0.0f;
    rotate.pos.z = 0.0f;
    rotate.flags = rwMATRIXTYPEORTHONORMAL;
    rotate.flags &= ~rwMATRIXINTERNALIDENTITY;

    switch (combineOp)
    {
    case rwCOMBINEREPLACE:
        rotate.flags = matrix->flags;
        *matrix = rotate;
        matrix->flags &= ~rwMATRIXINTERNALIDENTITY;
        break;
    case rwCOMBINEPRECONCAT:
        RwMatrixMultiply(&result, &rotate, matrix);
        result.flags = matrix->flags;
        *matrix = result;
        matrix->flags &= ~rwMATRIXINTERNALIDENTITY;
        break;
    case rwCOMBINEPOSTCONCAT:
        RwMatrixMultiply(&result, matrix, &rotate);
        result.flags = matrix->flags;
        *matrix = result;
        matrix->flags &= ~rwMATRIXINTERNALIDENTITY;
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

    result.flags = rwMATRIXTYPENORMAL;

    switch (combineOp)
    {
    case rwCOMBINEREPLACE:
        *matrix = *transform;
        matrix->flags = transform->flags;
        break;
    case rwCOMBINEPRECONCAT:
        RwMatrixMultiply(&result, transform, matrix);
        result.flags = matrix->flags;
        *matrix = result;
        matrix->flags &= ~(rwMATRIXTYPEMASK | rwMATRIXINTERNALIDENTITY);
        break;
    case rwCOMBINEPOSTCONCAT:
        RwMatrixMultiply(&result, matrix, transform);
        result.flags = matrix->flags;
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
