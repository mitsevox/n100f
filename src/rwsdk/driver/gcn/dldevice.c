#include <rwdlgcn.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <dolphin/mtx.h>

// The GameCube device: it picks the video mode, allocates the GX FIFO and two external frame
// buffers, and runs a three-deep ring of finished frames. Each frame's display copy is queued in
// the FIFO behind a breakpoint; the breakpoint callback and the retrace callbacks advance the ring
// and swap the frame buffer on the retrace once the frame's draw sync token has passed. With
// antialiasing the frame is drawn in two halves, each copied to its half of the frame buffer.

// One frame of the ring: the frame buffer it shows and the FIFO position of its display copy.
// Type and member names written for this project.
typedef struct DlFrame DlFrame;
struct DlFrame
{
    void* frameBuffer;
    void* fifoPos;
};

// The device's video modes (NTSC, PAL, MPAL and the application's own render mode) and the two
// errors _rwDlRenderModeSelect raises. Type and member names written for this project.
typedef struct DlVideoModes DlVideoModes;
struct DlVideoModes
{
    RwVideoMode modes[4];
    char badPixelFormat[0x18];
    char badTvFormat[0x14];
};

// The camera being updated and the device's open parameter. Type and member names written for
// this project.
typedef struct DlDeviceState DlDeviceState;
struct DlDeviceState
{
    RwCamera* camera;
    void* openParam;
};

// The ring of finished frames.
extern DlFrame lbl_8025E2B0[3];

// The application's render mode, copied by _rwDlRenderModeSelect.
extern GXRenderModeObj lbl_8025E2C8;

// The camera's view matrix: the inverse of its frame, offset by the view offset
// (_rwDlCameraBeginUpdate).
extern RwMatrix lbl_8025E304;

// Whether the next display copy clears the frame.
extern S32 lbl_8025F7F8;

// The pixel format the frame uses, and the one GX is set to.
extern S32 lbl_8025F7FC;
extern S32 lbl_8025F800;

// Which video mode is in use: 0 NTSC, 1 PAL, 2 MPAL, 0x2A the application's own.
extern S32 lbl_8025F804;

// Whether the frame is drawn in two halves, which half is being drawn, and the line where the
// halves meet.
extern S32 lbl_8025F808;
extern S32 lbl_8025E7F0;
extern S32 lbl_8025F85C;

// The FIFO and frame buffer allocation, the FIFO size and the 32-byte aligned FIFO.
extern void* lbl_8025F80C;
extern U32 lbl_8025E7F4;
extern void* lbl_8025F854;
extern GXFifoObj* lbl_8025F858;

// The ring's read and write positions, the token ring's write and read positions, whether the
// FIFO breakpoint is armed, whether a frame waits for its token, whether a frame waits for the
// retrace, whether a retrace has passed since, and which half a two-half frame is on.
extern S32 lbl_8025F810;
extern S32 lbl_8025F814;
extern S32 lbl_8025F818;
extern S32 lbl_8025F81C;
extern S32 lbl_8025F820;
extern S32 lbl_8025F824;
extern S32 lbl_8025F828;
extern S32 lbl_8025F82C;
extern S32 lbl_8025F830;

// The threads waiting for a free frame.
extern OSThreadQueue lbl_8025F834;

// The draw sync token of each frame in the ring.
extern U16 lbl_8025F83C[3];

// The frame buffer on screen, the one being drawn, and the two frame buffers.
extern void* lbl_8025F844;
extern void* lbl_8025F848;
extern void* lbl_8025F84C;
extern void* lbl_8025F850;

// Set to blank the screen until the first frame is shown.
extern S32 lbl_8025E7F8;

extern DlDeviceState lbl_8025F864;

// The draw sync token to issue next, and the last token the GPU has passed.
extern U16 lbl_8025E800;
extern U16 lbl_8025F890;

extern DlVideoModes lbl_801DDF90;

// The GX projection the camera sets.
extern F32 lbl_801DE04C[7];

// The device this driver registers.
extern RwDevice lbl_801DE068;

// The driver's standard functions, by RenderWare standard function index (one table entry).
typedef struct DlStandard DlStandard;
struct DlStandard
{
    S32 index;
    RwStandardFunc func;
};

