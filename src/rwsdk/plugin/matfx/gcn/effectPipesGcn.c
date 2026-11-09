#include <rpmatfx.h>
#include <rpmtexgcn.h>
#include <string.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>

// The GameCube material effects pipelines: an atomic and a world sector all-in-one pipeline whose
// render callback draws each mesh once more per effect (bump map, environment map, both, or a
// dual texture), and whose instance callbacks compute the NBT multitexture effects need.

// The draw sync token to issue next.
extern volatile U16 _RwDlTokenCurrent;

// The raster plugin's offset in a raster.
extern S32 _RwGameCubeRasterExtOffset;

// A matrix the environment map is relative to when it has no frame.
extern RwMatrix _RwDlInvCamLTM;

// The owner of the bump map texture coordinates' resource entries.
U8 MatFXGCNBumpMapResOwner[0x70];

// The material colors of the environment map passes; their alpha is set from the coefficient.
GXColor MatFXGCNEnvMapColor = { 255, 255, 255, 255 };
GXColor MatFXGCNBumpEnvMapColor = { 255, 255, 255, 255 };

// The vertex format given to geometries that need the NBT and have none, the two pipelines, and
// the all-in-one nodes' callbacks the pipelines' own ones call.
typedef void* (*GCNMTInstanceCB)(void* object, GCNObjectData* objectData);

GCNMTInstanceCB DefaultAtomicInstanceCallBack;
GCNMTInstanceCB DefaultAtomicReinstanceCallBack;
GCNMTInstanceCB DefaultSectorInstanceCallBack;
RxPipeline* _RpMatFXWorldSectorPipe;
RxPipeline* _RpMatFXAtomicPipe;
RpGameCubeVtxFmt* _rpGCMatFXVtxFmtNBT;

S32 _rwDlTokenQueryDone(U16 token);
RpGameCubeVtxFmt* RpGameCubeVtxFmtCreate(void);
void RpGameCubeVtxFmtDestroy(RpGameCubeVtxFmt* vtxFmt);
void RpGameCubeVtxFmtSetNormal(RpGameCubeVtxFmt* vtxFmt, U8 compType, U8 frac);
void RpGameCubeGeometrySetVtxFmt(RpGeometry* geometry, RpGameCubeVtxFmt* vtxFmt);
RxNodeDefinition* RxNodeDefinitionGetGameCubeWorldSectorAllInOne(void);
void _rwDlVtxFmtSetup(RpGameCubeVtxFmt* vtxFmt, GCNObjectData* objectData);
void _rwDlTransformSetup(const RwMatrix* matrix, S32 normals);
void _rwDlTextureSet(RwTexture* texture, U32 stage);
void RwGameCubeSetAlphaCompare(S32 mode);
void _rpGameCubeMTMeshRenderCallBack(GCNDisplayList* displayList, RpMaterial* material,
                                     void* object, GCNObjectData* objectData,
                                     const RwMatrix* matrix);
void _rpGameCubeMTPipeDataCalcNBTs(GCNObjectData* objectData, RpGameCubeVtxFmt* vtxFmt, S32 numVertices);
void _rxGCChannelSetup(GCNObjectData* objectData);
void _rxGCChannelMaterialSetup(GCNObjectData* objectData, RpMaterial* material);
void _rxGCTevSetup(GCNObjectData* objectData);
RwTexture* _rpMatFXTextureMaskCreate(const RwTexture* baseTexture, const RwTexture* effectTexture);

// The material effects plugin id, the multitexture platform, an atomic's object type, and the
// draw sync token wrap. Written for this project.
#define MATFXGCN_PLUGINID 0x120
#define MATFXGCN_PLATFORM 6
#define MATFXGCN_ATOMIC 1
#define MATFXGCN_TOKENWRAP 0xE000

// The bump map texture coordinates' resource entry: the entry, the draw sync token, then,
// 32-byte aligned, the coordinates. Written for this project.
#define MATFXGCN_TOKEN(_entry) (*(U16*)((_entry) + 1))

