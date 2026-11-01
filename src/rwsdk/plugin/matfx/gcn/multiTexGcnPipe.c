#include <rpmtexgcn.h>
#include <string.h>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>

// The GameCube multitexture pipeline support: frames that texture matrices can follow, the
// normals, binormals and tangents (NBT) an effect's bump mapping needs, and the per-mesh render
// callback that sends an effect's TEV setup to GX and draws the mesh.

void RwGameCubeSetAlphaCompare(S32 mode);
void _rwDlTextureSet(RwTexture* texture, U32 stage);

// The engine block of this plugin: the frames texture matrices can follow, by index.
// Written for this project.
typedef struct GCNMTGlobals GCNMTGlobals;
struct GCNMTGlobals
{
    U32 numFrames;
    RwFrame** frames;
};

// The plugin's offset in the engine instance.
extern S32 lbl_8025F698;

#define GCNMTGLOBAL(var) (((GCNMTGlobals*)((U8*)RwEngineInstance + lbl_8025F698))->var)

// A matrix the effects can be relative to (frame 3).
extern RwMatrix lbl_8025E304;

// When the lit color takes the first two TEV stages, these replace the color and alpha inputs
// of a stage that reads the rasterized color with the previous stage's output.
extern const S32 lbl_801DD400[16];
extern const S32 lbl_801DD440[8];

// The pre- and post-render callbacks of a multitexture, and their data. Written for this project.
typedef void (*GCNMTRenderCB)(RpMaterial* material, void* object, GCNObjectData* objectData,
                              void* data);

typedef struct GCNMTPlatformData GCNMTPlatformData;
struct GCNMTPlatformData
{
    GCNMTRenderCB preRender;
    GCNMTRenderCB postRender;
    void* data;
};

// A vertex's normal, binormal and tangent, as GX reads them (GX_VA_NBT).
typedef struct GCNMTNBT GCNMTNBT;
struct GCNMTNBT
{
    RwV3d normal;
    RwV3d binormal;
    RwV3d tangent;
};

// The plugin id and this platform. Written for this project.
#define MTGCN_PIPEPLUGINID 0x129
#define MTGCN_PLATFORM 6

// A binormal not computed yet.
#define MTGCN_NBTUNSET 3.402823466e+38f

// Whether a texture coordinate generator needs the NBT: it reads the binormal or the tangent, or
// it is a bump map generator.
#define MTGCN_TEXGENNEEDSNBT(_gen)                                                                 \
    ((_gen)->src == GX_TG_BINRM || (_gen)->src == GX_TG_TANGENT ||                                 \
     ((_gen)->func >= GX_TG_BUMP0 && (_gen)->func <= GX_TG_BUMP7))

void* GameCubeMTOpen(void* instance, S32 offset, S32 size)
{
    memset((U8*)RwEngineInstance + lbl_8025F698, 0, sizeof(GCNMTGlobals));

    return instance;
}

void* fn_80141954(void* instance, S32 offset, S32 size)
{
    return instance;
}