void GXSetProjectionv(const f32* ptr);
S32 _rwMatrixSetMultFn(void* multiplyFunc);
S32 _rwVectorSetMultFn(void* multPoints, void* multVectors);
void DlMatrixMultiplyPaired(RwMatrix* dst, const RwMatrix* a, const RwMatrix* b);
void DlV3dTransformPointsPaired(RwV3d* dst, const RwV3d* src, S32 count, const RwMatrix* matrix);
void DlV3dTransformVectorsPaired(RwV3d* dst, const RwV3d* src, S32 count,
                                 const RwMatrix* matrix);
void _rwDlRenderStateOpen(void);
void _rwDlRenderStateClose(void);
S32 _rwDlRasterPluginAttach(void);
S32 _rwDlTexturePluginAttach(void);
void _rwDlRasterCamera_ZClearRect(RwRaster* raster, RwRect* rect, RwRGBA* color, S32 flags);
S32 _rwDlTokenQueryDone(U16 token);
RwDevice* _rwDeviceGetHandle(void);

S32 _rwDlCameraClear(void* camera, void* color, S32 flags);
S32 _rwDlCameraBeginUpdate(void* out, void* camera, S32 flags);
S32 _rwDlCameraEndUpdate(void* out, void* camera, S32 flags);
S32 _rwDlRasterShowRaster(void* raster, void* inOut, S32 flags);
S32 _rwDlRGBToPixel(U32* pixelOut, const RwRGBA* rgba, S32 format);
S32 _rwDlPixelToRGB(RwRGBA* rgba, const U32* pixelIn, S32 format);
S32 _rwDlRasterSetFromImage(RwRaster* raster, RwImage* image);
S32 _rwDlImageGetFromRaster(RwImage* image, RwRaster* raster);
S32 fn_8017A4D8(void* out, RwRaster* raster, S32 flags);
S32 fn_8017A374(void* out, RwRaster* raster, S32 flags);
S32 fn_801771C4(void* out, void* inOut, S32 flags);
S32 fn_80179198(void** pixelsOut, RwRaster* raster, S32 flags);
S32 fn_80179614(void* out, RwRaster* raster, S32 flags);
S32 fn_801797E0(void** paletteOut, RwRaster* raster, S32 flags);
S32 fn_8017989C(void* out, RwRaster* raster, S32 flags);
S32 _rwDlRasterClear(void* raster, void* unused, S32 pixelValue);
S32 _rwDlRasterClearRect(void* raster, void* rect, S32 pixelValue);
S32 _rwDlRasterRender(void* raster, void* rect, S32 flags);
S32 _rwDlRasterRenderScaled(void* raster, void* rect, S32 flags);
S32 _rwDlRasterRenderFast(void* raster, void* rect, S32 flags);
S32 _rwDlSetRasterContext(void* out, void* raster, S32 flags);
S32 _rwDlRasterSubRaster(RwRaster* raster, RwRaster* parent, S32 flags);
S32 _rwDlNativeTextureGetSize(S32* size, RwTexture* texture);
S32 fn_8017C9E4(RwStream* stream, RwTexture* texture);
S32 fn_8017CCBC(RwStream* stream, RwTexture** textureRef);
S32 fn_80179BDC(void* out, void* inOut, S32 flags);

#define DLDEVICE_RING 3

// The draw sync token after _token; tokens count up to 0xE000 and wrap.
#define DLDEVICE_NEXTTOKEN(_token) (((_token) + 1) % 0xE000)

#define DLDEVICE_ERROR(_code, _string)                                                             \
    do                                                                                             \
    {                                                                                              \
        RwError err;                                                                               \
        err.pluginID = 1;                                                                          \
        err.errorCode = _rwerror(_code, _string);                                                  \
        RwErrorSet(&err);                                                                          \
    } while (0)

