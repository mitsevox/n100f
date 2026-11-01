#include <rwdlgcn.h>
#include <dolphin/vi.h>
#include <dolphin/mtx.h>

// Immediate-mode 2D drawing on the GameCube: each call sets GX up for screen-space vertices
// (position and color, plus texture coordinates when a texture is set), an orthographic
// projection over the whole frame and the matching viewport and scissor, writes the vertices
// straight into the GX FIFO, then puts the camera's projection, viewport and scissor back.

// The GX primitive for each RenderWare primitive type.
extern GXPrimitive lbl_801DDCC0[7];

// The position matrix 2D vertices use: identity with the Z axis flipped.
extern Mtx lbl_801DDCDC;

// The camera's projection, saved while 2D drawing replaces it.
extern F32 lbl_8025E290[7];

// Whether the frame is drawn in two halves, which half is being drawn, and the line where the
// halves meet (dldevice.c).
extern S32 lbl_8025F808;
extern S32 lbl_8025E7F0;
extern S32 lbl_8025F85C;

// The texture set for drawing; it has no raster when drawing is untextured (dltextur.c).
extern RwTexture* lbl_8025F87C;

void GXSetProjectionv(const f32* ptr);

// Writes one vertex: position, color, and texture coordinates when textured.
static inline void DlIm2DVertexTextured(const RwIm2DVertex* vert)
{
    GXPosition3f32(vert->x, vert->y, vert->z);
    GXColor4u8(vert->emissiveColor.red, vert->emissiveColor.green, vert->emissiveColor.blue,
               vert->emissiveColor.alpha);
    GXTexCoord2f32(vert->u, vert->v);
}

static inline void DlIm2DVertexUntextured(const RwIm2DVertex* vert)
{
    GXPosition3f32(vert->x, vert->y, vert->z);
    GXColor4u8(vert->emissiveColor.red, vert->emissiveColor.green, vert->emissiveColor.blue,
               vert->emissiveColor.alpha);
}

// Sets GX up for 2D vertices over the whole frame.
void _rw2DRenderPrimitiveInit(void)
{
    GXRenderModeObj* mode;
    Mtx44 projection;

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumTevStages(1);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);

    if (lbl_8025F87C->raster)
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

    mode = _RwDlRenderMode;

    if (mode->field_rendering)
    {
        GXSetViewportJitter(0.0f, 0.0f, mode->fbWidth, mode->xfbHeight, 0.0f, 1.0f,
                            VIGetNextField() ^ 1);
    }
    else
    {
        GXSetViewport(0.0f, 0.0f, mode->fbWidth, mode->xfbHeight, 0.0f, 1.0f);
    }

    if (lbl_8025F808 == 0)
    {
        GXSetScissor(0, 0, _RwDlRenderMode->fbWidth, _RwDlRenderMode->xfbHeight);
    }
    else if (lbl_8025E7F0 != 0)
    {
        GXSetScissor(0, 0, _RwDlRenderMode->fbWidth, lbl_8025F85C + 2);
        GXSetScissorBoxOffset(0, 0);
    }
    else
    {
        GXSetScissor(0, lbl_8025F85C - 2, _RwDlRenderMode->fbWidth, lbl_8025F85C + 2);
        GXSetScissorBoxOffset(0, lbl_8025F85C - 2);
    }

    C_MTXOrtho(projection, 0.0f, _RwDlRenderMode->xfbHeight - 1, 0.0f,
               _RwDlRenderMode->fbWidth - 1, 0.0f, 1.0f);
    GXGetProjectionv(lbl_8025E290);
    GXSetProjection(projection, GX_ORTHOGRAPHIC);
    GXLoadPosMtxImm(lbl_801DDCDC, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
}