// Whether a texture's raster has alpha, from the raster plugin's flags.
#define MATFXGCN_TEXTUREHASALPHA(_texture)                                                         \
    ((_texture) && (_texture)->raster &&                                                           \
     (*(U32*)((U8*)(_texture)->raster->parent + _RwGameCubeRasterExtOffset + 0x14) & 1))

// The display lists follow the instanced vertex arrays in the entry.
#define MATFXGCN_DISPLAYLISTS(_entry)                                                              \
    ((GCNDisplayList*)((U8*)((GCNVertexArrays*)((_entry) + 1)) + sizeof(GCNVertexArrays)))

// Offsets each vertex's base texture coordinates along the bump map light direction, into dst:
// the light (the bump map frame's at axis, or the camera's) is taken into the object's space,
// and each vertex's offset comes from the texture space of one of its triangles.
void MatFXGCNBumpMapTexCoords(RwTexCoords* dst, void* object, MatFXBumpMapData* bumpMap, RpMesh* mesh,
                 RwV3d* positions, RwV3d* normals, RwTexCoords* texCoords, S32 numVertices,
                 S32 flags)
{
    U8* done = (U8*)RwMalloc(numVertices);

    memset(done, 0, numVertices);

    if (done)
    {
        RwFrame* frame = bumpMap->frame;
        F32 scale = bumpMap->coef * bumpMap->invBumpWidth;
        RwV3d* at;
        RwV3d light;
        S32 i;
        S32 minIndex = 0x7FFFFFFF;
        S32 maxIndex = 0x80000000;
        U16* indices;

        if (!frame)
        {
            frame = RwCameraGetFrame(RwCameraGetCurrentCamera());
        }

        at = &RwFrameGetLTM(frame)->at;

        if (((RwObject*)object)->type == MATFXGCN_ATOMIC)
        {
            RwMatrix inverse;

            RwMatrixInvert(&inverse, RwFrameGetLTM(RpAtomicGetFrame((RpAtomic*)object)));
            RwV3dTransformVectors(&light, at, 1, &inverse);
        }
        else
        {
            light = *at;
        }

        RwV3dNormalize(&light, &light);

        indices = mesh->indices;

        for (i = 0; i < mesh->numIndices; i++)
        {
            U16 v0 = indices[i];

            if (!done[v0])
            {
                RwV3d* p0;
                RwV3d* p1;
                RwV3d* p2;
                RwV3d* n;
                RwTexCoords* t0;
                RwTexCoords* t1;
                RwTexCoords* t2;
                RwV3d e1;
                RwV3d e2;
                RwV3d s;
                RwV3d b;
                F32 uOffset;
                F32 vOffset;
                F32 recip;
                S32 v1;
                S32 v2;

                if (flags & rpGEOMETRYTRISTRIP)
                {
                    S32 i1;
                    S32 i2;

                    if (i < 2)
                    {
                        if (i % 2)
                        {
                            i1 = i + 2;
                            i2 = i + 1;
                        }
                        else
                        {
                            i1 = i + 1;
                            i2 = i + 2;
                        }
                    }
                    else
                    {
                        if (i % 2)
                        {
                            i1 = i - 1;
                            i2 = i - 2;
                        }
                        else
                        {
                            i1 = i - 2;
                            i2 = i - 1;
                        }
                    }

                    v1 = indices[i1];
                    v2 = indices[i2];

                    p0 = &positions[v0];
                    p1 = &positions[v1];
                    p2 = &positions[v2];
                    n = &normals[v0];
                    t0 = &texCoords[v0];
                    t1 = &texCoords[v1];
                    t2 = &texCoords[v2];
                }
                else
                {
                    v1 = indices[(i + 1) % 3 + (i / 3) * 3];
                    v2 = indices[(i + 2) % 3 + (i / 3) * 3];

                    p0 = &positions[v0];
                    p1 = &positions[v1];
                    p2 = &positions[v2];
                    n = &normals[v0];
                    t0 = &texCoords[v0];
                    t1 = &texCoords[v1];
                    t2 = &texCoords[v2];
                }


                RwV3dSubMacro(&e1, p1, p0);
                RwV3dSubMacro(&e2, p2, p0);
                RwV3dScaleMacro(&e1, &e1, t2->v - t0->v);
                RwV3dScaleMacro(&e2, &e2, t1->v - t0->v);
                RwV3dSubMacro(&s, &e1, &e2);

                recip = _rwInvSqrt(RwV3dDotProductMacro(&s, &s));
                RwV3dScaleMacro(&s, &s, recip);
                RwV3dCrossProductMacro(&b, &s, n);

                uOffset = RwV3dDotProductMacro(&s, &light);
                vOffset = RwV3dDotProductMacro(&b, &light);
                dst[indices[i]].u = t0->u - uOffset * scale;
                dst[indices[i]].v = t0->v - vOffset * scale;

                minIndex = ((S32)indices[i] < minIndex) ? indices[i] : minIndex;
                maxIndex = ((S32)indices[i] > maxIndex) ? indices[i] : maxIndex;

                done[indices[i]] = TRUE;
            }

        }

        DCFlushRange(&dst[minIndex], (maxIndex - minIndex + 1) * sizeof(RwTexCoords));
        GXInvalidateVtxCache();
    }

    RwFree(done);
}