// Computes the binormal and tangent of every vertex of a mesh that does not have them yet, from
// the positions, the normal and the first texture coordinates of one of its triangles.
void fn_80141958(RpMesh* mesh, GCNVertexArrays* arrays, RwPrimitiveType primType)
{
    RwV3d* positions = (RwV3d*)arrays->pos;
    GCNMTNBT* nbts = (GCNMTNBT*)arrays->nrm;
    RwTexCoords* texCoords = (RwTexCoords*)arrays->tex[0];
    U16* indices = mesh->indices;
    U32 i;
    U16 a;
    U16 b;

    for (i = 0; i < mesh->numIndices; i++)
    {
        U16 index = indices[i];
        GCNMTNBT* nbt = &nbts[index];

        if (nbt->binormal.x == MTGCN_NBTUNSET)
        {
            RwV3d* p0;
            RwV3d* p1;
            RwV3d* p2;
            RwTexCoords* t0;
            RwTexCoords* t1;
            RwTexCoords* t2;
            RwV3d s;
            F32 du1;
            F32 du2;
            F32 dv1;
            F32 dv2;
            F32 dot;
            F32 lengthSq;

            if (primType == rwPRIMTYPETRISTRIP)
            {
                if ((S32)i < 2)
                {
                    if ((S32)i == 0)
                    {
                        a = indices[1];
                        b = indices[2];
                    }
                    else
                    {
                        a = indices[2];
                        b = indices[0];
                    }
                }
                else
                {
                    a = indices[i - 2];
                    b = indices[i - 1];

                    if (a == b)
                    {
                        U16 next = indices[i + 1];

                        if (index == next)
                        {
                            i++;
                            a = indices[i + 1];

                            if (index == a)
                            {
                                i++;
                                a = indices[i + 1];
                            }

                            b = indices[i + 2];
                        }
                        else
                        {
                            a = b;
                            b = next;
                        }
                    }

                    if (i & 1)
                    {
                        U16 swap = b;
                        b = a;
                        a = swap;
                    }
                }
            }
            else
            {
                switch (i % 3)
                {
                case 0:
                    a = indices[i + 1];
                    b = indices[i + 2];
                    break;
                case 1:
                    a = indices[i + 1];
                    b = indices[i - 1];
                    break;
                case 2:
                    a = indices[i - 2];
                    b = indices[i - 1];
                    break;
                }
            }

            p0 = &positions[index];
            p1 = &positions[a];
            p2 = &positions[b];
            t0 = &texCoords[index];
            t1 = &texCoords[a];
            t2 = &texCoords[b];

            dv2 = t2->v - t0->v;
            dv1 = t1->v - t0->v;
            s.x = (p2->x - p0->x) * dv1 - (p1->x - p0->x) * dv2;
            s.y = (p2->y - p0->y) * dv1 - (p1->y - p0->y) * dv2;
            s.z = (p2->z - p0->z) * dv1 - (p1->z - p0->z) * dv2;
            du2 = t2->u - t0->u;
            du1 = t1->u - t0->u;

            dot = -(s.x * nbt->normal.x + s.y * nbt->normal.y + s.z * nbt->normal.z);
            s.x += dot * nbt->normal.x;
            s.y += dot * nbt->normal.y;
            s.z += dot * nbt->normal.z;

            lengthSq = s.x * s.x + s.y * s.y + s.z * s.z;

            if (lengthSq > 0.0f)
            {
                F32 recip = _rwInvSqrt(lengthSq);

                s.x *= recip;
                s.y *= recip;
                s.z *= recip;
            }

            nbt->tangent.x = s.y * nbt->normal.z - s.z * nbt->normal.y;
            nbt->tangent.y = s.z * nbt->normal.x - s.x * nbt->normal.z;
            nbt->tangent.z = s.x * nbt->normal.y - s.y * nbt->normal.x;

            if (dv1 * du2 < dv2 * du1)
            {
                nbt->binormal.x = -s.x;
                nbt->binormal.y = -s.y;
                nbt->binormal.z = -s.z;
            }
            else
            {
                nbt->binormal = s;
            }
        }
    }
}

