#include <rwdlgcn.h>
#include <string.h>
#include <dolphin/vi.h>
#include <dolphin/mtx.h>

// GameCube raster blits and clears: a raster is drawn into the current context (the camera
// raster) as one textured quad in screen space, and a camera or z-buffer context is cleared by
// drawing a quad that writes the color and/or depth; a texture context is cleared on the CPU.

// The raster that rendering and clears go to (_rwDlSetRasterContext).
RwRaster* lbl_8025F880;

// Whether the frame is drawn in two halves, which half is being drawn, and the line where the
// halves meet (dldevice.c).
extern S32 lbl_8025F808;
extern S32 lbl_8025E7F0;
extern S32 lbl_8025F85C;

// The pixel format the frame uses, and the one GX is set to (dldevice.c).
extern S32 lbl_8025F7FC;
extern S32 lbl_8025F800;

RwRGBA* RwRGBASetFromPixel(RwRGBA* rgbOut, U32 pixelValue, S32 rasterFormat);

// The depth of the quads, just in front of the far plane. Written for this project.
#define DLSPRITE_Z 0.99999994f

// Puts back the GX z mode, blend mode and cull mode the render state cache holds. Written for
// this project.
#define DLSPRITE_RESTORESTATES()                                                                   \
    do                                                                                             \
    {                                                                                              \
        GXSetZMode(GX_TRUE, (GXCompare)lbl_8025E348.zFunc, (GXBool)lbl_8025E348.zWriteEnable);    \
        GXSetBlendMode(GX_BM_BLEND, (GXBlendFactor)lbl_8025E348.gxSrcBlend,                        \
                       (GXBlendFactor)lbl_8025E348.gxDestBlend, GX_LO_CLEAR);                      \
        GXSetCullMode((GXCullMode)(lbl_8025E348.cullMode - 1));                                    \
    } while (0)

#define DLSPRITE_ERROR(_code)                                                                      \
    do                                                                                             \
    {                                                                                              \
        RwError err;                                                                               \
        err.pluginID = 1;                                                                          \
        err.errorCode = _rwerror _code;                                                            \
        RwErrorSet(&err);                                                                          \
    } while (0)

// The blit quad's orthographic projection (its x and y scales are set to the frame size) and its
// view matrix, the identity with the Z axis flipped.
F32 lbl_801DE478[7] = {1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f};
Mtx lbl_801DE494 = {
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, -1.0f, 0.0f},
};