// The destroy notify of the bump map coordinates' entry: wait until the GPU has passed its token.
void MatFXGCNBumpMapResEntryDestroy(RwResEntry* resEntry)
{
    if (MATFXGCN_TOKEN(resEntry) == _RwDlTokenCurrent)
    {
        GXSetDrawSync(_RwDlTokenCurrent);
        _RwDlTokenCurrent = (_RwDlTokenCurrent + 1) % MATFXGCN_TOKENWRAP;
    }

    while (!_rwDlTokenQueryDone(MATFXGCN_TOKEN(resEntry)))
    {
    }
}

// The atomic pipeline's instance callback: instance as usual, then compute the NBT when a
// multitexture effect needs them (giving the geometry a vertex format with NBT when it has none).
RpAtomic* _rpGCMatFXAtomicInstanceCallBack(RpAtomic* atomic, GCNObjectData* objectData)
{
    if (_rpGameCubeMTPipeDataQueryNBTs(objectData))
    {
        RpGeometry* geometry = atomic->geometry;
        RpGameCubeVtxFmt* vtxFmt = RpGameCubeGeometryGetVtxFmt(geometry);

        if (!vtxFmt)
        {
            RpGameCubeGeometrySetVtxFmt(geometry, _rpGCMatFXVtxFmtNBT);
            vtxFmt = _rpGCMatFXVtxFmtNBT;
        }

        if (!DefaultAtomicInstanceCallBack(atomic, objectData))
        {
            return NULL;
        }

        _rpGameCubeMTPipeDataCalcNBTs(objectData, vtxFmt, geometry->numVertices);
        return atomic;
    }

    return (RpAtomic*)DefaultAtomicInstanceCallBack(atomic, objectData);
}

// The atomic pipeline's reinstance callback: chains to the node's own reinstance, then recomputes
// the normal, binormal and tangent data when the geometry's polygons, vertices, normals or first
// texture coordinates were locked or the atomic is morphing.
RpAtomic* _rpGCMatFXAtomicReinstanceCallBack(RpAtomic* atomic, GCNObjectData* objectData)
{
    RpGeometry* geometry = atomic->geometry;
    RpGameCubeVtxFmt* vtxFmt = RpGameCubeGeometryGetVtxFmt(geometry);
    S32 recompute = FALSE;

    if (vtxFmt && vtxFmt->frac[1] && (geometry->flags & rpGEOMETRYNORMALS))
    {
        U32 locked = geometry->lockedSinceLastInst & 0x17;
        U32 morphing = geometry->numMorphTargets != 1 &&
                       (atomic->interpolator.flags & rpINTERPOLATORDIRTYINSTANCE);

        recompute = locked || morphing;
    }

    if (!DefaultAtomicReinstanceCallBack(atomic, objectData))
    {
        return NULL;
    }

    if (recompute)
    {
        _rpGameCubeMTPipeDataCalcNBTs(objectData, vtxFmt, geometry->numVertices);
    }

    return atomic;
}