// Puts back the camera's projection, and its viewport and scissor when it draws to the frame
// buffer (a raster that is its own parent keeps the full-frame settings).
void _rw2DRenderPrimativeTerm(void)
{
    RwRaster* raster = RwCameraGetCurrentCamera()->frameBuffer;

    if (raster != raster->parent)
    {
        if (lbl_8025F808 == 0)
        {
            if (_RwDlRenderMode->field_rendering)
            {
                GXSetViewportJitter(raster->nOffsetX, raster->nOffsetY, raster->width,
                                    raster->height, 0.0f, 1.0f, VIGetNextField() ^ 1);
            }
            else
            {
                GXSetViewport(raster->nOffsetX, raster->nOffsetY, raster->width, raster->height,
                              0.0f, 1.0f);
            }

            GXSetScissor(raster->nOffsetX, raster->nOffsetY, raster->width, raster->height);
        }
        else
        {
            if (_RwDlRenderMode->field_rendering)
            {
                GXSetViewportJitter(raster->nOffsetX, raster->nOffsetY, raster->width,
                                    raster->height, 0.0f, 1.0f, VIGetNextField() ^ 1);
            }
            else
            {
                GXSetViewport(raster->nOffsetX, raster->nOffsetY * 2, raster->width,
                              raster->height * 2, 0.0f, 1.0f);
            }

            if (lbl_8025E7F0 != 0)
            {
                if ((raster->nOffsetY + raster->height) * 2 <= lbl_8025F85C + 2)
                {
                    GXSetScissor(raster->nOffsetX, raster->nOffsetY * 2, _RwDlRenderMode->fbWidth,
                                 raster->height * 2);
                }
                else if (raster->nOffsetY * 2 > lbl_8025F85C + 2)
                {
                    GXSetScissor(0, 0, _RwDlRenderMode->fbWidth, lbl_8025F85C + 2);
                }
                else
                {
                    GXSetScissor(raster->nOffsetX, raster->nOffsetY * 2, _RwDlRenderMode->fbWidth,
                                 lbl_8025F85C + 2);
                }

                GXSetScissorBoxOffset(0, 0);
            }
            else
            {
                if (raster->nOffsetY * 2 >= lbl_8025F85C - 2)
                {
                    GXSetScissor(raster->nOffsetX, raster->nOffsetY * 2, _RwDlRenderMode->fbWidth,
                                 raster->height * 2);
                }
                else if ((raster->nOffsetY + raster->height) * 2 < lbl_8025F85C + 2)
                {
                    GXSetScissor(0, lbl_8025F85C - 2, _RwDlRenderMode->fbWidth,
                                 lbl_8025F85C + 2);
                }
                else
                {
                    GXSetScissor(raster->nOffsetX, lbl_8025F85C - 2, _RwDlRenderMode->fbWidth,
                                 raster->height * 2);
                }

                GXSetScissorBoxOffset(0, lbl_8025F85C - 2);
            }
        }
    }

    GXSetProjectionv(lbl_8025E290);
}

S32 _rwDlIm2DRenderTriangle(RwIm2DVertex* vertices, S32 numVertices, S32 vert1, S32 vert2,
                            S32 vert3)
{
    RwIm2DVertex* v1 = &vertices[vert1];
    RwIm2DVertex* v2 = &vertices[vert2];
    RwIm2DVertex* v3 = &vertices[vert3];

    _rw2DRenderPrimitiveInit();
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);

    if (lbl_8025F87C->raster)
    {
        DlIm2DVertexTextured(v1);
        DlIm2DVertexTextured(v2);
        DlIm2DVertexTextured(v3);
    }
    else
    {
        DlIm2DVertexUntextured(v1);
        DlIm2DVertexUntextured(v2);
        DlIm2DVertexUntextured(v3);
    }

    _rw2DRenderPrimativeTerm();

    return TRUE;
}

S32 _rwDlIm2DRenderLine(RwIm2DVertex* vertices, S32 numVertices, S32 vert1, S32 vert2)
{
    RwIm2DVertex* v1 = &vertices[vert1];
    RwIm2DVertex* v2 = &vertices[vert2];

    _rw2DRenderPrimitiveInit();
    GXBegin(GX_LINES, GX_VTXFMT0, 2);

    if (lbl_8025F87C->raster)
    {
        DlIm2DVertexTextured(v1);
        DlIm2DVertexTextured(v2);
    }
    else
    {
        DlIm2DVertexUntextured(v1);
        DlIm2DVertexUntextured(v2);
    }

    _rw2DRenderPrimativeTerm();

    return TRUE;
}

#define DL2DREND_ERROR(_code)                                                                      \
    do                                                                                             \
    {                                                                                              \
        RwError err;                                                                               \
        err.pluginID = 1;                                                                          \
        err.errorCode = _rwerror(_code);                                                           \
        RwErrorSet(&err);                                                                          \
    } while (0)