// Sets the viewport to the whole frame and the scissor to the rectangle (clipped to the half of
// the frame being drawn), and loads the screen-space projection.
void _rwDlRasterRenderQuadInit(RwRect* rect)
{
    GXRenderModeObj* renderMode;

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE,
                      GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumChans(0);

    renderMode = _RwDlRenderMode;

    if (renderMode->field_rendering)
    {
        GXSetViewportJitter(0.0f, 0.0f, renderMode->fbWidth, renderMode->xfbHeight, 0.0f, 1.0f,
                            VIGetNextField() ^ 1);
    }
    else
    {
        GXSetViewport(0.0f, 0.0f, renderMode->fbWidth, renderMode->xfbHeight, 0.0f, 1.0f);
    }

    if (!lbl_8025F808)
    {
        GXSetScissor(lbl_8025F880->nOffsetX + rect->x, lbl_8025F880->nOffsetY + rect->y, rect->w,
                     rect->h);
    }
    else if (lbl_8025E7F0)
    {
        S32 y = (lbl_8025F880->nOffsetY << 1) + rect->y;

        if (rect->h + y <= (lbl_8025F85C + 2))
        {
            GXSetScissor(lbl_8025F880->nOffsetX + rect->x, y, rect->w, rect->h);
        }
        else if (y > lbl_8025F85C + 2)
        {
            GXSetScissor(0, 0, _RwDlRenderMode->fbWidth, lbl_8025F85C + 2);
        }
        else
        {
            GXSetScissor(lbl_8025F880->nOffsetX + rect->x, y, rect->w, (lbl_8025F85C + 2) - y);
        }

        GXSetScissorBoxOffset(0, 0);
    }
    else
    {
        S32 y = (lbl_8025F880->nOffsetY << 1) + rect->y;

        if (y >= lbl_8025F85C - 2)
        {
            GXSetScissor(lbl_8025F880->nOffsetX + rect->x, y, rect->w, rect->h);
        }
        else if (rect->h + y < lbl_8025F85C - 2)
        {
            GXSetScissor(0, lbl_8025F85C - 2, _RwDlRenderMode->fbWidth, lbl_8025F85C + 2);
        }
        else
        {
            GXSetScissor(lbl_8025F880->nOffsetX + rect->x, lbl_8025F85C - 2, rect->w,
                         rect->h + y - (lbl_8025F85C - 2));
        }

        GXSetScissorBoxOffset(0, lbl_8025F85C - 2);
    }

    lbl_801DE478[1] = 2.0f / (F32)(_RwDlRenderMode->fbWidth - 1);
    lbl_801DE478[3] = -2.0f / (F32)(_RwDlRenderMode->xfbHeight - 1);
    GXSetProjectionv(lbl_801DE478);
    GXLoadPosMtxImm(lbl_801DE494, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
}

// Draws the raster into the context's rectangle as a textured quad, filtered when scaled and
// blended by alpha when asked; the render states it changes are put back afterwards.
void _rwDlRasterRenderQuad(RwRaster* raster, RwRect* rect, S32 scaled, S32 alpha)
{
    RwRaster* top = raster->parent;
    void* fogEnable;
    void* filter;
    void* addressU;
    void* addressV;
    void* textureRaster;
    F32 scaleU;
    F32 scaleV;

    _rwDlGetRenderState(rwRENDERSTATEFOGENABLE, &fogEnable);
    _rwDlGetRenderState(rwRENDERSTATETEXTUREFILTER, &filter);
    _rwDlGetRenderState(rwRENDERSTATETEXTUREADDRESSU, &addressU);
    _rwDlGetRenderState(rwRENDERSTATETEXTUREADDRESSV, &addressV);
    _rwDlGetRenderState(rwRENDERSTATETEXTURERASTER, &textureRaster);

    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    RwGameCubeSetAlphaCompare(1);

    if (alpha)
    {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    }
    else
    {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    }

    _rwDlSetRenderState(rwRENDERSTATEFOGENABLE, (void*)FALSE);
    _rwDlSetRenderState(rwRENDERSTATETEXTUREFILTER,
                        (void*)(scaled ? rwFILTERLINEAR : rwFILTERNEAREST));
    _rwDlSetRenderState(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);
    _rwDlSetRenderState(rwRENDERSTATETEXTURERASTER, top);
    _rwDlTextureRasterFlush();
    _rwDlRasterRenderQuadInit(rect);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    if (scaled)
    {
        scaleU = 1.0f / top->width;
        scaleV = 1.0f / top->height;

        GXPosition3f32(lbl_8025F880->nOffsetX + rect->x, lbl_8025F880->nOffsetY + rect->y,
                       DLSPRITE_Z);
        GXTexCoord2f32(raster->nOffsetX * scaleU, raster->nOffsetY * scaleV);

        GXPosition3f32(lbl_8025F880->nOffsetX + rect->x, lbl_8025F880->nOffsetY + rect->y + rect->h,
                       DLSPRITE_Z);
        GXTexCoord2f32(raster->nOffsetX * scaleU, scaleV * (raster->nOffsetY + raster->height));

        GXPosition3f32(lbl_8025F880->nOffsetX + rect->x + rect->w,
                       lbl_8025F880->nOffsetY + rect->y + rect->h, DLSPRITE_Z);
        GXTexCoord2f32(scaleU * (raster->nOffsetX + raster->width),
                       scaleV * (raster->nOffsetY + raster->height));

        GXPosition3f32(lbl_8025F880->nOffsetX + rect->x + rect->w, lbl_8025F880->nOffsetY + rect->y,
                       DLSPRITE_Z);
        GXTexCoord2f32(scaleU * (raster->nOffsetX + raster->width), raster->nOffsetY * scaleV);
    }
    else
    {
        scaleU = 1.0f / top->width;
        scaleV = 1.0f / top->height;

        GXPosition3f32(lbl_8025F880->nOffsetX + rect->x, lbl_8025F880->nOffsetY + rect->y,
                       DLSPRITE_Z);
        GXTexCoord2f32(raster->nOffsetX * scaleU, raster->nOffsetY * scaleV);

        GXPosition3f32(lbl_8025F880->nOffsetX + rect->x,
                       lbl_8025F880->nOffsetY + rect->y + raster->height, DLSPRITE_Z);
        GXTexCoord2f32(raster->nOffsetX * scaleU, scaleV * (raster->nOffsetY + raster->height));

        GXPosition3f32(lbl_8025F880->nOffsetX + rect->x + raster->width,
                       lbl_8025F880->nOffsetY + rect->y + raster->height, DLSPRITE_Z);
        GXTexCoord2f32(scaleU * (raster->nOffsetX + raster->width),
                       scaleV * (raster->nOffsetY + raster->height));

        GXPosition3f32(lbl_8025F880->nOffsetX + rect->x + raster->width,
                       lbl_8025F880->nOffsetY + rect->y, DLSPRITE_Z);
        GXTexCoord2f32(scaleU * (raster->nOffsetX + raster->width), raster->nOffsetY * scaleV);
    }

    DLSPRITE_RESTORESTATES();
    _rwDlSetRenderState(rwRENDERSTATEFOGENABLE, fogEnable);
    _rwDlSetRenderState(rwRENDERSTATETEXTUREFILTER, filter);
    _rwDlSetRenderState(rwRENDERSTATETEXTUREADDRESSU, addressU);
    _rwDlSetRenderState(rwRENDERSTATETEXTUREADDRESSV, addressV);
    _rwDlSetRenderState(rwRENDERSTATETEXTURERASTER, textureRaster);
}

// Draws the raster at the rectangle's position, at its own size or scaled to the rectangle. Only
// a camera context can be drawn to.
S32 _rwDlRasterRenderGeneric(RwRaster* raster, RwRect* rect, S32 scaled, S32 alpha)
{
    RwRect dst;

    dst.x = rect->x;
    dst.y = rect->y;

    if (scaled)
    {
        dst.w = rect->w;
        dst.h = rect->h;
    }
    else
    {
        dst.w = raster->width;
        dst.h = raster->height;
    }

    switch (lbl_8025F880->cType)
    {
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:
    case rwRASTERTYPECAMERATEXTURE:
        switch (raster->cType)
        {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPEZBUFFER:
        case rwRASTERTYPECAMERA:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
            DLSPRITE_ERROR((2, "SRC, DST Raster render combination not supported"));
            return FALSE;
        case 3:
            break;
        default:
            break;
        }
        break;
    case rwRASTERTYPECAMERA:
        switch (raster->cType)
        {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
            _rwDlRasterRenderQuad(raster, &dst, scaled, alpha);
            break;
        case rwRASTERTYPEZBUFFER:
        case rwRASTERTYPECAMERA:
            DLSPRITE_ERROR((2, "SRC, DST Raster render combination not supported"));
            return FALSE;
        case 3:
            break;
        default:
            break;
        }
        break;
    default:
        DLSPRITE_ERROR((2, "SRC, DST Raster render combination not supported"));
        return FALSE;
    }

    return TRUE;
}

S32 _rwDlRasterRender(void* raster, void* rect, S32 flags)
{
    return _rwDlRasterRenderGeneric((RwRaster*)raster, (RwRect*)rect, FALSE, TRUE);
}

S32 _rwDlRasterRenderFast(void* raster, void* rect, S32 flags)
{
    return _rwDlRasterRenderGeneric((RwRaster*)raster, (RwRect*)rect, FALSE, FALSE);
}

S32 _rwDlRasterRenderScaled(void* raster, void* rect, S32 flags)
{
    return _rwDlRasterRenderGeneric((RwRaster*)raster, (RwRect*)rect, TRUE, TRUE);
}

// The clear quad's orthographic projection and its view matrix, the identity with the Z axis
// flipped.
F32 lbl_801DE4F8[7] = {1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f};
Mtx lbl_801DE514 = {
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, -1.0f, 0.0f},
};

