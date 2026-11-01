#include <rpmatfx.h>
#include <rpmtexgcn.h>
#include <string.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>

// The GameCube material effects pipelines: an atomic and a world sector all-in-one pipeline whose
// render callback draws each mesh once more per effect (bump map, environment map, both, or a
// dual texture), and whose instance callbacks compute the NBT multitexture effects need.

// The draw sync token to issue next.
extern U16 lbl_8025E800;

// The raster plugin's offset in a raster.
extern S32 lbl_8025F870;

// A matrix the environment map is relative to when it has no frame.
extern RwMatrix lbl_8025E304;

// The owner of the bump map texture coordinates' resource entries.
extern U8 lbl_8025D958[0x70];

// The material colors of the environment map passes; their alpha is set from the coefficient.
extern GXColor lbl_8025E780;
extern GXColor lbl_8025E784;

// The vertex format given to geometries that need the NBT and have none, the two pipelines, and
// the all-in-one nodes' callbacks the pipelines' own ones call.
typedef void* (*GCNMTInstanceCB)(void* object, GCNObjectData* objectData);

extern RpGameCubeVtxFmt* lbl_8025F680;
extern RxPipeline* lbl_8025F684;
extern RxPipeline* lbl_8025F688;
extern GCNMTInstanceCB lbl_8025F68C;
extern GCNMTInstanceCB lbl_8025F690;
extern GCNMTInstanceCB lbl_8025F694;

S32 _rwDlTokenQueryDone(U16 token);
RpGameCubeVtxFmt* fn_8015D818(const RpGeometry* geometry);
RpGameCubeVtxFmt* fn_8015D824(const RpWorld* world);
RpGameCubeVtxFmt* RpGameCubeVtxFmtCreate(void);
void RpGameCubeVtxFmtDestroy(RpGameCubeVtxFmt* vtxFmt);
void RpGameCubeVtxFmtSetNormal(RpGameCubeVtxFmt* vtxFmt, U8 compType, U8 frac);
void RpGameCubeGeometrySetVtxFmt(RpGeometry* geometry, RpGameCubeVtxFmt* vtxFmt);
void RxGameCubeAllInOneSetRenderCallBack(RxPipelineNode* node, void* callback);
void _rxGameCubeAllInOneSetInstanceCallBack(RxPipelineNode* node, void* callback);
void* _rxGameCubeAllInOneGetInstanceCallBack(RxPipelineNode* node);
void _rxGameCubeAllInOneSetReinstanceCallBack(RxPipelineNode* node, void* callback);
void* _rxGameCubeAllInOneGetReinstanceCallBack(RxPipelineNode* node);
RxNodeDefinition* RxNodeDefinitionGetGameCubeWorldSectorAllInOne(void);
void _rwDlVtxFmtSetup(RpGameCubeVtxFmt* vtxFmt, GCNObjectData* objectData);
void _rwDlTransformSetup(const RwMatrix* matrix, S32 normals);
void _rwDlTextureSet(RwTexture* texture, U32 stage);
void RwGameCubeSetAlphaCompare(S32 mode);
void _rpGameCubeMTMeshRenderCallBack(GCNDisplayList* displayList, RpMaterial* material,
                                     void* object, GCNObjectData* objectData,
                                     const RwMatrix* matrix);
void fn_80141D20(GCNObjectData* objectData, RpGameCubeVtxFmt* vtxFmt, S32 numVertices);
void fn_801588E8(GCNObjectData* objectData);
void fn_80158B20(GCNObjectData* objectData, RpMaterial* material);
void fn_80158DC4(GCNObjectData* objectData);
RwTexture* fn_8013E95C(const RwTexture* baseTexture, const RwTexture* effectTexture);

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
     (*(U32*)((U8*)(_texture)->raster + lbl_8025F870 + 0x14) & 1))

// The display lists follow the instanced vertex arrays in the entry.
#define MATFXGCN_DISPLAYLISTS(_entry)                                                              \
    ((GCNDisplayList*)((U8*)((GCNVertexArrays*)((_entry) + 1)) + sizeof(GCNVertexArrays)))