S32 _rwDlIm2DRenderPrimitive(RwPrimitiveType primType, RwIm2DVertex* vertices, S32 numVertices)
{
    S32 count;

    _rw2DRenderPrimitiveInit();
    GXBegin(lbl_801DDCC0[primType], GX_VTXFMT0, numVertices);

    switch (primType)
    {
    case rwPRIMTYPEPOLYLINE:
    case rwPRIMTYPETRISTRIP:
    case rwPRIMTYPETRIFAN:
        count = numVertices;

        if (lbl_8025F87C->raster)
        {
            while (count--)
            {
                DlIm2DVertexTextured(vertices);
                vertices++;
            }
        }
        else
        {
            while (count--)
            {
                DlIm2DVertexUntextured(vertices);
                vertices++;
            }
        }
        break;

    case rwPRIMTYPELINELIST:
        count = numVertices >> 1;

        if (lbl_8025F87C->raster)
        {
            while (count--)
            {
                DlIm2DVertexTextured(&vertices[0]);
                DlIm2DVertexTextured(&vertices[1]);
                vertices += 2;
            }
        }
        else
        {
            while (count--)
            {
                DlIm2DVertexUntextured(&vertices[0]);
                DlIm2DVertexUntextured(&vertices[1]);
                vertices += 2;
            }
        }
        break;

    case rwPRIMTYPETRILIST:
        count = numVertices / 3;

        if (lbl_8025F87C->raster)
        {
            while (count--)
            {
                DlIm2DVertexTextured(&vertices[0]);
                DlIm2DVertexTextured(&vertices[1]);
                DlIm2DVertexTextured(&vertices[2]);
                vertices += 3;
            }
        }
        else
        {
            while (count--)
            {
                DlIm2DVertexUntextured(&vertices[0]);
                DlIm2DVertexUntextured(&vertices[1]);
                DlIm2DVertexUntextured(&vertices[2]);
                vertices += 3;
            }
        }
        break;

    default:
        DL2DREND_ERROR(0x25);
        break;
    }

    _rw2DRenderPrimativeTerm();

    return TRUE;
}

S32 _rwDlIm2DRenderIndexedPrimitive(RwPrimitiveType primType, RwIm2DVertex* vertices,
                                    S32 numVertices, RwImVertexIndex* indices, S32 numIndices)
{
    S32 count;

    _rw2DRenderPrimitiveInit();
    GXBegin(lbl_801DDCC0[primType], GX_VTXFMT0, numIndices);

    switch (primType)
    {
    case rwPRIMTYPEPOLYLINE:
    case rwPRIMTYPETRISTRIP:
    case rwPRIMTYPETRIFAN:
        count = numIndices;

        if (lbl_8025F87C->raster)
        {
            while (count--)
            {
                DlIm2DVertexTextured(&vertices[*indices]);
                indices++;
            }
        }
        else
        {
            while (count--)
            {
                DlIm2DVertexUntextured(&vertices[*indices]);
                indices++;
            }
        }
        break;

    case rwPRIMTYPELINELIST:
        count = numIndices >> 1;

        if (lbl_8025F87C->raster)
        {
            while (count--)
            {
                DlIm2DVertexTextured(&vertices[indices[0]]);
                DlIm2DVertexTextured(&vertices[indices[1]]);
                indices += 2;
            }
        }
        else
        {
            while (count--)
            {
                DlIm2DVertexUntextured(&vertices[indices[0]]);
                DlIm2DVertexUntextured(&vertices[indices[1]]);
                indices += 2;
            }
        }
        break;

    case rwPRIMTYPETRILIST:
        count = numIndices / 3;

        if (lbl_8025F87C->raster)
        {
            while (count--)
            {
                DlIm2DVertexTextured(&vertices[indices[0]]);
                DlIm2DVertexTextured(&vertices[indices[1]]);
                DlIm2DVertexTextured(&vertices[indices[2]]);
                indices += 3;
            }
        }
        else
        {
            while (count--)
            {
                DlIm2DVertexUntextured(&vertices[indices[0]]);
                DlIm2DVertexUntextured(&vertices[indices[1]]);
                DlIm2DVertexUntextured(&vertices[indices[2]]);
                indices += 3;
            }
        }
        break;

    default:
        DL2DREND_ERROR(0x25);
        break;
    }

    _rw2DRenderPrimativeTerm();

    return TRUE;
}
