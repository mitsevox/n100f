#include <rpworld.h>
#include <rwdlgcn.h>
#include <dolphin/gx.h>

// GameCube pipeline support: waiting for the GPU to finish with an instanced object before its
// memory is reused, and the callbacks the all-in-one nodes run to instance and draw objects.

// The draw sync token to issue next.
extern U16 lbl_8025E800;

S32 _rwDlTokenQueryDone(U16 token);
void _rwDlVtxFmtSetup(RpGameCubeVtxFmt* vtxFmt, GCNObjectData* objectData);
void _rwDlTransformSetup(const RwMatrix* matrix, S32 normals);
void _rwDlTextureSet(RwTexture* texture, U32 stage);

// Draw sync tokens wrap at this value; an atomic's object type. Written for this project.
#define GCPIPE_TOKENWRAP 0xE000
#define GCPIPE_ATOMIC 1

// Called when an instanced object's resource entry is freed: if the GPU may still be drawing it,
// issue a draw sync token for it now, then wait until the GPU has passed its token.
void _rxGCResEntryWaitDone(RwResEntry* resEntry)
{
    GCNVertexArrays* arrays = (GCNVertexArrays*)(resEntry + 1);

    if (arrays->syncToken == lbl_8025E800)
    {
        GXSetDrawSync(lbl_8025E800);
        lbl_8025E800 = (lbl_8025E800 + 1) % GCPIPE_TOKENWRAP;
    }

    while (!_rwDlTokenQueryDone(arrays->syncToken))
    {
    }

    GXInvalidateVtxCache();
}

// The lighting channel colors: opaque white (the material color when it does not modulate),
// opaque black (no ambient light) and transparent black (the local lights' channel ambient).
static const GXColor gcpipeWhite = { 0xFF, 0xFF, 0xFF, 0xFF };
static const GXColor gcpipeBlack = { 0x00, 0x00, 0x00, 0xFF };
static const GXColor gcpipeClear = { 0x00, 0x00, 0x00, 0x00 };

// Sets up the lighting channels for an object. With hardware lights, channel 0 takes the global
// lights (and the local ones too when there are no global ones) and, when there are both,
// channel 1 takes the local lights with spot attenuation; the ambient comes from the prelight
// colors when the object is prelit. Without lights the channels pass the prelight or material
// color straight through.
void fn_801588E8(GCNObjectData* objectData)
{
    U32 flags = objectData->flags;

    if (objectData->numLights > 0)
    {
        GXColorSrc ambSrc;

        if (flags & rpGEOMETRYPRELIT)
        {
            ambSrc = GX_SRC_VTX;
        }
        else
        {
            ambSrc = GX_SRC_REG;

            if (!objectData->ambientUsed)
            {
                GXSetChanAmbColor(GX_COLOR0A0, gcpipeBlack);
            }
        }

        if (objectData->globalMask && objectData->localMask)
        {
            GXSetNumChans(2);
            GXSetChanCtrl(GX_COLOR0A0, GX_ENABLE, ambSrc, GX_SRC_REG, objectData->globalMask,
                          GX_DF_CLAMP, GX_AF_NONE);
            GXSetChanCtrl(GX_COLOR1A1, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, objectData->localMask,
                          GX_DF_CLAMP, GX_AF_SPOT);

            if (!(flags & rpGEOMETRYMODULATEMATERIALCOLOR))
            {
                GXSetChanMatColor(GX_COLOR0A0, gcpipeWhite);
                GXSetChanMatColor(GX_COLOR1A1, gcpipeWhite);
            }

            GXSetChanAmbColor(GX_COLOR1A1, gcpipeClear);
            return;
        }

        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_ENABLE, ambSrc, GX_SRC_REG,
                      objectData->globalMask | objectData->localMask, GX_DF_CLAMP,
                      objectData->localMask ? GX_AF_SPOT : GX_AF_NONE);

        if (!(flags & rpGEOMETRYMODULATEMATERIALCOLOR))
        {
            GXSetChanMatColor(GX_COLOR0A0, gcpipeWhite);
        }
    }
    else
    {
        if (flags & rpGEOMETRYPRELIT)
        {
            if (flags & rpGEOMETRYMODULATEMATERIALCOLOR)
            {
                GXSetNumChans(1);
                GXSetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_VTX, GX_SRC_REG, GX_LIGHT_NULL,
                              GX_DF_NONE, GX_AF_NONE);
                return;
            }

            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL,
                          GX_DF_NONE, GX_AF_NONE);
            return;
        }

        if (!objectData->ambientUsed && !(flags & rpGEOMETRYMODULATEMATERIALCOLOR))
        {
            GXSetChanMatColor(GX_COLOR0A0, gcpipeBlack);
        }

        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                      GX_AF_NONE);
    }
}