// Sets up the clear quad: an untextured quad in the material color. A camera texture is cleared
// at its own pixel format over the whole 640x528 frame buffer.
void _rwDlRasterCamera_ZClearRectInit(RwRaster* raster, RwRect* rect)
{
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);

    if (raster->cType == rwRASTERTYPECAMERATEXTURE)
    {
        if (RWGCRASTEREXT(raster)->flags & RWGCRASTER_ALPHA)
        {
            if (lbl_8025F7FC != GX_PF_RGBA6_Z24)
            {
                GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
                lbl_8025F800 = GX_PF_RGBA6_Z24;
            }
        }
        else if (lbl_8025F7FC != GX_PF_RGB8_Z24)
        {
            GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
            lbl_8025F800 = GX_PF_RGB8_Z24;
        }

        GXSetViewport(0.0f, 0.0f, 640.0f, 528.0f, 0.0f, 1.0f);
        GXSetScissor(0, 0, 640, 528);
        lbl_801DE4F8[1] = 2.0f / 639.0f;
        lbl_801DE4F8[3] = -2.0f / 527.0f;
    }
    else
    {
        GXRenderModeObj* renderMode;

        if (lbl_8025F800 != lbl_8025F7FC)
        {
            GXSetPixelFmt((GXPixelFmt)lbl_8025F7FC, GX_ZC_LINEAR);
            lbl_8025F800 = lbl_8025F7FC;
        }

        renderMode = _RwDlRenderMode;

        if (renderMode->field_rendering)
        {
            GXSetViewportJitter(0.0f, 0.0f, renderMode->fbWidth, renderMode->xfbHeight, 0.0f,
                                1.0f, VIGetNextField() ^ 1);
        }
        else
        {
            GXSetViewport(0.0f, 0.0f, renderMode->fbWidth, renderMode->xfbHeight, 0.0f, 1.0f);
        }

        if (!lbl_8025F808)
        {
            GXSetScissor(0, 0, _RwDlRenderMode->fbWidth, _RwDlRenderMode->xfbHeight);
        }
        else if (lbl_8025E7F0)
        {
            GXSetScissor(0, 0, _RwDlRenderMode->fbWidth, lbl_8025F85C + 2);
            GXSetScissorBoxOffset(0, 0);
        }
        else
        {
            GXSetScissor(0, lbl_8025F85C - 2, _RwDlRenderMode->fbWidth, lbl_8025F85C + 2);
            GXSetScissorBoxOffset(0, lbl_8025F85C - 2);
        }

        lbl_801DE4F8[1] = 2.0f / (F32)(_RwDlRenderMode->fbWidth - 1);
        lbl_801DE4F8[3] = -2.0f / (F32)(_RwDlRenderMode->xfbHeight - 1);
    }

    GXSetProjectionv(lbl_801DE4F8);
    GXLoadPosMtxImm(lbl_801DE514, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
}