// Whether a mesh's material has a GameCube effect whose texture coordinates need the NBT.
static S32 MeshNeedsNBTs(RpMesh* mesh)
{
    RpMultiTexture* multiTexture = RpMaterialGetMultiTexture(mesh->material, MTGCN_PLATFORM);

    if (multiTexture && multiTexture->effect &&
        multiTexture->effect->platform == MTGCN_PLATFORM)
    {
        GCNMTConfig* config = RpGameCubeMTEffectGetConfig(multiTexture->effect);
        S32 i;

        for (i = 0; i < config->numTexCoordGens; i++)
        {
            if (MTGCN_TEXGENNEEDSNBT(&config->texCoordGens[i]))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

// Recomputes the binormals and tangents of an instanced object for the meshes that need them.
void fn_80141D20(GCNObjectData* objectData, RpGameCubeVtxFmt* vtxFmt, S32 numVertices)
{
    RpMeshHeader* meshHeader = objectData->meshHeader;
    GCNVertexArrays* arrays = (GCNVertexArrays*)(objectData->repEntry + 1);
    GCNMTNBT* nbt = (GCNMTNBT*)arrays->nrm;
    RwPrimitiveType primType =
        MESHGLOBAL(meshFlagsToPrimType)[meshHeader->flags & rpMESHHEADERPRIMMASK];
    RpMesh* mesh;
    S32 i;

    for (i = 0; i < numVertices; i++)
    {
        nbt[i].binormal.x = MTGCN_NBTUNSET;
    }

    mesh = (RpMesh*)(meshHeader + 1);

    for (i = 0; i < meshHeader->numMeshes; i++)
    {
        if (MeshNeedsNBTs(mesh))
        {
            fn_80141958(mesh, arrays, primType);
        }

        mesh++;
    }

    DCFlushRange(arrays->nrm, numVertices * arrays->nrmStride);
    GXInvalidateVtxCache();
}

S32 _rpGameCubeMTPipeDataQueryNBTs(GCNObjectData* objectData)
{
    RpMeshHeader* meshHeader = objectData->meshHeader;
    RpMesh* mesh = (RpMesh*)(meshHeader + 1);
    S32 i;

    for (i = 0; i < meshHeader->numMeshes; i++)
    {
        if (MeshNeedsNBTs(mesh))
        {
            return TRUE;
        }

        mesh++;
    }

    return FALSE;
}

// The matrix a texture matrix is relative to: 16 and up are the plugin's frames (the inverse of
// the frame's LTM, after the object's matrix when there is one), 3 is lbl_8025E304 and 2 the
// object's matrix. Returns NULL when the texture matrix is used as it is.
RwMatrix* GetTexFrameMatrix(S32 frame, const RwMatrix* matrix, RwMatrix* result)
{
    if (frame >= 0x10)
    {
        U32 index = frame - 0x10;

        if (index < GCNMTGLOBAL(numFrames) && GCNMTGLOBAL(frames) && GCNMTGLOBAL(frames)[index])
        {
            if (matrix)
            {
                RwMatrix inverse;

                RwMatrixInvert(&inverse, RwFrameGetLTM(GCNMTGLOBAL(frames)[index]));
                RwMatrixMultiply(result, matrix, &inverse);
            }
            else
            {
                RwMatrixInvert(result, RwFrameGetLTM(GCNMTGLOBAL(frames)[index]));
            }

            return result;
        }

        if (matrix)
        {
            return (RwMatrix*)matrix;
        }

        RwMatrixSetIdentityMacro(result);

        return result;
    }

    switch (frame)
    {
    case 3:
        if (matrix)
        {
            RwMatrixMultiply(result, matrix, &lbl_8025E304);
            return result;
        }

        return &lbl_8025E304;
    case 2:
        if (matrix)
        {
            return (RwMatrix*)matrix;
        }

        return NULL;
    default:
        return NULL;
    }
}

// Sends an effect's TEV setup to GX: colors, texture and indirect matrices, indirect stages, TEV
// stages and texture coordinate generators, then the alpha compare and blend modes. With
// lighting, one or two stages first add the lit color channels.
void _rpGameCubeMTEffectSend(RpMTEffect* effect, U32 numChannels, U32 numTextures,
                             const U8* coords, const RwMatrix* matrix)
{
    GCNMTConfig* config = RpGameCubeMTEffectGetConfig(effect);
    S8 texMapOfCoord[8];
    S32 firstStage;
    S32 lighting;
    S32 i;

    if (config->flags & GCNMT_TEVCOLORS)
    {
        GXSetTevColorS10(GX_TEVREG0, ((GXColorS10*)config->tevColors)[0]);
        GXSetTevColorS10(GX_TEVREG1, ((GXColorS10*)config->tevColors)[1]);
        GXSetTevColorS10(GX_TEVREG2, ((GXColorS10*)config->tevColors)[2]);
        GXSetTevColorS10(GX_TEVPREV, ((GXColorS10*)config->tevColors)[3]);
    }

    if (config->flags & GCNMT_KCOLORS)
    {
        GXSetTevKColor(GX_KCOLOR0, ((GXColor*)config->kColors)[0]);
        GXSetTevKColor(GX_KCOLOR1, ((GXColor*)config->kColors)[1]);
        GXSetTevKColor(GX_KCOLOR2, ((GXColor*)config->kColors)[2]);
        GXSetTevKColor(GX_KCOLOR3, ((GXColor*)config->kColors)[3]);
    }

    if (config->flags & GCNMT_ENVMATRIX)
    {
        RwMatrix tmp;
        const RwMatrix* env;
        F32 mtx[2][4];

        if (matrix)
        {
            env = RwMatrixMultiply(&tmp, matrix, &lbl_8025E304);
        }
        else
        {
            env = &lbl_8025E304;
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

    for (i = 0; i < config->numTexMatrices; i++)
    {
        GCNMTTexMatrix* texMatrix = &config->texMatrices[i];
        RwMatrix frameMatrix;
        const RwMatrix* frame = GetTexFrameMatrix(texMatrix->frame, matrix, &frameMatrix);

        if (frame)
        {
            F32 m[3][4];
            F32 out[3][4];

            m[0][0] = frame->right.x;
            m[0][1] = frame->up.x;
            m[0][2] = frame->at.x;
            m[1][0] = frame->right.y;
            m[1][1] = frame->up.y;
            m[1][2] = frame->at.y;
            m[2][0] = frame->right.z;
            m[2][1] = frame->up.z;
            m[2][2] = frame->at.z;

            if (texMatrix->flags & GCNMTTEXMTX_TRANSLATE)
            {
                m[0][3] = frame->pos.x;
                m[1][3] = frame->pos.y;
                m[2][3] = frame->pos.z;
            }
            else
            {
                m[0][3] = 0.0f;
                m[1][3] = 0.0f;
                m[2][3] = 0.0f;
            }

            PSMTXConcat(texMatrix->matrix, m, out);
            GXLoadTexMtxImm(out, texMatrix->id, texMatrix->flags & GCNMTTEXMTX_TYPE);
        }
        else
        {
            GXLoadTexMtxImm(texMatrix->matrix, texMatrix->id, texMatrix->flags & GCNMTTEXMTX_TYPE);
        }
    }

    for (i = 0; i < config->numIndMatrices; i++)
    {
        GCNMTIndMatrix* indMatrix = &config->indMatrices[i];
        RwMatrix frameMatrix;
        const RwMatrix* frame = GetTexFrameMatrix(indMatrix->frame, matrix, &frameMatrix);
        F32 offset[2][3];
        F32(*src)[3];

        if (frame)
        {
            offset[0][0] = indMatrix->offset[0][0] * frame->right.x +
                           indMatrix->offset[0][1] * frame->right.y +
                           indMatrix->offset[0][2] * frame->right.z;
            offset[0][1] = indMatrix->offset[0][0] * frame->up.x +
                           indMatrix->offset[0][1] * frame->up.y +
                           indMatrix->offset[0][2] * frame->up.z;
            offset[0][2] = indMatrix->offset[0][0] * frame->at.x +
                           indMatrix->offset[0][1] * frame->at.y +
                           indMatrix->offset[0][2] * frame->at.z;
            offset[1][0] = indMatrix->offset[1][0] * frame->right.x +
                           indMatrix->offset[1][1] * frame->right.y +
                           indMatrix->offset[1][2] * frame->right.z;
            offset[1][1] = indMatrix->offset[1][0] * frame->up.x +
                           indMatrix->offset[1][1] * frame->up.y +
                           indMatrix->offset[1][2] * frame->up.z;
            offset[1][2] = indMatrix->offset[1][0] * frame->at.x +
                           indMatrix->offset[1][1] * frame->at.y +
                           indMatrix->offset[1][2] * frame->at.z;
            src = offset;
        }
        else
        {
            src = indMatrix->offset;
        }

        GXSetIndTexMtx(indMatrix->id, src, indMatrix->scaleExp);
    }

    if (config->flags & GCNMT_LIGHTING)
    {
        if (numChannels > 1)
        {
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR1A1);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            firstStage = 2;
            lighting = 3;
        }
        else
        {
            firstStage = 0;
            lighting = 2;
        }
    }
    else
    {
        firstStage = 0;
        lighting = 1;
    }

    for (i = 0; i < config->numTexCoordGens; i++)
    {
        texMapOfCoord[i] = -1;
    }

    if (config->numIndStages)
    {
        GXSetNumIndStages(config->numIndStages);

        for (i = 0; i < config->numIndStages; i++)
        {
            GCNMTIndStage* indStage = &config->indStages[i];

            if (!(indStage->flags & 1))
            {
                GXSetIndTexOrder(i, indStage->texCoord, indStage->texMap);
                GXSetIndTexCoordScale(i, indStage->scaleS, indStage->scaleT);
                texMapOfCoord[indStage->texCoord] = indStage->texMap;
            }
        }
    }

    if (config->numTevStages)
    {
        GXSetNumTevStages(config->numTevStages + firstStage);

        for (i = 0; i < config->numTevStages; i++)
        {
            GCNMTTevStage* tevStage = &config->tevStages[i];

            if (!(tevStage->flags & GCNMTTEV_DISABLED))
            {
                S32 stage = firstStage + i;
                S32 channel;

                if (tevStage->indirect)
                {
                    GCNMTTevInd tevInd;

                    RpGameCubeTevIndUnpack(&tevInd, &tevStage->indirect);
                    GXSetTevIndirect(stage, tevInd.indStage, tevInd.format, tevInd.bias,
                                     tevInd.matrix, tevInd.wrapS, tevInd.wrapT, tevInd.addPrev,
                                     tevInd.utcLod, tevInd.alpha);
                }
                else
                {
                    GXSetTevDirect(stage);
                }

                if (tevStage->channel <= GX_COLOR1A1 && lighting == 3)
                {
                    GXSetTevColorIn(stage, lbl_801DD400[tevStage->colorIn[0]],
                                    lbl_801DD400[tevStage->colorIn[1]],
                                    lbl_801DD400[tevStage->colorIn[2]],
                                    lbl_801DD400[tevStage->colorIn[3]]);
                    GXSetTevAlphaIn(stage, lbl_801DD440[tevStage->alphaIn[0]],
                                    lbl_801DD440[tevStage->alphaIn[1]],
                                    lbl_801DD440[tevStage->alphaIn[2]],
                                    lbl_801DD440[tevStage->alphaIn[3]]);
                    channel = GX_COLOR_NULL;
                }
                else
                {
                    GXSetTevColorIn(stage, tevStage->colorIn[0], tevStage->colorIn[1],
                                    tevStage->colorIn[2], tevStage->colorIn[3]);
                    GXSetTevAlphaIn(stage, tevStage->alphaIn[0], tevStage->alphaIn[1],
                                    tevStage->alphaIn[2], tevStage->alphaIn[3]);
                    channel = tevStage->channel;

                    if (channel <= GX_COLOR1A1 && lighting == 2)
                    {
                        channel = GX_COLOR0A0;
                    }
                }

                GXSetTevOrder(stage, tevStage->texCoord, tevStage->texMap, channel);
                GXSetTevKColorSel(stage, tevStage->kColorSel);
                GXSetTevKAlphaSel(stage, tevStage->kAlphaSel);
                GXSetTevColorOp(stage, tevStage->colorOp, tevStage->colorBias,
                                tevStage->colorScale, tevStage->colorClamp, tevStage->colorOut);
                GXSetTevAlphaOp(stage, tevStage->alphaOp, tevStage->alphaBias,
                                tevStage->alphaScale, tevStage->alphaClamp, tevStage->alphaOut);
                texMapOfCoord[tevStage->texCoord] = tevStage->texMap;
            }
        }
    }

    GXSetNumTexGens(config->numTexCoordGens);

    for (i = 0; i < config->numTexCoordGens; i++)
    {
        GCNMTTexCoordGen* gen = &config->texCoordGens[i];

        if (!(gen->flags & GCNMTTEXGEN_DISABLED))
        {
            S32 src = gen->src;

            if (coords && src >= GX_TG_TEX0 && src <= GX_TG_TEX7 && texMapOfCoord[i] >= 0)
            {
                src = coords[texMapOfCoord[i]] + GX_TG_TEX0;
            }

            GXSetTexCoordGen2(i, gen->func, src, gen->mtx, (gen->flags >> 1) & 1, gen->postMtx);

            if (gen->flags & GCNMTTEXGEN_MANUALSCALE)
            {
                GXSetTexCoordScaleManually(i, GX_TRUE, gen->scaleS, gen->scaleT);
            }
        }
    }

    if (config->flags & GCNMT_ALPHACOMPARE)
    {
        RwGameCubeSetAlphaCompare(3);
    }
    else
    {
        RwGameCubeSetAlphaCompare(1);
    }

    if (config->flags & GCNMT_BLEND)
    {
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)config->srcBlend);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)config->destBlend);
    }
}