// The world sector pipeline's instance callback.
RpWorldSector* _rpGCMatFXSectorInstanceCallBack(RpWorldSector* sector, GCNObjectData* objectData)
{
    RpGameCubeVtxFmt* vtxFmt;

    if (!DefaultSectorInstanceCallBack(sector, objectData))
    {
        return NULL;
    }

    vtxFmt = RpGameCubeWorldGetVtxFmt((RpWorld*)RWSRCGLOBAL(curWorld));

    if (vtxFmt && vtxFmt->frac[1])
    {
        _rpGameCubeMTPipeDataCalcNBTs(objectData, vtxFmt, sector->numVertices);
    }

    return sector;
}

// The alpha compare mode for a texture: 3 when its raster's parent has alpha, otherwise 1.
static S32 MatFXGCNAlphaRef(RwTexture* texture)
{
    S32 ref = 1;

    if (MATFXGCN_TEXTUREHASALPHA(texture))
    {
        ref = 3;
    }

    return ref;
}

// Loads the environment map texture matrix: the inverse of the map's frame (or _RwDlInvCamLTM),
// after the object's matrix, scaled and offset into texture space. The matrix is a full 3x4 Mtx
// of which the 2x4 load reads two rows.
static void LoadEnvMapMatrix(RwFrame* frame, const RwMatrix* matrix)
{
    RwMatrix tmp;
    RwMatrix inverse;
    F32 mtx[3][4];
    const RwMatrix* env;

    if (frame)
    {
        RwMatrixInvert(&inverse, RwFrameGetLTM(frame));
        env = &inverse;
    }
    else
    {
        env = &_RwDlInvCamLTM;
    }

    if (matrix)
    {
        RwMatrixMultiply(&tmp, matrix, env);
        env = &tmp;
    }

    mtx[0][0] = -0.5f * env->right.x;
    mtx[0][1] = -0.5f * env->up.x;
    mtx[0][2] = -0.5f * env->at.x;
    mtx[0][3] = 0.5f;
    mtx[1][0] = -0.5f * env->right.y;
    mtx[1][1] = -0.5f * env->up.y;
    mtx[1][2] = -0.5f * env->at.y;
    mtx[1][3] = 0.5f;
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
}

// Sets up the lighting channels of an environment map pass: unlit, with a material color whose
// alpha is the map's coefficient.
static void SetupEnvMapChannels(GXColor* color, MatFXEnvMapData* envMap)
{
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP,
                  GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP,
                  GX_AF_NONE);
    color->a = 255.0f * envMap->coef;
    GXSetChanMatColor(GX_COLOR0A0, *color);
}

// Sets up the TEV of an environment map pass: one texture coordinate from the normal through the
// environment matrix, modulated by the channel color.
static void SetupEnvMapTev(void)
{
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_NRM, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
}