// Offsets each vertex's base texture coordinates along the bump map light direction, into dst:
// the light (the bump map frame's at axis, or the camera's) is taken into the object's space,
// and each vertex's offset comes from the texture space of one of its triangles.
void fn_8013FA64(RwTexCoords* dst, void* object, MatFXBumpMapData* bumpMap, RpMesh* mesh,
                 RwV3d* positions, RwV3d* normals, RwTexCoords* texCoords, S32 numVertices,
                 U32 flags)
{
    U8* done = (U8*)RwMalloc(numVertices);

    memset(done, 0, numVertices);

    if (done)
    {
        RwFrame* frame = bumpMap->frame;
        S32 minIndex = 0x7FFFFFFF;
        S32 maxIndex = 0x80000000;
        F32 scale = bumpMap->coef * bumpMap->invBumpWidth;
        RwV3d* at;
        RwV3d light;
        U16* indices;
        U16* index;
        U32 i;

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
        index = indices;

        for (i = 0; i < mesh->numIndices; i++)
        {
            U16 v0 = *index;

            if (!done[v0])
            {
                RwV3d* p0;
                RwV3d* p1;
                RwV3d* p2;
                RwV3d* n;
                RwTexCoords* t0;
                RwTexCoords* t1;
                RwTexCoords* t2;
                RwV3d s;
                F32 dv1;
                F32 dv2;
                F32 recip;
                U16 v1;
                U16 v2;

                if (flags & rpGEOMETRYTRISTRIP)
                {
                    S32 i1;
                    S32 i2;

                    if ((S32)i < 2)
                    {
                        if ((S32)i % 2)
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
                        if ((S32)i % 2)
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
                }
                else
                {
                    S32 base = (i / 3) * 3;

                    v1 = indices[(i + 1) % 3 + base];
                    v2 = indices[(i + 2) % 3 + base];
                }

                p0 = &positions[v0];
                n = &normals[v0];
                t0 = &texCoords[v0];
                p1 = &positions[v1];
                t1 = &texCoords[v1];
                p2 = &positions[v2];
                t2 = &texCoords[v2];

                dv2 = t2->v - t0->v;
                dv1 = t1->v - t0->v;
                s.x = (p1->x - p0->x) * dv2 - (p2->x - p0->x) * dv1;
                s.y = (p1->y - p0->y) * dv2 - (p2->y - p0->y) * dv1;
                s.z = (p1->z - p0->z) * dv2 - (p2->z - p0->z) * dv1;

                recip = _rwInvSqrt(s.x * s.x + s.y * s.y + s.z * s.z);
                s.x *= recip;
                s.y *= recip;
                s.z *= recip;

                dst[*index].u = t0->u - (s.x * light.x + s.y * light.y + s.z * light.z) * scale;
                dst[*index].v = t0->v - ((s.y * n->z - s.z * n->y) * light.x +
                                         (s.z * n->x - s.x * n->z) * light.y +
                                         (s.x * n->y - s.y * n->x) * light.z) *
                                            scale;

                if ((S32)*index < minIndex)
                {
                    minIndex = *index;
                }

                if ((S32)*index > maxIndex)
                {
                    maxIndex = *index;
                }

                done[*index] = TRUE;
            }

            index++;
        }

        DCFlushRange(&dst[minIndex], (maxIndex - minIndex + 1) * sizeof(RwTexCoords));
        GXInvalidateVtxCache();
    }

    RwFree(done);
}

// The destroy notify of the bump map coordinates' entry: wait until the GPU has passed its token.
void fn_8013FEC0(RwResEntry* resEntry)
{
    if (MATFXGCN_TOKEN(resEntry) == lbl_8025E800)
    {
        GXSetDrawSync(lbl_8025E800);
        lbl_8025E800 = (lbl_8025E800 + 1) % MATFXGCN_TOKENWRAP;
    }

    while (!_rwDlTokenQueryDone(MATFXGCN_TOKEN(resEntry)))
    {
    }
}

// The atomic pipeline's instance callback: instance as usual, then compute the NBT when a
// multitexture effect needs them (giving the geometry a vertex format with NBT when it has none).
RpAtomic* fn_8013FF48(RpAtomic* atomic, GCNObjectData* objectData)
{
    if (_rpGameCubeMTPipeDataQueryNBTs(objectData))
    {
        RpGeometry* geometry = atomic->geometry;
        RpGameCubeVtxFmt* vtxFmt = fn_8015D818(geometry);

        if (!vtxFmt)
        {
            RpGameCubeGeometrySetVtxFmt(geometry, lbl_8025F680);
            vtxFmt = lbl_8025F680;
        }

        if (!lbl_8025F694(atomic, objectData))
        {
            return NULL;
        }

        fn_80141D20(objectData, vtxFmt, geometry->numVertices);
        return atomic;
    }

    return (RpAtomic*)lbl_8025F694(atomic, objectData);
}

RpAtomic* _rpGCMatFXAtomicReinstanceCallBack(RpAtomic* atomic, GCNObjectData* objectData)
{
    RpGeometry* geometry = atomic->geometry;
    RpGameCubeVtxFmt* vtxFmt = fn_8015D818(geometry);
    S32 recompute = FALSE;

    if (vtxFmt && vtxFmt->frac[1] && (geometry->flags & rpGEOMETRYNORMALS))
    {
        U32 morphing = FALSE;

        if (geometry->numMorphTargets != 1 &&
            (atomic->interpolator.flags & rpINTERPOLATORDIRTYINSTANCE))
        {
            morphing = TRUE;
        }

        recompute = (geometry->lockedSinceLastInst & 0x17) || morphing;
    }

    if (!lbl_8025F690(atomic, objectData))
    {
        return NULL;
    }

    if (recompute)
    {
        fn_80141D20(objectData, vtxFmt, geometry->numVertices);
    }

    return atomic;
}

// The world sector pipeline's instance callback.
RpWorldSector* fn_801400F8(RpWorldSector* sector, GCNObjectData* objectData)
{
    RpGameCubeVtxFmt* vtxFmt;

    if (!lbl_8025F68C(sector, objectData))
    {
        return NULL;
    }

    vtxFmt = fn_8015D824((RpWorld*)RWSRCGLOBAL(curWorld));

    if (vtxFmt && vtxFmt->frac[1])
    {
        fn_80141D20(objectData, vtxFmt, sector->numVertices);
    }

    return sector;
}

// Loads the environment map texture matrix: the inverse of the map's frame (or lbl_8025E304),
// after the object's matrix, scaled and offset into texture space.
static void LoadEnvMapMatrix(RwFrame* frame, const RwMatrix* matrix)
{
    RwMatrix inverse;
    RwMatrix tmp;
    const RwMatrix* env;
    F32 mtx[2][4];

    if (frame)
    {
        RwMatrixInvert(&inverse, RwFrameGetLTM(frame));
        env = &inverse;
    }
    else
    {
        env = &lbl_8025E304;
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

// Sets up an environment map pass: additive blending, a material color whose alpha is the
// coefficient, one texture coordinate from the normal through the environment matrix.
static void SetupEnvMapPass(GXColor* color, F32 coef)
{
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    color->a = 255.0f * coef;
    GXSetChanMatColor(GX_COLOR0A0, *color);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_NRM, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
}

// The render callback of both pipelines: draws each mesh with its multitexture, or with its
// material effect's passes, or plainly.
void* fn_80140178(void* object, GCNObjectData* objectData)
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

    arrays->syncToken = lbl_8025E800;

    if (((RwObject*)object)->type == MATFXGCN_ATOMIC)
    {
        RpGeometry* geometry = ((RpAtomic*)object)->geometry;

        numVertices = geometry->numVertices;
        flags = geometry->flags;
        meshHeader = geometry->mesh;
        matrix = RwFrameGetLTM(RpAtomicGetFrame((RpAtomic*)object));
        _rwDlVtxFmtSetup(fn_8015D818(((RpAtomic*)object)->geometry), objectData);
    }
    else
    {
        RpWorld* world;

        numVertices = RpWorldSectorGetNumVertices((RpWorldSector*)object);
        meshHeader = ((RpWorldSector*)object)->mesh;
        matrix = NULL;
        world = (RpWorld*)RWSRCGLOBAL(curWorld);
        flags = world->flags;
        _rwDlVtxFmtSetup(fn_8015D824(world), objectData);
    }

    _rwDlTransformSetup(matrix, (objectData->flags >> 4) & 1);

    numMeshes = meshHeader->numMeshes;
    mesh = (RpMesh*)(meshHeader + 1);

    while (numMeshes--)
    {
        RpMaterial* material = mesh->material;
        MatFXMaterialData* matFX;

        if (resetChannels)
        {
            fn_801588E8(objectData);
            resetChannels = FALSE;
        }

        fn_80158B20(objectData, material);
        matFX = MATFXMATERIALGETDATA(mesh->material);

        if (RpMaterialGetMultiTexture(mesh->material, MATFXGCN_PLATFORM))
        {
            _rpGameCubeMTMeshRenderCallBack(displayList, mesh->material, object,
                                            objectData, matrix);
            resetTev = TRUE;
        }
        else if (matFX)
        {
            if (resetTev)
            {
                fn_80158DC4(objectData);
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
                SetupEnvMapPass(&lbl_8025E780, envMap->coef);
                resetChannels = TRUE;
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
                MatFXDualData* dual = &MATFXMATERIALGETDATA(mesh->material)->effects[0].data.dual;
                RwTexture* texture;
                S32 srcBlend;
                S32 destBlend;

                RwRenderStateGet(rwRENDERSTATESRCBLEND, &srcBlend);
                RwRenderStateGet(rwRENDERSTATEDESTBLEND, &destBlend);
                texture = mesh->material->texture;
                _rwDlTextureSet(texture, 0);
                RwGameCubeSetAlphaCompare(MATFXGCN_TEXTUREHASALPHA(texture) ? 3 : 1);
                GXCallDisplayList(displayList->list, displayList->size);
                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)dual->srcBlendMode);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)dual->dstBlendMode);

                if (objectData->flags & rpGEOMETRYTEXTURED2)
                {
                    GXSetArray(GX_VA_TEX0, arrays->tex[1], arrays->texStride[1]);
                }

                texture = dual->texture;
                _rwDlTextureSet(texture, 0);
                RwGameCubeSetAlphaCompare(MATFXGCN_TEXTUREHASALPHA(texture) ? 3 : 1);
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
            case rpMATFXEFFECTBUMPENVMAP:
            {
                MatFXMaterialData* data = MATFXMATERIALGETDATA(mesh->material);
                MatFXBumpMapData* bumpMap = &data->effects[0].data.bumpMap;
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
                    bumpEntry = RwResourcesAllocateResEntry(
                        lbl_8025D958, NULL, numVertices * sizeof(RwTexCoords) + 6, fn_8013FEC0);
                    MATFXGCN_TOKEN(bumpEntry) = lbl_8025E800;
                    bumpTexCoords =
                        (RwTexCoords*)(((U32)bumpEntry + sizeof(RwResEntry) + 6 + 0x1F) & ~0x1F);
                }

                fn_8013FA64(bumpTexCoords, object, bumpMap, mesh, (RwV3d*)arrays->pos,
                            (RwV3d*)arrays->nrm, (RwTexCoords*)arrays->tex[0], numVertices,
                            flags);
                GXSetArray(GX_VA_TEX0, bumpTexCoords, sizeof(RwTexCoords));
                GXCallDisplayList(displayList->list, displayList->size);
                GXSetArray(GX_VA_TEX0, arrays->tex[0], sizeof(RwTexCoords));

                if (matFX->flags == rpMATFXEFFECTBUMPENVMAP)
                {
                    MatFXEnvMapData* envMap = &data->effects[1].data.envMap;

                    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
                    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
                    SetupEnvMapPass(&lbl_8025E784, envMap->coef);
                    resetChannels = TRUE;
                    resetTev = TRUE;
                    _rwDlTextureSet(envMap->texture, 0);
                    LoadEnvMapMatrix(envMap->frame, matrix);
                    GXCallDisplayList(displayList->list, displayList->size);
                }

                RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)srcBlend);
                RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)destBlend);
                break;
            }
            default:
            {
                RwTexture* texture = mesh->material->texture;

                _rwDlTextureSet(texture, 0);
                RwGameCubeSetAlphaCompare(MATFXGCN_TEXTUREHASALPHA(texture) ? 3 : 1);
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
                fn_80158DC4(objectData);
                resetTev = FALSE;
            }

            texture = mesh->material->texture;
            _rwDlTextureSet(texture, 0);
            RwGameCubeSetAlphaCompare(MATFXGCN_TEXTUREHASALPHA(texture) ? 3 : 1);
            GXCallDisplayList(displayList->list, displayList->size);
        }

        displayList++;
        mesh++;
    }

    return object;
}