// Sets a material's colors in the lighting channels: the ambient register (the summed ambient
// scaled by the material's ambient) and the material register (its color, when it modulates).
// Without lights the channel passes the material register through, so the ambient is folded
// into it.
void fn_80158B20(GCNObjectData* objectData, RpMaterial* material)
{
    GXColor color;

    if (objectData->numLights > 0)
    {
        if (objectData->ambientUsed && !(objectData->flags & rpGEOMETRYPRELIT))
        {
            F32 scale = 255.0f * material->surfaceProps.ambient;

            color.r = (U8)(scale * objectData->ambient.red);
            color.g = (U8)(scale * objectData->ambient.green);
            color.b = (U8)(scale * objectData->ambient.blue);
            color.a = 0xFF;
            GXSetChanAmbColor(GX_COLOR0A0, color);
        }

        if (objectData->flags & rpGEOMETRYMODULATEMATERIALCOLOR)
        {
            GXSetChanMatColor(GX_COLOR0A0, *(GXColor*)&material->color);

            if (objectData->globalMask && objectData->localMask)
            {
                GXSetChanMatColor(GX_COLOR1A1, *(GXColor*)&material->color);
            }
        }
    }
    else
    {
        U32 flags = objectData->flags;

        if (flags & rpGEOMETRYPRELIT)
        {
            if (flags & rpGEOMETRYMODULATEMATERIALCOLOR)
            {
                GXSetChanMatColor(GX_COLOR0A0, *(GXColor*)&material->color);
            }
        }
        else if (objectData->ambientUsed)
        {
            F32 ambient = material->surfaceProps.ambient;

            if (flags & rpGEOMETRYMODULATEMATERIALCOLOR)
            {
                color.r = (U8)(material->color.red * (ambient * objectData->ambient.red));
                color.g = (U8)(material->color.green * (ambient * objectData->ambient.green));
                color.b = (U8)(material->color.blue * (ambient * objectData->ambient.blue));
                color.a = material->color.alpha;
            }
            else
            {
                F32 scale = ambient * 255.0f;

                color.r = (U8)(scale * objectData->ambient.red);
                color.g = (U8)(scale * objectData->ambient.green);
                color.b = (U8)(scale * objectData->ambient.blue);
                color.a = 0xFF;
            }

            GXSetChanMatColor(GX_COLOR0A0, color);
        }
        else if (flags & rpGEOMETRYMODULATEMATERIALCOLOR)
        {
            color.r = color.g = color.b = 0;
            color.a = material->color.alpha;
            GXSetChanMatColor(GX_COLOR0A0, color);
        }
    }
}

// Sets up the texture environment: one stage per lighting channel in use, the first modulating
// the texture (or passing the lit color when untextured) and the second adding channel 1.
void fn_80158DC4(GCNObjectData* objectData)
{
    U32 numStages = (objectData->globalMask && objectData->localMask) ? 2 : 1;

    GXSetNumTevStages(numStages);

    if (objectData->flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2))
    {
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE,
                          GX_PTIDENTITY);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);

        if (numStages > 1)
        {
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR1A1);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_CPREV);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
        }
    }
    else
    {
        GXSetNumTexGens(0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

        if (numStages > 1)
        {
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR1A1);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_RASC, GX_CC_ONE, GX_CC_CPREV);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
        }
    }
}