// The render callback of both pipelines: draws each mesh with its multitexture, or with its
// material effect's passes, or plainly.
void* _rpGCMatFXRenderCallback(void* object, GCNObjectData* objectData)
{
    RwResEntry* entry = objectData->repEntry;
    GCNVertexArrays* arrays = (GCNVertexArrays*)(entry + 1);
    GCNDisplayList* displayList = MATFXGCN_DISPLAYLISTS(entry);
    RwResEntry* bumpEntry = NULL;
    RwTexCoords* bumpTexCoords = NULL;
    S32 resetTev = TRUE;
    S32 resetChannels = TRUE;
    const RwMatrix* matrix;
    RpMeshHeader* meshHeader;
    RpMesh* mesh;
    S32 numVertices;
    U32 flags;
    S32 numMeshes;
    S32 bumpSize;

    arrays->syncToken = _RwDlTokenCurrent;

    if (((RwObject*)object)->type == MATFXGCN_ATOMIC)
    {
        RpGeometry* geometry = ((RpAtomic*)object)->geometry;

        numVertices = geometry->numVertices;
        flags = geometry->flags;
        meshHeader = geometry->mesh;
        matrix = RwFrameGetLTM(RpAtomicGetFrame((RpAtomic*)object));
        _rwDlVtxFmtSetup(RpGameCubeGeometryGetVtxFmt(((RpAtomic*)object)->geometry), objectData);
    }
    else
    {
        RpWorld* world;

        numVertices = RpWorldSectorGetNumVertices((RpWorldSector*)object);
        meshHeader = ((RpWorldSector*)object)->mesh;
        matrix = NULL;
        world = (RpWorld*)RWSRCGLOBAL(curWorld);
        flags = world->flags;
        _rwDlVtxFmtSetup(RpGameCubeWorldGetVtxFmt(world), objectData);
    }

    _rwDlTransformSetup(matrix, (objectData->flags >> 4) & 1);

    numMeshes = meshHeader->numMeshes;
    mesh = (RpMesh*)(meshHeader + 1);
    bumpSize = numVertices * sizeof(RwTexCoords);

    while (numMeshes--)
    {
        RpMultiTexture* multiTexture;
        MatFXMaterialData* matFX;

        if (resetChannels)
        {
            _rxGCChannelSetup(objectData);
            resetChannels = FALSE;
        }

        _rxGCChannelMaterialSetup(objectData, mesh->material);
        multiTexture = RpMaterialGetMultiTexture(mesh->material, MATFXGCN_PLATFORM);
        matFX = MATFXMATERIALGETDATA(mesh->material);

        if (multiTexture)
        {
            _rpGameCubeMTMeshRenderCallBack(displayList, mesh->material, object,
                                            objectData, matrix);
            resetTev = TRUE;
        }
        else if (matFX)
        {
            if (resetTev)
            {
                _rxGCTevSetup(objectData);
                resetTev = FALSE;
            }

            switch (matFX->flags)
            {
            case rpMATFXEFFECTENVMAP:
            {
                MatFXEnvMapData* envMap = &MATFXMATERIALGETDATA(mesh->material)->effects[0].data.envMap;
                S32 srcBlend;
                S32 destBlend;

                RwRenderStateGet(rwRENDERSTATESRCBLEND, &srcBlend);
                RwRenderStateGet(rwRENDERSTATEDESTBLEND, &destBlend);
                _rwDlTextureSet(mesh->material->texture, 0);
                RwGameCubeSetAlphaCompare(1);
                GXCallDisplayList(displayList->list, displayList->size);
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
                SetupEnvMapChannels(&MatFXGCNEnvMapColor, envMap);
                resetChannels = TRUE;
                SetupEnvMapTev();
                resetTev = TRUE;
                _rwDlTextureSet(envMap->texture, 0);
                LoadEnvMapMatrix(envMap->frame, matrix);
                GXCallDisplayList(displayList->list, displayList->size);
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)srcBlend);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)destBlend);
                break;
            }
            case rpMATFXEFFECTDUAL:
            {
                RwTexture* texture;
                MatFXDualData* dual = &MATFXMATERIALGETDATA(mesh->material)->effects[0].data.dual;
                S32 srcBlend;
                S32 destBlend;

                RwRenderStateGet(rwRENDERSTATESRCBLEND, &srcBlend);
                RwRenderStateGet(rwRENDERSTATEDESTBLEND, &destBlend);
                texture = mesh->material->texture;
                _rwDlTextureSet(texture, 0);
                RwGameCubeSetAlphaCompare(MatFXGCNAlphaRef(texture));
                GXCallDisplayList(displayList->list, displayList->size);
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)dual->srcBlendMode);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)dual->dstBlendMode);

                if (objectData->flags & rpGEOMETRYTEXTURED2)
                {
                    GXSetArray(GX_VA_TEX0, arrays->tex[1], arrays->texStride[1]);
                }

                texture = dual->texture;
                _rwDlTextureSet(texture, 0);
                RwGameCubeSetAlphaCompare(MatFXGCNAlphaRef(texture));
                GXCallDisplayList(displayList->list, displayList->size);

                if (objectData->flags & rpGEOMETRYTEXTURED2)
                {
                    GXSetArray(GX_VA_TEX0, arrays->tex[0], arrays->texStride[0]);
                }

                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)srcBlend);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)destBlend);
                break;
            }
            case rpMATFXEFFECTBUMPMAP:
            {
                MatFXBumpMapData* bumpMap = &MATFXMATERIALGETDATA(mesh->material)->effects[0].data.bumpMap;
                S32 srcBlend;
                S32 destBlend;

                RwRenderStateGet(rwRENDERSTATESRCBLEND, &srcBlend);
                RwRenderStateGet(rwRENDERSTATEDESTBLEND, &destBlend);
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDINVSRCALPHA);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDZERO);
                _rwDlTextureSet(bumpMap->bumpTexture, 0);
                RwGameCubeSetAlphaCompare(1);
                GXCallDisplayList(displayList->list, displayList->size);
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);

                if (!bumpEntry)
                {
                    // FAKEMATCH: the entry size is set to 6 and bumpSize added in a second statement; real form believed
                    // to be one expression, bumpSize + 6, passed straight to RwResourcesAllocateResEntry.
                    S32 size = 6;

                    size += bumpSize;
                    bumpEntry = RwResourcesAllocateResEntry(MatFXGCNBumpMapResOwner, NULL, size, MatFXGCNBumpMapResEntryDestroy);
                    MATFXGCN_TOKEN(bumpEntry) = _RwDlTokenCurrent;
                    bumpTexCoords =
                        (RwTexCoords*)(((U32)bumpEntry + sizeof(RwResEntry) + 2 + 0x1F) & ~0x1F);
                }

                MatFXGCNBumpMapTexCoords(bumpTexCoords, object, bumpMap, mesh, (RwV3d*)arrays->pos,
                            (RwV3d*)arrays->nrm, (RwTexCoords*)arrays->tex[0], numVertices,
                            flags);
                GXSetArray(GX_VA_TEX0, bumpTexCoords, sizeof(RwTexCoords));
                GXCallDisplayList(displayList->list, displayList->size);
                GXSetArray(GX_VA_TEX0, arrays->tex[0], sizeof(RwTexCoords));
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)srcBlend);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)destBlend);
                break;
            }
            case rpMATFXEFFECTBUMPENVMAP:
            {
                MatFXBumpMapData* bumpMap = &MATFXMATERIALGETDATA(mesh->material)->effects[0].data.bumpMap;
                MatFXEnvMapData* envMap = &MATFXMATERIALGETDATA(mesh->material)->effects[1].data.envMap;
                S32 srcBlend;
                S32 destBlend;

                RwRenderStateGet(rwRENDERSTATESRCBLEND, &srcBlend);
                RwRenderStateGet(rwRENDERSTATEDESTBLEND, &destBlend);
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDINVSRCALPHA);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDZERO);
                _rwDlTextureSet(bumpMap->bumpTexture, 0);
                RwGameCubeSetAlphaCompare(1);
                GXCallDisplayList(displayList->list, displayList->size);
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);

                if (!bumpEntry)
                {
                    // FAKEMATCH: the entry size is set to 6 and bumpSize added in a second statement; real form believed
                    // to be one expression, bumpSize + 6, passed straight to RwResourcesAllocateResEntry.
                    S32 size = 6;

                    size += bumpSize;
                    bumpEntry = RwResourcesAllocateResEntry(MatFXGCNBumpMapResOwner, NULL, size, MatFXGCNBumpMapResEntryDestroy);
                    MATFXGCN_TOKEN(bumpEntry) = _RwDlTokenCurrent;
                    bumpTexCoords =
                        (RwTexCoords*)(((U32)bumpEntry + sizeof(RwResEntry) + 2 + 0x1F) & ~0x1F);
                }

                MatFXGCNBumpMapTexCoords(bumpTexCoords, object, bumpMap, mesh, (RwV3d*)arrays->pos,
                            (RwV3d*)arrays->nrm, (RwTexCoords*)arrays->tex[0], numVertices,
                            flags);
                GXSetArray(GX_VA_TEX0, bumpTexCoords, sizeof(RwTexCoords));
                GXCallDisplayList(displayList->list, displayList->size);
                GXSetArray(GX_VA_TEX0, arrays->tex[0], sizeof(RwTexCoords));
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
                SetupEnvMapChannels(&MatFXGCNBumpEnvMapColor, envMap);
                resetChannels = TRUE;
                SetupEnvMapTev();
                resetTev = TRUE;
                _rwDlTextureSet(envMap->texture, 0);
                LoadEnvMapMatrix(envMap->frame, matrix);
                GXCallDisplayList(displayList->list, displayList->size);
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)srcBlend);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)destBlend);
                break;
            }
            default:
            {
                RwTexture* texture = mesh->material->texture;

                _rwDlTextureSet(texture, 0);
                RwGameCubeSetAlphaCompare(MatFXGCNAlphaRef(texture));
                GXCallDisplayList(displayList->list, displayList->size);
                break;
            }
            }
        }
        else
        {
            RwTexture* texture;

            if (resetTev)
            {
                _rxGCTevSetup(objectData);
                resetTev = FALSE;
            }

            texture = mesh->material->texture;
            _rwDlTextureSet(texture, 0);
            RwGameCubeSetAlphaCompare(MatFXGCNAlphaRef(texture));
            GXCallDisplayList(displayList->list, displayList->size);
        }

        displayList++;
        mesh++;
    }

    return object;
}

