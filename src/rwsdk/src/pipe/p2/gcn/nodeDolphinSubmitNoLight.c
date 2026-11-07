#include <rwcore.h>
#include <dolphin/gx.h>

// The GameCube immediate-mode 3D nodes: the instance node only keeps the vertex pool for the
// submit node, which sends the pool's vertices (position, color and, when textured, texture
// coordinates) straight to the GX FIFO, unlit, as the pool's primitive.

S32 _rwDlImmInstanceNode(RxPipelineNode* self, RxPipelineNodeParam* params);
S32 DlSubmitNode(RxPipelineNode* self, RxPipelineNodeParam* params);
void _rwDlTransformSetup(const RwMatrix* matrix, S32 normals);
void _rwDlTextureRasterFlush(void);

// The GX primitive for each RenderWare primitive type (none, line list, polyline, triangle
// list, triangle strip, triangle fan, point list).
GXPrimitive lbl_801DF0E8[7] = {
    0, GX_LINES, GX_LINESTRIP, GX_TRIANGLES, GX_TRIANGLESTRIP, GX_TRIANGLEFAN, GX_POINTS,
};

// The node definitions: each names its node and points at its node body; neither has clusters,
// outputs or private data.
char lbl_801DF104[] = "ImmInstance.csl";

RxNodeDefinition lbl_801DF114 = {
    lbl_801DF104,
    { _rwDlImmInstanceNode, NULL, NULL, NULL, NULL, NULL, NULL },
    { 0, NULL, NULL, 0, NULL },
    0,
    rxNODEDEFCONST,
    0,
};

char lbl_801DF154[] = "SubmitNoLight.csl";

RxNodeDefinition lbl_801DF168 = {
    lbl_801DF154,
    { DlSubmitNode, NULL, NULL, NULL, NULL, NULL, NULL },
    { 0, NULL, NULL, 0, NULL },
    0,
    rxNODEDEFCONST,
    0,
};

// The vertex pool the instance node passed on.
rwIm3DPool* lbl_8025F8B8;

// The pool flag for textured vertices. Written for this project.
#define DLSUBMIT_VERTEXUV 0x01

// One vertex into the FIFO through the GX vertex functions: position and color, and texture
// coordinates when textured.
#define DLSUBMIT_POSCOLOR(_vert)                                                                   \
    do                                                                                             \
    {                                                                                              \
        RwIm3DVertex* _v = (_vert);                                                                \
                                                                                                   \
        GXPosition3f32(_v->x, _v->y, _v->z);                                                       \
        GXColor4u8(_v->r, _v->g, _v->b, _v->a);                                                    \
    } while (0)

#define DLSUBMIT_POSCOLORUV(_vert)                                                                 \
    do                                                                                             \
    {                                                                                              \
        RwIm3DVertex* _v = (_vert);                                                                \
                                                                                                   \
        GXPosition3f32(_v->x, _v->y, _v->z);                                                       \
        GXColor4u8(_v->r, _v->g, _v->b, _v->a);                                                    \
        GXTexCoord2f32(_v->u, _v->v);                                                              \
    } while (0)

S32 _rwDlImmInstanceNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    lbl_8025F8B8 = (rwIm3DPool*)params->dataParam;

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetGameCubeImmInstance(void)
{
    return &lbl_801DF114;
}

// Sets up GX for unlit position and color vertices, with one texture stage when textured.
static void DlSubmitSetup(_rwIm3DPoolStash* stash)
{
    _rwDlTransformSetup(stash->ltm, FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumTevStages(1);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);

    if (stash->flags & DLSUBMIT_VERTEXUV)
    {
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE,
                          GX_PTIDENTITY);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        _rwDlTextureRasterFlush();
    }
    else
    {
        GXSetNumTexGens(0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    }
}