// Draws one mesh with its material's multitexture: sets the textures, runs the pre-render
// callback, sends the effect, draws the display list and undoes what the effect set.
void _rpGameCubeMTMeshRenderCallBack(GCNDisplayList* displayList, RpMaterial* material,
                                     void* object, GCNObjectData* objectData,
                                     const RwMatrix* matrix)
{
    RpMultiTexture* multiTexture = RpMaterialGetMultiTexture(material, MTGCN_PLATFORM);
    GCNMTPlatformData* platformData = (GCNMTPlatformData*)multiTexture->platformData;
    S32 srcBlend;
    S32 destBlend;
    U32 i;

    RwRenderStateGet(rwRENDERSTATESRCBLEND, &srcBlend);
    RwRenderStateGet(rwRENDERSTATEDESTBLEND, &destBlend);

    for (i = 0; i < multiTexture->numTextures; i++)
    {
        _rwDlTextureSet(multiTexture->textures[i], i);
    }

    if (platformData->preRender)
    {
        platformData->preRender(material, object, objectData, platformData->data);
    }

    if (multiTexture->effect)
    {
        GCNMTConfig* config;
        S32 twoChannels = FALSE;
        S32 j;

        if (objectData->globalMask && objectData->localMask)
        {
            twoChannels = TRUE;
        }

        _rpGameCubeMTEffectSend(multiTexture->effect, (twoChannels != 0) + 1,
                                multiTexture->numTextures, multiTexture->coords, matrix);
        GXCallDisplayList(displayList->list, displayList->size);

        config = RpGameCubeMTEffectGetConfig(multiTexture->effect);

        for (j = 0; j < config->numTexCoordGens; j++)
        {
            if (config->texCoordGens[j].flags & GCNMTTEXGEN_MANUALSCALE)
            {
                GXSetTexCoordScaleManually(j, GX_FALSE, 0, 0);
            }
        }

        if (config->numIndStages)
        {
            GXSetNumIndStages(0);

            for (j = 0; j < config->numIndStages; j++)
            {
                GXSetIndTexCoordScale(j, GX_ITS_1, GX_ITS_1);
            }

            for (j = 0; j < config->numTevStages; j++)
            {
                if (config->tevStages[j].indirect)
                {
                    GXSetTevDirect(j);
                }
            }
        }
    }
    else
    {
        GXCallDisplayList(displayList->list, displayList->size);
    }

    if (platformData->postRender)
    {
        platformData->postRender(material, object, objectData, platformData->data);
    }

    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)srcBlend);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)destBlend);
}

S32 _rpGameCubeMTPipePluginAttach(void)
{
    lbl_8025F698 = RwEngineRegisterPlugin(sizeof(GCNMTGlobals), MTGCN_PIPEPLUGINID,
                                          GameCubeMTOpen, fn_80141954);

    return lbl_8025F698 >= 0;
}