// Builds the atomic pipeline: the GameCube all-in-one node with this plugin's instance, reinstance
// and render callbacks, keeping the node's own instance and reinstance callbacks to chain to.
static RxPipeline* MatFXGCNAtomicPipelineCreate(void)
{
    RxPipeline* pipeline = RxPipelineCreate();

    if (pipeline)
    {
        RxLockedPipe* lpipe;

        pipeline->pluginId = MATFXGCN_PLUGINID;
        lpipe = RxPipelineLock(pipeline);

        if (lpipe)
        {
            RxNodeDefinition* nodeDef = RxNodeDefinitionGetGameCubeAtomicAllInOne();
            RxPipelineNode* node;

            RxLockedPipeUnlock(RxLockedPipeAddFragment(lpipe, NULL, nodeDef, NULL));
            node = RxPipelineFindNodeByName(pipeline, nodeDef->name, NULL, NULL);
            DefaultAtomicInstanceCallBack = (GCNMTInstanceCB)_rxGameCubeAllInOneGetInstanceCallBack(node);
            DefaultAtomicReinstanceCallBack = (GCNMTInstanceCB)_rxGameCubeAllInOneGetReinstanceCallBack(node);
            _rxGameCubeAllInOneSetInstanceCallBack(node, _rpGCMatFXAtomicInstanceCallBack);
            _rxGameCubeAllInOneSetReinstanceCallBack(node, _rpGCMatFXAtomicReinstanceCallBack);
            RxGameCubeAllInOneSetRenderCallBack(node, _rpGCMatFXRenderCallback);

            return pipeline;
        }

        _rxPipelineDestroy(pipeline);
    }

    return NULL;
}