// Hands the next frame of the ring to the frame buffer: the FIFO has reached the frame's display
// copy. The breakpoint moves on to the frame after it, or is disarmed when the ring is empty, and
// a thread waiting for a free frame wakes. A whole frame waits for its token; of a two-half frame,
// only the second half does.
void _rwDlBreakPtCallback(void)
{
    U32 next;

    if (lbl_8025E2B0[lbl_8025F810].frameBuffer != lbl_8025F844)
    {
        next = (lbl_8025F810 + 1) % DLDEVICE_RING;

        if (next == lbl_8025F814)
        {
            GXDisableBreakPt();
            lbl_8025F820 = 0;
        }
        else
        {
            GXEnableBreakPt(lbl_8025E2B0[next].fifoPos);
        }

        lbl_8025F810 = next;
        OSWakeupThread(&lbl_8025F834);

        if (lbl_8025F808 == 0)
        {
            lbl_8025F824 = 1;
        }
        else if (lbl_8025F830 == 1)
        {
            lbl_8025F824 = 1;
            lbl_8025F830 = 0;
        }
        else
        {
            lbl_8025F830 = 1;
        }
    }
    else
    {
        lbl_8025F828 = 1;
    }
}

// Before each retrace: once the waiting frame's token has passed, shows the other frame buffer
// (unblanking the screen after the first frame).
void _rwDlVIPreRetraceCallback(u32 retraceCount)
{
    S16 token = lbl_8025F83C[lbl_8025F81C];

    if (lbl_8025F824 == 1 && _rwDlTokenQueryDone(token))
    {
        lbl_8025F844 = (lbl_8025F844 == lbl_8025F850) ? lbl_8025F84C : lbl_8025F850;
        VISetNextFrameBuffer(lbl_8025F844);

        if (lbl_8025E7F8 != 0)
        {
            VISetBlack(FALSE);
            lbl_8025E7F8 = 0;
        }

        VIFlush();
        lbl_8025F82C = 1;
        lbl_8025F81C = (lbl_8025F81C + 1) % DLDEVICE_RING;
        lbl_8025F824 = 0;
    }
}

// After each retrace: a frame the breakpoint reached while its frame buffer was still on screen
// moves on now that the screen has swapped.
void _rwDlVIPostRetraceCallback(u32 retraceCount)
{
    U32 next;

    if (lbl_8025F828 != 0 && lbl_8025F82C != 0)
    {
        next = (lbl_8025F810 + 1) % DLDEVICE_RING;

        if (next == lbl_8025F814)
        {
            GXDisableBreakPt();
            lbl_8025F820 = 0;
        }
        else
        {
            GXEnableBreakPt(lbl_8025E2B0[next].fifoPos);
        }

        lbl_8025F810 = next;
        OSWakeupThread(&lbl_8025F834);

        if (lbl_8025F808 == 0)
        {
            lbl_8025F824 = 1;
        }
        else if (lbl_8025F830 == 1)
        {
            lbl_8025F824 = 1;
            lbl_8025F830 = 0;
        }
        else
        {
            lbl_8025F830 = 1;
        }

        lbl_8025F828 = 0;
    }

    lbl_8025F82C = 0;
}

// The standard function the driver does not provide.
S32 _rwDlNullStandard(void* out, void* inOut, S32 flags)
{
    return 0;
}

// Picks the render mode: the application's own (copied, with the pixel format it asks for, or
// RGB565 with antialiasing, and recorded as the fourth video mode, whose depth follows the pixel
// format asked for), or else the TV format's default.
void _rwDlRenderModeSelect(GXRenderModeObj* renderMode, S32 pixelFormat)
{
    DlVideoModes* modes = &lbl_801DDF90;
    S32 format;

    if (renderMode != NULL)
    {
        format = pixelFormat;
        lbl_8025E2C8 = *renderMode;
        _RwDlRenderMode = &lbl_8025E2C8;
        lbl_8025F804 = 0x2A;

        if (lbl_8025E2C8.aa)
        {
            format = GX_PF_RGB565_Z16;
        }

        lbl_8025F7FC = format;
        lbl_8025F800 = format;
        modes->modes[3].width = lbl_8025E2C8.fbWidth;
        modes->modes[3].height = lbl_8025E2C8.efbHeight;

        switch (pixelFormat)
        {
        case GX_PF_RGB8_Z24:
        case GX_PF_RGBA6_Z24:
            modes->modes[3].depth = 24;
            break;
        case GX_PF_RGB565_Z16:
            modes->modes[3].depth = 16;
            break;
        default:
            DLDEVICE_ERROR(2, modes->badPixelFormat);
            break;
        }
    }
    else
    {
        switch (VIGetTvFormat())
        {
        case VI_NTSC:
            lbl_8025F804 = 0;
            _RwDlRenderMode = &GXNtsc480IntDf;
            break;
        case VI_PAL:
        case VI_EURGB60:
            lbl_8025F804 = 1;
            _RwDlRenderMode = &GXPal528IntDf;
            break;
        case VI_MPAL:
            lbl_8025F804 = 2;
            _RwDlRenderMode = &GXMpal480IntDf;
            break;
        default:
            DLDEVICE_ERROR(2, modes->badTvFormat);
            break;
        }
    }
}