// Clears the rectangle of the frame: the color (flags & 1) to the given color and/or the depth
// (flags & 2) to the far plane, by drawing a quad with the depth test passing always.
void _rwDlRasterCamera_ZClearRect(RwRaster* raster, RwRect* rect, RwRGBA* color, S32 flags)
{
    GXColor matColor;
    S32 fogWasEnabled = FALSE;

    if (flags & 1)
    {
        matColor.r = color->red;
        matColor.g = color->green;
        matColor.b = color->blue;
        matColor.a = color->alpha;

        if (flags & 2)
        {
            GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
        }
        else
        {
            GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
        }
    }
    else if (flags & 2)
    {
        matColor.r = 0;
        matColor.g = 0;
        matColor.b = 0;
        matColor.a = 0xFF;
        GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
        GXSetColorUpdate(GX_FALSE);
    }
    else
    {
        return;
    }

    GXSetChanMatColor(GX_COLOR0A0, matColor);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetCullMode(GX_CULL_NONE);
    GXSetAlphaCompare(GX_ALWAYS, lbl_8025E348.alphaRef0, GX_AOP_AND, GX_ALWAYS,
                      lbl_8025E348.alphaRef1);
    GXSetZCompLoc(GX_TRUE);

    if (lbl_8025E348.fogEnable)
    {
        _rwDlRenderStateFogEnable(FALSE);
        fogWasEnabled = TRUE;
    }

    lbl_8025E348.alphaTest = 1;
    _rwDlRasterCamera_ZClearRectInit(raster, rect);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(rect->x, rect->y, DLSPRITE_Z);
    GXPosition3f32(rect->x, rect->y + rect->h, DLSPRITE_Z);
    GXPosition3f32(rect->x + rect->w, rect->y + rect->h, DLSPRITE_Z);
    GXPosition3f32(rect->x + rect->w, rect->y, DLSPRITE_Z);

    if (fogWasEnabled)
    {
        _rwDlRenderStateFogEnable(TRUE);
    }

    DLSPRITE_RESTORESTATES();
    GXSetColorUpdate(GX_TRUE);
}

