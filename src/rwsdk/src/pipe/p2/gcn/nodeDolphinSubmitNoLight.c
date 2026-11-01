#include <rwcore.h>
#include <dolphin/gx.h>

// The GameCube immediate-mode 3D nodes: the instance node only keeps the vertex pool for the
// submit node, which sends the pool's vertices (position, color and, when textured, texture
// coordinates) straight to the GX FIFO, unlit, as the pool's primitive.

// The vertex pool the instance node passed on.
extern rwIm3DPool* lbl_8025F8B8;

// The GX primitive for each RenderWare primitive type.
extern const GXPrimitive lbl_801DF0E8[7];

// The node definitions.
extern RxNodeDefinition lbl_801DF114;
extern RxNodeDefinition lbl_801DF168;

void _rwDlTransformSetup(const RwMatrix* matrix, S32 normals);
void fn_8017ABB4(void);

// The pool flag for textured vertices. Written for this project.
#define DLSUBMIT_VERTEXUV 0x01

// One vertex into the FIFO: position and color, and texture coordinates when textured.
#define DLSUBMIT_POSCOLOR(_vert)                                                                   \
    do                                                                                             \
    {                                                                                              \
        const RwIm3DVertex* _v = (_vert);                                                          \
                                                                                                   \
        GXWGFifo.f32 = _v->x;                                                                      \
        GXWGFifo.f32 = _v->y;                                                                      \
        GXWGFifo.f32 = _v->z;                                                                      \
        GXWGFifo.u8 = _v->r;                                                                       \
        GXWGFifo.u8 = _v->g;                                                                       \
        GXWGFifo.u8 = _v->b;                                                                       \
        GXWGFifo.u8 = _v->a;                                                                       \
    } while (0)

#define DLSUBMIT_POSCOLORUV(_vert)                                                                 \
    do                                                                                             \
    {                                                                                              \
        const RwIm3DVertex* _v = (_vert);                                                          \
                                                                                                   \
        GXWGFifo.f32 = _v->x;                                                                      \
        GXWGFifo.f32 = _v->y;                                                                      \
        GXWGFifo.f32 = _v->z;                                                                      \
        GXWGFifo.u8 = _v->r;                                                                       \
        GXWGFifo.u8 = _v->g;                                                                       \
        GXWGFifo.u8 = _v->b;                                                                       \
        GXWGFifo.u8 = _v->a;                                                                       \
        GXWGFifo.f32 = _v->u;                                                                      \
        GXWGFifo.f32 = _v->v;                                                                      \
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
        fn_8017ABB4();
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
        case rwPRIMTYPELINELIST:
        {
            U16 n = stash->numIndices / 2;

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
        case rwPRIMTYPETRILIST:
        {
            S32 n = stash->numIndices / 3;

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
        case rwPRIMTYPELINELIST:
        {
            U16 n = lbl_8025F8B8->numElements / 2;

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
        case rwPRIMTYPETRILIST:
        {
            S32 n = lbl_8025F8B8->numElements / 3;

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