// Creates the atomic and world sector pipelines, keeping the default callbacks the pipelines'
// own ones call.
S32 fn_80140AF4(void)
{
    RxPipeline* pipeline;
    RxLockedPipe* lpipe;
    RxNodeDefinition* nodeDef;
    RxPipelineNode* node;

    lbl_8025F680 = RpGameCubeVtxFmtCreate();
    RpGameCubeVtxFmtSetNormal(lbl_8025F680, 4, 1);

    pipeline = RxPipelineCreate();

    if (pipeline)
    {
        pipeline->pluginId = MATFXGCN_PLUGINID;
        lpipe = RxPipelineLock(pipeline);

        if (lpipe)
        {
            nodeDef = RxNodeDefinitionGetGameCubeAtomicAllInOne();
            RxLockedPipeUnlock(RxLockedPipeAddFragment(lpipe, NULL, nodeDef, NULL));
            node = RxPipelineFindNodeByName(pipeline, nodeDef->name, NULL, NULL);
            lbl_8025F694 = (GCNMTInstanceCB)_rxGameCubeAllInOneGetInstanceCallBack(node);
            lbl_8025F690 = (GCNMTInstanceCB)_rxGameCubeAllInOneGetReinstanceCallBack(node);
            _rxGameCubeAllInOneSetInstanceCallBack(node, fn_8013FF48);
            _rxGameCubeAllInOneSetReinstanceCallBack(node, _rpGCMatFXAtomicReinstanceCallBack);
            RxGameCubeAllInOneSetRenderCallBack(node, fn_80140178);
        }
        else
        {
            _rxPipelineDestroy(pipeline);
            pipeline = NULL;
        }
    }
    else
    {
        pipeline = NULL;
    }

    lbl_8025F684 = pipeline;

    pipeline = RxPipelineCreate();

    if (pipeline)
    {
        pipeline->pluginId = MATFXGCN_PLUGINID;
        lpipe = RxPipelineLock(pipeline);

        if (lpipe)
        {
            nodeDef = RxNodeDefinitionGetGameCubeWorldSectorAllInOne();
            RxLockedPipeUnlock(RxLockedPipeAddFragment(lpipe, NULL, nodeDef, NULL));
            node = RxPipelineFindNodeByName(pipeline, nodeDef->name, NULL, NULL);
            lbl_8025F68C = (GCNMTInstanceCB)_rxGameCubeAllInOneGetInstanceCallBack(node);
            _rxGameCubeAllInOneSetInstanceCallBack(node, fn_801400F8);
            RxGameCubeAllInOneSetRenderCallBack(node, fn_80140178);
        }
        else
        {
            _rxPipelineDestroy(pipeline);
            pipeline = NULL;
        }
    }
    else
    {
        pipeline = NULL;
    }

    lbl_8025F688 = pipeline;

    return TRUE;
}

