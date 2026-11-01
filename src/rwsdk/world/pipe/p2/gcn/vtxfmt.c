#include <rpworld.h>
#include <dolphin/gx.h>

#define VTXFMT_PLUGIN_ID 0x511

RpGameCubeVtxFmt* RpGameCubeVtxFmtInit(RpGameCubeVtxFmt* vtxFmt);
void RpGameCubeVtxFmtDestroy(RpGameCubeVtxFmt* vtxFmt);

// The compiler lays out uninitialized variables in reverse order of definition.
static RwModuleInfo _RpVtxFmtModule;
S32 _rpDlWorldVtxFmtOffset;
S32 _rpDlGeomVtxFmtOffset;
RpGameCubeVtxFmt _RpDlVtxFmtDefault;

// Describes the instanced arrays to GX for drawing: each attribute the object has (positions
// always; normals, or normal-binormal-tangent triples when the format's normal fraction is set;
// prelit colors; each texture coordinate set with an array) gets its index type, its component
// format and fraction from the vertex format (the default when none is given), and its array.
void _rwDlVtxFmtSetup(RpGameCubeVtxFmt* vtxFmt, GCNObjectData* objectData)
{
    GCNVertexArrays* texArrays;
    GXAttr attr;
    GCNVertexArrays* arrays;
    S32 i;

    if (!vtxFmt)
    {
        vtxFmt = &_RpDlVtxFmtDefault;
    }

    arrays = (GCNVertexArrays*)(objectData->repEntry + 1);
    GXClearVtxDesc();

    GXSetVtxDesc(GX_VA_POS, arrays->indexType);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, vtxFmt->compType[0], vtxFmt->frac[0]);
    GXSetArray(GX_VA_POS, arrays->pos, arrays->posStride);

    if (objectData->flags & rpGEOMETRYNORMALS)
    {
        if (vtxFmt->frac[1])
        {
            GXSetVtxDesc(GX_VA_NBT, arrays->indexType);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NBT, GX_NRM_NBT, vtxFmt->compType[1], 0);
            GXSetArray(GX_VA_NBT, arrays->nrm, arrays->nrmStride);
        }
        else
        {
            GXSetVtxDesc(GX_VA_NRM, arrays->indexType);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, vtxFmt->compType[1], 0);
            GXSetArray(GX_VA_NRM, arrays->nrm, arrays->nrmStride);
        }
    }

    if (objectData->flags & rpGEOMETRYPRELIT)
    {
        GXSetVtxDesc(GX_VA_CLR0, arrays->indexType);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, vtxFmt->compType[10] > GX_RGBX8, vtxFmt->compType[10],
                        0);
        GXSetArray(GX_VA_CLR0, arrays->clr, arrays->clrStride);
    }

    if (objectData->flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2))
    {
        texArrays = arrays;
        for (i = 0; i < 8 && texArrays->tex[0]; i++)
        {
            attr = (GXAttr)(GX_VA_TEX0 + i);
            GXSetVtxDesc(attr, arrays->indexType);
            GXSetVtxAttrFmt(GX_VTXFMT0, attr, GX_TEX_ST, vtxFmt->compType[2 + i],
                            vtxFmt->frac[2 + i]);
            GXSetArray(attr, texArrays->tex[0], arrays->texStride[i]);
            texArrays = (GCNVertexArrays*)((U8*)texArrays + sizeof(void*));
        }
    }
}

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
    vtxFmt->flags = 1;
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

// The vertex format a geometry carries in its plugin slot.
RpGameCubeVtxFmt* fn_8015D818(const RpGeometry* geometry)
{
    return *(RpGameCubeVtxFmt**)((U8*)geometry + _rpDlGeomVtxFmtOffset);
}

// The vertex format a world carries in its plugin slot, used for all its sectors.
RpGameCubeVtxFmt* fn_8015D824(const RpWorld* world)
{
    return *(RpGameCubeVtxFmt**)((U8*)world + _rpDlWorldVtxFmtOffset);
}
