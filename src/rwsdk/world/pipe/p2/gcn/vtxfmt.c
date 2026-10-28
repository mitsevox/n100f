#include <rpworld.h>

#define VTXFMT_PLUGIN_ID 0x511

// A GameCube vertex format: a component type and a fraction per vertex attribute, shared by
// reference count.
typedef struct RpGameCubeVtxFmt RpGameCubeVtxFmt;
struct RpGameCubeVtxFmt
{
    U8 compType[12];
    U8 frac[10];
    U16 refCount;
};

RpGameCubeVtxFmt* RpGameCubeVtxFmtInit(RpGameCubeVtxFmt* vtxFmt);
void RpGameCubeVtxFmtDestroy(RpGameCubeVtxFmt* vtxFmt);

static RpGameCubeVtxFmt _RpDlVtxFmtDefault;
S32 _rpDlGeomVtxFmtOffset;
S32 _rpDlWorldVtxFmtOffset;
static RwModuleInfo _RpVtxFmtModule;

RpGameCubeVtxFmt* _rpGameCubeVtxFmtGetDefault(void)
{
    return &_RpDlVtxFmtDefault;
}

static void* _rxDlVertexFmtConst(void* object, S32 offset, S32 size)
{
    *(RpGameCubeVtxFmt**)((U8*)object + offset) = NULL;

    return object;
}

static void* _rxDlVertexFmtDest(void* object, S32 offset, S32 size)
{
    RpGameCubeVtxFmt* vtxFmt = *(RpGameCubeVtxFmt**)((U8*)object + offset);

    if (vtxFmt)
    {
        RpGameCubeVtxFmtDestroy(vtxFmt);
    }

    return object;
}

static void* _rpDlVtxFmtOpen(void* instance, S32 offset, S32 size)
{
    if (++_RpVtxFmtModule.numInstances == 1)
    {
        RpGameCubeVtxFmtInit(&_RpDlVtxFmtDefault);
    }

    return instance;
}

static void* _rpDlVtxFmtClose(void* instance, S32 offset, S32 size)
{
    _RpVtxFmtModule.numInstances--;

    return instance;
}

S32 _rpDlVtxFmtPluginAttach(void)
{
    if (RwEngineRegisterPlugin(0, VTXFMT_PLUGIN_ID, _rpDlVtxFmtOpen, _rpDlVtxFmtClose) < 0)
    {
        return FALSE;
    }

    _rpDlGeomVtxFmtOffset =
        RpGeometryRegisterPlugin(sizeof(RpGameCubeVtxFmt*), VTXFMT_PLUGIN_ID, _rxDlVertexFmtConst,
                                 _rxDlVertexFmtDest, NULL);

    if (_rpDlGeomVtxFmtOffset < 0)
    {
        return FALSE;
    }

    _rpDlWorldVtxFmtOffset =
        RpWorldRegisterPlugin(sizeof(RpGameCubeVtxFmt*), VTXFMT_PLUGIN_ID, _rxDlVertexFmtConst,
                              _rxDlVertexFmtDest, NULL);

    if (_rpDlWorldVtxFmtOffset < 0)
    {
        return FALSE;
    }

    return TRUE;
}

void RpGameCubeVtxFmtSetNormal(RpGameCubeVtxFmt* vtxFmt, U8 compType, U8 frac)
{
    vtxFmt->compType[1] = compType;
    vtxFmt->frac[1] = frac;
}

RpGameCubeVtxFmt* RpGameCubeVtxFmtInit(RpGameCubeVtxFmt* vtxFmt)
{
    vtxFmt->compType[0] = 4;
    vtxFmt->compType[1] = 4;
    vtxFmt->compType[2] = 4;
    vtxFmt->compType[3] = 4;
    vtxFmt->compType[4] = 4;
    vtxFmt->compType[5] = 4;
    vtxFmt->compType[6] = 4;
    vtxFmt->compType[7] = 4;
    vtxFmt->compType[8] = 4;
    vtxFmt->compType[9] = 4;
    vtxFmt->compType[10] = 5;
    vtxFmt->compType[11] = 1;
    vtxFmt->frac[0] = 0;
    vtxFmt->frac[2] = 0;
    vtxFmt->frac[3] = 0;
    vtxFmt->frac[4] = 0;
    vtxFmt->frac[5] = 0;
    vtxFmt->frac[6] = 0;
    vtxFmt->frac[7] = 0;
    vtxFmt->frac[8] = 0;
    vtxFmt->frac[9] = 0;
    vtxFmt->frac[1] = 0;
    vtxFmt->refCount = 1;

    return vtxFmt;
}

RpGameCubeVtxFmt* RpGameCubeVtxFmtCreate(void)
{
    RpGameCubeVtxFmt* vtxFmt = (RpGameCubeVtxFmt*)RwMalloc(sizeof(RpGameCubeVtxFmt));

    RpGameCubeVtxFmtInit(vtxFmt);

    return vtxFmt;
}

void RpGameCubeVtxFmtDestroy(RpGameCubeVtxFmt* vtxFmt)
{
    if (vtxFmt->refCount == 1)
    {
        RwFree(vtxFmt);
    }
    else
    {
        vtxFmt->refCount--;
    }
}

void RpGameCubeGeometrySetVtxFmt(RpGeometry* geometry, RpGameCubeVtxFmt* vtxFmt)
{
    RpGameCubeVtxFmt** slot = (RpGameCubeVtxFmt**)((U8*)geometry + _rpDlGeomVtxFmtOffset);

    if (*slot)
    {
        RpGameCubeVtxFmtDestroy(*slot);
    }

    *slot = vtxFmt;
    (*slot)->refCount++;
}