// Whether a texture's raster has alpha, from the raster plugin's flags.
#define GCPIPE_TEXTUREHASALPHA(_texture)                                                           \
    ((_texture) && (_texture)->raster && (RWGCRASTEREXT((_texture)->raster->parent)->flags & 1))

// The default render callback: marks the instanced entry with this frame's draw sync token, sets
// up the vertex format, transform, lighting channels and texture environment, then calls each
// mesh's display list, setting its texture and alpha compare when textured and its colors when
// they reach the channels.
void* fn_80158FB0(void* object, GCNObjectData* objectData)
{
    RwResEntry* repEntry = objectData->repEntry;
    GCNDisplayList* displayList = (GCNDisplayList*)((U8*)(repEntry + 1) + sizeof(GCNVertexArrays));
    U32 numMeshes;
    S32 matColors;
    RpMesh* mesh;
    RpMeshHeader* meshHeader;
    const RwMatrix* matrix;

    ((GCNVertexArrays*)(repEntry + 1))->syncToken = lbl_8025E800;

    if (((RwObject*)object)->type == GCPIPE_ATOMIC)
    {
        matrix = RwFrameGetLTM(RpAtomicGetFrame((RpAtomic*)object));
        _rwDlVtxFmtSetup(fn_8015D818(((RpAtomic*)object)->geometry), objectData);
    }
    else
    {
        matrix = NULL;
        _rwDlVtxFmtSetup(fn_8015D824((RpWorld*)RWSRCGLOBAL(curWorld)), objectData);
    }

    _rwDlTransformSetup(matrix, (objectData->flags >> 4) & 1);
    fn_801588E8(objectData);
    fn_80158DC4(objectData);

    matColors = (objectData->flags & rpGEOMETRYMODULATEMATERIALCOLOR) ||
                (objectData->ambientUsed && !(objectData->flags & rpGEOMETRYPRELIT));

    meshHeader = objectData->meshHeader;
    numMeshes = meshHeader->numMeshes;
    mesh = (RpMesh*)(meshHeader + 1);

    if (objectData->flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2))
    {
        while (numMeshes--)
        {
            RwTexture* texture = mesh->material->texture;
            S32 alphaCompare;

            _rwDlTextureSet(texture, GX_TEXMAP0);
            alphaCompare = 1;

            if (texture && texture->raster && (RWGCRASTEREXT(texture->raster->parent)->flags & 1))
            {
                alphaCompare = 3;
            }

            RwGameCubeSetAlphaCompare(alphaCompare);

            if (matColors)
            {
                fn_80158B20(objectData, mesh->material);
            }

            GXCallDisplayList(displayList->list, displayList->size);
            mesh++;
            displayList++;
        }
    }
    else if (matColors)
    {
        while (numMeshes--)
        {
            fn_80158B20(objectData, mesh->material);
            GXCallDisplayList(displayList->list, displayList->size);
            mesh++;
            displayList++;
        }
    }
    else
    {
        while (numMeshes--)
        {
            GXCallDisplayList(displayList->list, displayList->size);
            displayList++;
        }
    }

    return object;
}

void _rxGameCubeAllInOneSetInstanceCallBack(RxPipelineNode* node, void* callback)
{
    ((GCNAllInOnePrivateData*)node->privateData)->instanceCB = callback;
}

void* _rxGameCubeAllInOneGetInstanceCallBack(RxPipelineNode* node)
{
    return ((GCNAllInOnePrivateData*)node->privateData)->instanceCB;
}

void _rxGameCubeAllInOneSetReinstanceCallBack(RxPipelineNode* node, void* callback)
{
    ((GCNAllInOnePrivateData*)node->privateData)->reinstanceCB = callback;
}

void* _rxGameCubeAllInOneGetReinstanceCallBack(RxPipelineNode* node)
{
    return ((GCNAllInOnePrivateData*)node->privateData)->reinstanceCB;
}

void RxGameCubeAllInOneSetRenderCallBack(RxPipelineNode* node, void* callback)
{
    ((GCNAllInOnePrivateData*)node->privateData)->renderCB = callback;
}