// Builds the world sector pipeline the same way, chaining only the instance callback.
static RxPipeline* MatFXGCNWorldSectorPipelineCreate(void)
{
    RxPipeline* pipeline = RxPipelineCreate();

    if (pipeline)
    {
        RxLockedPipe* lpipe;

        pipeline->pluginId = MATFXGCN_PLUGINID;
        lpipe = RxPipelineLock(pipeline);

        if (lpipe)
        {
            RxNodeDefinition* nodeDef = RxNodeDefinitionGetGameCubeWorldSectorAllInOne();
            RxPipelineNode* node;

            RxLockedPipeUnlock(RxLockedPipeAddFragment(lpipe, NULL, nodeDef, NULL));
            node = RxPipelineFindNodeByName(pipeline, nodeDef->name, NULL, NULL);
            DefaultSectorInstanceCallBack = (GCNMTInstanceCB)_rxGameCubeAllInOneGetInstanceCallBack(node);
            _rxGameCubeAllInOneSetInstanceCallBack(node, _rpGCMatFXSectorInstanceCallBack);
            RxGameCubeAllInOneSetRenderCallBack(node, _rpGCMatFXRenderCallback);

            return pipeline;
        }

        _rxPipelineDestroy(pipeline);
    }

    return NULL;
}

// Creates the vertex format and the atomic and world sector pipelines; the counterpart of
// _rpMatFXPipelinesDestroy.
S32 _rpMatFXPipelinesCreate(void)
{
    _rpGCMatFXVtxFmtNBT = RpGameCubeVtxFmtCreate();
    RpGameCubeVtxFmtSetNormal(_rpGCMatFXVtxFmtNBT, 4, 1);

    _RpMatFXAtomicPipe = MatFXGCNAtomicPipelineCreate();
    _RpMatFXWorldSectorPipe = MatFXGCNWorldSectorPipelineCreate();

    return TRUE;
}

