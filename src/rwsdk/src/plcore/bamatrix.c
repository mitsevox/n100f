#include <rwcore.h>

#define rwMATRIXTYPEMASK 0x00000003

typedef struct RwMatrixTolerance RwMatrixTolerance;
struct RwMatrixTolerance
{
    F32 Normal;
    F32 Orthogonal;
    F32 Identity;
};

// The matrix module's data in the engine instance. Partial: only the tolerances are declared.
typedef struct MatrixModuleData MatrixModuleData;
struct MatrixModuleData
{
    U8 unk0[0xC];
    RwMatrixTolerance tolerance;
};

#define MATRIX_DATA(var) \
    (((MatrixModuleData*)((U8*)RwEngineInstance + matrixModule.globalsOffset))->var)

static RwModuleInfo matrixModule;

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