// Clears the rectangle of the context to the pixel value: a camera clears its color, a z-buffer
// its depth, and a texture is filled on the CPU through a lock.
S32 _rwDlRasterClearGeneric(RwRaster* raster, RwRect* rect, S32 pixelValue)
{
    S32 locked;

    switch (lbl_8025F880->cType)
    {
    case rwRASTERTYPECAMERA:
    {
        RwRGBA color;

        RwRGBASetFromPixel(&color, pixelValue, RwRasterGetFormat(lbl_8025F880));
        rect->x += lbl_8025F880->nOffsetX;
        rect->y += lbl_8025F880->nOffsetY;
        _rwDlRasterCamera_ZClearRect(raster, rect, &color, 1);
        break;
    }
    case rwRASTERTYPEZBUFFER:
        rect->x += lbl_8025F880->nOffsetX;
        rect->y += lbl_8025F880->nOffsetY;
        _rwDlRasterCamera_ZClearRect(raster, rect, NULL, 2);
        break;
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:
    case rwRASTERTYPECAMERATEXTURE:
        locked = FALSE;

        if (lbl_8025F880->parent == lbl_8025F880 && !lbl_8025F880->nOffsetX &&
            !lbl_8025F880->nOffsetY && lbl_8025F880->width == rect->w &&
            lbl_8025F880->height == rect->h)
        {
            if (!(lbl_8025F880->privateFlags & rwRASTERPIXELLOCKEDWRITE))
            {
                locked = TRUE;
                RwRasterLock(lbl_8025F880, 0, rwRASTERLOCKWRITE | rwRASTERLOCKRAW);
            }

            if (pixelValue == 0 || pixelValue == -1)
            {
                memset(lbl_8025F880->cpPixels, pixelValue,
                       lbl_8025F880->stride * lbl_8025F880->height);

                if (locked)
                {
                    RwRasterUnlock(lbl_8025F880);
                }

                return TRUE;
            }
        }
        else if (!(lbl_8025F880->privateFlags & rwRASTERPIXELLOCKEDWRITE))
        {
            locked = TRUE;
            RwRasterLock(lbl_8025F880, 0, rwRASTERLOCKREADWRITE);
        }

        switch (lbl_8025F880->depth)
        {
        case 4:
        {
            U32 y;

            pixelValue = (((pixelValue << 4) & 0xF0) | (pixelValue & 0xF)) & 0xFF;

            for (y = 0; y < rect->h; y++)
            {
                memset(lbl_8025F880->cpPixels + lbl_8025F880->stride * (rect->y + y) +
                           (rect->x >> 1),
                       pixelValue, rect->w >> 1);
            }
            break;
        }
        case 8:
        {
            U32 y;

            pixelValue = (U8)pixelValue;

            for (y = 0; y < rect->h; y++)
            {
                memset(lbl_8025F880->cpPixels + lbl_8025F880->stride * (rect->y + y) + rect->x,
                       pixelValue, rect->w);
            }
            break;
        }
        case 16:
        {
            U32 y;
            U16 value = pixelValue;

            for (y = 0; y < rect->h; y++)
            {
                U16* pixels = (U16*)(lbl_8025F880->cpPixels +
                                     lbl_8025F880->stride * (rect->y + y) + (rect->x << 1));
                U32 x;

                for (x = 0; x < rect->w; x++)
                {
                    *pixels++ = value;
                }
            }
            break;
        }
        case 32:
        {
            U32 y;

            for (y = 0; y < rect->h; y++)
            {
                U32* pixels = (U32*)(lbl_8025F880->cpPixels +
                                     lbl_8025F880->stride * (rect->y + y) + (rect->x << 2));
                U32 x;

                for (x = 0; x < rect->w; x++)
                {
                    *pixels++ = pixelValue;
                }
            }
            break;
        }
        case 24:
        default:
            DLSPRITE_ERROR((0x8000000C));
            break;
        }

        if (locked)
        {
            RwRasterUnlock(lbl_8025F880);
        }
        break;
    default:
        DLSPRITE_ERROR((0x8000000D));
        return FALSE;
    }

    return TRUE;
}

S32 _rwDlRasterClearRect(void* raster, void* rect, S32 pixelValue)
{
    return _rwDlRasterClearGeneric(lbl_8025F880, (RwRect*)rect, pixelValue);
}

S32 _rwDlRasterClear(void* raster, void* unused, S32 pixelValue)
{
    RwRect rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = lbl_8025F880->width;
    rect.h = lbl_8025F880->height;

    return _rwDlRasterClearGeneric(lbl_8025F880, &rect, pixelValue);
}

S32 _rwDlSetRasterContext(void* out, void* raster, S32 flags)
{
    lbl_8025F880 = (RwRaster*)raster;

    return TRUE;
}