S32 _rpMatFXPipelinesDestroy(void)
{
    if (lbl_8025F680)
    {
        RpGameCubeVtxFmtDestroy(lbl_8025F680);
        lbl_8025F680 = NULL;
    }

    if (lbl_8025F684)
    {
        _rxPipelineDestroy(lbl_8025F684);
        lbl_8025F684 = NULL;
    }

    if (lbl_8025F688)
    {
        _rxPipelineDestroy(lbl_8025F688);
        lbl_8025F688 = NULL;
    }

    return TRUE;
}

RpAtomic* _rpMatFXPipelineAtomicSetup(RpAtomic* atomic)
{
    atomic->pipeline = lbl_8025F684;

    return atomic;
}

RpWorldSector* _rpMatFXPipelineWorldSectorSetup(RpWorldSector* worldSector)
{
    worldSector->pipeline = lbl_8025F688;

    return worldSector;
}

// The per-effect material setup hooks: nothing to do on this platform.
void fn_80140D0C(RpMaterial* material)
{
}

void fn_80140D10(RpMaterial* material)
{
}

void fn_80140D14(RpMaterial* material)
{
}

S32 _rpMatFXSetupDualRenderState(MatFXDualData* dualData, RwRenderState renderState)
{
    return TRUE;
}

RwTexture* _rpMatFXSetupBumpMapTexture(const RwTexture* baseTexture,
                                       const RwTexture* effectTexture)
{
    return fn_8013E95C(baseTexture, effectTexture);
}