// The driver's system function: RenderWare's device requests (open, close, start, stop, the
// video mode queries, the standard function table, and so on), by request number.
S32 _rwDlSystem(S32 request, void* out, void* inOut, S32 numIn)
{
    switch (request)
    {
    case 7:
        return numIn < 1;
    case 5:
        *(S32*)out = 1;
        return TRUE;
    case 6:
        if (numIn < 1)
        {
            switch (lbl_8025F804)
            {
            case 0:
                *(RwVideoMode*)out = lbl_801DDF90.modes[0];
                break;
            case 1:
            case 5:
                *(RwVideoMode*)out = lbl_801DDF90.modes[1];
                break;
            case 2:
                *(RwVideoMode*)out = lbl_801DDF90.modes[2];
                break;
            case 0x2A:
                *(RwVideoMode*)out = lbl_801DDF90.modes[3];
                break;
            }

            return TRUE;
        }

        return FALSE;
    case 10:
        *(S32*)out = 0;
        return TRUE;
    case 8:
        return TRUE;
    case 4:
        *(RwDevice*)out = *_rwDeviceGetHandle();
        lbl_8025F864.openParam = inOut;
        _rwDlRasterPluginAttach();
        _rwDlTexturePluginAttach();
        return TRUE;
    case 0:
    {
        void** params = *(void***)inOut;
        U32 frameSize;

        if (params != NULL)
        {
            _rwDlRenderModeSelect(params[0], (S32)params[1]);
            lbl_8025E7F4 = (U32)params[2];
        }
        else
        {
            _rwDlRenderModeSelect(NULL, lbl_8025F7FC);
        }

        frameSize = ((_RwDlRenderMode->fbWidth + 15) & 0xFFF0) * _RwDlRenderMode->xfbHeight;
        lbl_8025F80C = RwMalloc(lbl_8025E7F4 + frameSize * 4 + 0x1F);
        lbl_8025F854 = (void*)(((U32)lbl_8025F80C + 0x1F) & ~0x1F);
        DCInvalidateRange(lbl_8025F854, lbl_8025E7F4);
        lbl_8025F850 = (U8*)lbl_8025F854 + lbl_8025E7F4;
        lbl_8025F84C = (U8*)lbl_8025F850 + frameSize * 2;
        lbl_8025F844 = lbl_8025F850;
        lbl_8025F848 = lbl_8025F84C;
        lbl_8025F858 = GXInit(lbl_8025F854, lbl_8025E7F4);

        while ((U16)GXReadDrawSync() != lbl_8025F890)
        {
            GXSetDrawSync(lbl_8025F890);
        }

        return TRUE;
    }
    case 1:
        RwFree(lbl_8025F80C);
        lbl_8025F84C = NULL;
        lbl_8025F850 = NULL;
        lbl_8025F854 = NULL;
        lbl_8025F80C = NULL;
        return TRUE;
    case 2:
    {
        GXRenderModeObj* renderMode = _RwDlRenderMode;
        S32 pixelFormat = lbl_8025F7FC;

        VIConfigure(renderMode);
        VISetBlack(TRUE);
        VIFlush();

        if (renderMode->field_rendering)
        {
            GXSetViewportJitter(0.0f, 0.0f, renderMode->fbWidth, renderMode->xfbHeight, 0.0f, 1.0f,
                                VIGetNextField() ^ 1);
        }
        else
        {
            GXSetViewport(0.0f, 0.0f, renderMode->fbWidth, renderMode->xfbHeight, 0.0f, 1.0f);
        }

        GXSetScissor(0, 0, renderMode->fbWidth, renderMode->efbHeight);
        GXSetScissorBoxOffset(0, 0);
        GXSetDispCopySrc(0, 0, renderMode->fbWidth, renderMode->efbHeight);
        GXSetDispCopyDst(renderMode->fbWidth, renderMode->xfbHeight);
        GXSetCopyFilter(renderMode->aa, renderMode->sample_pattern, GX_TRUE, renderMode->vfilter);

        if (renderMode->aa && renderMode->xfbHeight == renderMode->viHeight)
        {
            GXSetDispCopyYScale(1.0f);
            lbl_8025F808 = 1;
            lbl_8025F85C = renderMode->xfbHeight / 2;
            lbl_8025E7F0 = 1;
        }
        else
        {
            GXSetDispCopyYScale((F32)renderMode->xfbHeight / (F32)renderMode->efbHeight);
            lbl_8025F808 = 0;
        }

        GXSetFieldMode(renderMode->field_rendering,
                       renderMode->xfbHeight < renderMode->viHeight);
        GXSetPixelFmt(pixelFormat, GX_ZC_LINEAR);
        lbl_8025E7F8 = 1;
        VIWaitForRetrace();

        if (_RwDlRenderMode->viTVmode & 1)
        {
            VIWaitForRetrace();
        }

        GXSetDispCopyGamma(GX_GM_1_0);
        VISetPreRetraceCallback(_rwDlVIPreRetraceCallback);
        VISetPostRetraceCallback(_rwDlVIPostRetraceCallback);
        GXSetBreakPtCallback(_rwDlBreakPtCallback);
        OSInitThreadQueue(&lbl_8025F834);
        return TRUE;
    }
    case 17:
        _rwMatrixSetMultFn(DlMatrixMultiplyPaired);
        _rwVectorSetMultFn(DlV3dTransformPointsPaired, DlV3dTransformVectorsPaired);
        _rwDlRenderStateOpen();
        return TRUE;
    case 18:
        _rwDlRenderStateClose();
        return TRUE;
    case 3:
        return TRUE;
    case 9:
        return TRUE;
    case 11:
    {
        DlStandard standards[] = {
            {1, _rwDlCameraBeginUpdate},
            {10, _rwDlCameraEndUpdate},
            {21, _rwDlCameraClear},
            {20, _rwDlRasterShowRaster},
            {2, (RwStandardFunc)_rwDlRGBToPixel},
            {3, (RwStandardFunc)_rwDlPixelToRGB},
            {7, (RwStandardFunc)_rwDlRasterSetFromImage},
            {6, (RwStandardFunc)_rwDlImageGetFromRaster},
            {5, (RwStandardFunc)fn_8017A4D8},
            {4, (RwStandardFunc)fn_8017A374},
            {9, fn_801771C4},
            {8, (RwStandardFunc)_rwDlTextureSetRaster},
            {15, (RwStandardFunc)fn_80179198},
            {16, (RwStandardFunc)fn_80179614},
            {23, (RwStandardFunc)fn_801797E0},
            {24, (RwStandardFunc)fn_8017989C},
            {14, _rwDlRasterClear},
            {13, _rwDlRasterClearRect},
            {17, _rwDlRasterRender},
            {18, _rwDlRasterRenderScaled},
            {19, _rwDlRasterRenderFast},
            {11, _rwDlSetRasterContext},
            {12, (RwStandardFunc)_rwDlRasterSubRaster},
            {25, (RwStandardFunc)_rwDlNativeTextureGetSize},
            {27, (RwStandardFunc)fn_8017C9E4},
            {26, (RwStandardFunc)fn_8017CCBC},
            {28, fn_80179BDC},
        };
        RwStandardFunc* funcs = out;
        S32 count = sizeof(standards) / sizeof(standards[0]);
        S32 i;

        for (i = 0; i < numIn; i++)
        {
            funcs[i] = _rwDlNullStandard;
        }

        while (count--)
        {
            if (standards[count].index < numIn && standards[count].index >= 0)
            {
                funcs[standards[count].index] = standards[count].func;
            }
        }

        return TRUE;
    }
    case 19:
        *(S32*)out = 0x400;
        return TRUE;
    }

    return FALSE;
}