S32 _rpMatFXPipelinesDestroy(void)
{
    if (_rpGCMatFXVtxFmtNBT)
    {
        RpGameCubeVtxFmtDestroy(_rpGCMatFXVtxFmtNBT);
        _rpGCMatFXVtxFmtNBT = NULL;
    }

    if (_RpMatFXAtomicPipe)
    {
        _rxPipelineDestroy(_RpMatFXAtomicPipe);
        _RpMatFXAtomicPipe = NULL;
    }

    if (_RpMatFXWorldSectorPipe)
    {
        _rxPipelineDestroy(_RpMatFXWorldSectorPipe);
        _RpMatFXWorldSectorPipe = NULL;
    }

    return TRUE;
}

RpAtomic* _rpMatFXPipelineAtomicSetup(RpAtomic* atomic)
{
    atomic->pipeline = _RpMatFXAtomicPipe;

    return atomic;
}

RpWorldSector* _rpMatFXPipelineWorldSectorSetup(RpWorldSector* worldSector)
{
    worldSector->pipeline = _RpMatFXWorldSectorPipe;

    return worldSector;
}

// The per-effect material setup hooks: nothing to do on this platform.
void MatFXGCNSetupEnvMap(RpMaterial* material)
{
}

void MatFXGCNSetupBumpMap(RpMaterial* material)
{
}

void MatFXGCNSetupDual(RpMaterial* material)
{
}

S32 _rpMatFXSetupDualRenderState(MatFXDualData* dualData, RwRenderState renderState)
{
    return TRUE;
}

RwTexture* _rpMatFXSetupBumpMapTexture(const RwTexture* baseTexture,
                                       const RwTexture* effectTexture)
{
    return _rpMatFXTextureMaskCreate(baseTexture, effectTexture);
}
