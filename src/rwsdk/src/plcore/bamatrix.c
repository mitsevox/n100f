#include <rwcore.h>

#define rwMATRIXTYPEMASK 0x00000003

// The matrix module's data in the engine instance. Partial: the words at +0x4 and +0x8 are not declared.
typedef struct MatrixModuleData MatrixModuleData;
struct MatrixModuleData
{
    RwFreeList* freeList;
    U8 unk4[0x8];
    RwMatrixTolerance tolerance;
};

#define MATRIX_DATA(var) \
    (((MatrixModuleData*)((U8*)RwEngineInstance + matrixModule.globalsOffset))->var)

static RwModuleInfo matrixModule;

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