// Clears the camera: by drawing a quad over its frame buffer's area (twice the height when the
// frame is drawn in halves), or, when the next display copy clears the frame, by setting that
// copy's clear color (and the far depth) if the image is cleared.
S32 _rwDlCameraClear(void* camera, void* color, S32 flags)
{
    RwRect rect;
    RwRaster* raster;

    if (lbl_8025F7F8 == 0)
    {
        raster = ((RwCamera*)camera)->frameBuffer;

        if (lbl_8025F808 == 0)
        {
            rect.x = raster->nOffsetX;
            rect.y = raster->nOffsetY;
            rect.w = raster->width;
            rect.h = raster->height;
        }
        else
        {
            rect.x = raster->nOffsetX;
            rect.y = raster->nOffsetY * 2;
            rect.w = raster->width;
            rect.h = raster->height * 2;
        }

        _rwDlRasterCamera_ZClearRect(raster, &rect, color, flags);
    }
    else if (flags & rwCAMERACLEARIMAGE)
    {
        GXSetCopyClear(*(GXColor*)color, 0xFFFFFF);
    }

    return TRUE;
}

// Starts rendering through the camera: builds its view matrix (the inverse of its frame, offset
// by the view offset), loads its projection, and sets the viewport and scissor to its frame
// buffer, or to the half of the frame being drawn.
S32 _rwDlCameraBeginUpdate(void* out, void* object, S32 flags)
{
    RwCamera* camera = object;
    RwMatrix offset;
    RwRaster* raster;

    GXSetCurrentGXThread();
    lbl_8025F864.camera = camera;

    offset.flags = 0;
    offset.right.x = 1.0f;
    offset.right.y = 0.0f;
    offset.right.z = 0.0f;
    offset.up.x = 0.0f;
    offset.up.y = 1.0f;
    offset.up.z = 0.0f;
    offset.at.x = -camera->viewOffset.x;
    offset.at.y = camera->viewOffset.y;
    offset.at.z = 1.0f;
    offset.pos.x = camera->viewOffset.x;
    offset.pos.y = -camera->viewOffset.y;
    offset.pos.z = 0.0f;

    lbl_8025E304.flags = 0x20003;
    RwMatrixInvert(&lbl_8025E304, RwFrameGetLTM(RwCameraGetFrame(camera)));
    RwMatrixTransform(&lbl_8025E304, &offset, rwCOMBINEPOSTCONCAT);

    lbl_801DE04C[1] = camera->recipViewWindow.x;
    lbl_801DE04C[3] = camera->recipViewWindow.y;

    if (camera->projectionType == rwPARALLEL)
    {
        lbl_801DE04C[0] = 1.0f;
        lbl_801DE04C[5] = -1.0f / (camera->farPlane - camera->nearPlane);
        lbl_801DE04C[6] = camera->farPlane * lbl_801DE04C[5];
    }
    else
    {
        lbl_801DE04C[0] = 0.0f;
        lbl_801DE04C[5] = -camera->nearPlane / (camera->farPlane - camera->nearPlane);
        lbl_801DE04C[6] = camera->farPlane * lbl_801DE04C[5];
    }

    GXSetProjectionv(lbl_801DE04C);
    raster = camera->frameBuffer;

    if (raster->cType & (rwRASTERTYPEZBUFFER | rwRASTERTYPETEXTURE))
    {
        if (RWGCRASTEREXT(raster)->flags & RWGCRASTER_ALPHA)
        {
            if (lbl_8025F800 != GX_PF_RGBA6_Z24)
            {
                GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
                lbl_8025F800 = GX_PF_RGBA6_Z24;
            }
        }
        else if (lbl_8025F800 != GX_PF_RGB8_Z24)
        {
            GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
            lbl_8025F800 = GX_PF_RGB8_Z24;
        }

        GXSetViewport(raster->nOffsetX, raster->nOffsetY, raster->width, raster->height, 0.0f,
                      1.0f);
        GXSetScissor(raster->nOffsetX, raster->nOffsetY, raster->width, raster->height);
        GXSetScissorBoxOffset(0, 0);
    }
    else
    {
        if (lbl_8025F800 != lbl_8025F7FC)
        {
            GXSetPixelFmt(lbl_8025F7FC, GX_ZC_LINEAR);
            lbl_8025F800 = lbl_8025F7FC;
        }

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

    return TRUE;
}

S32 _rwDlCameraEndUpdate(void* out, void* camera, S32 flags)
{
    lbl_8025F864.camera = NULL;

    return TRUE;
}

// Finishes the frame: waits while the ring is full, queues the frame (its frame buffer, the FIFO
// position of its display copy and its draw sync token), arms the breakpoint if needed, and copies
// the frame to the frame buffer being drawn, which then swaps. Of a two-half frame, the first half
// is copied to the top of the frame buffer and the second to the bottom, after which it swaps.
S32 _rwDlRasterShowRaster(void* raster, void* inOut, S32 flags)
{
    void* readPtr;
    void* writePtr;
    BOOL enabled;
    S32 full;
    U32 stride;

    enabled = OSDisableInterrupts();
    full = (lbl_8025F814 - lbl_8025F810 == -1 || lbl_8025F814 - lbl_8025F810 == 2) ? 1 : 0;
    OSRestoreInterrupts(enabled);

    if (full == 1)
    {
        OSSleepThread(&lbl_8025F834);
    }

    if (lbl_8025F808 == 0)
    {
        GXFlush();
        GXGetFifoPtrs(GXGetCPUFifo(), &readPtr, &writePtr);
        enabled = OSDisableInterrupts();
        lbl_8025E2B0[lbl_8025F814].fifoPos = writePtr;
        lbl_8025E2B0[lbl_8025F814].frameBuffer = lbl_8025F848;
        lbl_8025F83C[lbl_8025F818] = lbl_8025E800;
        lbl_8025F814 = (lbl_8025F814 + 1) % DLDEVICE_RING;
        lbl_8025F818 = (lbl_8025F818 + 1) % DLDEVICE_RING;
        OSRestoreInterrupts(enabled);

        if (lbl_8025F820 == 0)
        {
            lbl_8025F820 = 1;
            GXEnableBreakPt(writePtr);
        }

        GXCopyDisp(lbl_8025F848, lbl_8025F7F8);
        GXSetDrawSync(lbl_8025E800);
        lbl_8025E800 = DLDEVICE_NEXTTOKEN(lbl_8025E800);
        GXFlush();

        lbl_8025F848 = (lbl_8025F848 == lbl_8025F850) ? lbl_8025F84C : lbl_8025F850;
    }
    else if (lbl_8025E7F0 != 0)
    {
        GXFlush();
        GXGetFifoPtrs(GXGetCPUFifo(), &readPtr, &writePtr);
        enabled = OSDisableInterrupts();
        lbl_8025E2B0[lbl_8025F814].fifoPos = writePtr;
        lbl_8025E2B0[lbl_8025F814].frameBuffer = lbl_8025F848;
        lbl_8025F814 = (lbl_8025F814 + 1) % DLDEVICE_RING;
        OSRestoreInterrupts(enabled);

        if (lbl_8025F820 == 0)
        {
            lbl_8025F820 = 1;
            GXEnableBreakPt(writePtr);
        }

        GXCopyDisp(lbl_8025F848, lbl_8025F7F8);
        GXSetDrawSync(lbl_8025E800);
        lbl_8025E800 = DLDEVICE_NEXTTOKEN(lbl_8025E800);
        GXFlush();
        lbl_8025E7F0 = 0;
    }
    else
    {
        GXFlush();
        GXGetFifoPtrs(GXGetCPUFifo(), &readPtr, &writePtr);
        enabled = OSDisableInterrupts();
        lbl_8025E2B0[lbl_8025F814].fifoPos = writePtr;
        lbl_8025E2B0[lbl_8025F814].frameBuffer = lbl_8025F848;
        lbl_8025F83C[lbl_8025F818] = lbl_8025E800;
        lbl_8025F814 = (lbl_8025F814 + 1) % DLDEVICE_RING;
        lbl_8025F818 = (lbl_8025F818 + 1) % DLDEVICE_RING;
        OSRestoreInterrupts(enabled);

        if (lbl_8025F820 == 0)
        {
            lbl_8025F820 = 1;
            GXEnableBreakPt(writePtr);
        }

        GXSetCopyClamp(GX_CLAMP_BOTTOM);
        GXSetDispCopySrc(0, 2, _RwDlRenderMode->fbWidth, _RwDlRenderMode->efbHeight - 2);
        stride = (_RwDlRenderMode->fbWidth + 15) & 0xFFF0;
        GXCopyDisp((U8*)lbl_8025F848 + stride * (_RwDlRenderMode->efbHeight - 2) * 2,
                   lbl_8025F7F8);
        GXSetDrawSync(lbl_8025E800);
        lbl_8025E800 = DLDEVICE_NEXTTOKEN(lbl_8025E800);
        GXSetCopyClamp(GX_CLAMP_TOP | GX_CLAMP_BOTTOM);
        GXSetDispCopySrc(0, 0, _RwDlRenderMode->fbWidth, _RwDlRenderMode->efbHeight);
        GXFlush();

        lbl_8025F848 = (lbl_8025F848 == lbl_8025F850) ? lbl_8025F84C : lbl_8025F850;
        lbl_8025E7F0 = 1;
    }

    return TRUE;
}

RwDevice* _rwDeviceGetHandle(void)
{
    return &lbl_801DE068;
}

// Loads the object's transform (the matrix, or none, times the camera's view matrix) as GX's
// position matrix, and as its normal matrix when lighting needs it. RenderWare's matrices are
// row vectors; GX takes the transpose, with its x and z rows negated.
void _rwDlTransformSetup(RwMatrix* matrix, S32 normals)
{
    RwMatrix combined;
    RwMatrix* view;
    Mtx mtx;

    if (matrix != NULL)
    {
        combined.flags = 0x20003;
        RwMatrixMultiply(&combined, matrix, &lbl_8025E304);
        view = &combined;
    }
    else
    {
        view = &lbl_8025E304;
    }

    mtx[0][0] = -view->right.x;
    mtx[0][1] = -view->up.x;
    mtx[0][2] = -view->at.x;
    mtx[0][3] = -view->pos.x;
    mtx[1][0] = view->right.y;
    mtx[1][1] = view->up.y;
    mtx[1][2] = view->at.y;
    mtx[1][3] = view->pos.y;
    mtx[2][0] = -view->right.z;
    mtx[2][1] = -view->up.z;
    mtx[2][2] = -view->at.z;
    mtx[2][3] = -view->pos.z;

    GXLoadPosMtxImm(mtx, GX_PNMTX0);

    if (normals)
    {
        GXLoadNrmMtxImm(mtx, GX_PNMTX0);
    }

    GXSetCurrentMtx(GX_PNMTX0);
}

// Copies the frame to the raster's texels (an unfiltered texture copy of its size from the top
// left), then restores the render mode's copy filter and drops the texture cache.
void fn_80178A3C(RwRaster* raster)
{
    RwGCRasterExt* ext = RWGCRASTEREXT(raster);

    GXSetCopyFilter(GX_FALSE, NULL, GX_FALSE, NULL);
    GXSetTexCopySrc(0, 0, raster->width, raster->height);
    GXSetTexCopyDst(raster->width, raster->height, ext->format, GX_FALSE);
    GXCopyTex(ext->pixels, lbl_8025F7F8);
    GXPixModeSync();
    GXSetCopyFilter(_RwDlRenderMode->aa, _RwDlRenderMode->sample_pattern, GX_TRUE,
                    _RwDlRenderMode->vfilter);

    if (ext->region != NULL)
    {
        GXInvalidateTexRegion(ext->region);
    }
    else
    {
        GXInvalidateTexAll();
    }
}