// Sends the pool's vertices, through its indices when it has them, as its primitive.
S32 DlSubmitNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    rwIm3DPool* pool = lbl_8025F8B8;
    _rwIm3DPoolStash* stash = &pool->stash;
    RwIm3DVertex* verts = pool->elements;

    if (stash->indices)
    {
        RwImVertexIndex* index = stash->indices;

        DlSubmitSetup(stash);
        GXBegin(lbl_801DF0E8[stash->primType], GX_VTXFMT0, stash->numIndices);

        switch (stash->primType)
        {
        case rwPRIMTYPEPOLYLINE:
        case rwPRIMTYPETRISTRIP:
        case rwPRIMTYPETRIFAN:
        {
            U16 n = stash->numIndices;

            if (stash->flags & DLSUBMIT_VERTEXUV)
            {
                while (n--)
                {
                    DLSUBMIT_POSCOLORUV(&verts[*index]);
                    index++;
                }
            }
            else
            {
                while (n--)
                {
                    DLSUBMIT_POSCOLOR(&verts[*index]);
                    index++;
                }
            }

            break;
        }
        case rwPRIMTYPETRILIST:
        {
            U16 n = stash->numIndices / 3;

            if (stash->flags & DLSUBMIT_VERTEXUV)
            {
                while (n--)
                {
                    DLSUBMIT_POSCOLORUV(&verts[index[0]]);
                    DLSUBMIT_POSCOLORUV(&verts[index[1]]);
                    DLSUBMIT_POSCOLORUV(&verts[index[2]]);
                    index += 3;
                }
            }
            else
            {
                while (n--)
                {
                    DLSUBMIT_POSCOLOR(&verts[index[0]]);
                    DLSUBMIT_POSCOLOR(&verts[index[1]]);
                    DLSUBMIT_POSCOLOR(&verts[index[2]]);
                    index += 3;
                }
            }

            break;
        }
        case rwPRIMTYPELINELIST:
        {
            U16 n = stash->numIndices >> 1;

            if (stash->flags & DLSUBMIT_VERTEXUV)
            {
                while (n--)
                {
                    DLSUBMIT_POSCOLORUV(&verts[index[0]]);
                    DLSUBMIT_POSCOLORUV(&verts[index[1]]);
                    index += 2;
                }
            }
            else
            {
                while (n--)
                {
                    DLSUBMIT_POSCOLOR(&verts[index[0]]);
                    DLSUBMIT_POSCOLOR(&verts[index[1]]);
                    index += 2;
                }
            }

            break;
        }
        default:
        {
            RwError err;
            err.pluginID = 1;
            err.errorCode = _rwerror(0x25);
            RwErrorSet(&err);
            break;
        }
        }
    }
    else
    {
        DlSubmitSetup(stash);
        GXBegin(lbl_801DF0E8[stash->primType], GX_VTXFMT0, lbl_8025F8B8->numElements);

        switch (stash->primType)
        {
        case rwPRIMTYPEPOLYLINE:
        case rwPRIMTYPETRISTRIP:
        case rwPRIMTYPETRIFAN:
        {
            U16 n = lbl_8025F8B8->numElements;

            if (stash->flags & DLSUBMIT_VERTEXUV)
            {
                while (n--)
                {
                    DLSUBMIT_POSCOLORUV(verts);
                    verts++;
                }
            }
            else
            {
                while (n--)
                {
                    DLSUBMIT_POSCOLOR(verts);
                    verts++;
                }
            }

            break;
        }
        case rwPRIMTYPETRILIST:
        {
            U16 n = lbl_8025F8B8->numElements / 3;

            if (stash->flags & DLSUBMIT_VERTEXUV)
            {
                while (n--)
                {
                    DLSUBMIT_POSCOLORUV(&verts[0]);
                    DLSUBMIT_POSCOLORUV(&verts[1]);
                    DLSUBMIT_POSCOLORUV(&verts[2]);
                    verts += 3;
                }
            }
            else
            {
                while (n--)
                {
                    DLSUBMIT_POSCOLOR(&verts[0]);
                    DLSUBMIT_POSCOLOR(&verts[1]);
                    DLSUBMIT_POSCOLOR(&verts[2]);
                    verts += 3;
                }
            }

            break;
        }
        case rwPRIMTYPELINELIST:
        {
            U16 n = lbl_8025F8B8->numElements >> 1;

            if (stash->flags & DLSUBMIT_VERTEXUV)
            {
                while (n--)
                {
                    DLSUBMIT_POSCOLORUV(&verts[0]);
                    DLSUBMIT_POSCOLORUV(&verts[1]);
                    verts += 2;
                }
            }
            else
            {
                while (n--)
                {
                    DLSUBMIT_POSCOLOR(&verts[0]);
                    DLSUBMIT_POSCOLOR(&verts[1]);
                    verts += 2;
                }
            }

            break;
        }
        default:
        {
            RwError err;
            err.pluginID = 1;
            err.errorCode = _rwerror(0x25);
            RwErrorSet(&err);
            break;
        }
        }
    }

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetGameCubeSubmitNoLight(void)
{
    return &lbl_801DF168;
}
