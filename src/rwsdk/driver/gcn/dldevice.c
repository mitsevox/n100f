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

// The camera being updated and the device's open parameter. Type and member names written for
// this project.
typedef struct DlDeviceState DlDeviceState;
struct DlDeviceState
{
    RwCamera* camera;
    void* openParam;
};

// The ring of finished frames.
DlFrame _RwGCFrameQueue[3];

// The application's render mode, copied by _rwDlRenderModeSelect.
GXRenderModeObj _RwGameCubeRenderModeObj;

// The camera's view matrix: the inverse of its frame, offset by the view offset
// (_rwDlCameraBeginUpdate).
RwMatrix _RwDlInvCamLTM;

// Which half of a two-half frame is being drawn.
S32 _RwDlFSAATop = 1;

// The FIFO size.
U32 _RwDlFifoSize = 0x40000;

// Set to blank the screen until the first frame is shown.
S32 _RwDlFirstFrame = 1;

// The camera being updated and the device's open parameter.
DlDeviceState dgGGlobals;

// The render mode in use.
GXRenderModeObj* _RwDlRenderMode;

// The line where the halves of a two-half frame meet.
S32 _RwDlHalfHeight;

// The FIFO object and the 32-byte aligned FIFO.
GXFifoObj* _RwDlDefaultFifoObj;
void* _RwDlDefaultFifo;

// The two frame buffers, the one being drawn, and the one on screen.
void* _RwGCXFB1;
void* _RwGCXFB2;
void* _RwGCXFBCopy;
void* _RwGCXFBDisp;

// The draw sync token of each frame in the ring.
U16 _RwDlFrameSwap[3];

// The threads waiting for a free frame.
OSThreadQueue _RwDlWaitingDoneRender;

// Which half a two-half frame is on, whether a retrace has passed since, whether a frame waits
// for the retrace, whether a frame waits for its token, whether the FIFO breakpoint is armed, the
// token ring's read and write positions, and the ring's write and read positions. The frame
// ring's positions and the breakpoint flag are shared between the game thread and the breakpoint
// interrupt; they, the token ring's positions and the token wait flag are volatile.
S32 DlFSAAHalf;
S32 _RwDlFrameGo;
S32 _RwDlFrameWait;
volatile S32 _RwDlFrameReadyOnToken;
volatile S32 _RwDlBreakPointEnabled;
volatile S32 _RwDlFrameTokenCurrent;
volatile S32 _RwDlFrameTokenNew;
volatile S32 _RwDlFrameNew;
volatile S32 _RwDlFrameCurrent;

// The FIFO and frame buffer allocation.
void* _RwDl_FIFO_XFB;

// Whether the frame is drawn in two halves.
S32 _RwDlFSAA;

// Which video mode is in use: 0 NTSC, 1 PAL, 2 MPAL, 0x2A the application's own.
S32 _RwGameCubeVideoMode;

// The pixel format GX is set to, and the one the frame uses.
S32 _RwDlCurPixelFormat;
S32 _RwDlPixelFormat;

// Whether the next display copy clears the frame.
S32 _RwDlCopyClear;

// The draw sync token to issue next, and the last token the GPU has passed.
extern volatile U16 _RwDlTokenCurrent;
extern volatile U16 _RwDlTokenLastSeen;

// The device's video modes: NTSC, PAL, MPAL and the application's own render mode.
RwVideoMode _RwDlVideoModes[4] = {
    {640, 480, 24, rwVIDEOMODEEXCLUSIVE},
    {640, 528, 24, rwVIDEOMODEEXCLUSIVE},
    {640, 480, 24, rwVIDEOMODEEXCLUSIVE},
    {0, 0, 0, rwVIDEOMODEEXCLUSIVE},
};

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
S32 _rwDlRasterSetFromImage(void* rasterIn, void* imageIn, S32 flags);
S32 _rwDlImageGetFromRaster(void* imageIn, void* rasterIn, S32 flags);
S32 _rwDlRasterDestroy(void* out, RwRaster* raster, S32 flags);
S32 _rwDlRasterCreate(void* out, RwRaster* raster, S32 flags);
S32 _rwDlImageFindRasterFormat(void* out, void* inOut, S32 flags);
S32 _rwDlRasterLock(void* pixelsOut, void* rasterIn, S32 flags);
S32 _rwDlRasterUnlock(void* out, void* rasterIn, S32 flags);
S32 _rwDlRasterLockPalette(void** paletteOut, RwRaster* raster, S32 flags);
S32 _rwDlRasterUnlockPalette(void* out, RwRaster* raster, S32 flags);
S32 _rwDlRasterClear(void* raster, void* unused, S32 pixelValue);
S32 _rwDlRasterClearRect(void* raster, void* rect, S32 pixelValue);
S32 _rwDlRasterRender(void* raster, void* rect, S32 flags);
S32 _rwDlRasterRenderScaled(void* raster, void* rect, S32 flags);
S32 _rwDlRasterRenderFast(void* raster, void* rect, S32 flags);
S32 _rwDlSetRasterContext(void* out, void* raster, S32 flags);
S32 _rwDlRasterSubRaster(RwRaster* raster, RwRaster* parent, S32 flags);
S32 _rwDlNativeTextureGetSize(void* sizeOut, void* textureIn, S32 unused);
S32 _rwDlNativeTextureWrite(RwStream* stream, RwTexture* texture);
S32 _rwDlNativeTextureRead(RwStream* stream, RwTexture** textureRef);
S32 _rwDlRasterGetNumMipLevels(void* out, void* inOut, S32 flags);

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

    if (_RwGCFrameQueue[_RwDlFrameCurrent].frameBuffer != _RwGCXFBDisp)
    {
        next = (_RwDlFrameCurrent + 1) % DLDEVICE_RING;

        if (next == _RwDlFrameNew)
        {
            GXDisableBreakPt();
            _RwDlBreakPointEnabled = 0;
        }
        else
        {
            GXEnableBreakPt(_RwGCFrameQueue[next].fifoPos);
        }

        _RwDlFrameCurrent = next;
        OSWakeupThread(&_RwDlWaitingDoneRender);

        if (_RwDlFSAA == 0)
        {
            _RwDlFrameReadyOnToken = 1;
        }
        else if (DlFSAAHalf == 1)
        {
            _RwDlFrameReadyOnToken = 1;
            DlFSAAHalf = 0;
        }
        else
        {
            DlFSAAHalf = 1;
        }
    }
    else
    {
        _RwDlFrameWait = 1;
    }
}

// Before each retrace: once the waiting frame's token has passed, shows the other frame buffer
// (unblanking the screen after the first frame).
void _rwDlVIPreRetraceCallback(u32 retraceCount)
{
    S16 token = _RwDlFrameSwap[_RwDlFrameTokenCurrent];

    if (_RwDlFrameReadyOnToken == 1 && _rwDlTokenQueryDone(token))
    {
        _RwGCXFBDisp = (_RwGCXFBDisp == _RwGCXFB1) ? _RwGCXFB2 : _RwGCXFB1;
        VISetNextFrameBuffer(_RwGCXFBDisp);

        if (_RwDlFirstFrame != 0)
        {
            VISetBlack(FALSE);
            _RwDlFirstFrame = 0;
        }

        VIFlush();
        _RwDlFrameTokenCurrent = (_RwDlFrameTokenCurrent + 1) % DLDEVICE_RING;
        _RwDlFrameReadyOnToken = 0;
        _RwDlFrameGo = 1;
    }
}

// After each retrace: a frame the breakpoint reached while its frame buffer was still on screen
// moves on now that the screen has swapped.
void _rwDlVIPostRetraceCallback(u32 retraceCount)
{
    U32 next;

    if (_RwDlFrameWait != 0 && _RwDlFrameGo != 0)
    {
        next = (_RwDlFrameCurrent + 1) % DLDEVICE_RING;

        if (next == _RwDlFrameNew)
        {
            GXDisableBreakPt();
            _RwDlBreakPointEnabled = 0;
        }
        else
        {
            GXEnableBreakPt(_RwGCFrameQueue[next].fifoPos);
        }

        _RwDlFrameCurrent = next;
        OSWakeupThread(&_RwDlWaitingDoneRender);

        if (_RwDlFSAA == 0)
        {
            _RwDlFrameReadyOnToken = 1;
        }
        else if (DlFSAAHalf == 1)
        {
            _RwDlFrameReadyOnToken = 1;
            DlFSAAHalf = 0;
        }
        else
        {
            DlFSAAHalf = 1;
        }

        _RwDlFrameWait = 0;
    }

    _RwDlFrameGo = 0;
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
    if (renderMode != NULL)
    {
        GXPixelFmt format;

        _RwGameCubeRenderModeObj = *renderMode;
        _RwDlRenderMode = &_RwGameCubeRenderModeObj;
        _RwGameCubeVideoMode = 0x2A;

        if (_RwDlRenderMode->aa)
        {
            format = GX_PF_RGB565_Z16;
        }
        else
        {
            format = pixelFormat;
        }

        _RwDlPixelFormat = format;
        _RwDlCurPixelFormat = format;
        _RwDlVideoModes[3].width = _RwDlRenderMode->fbWidth;
        _RwDlVideoModes[3].height = _RwDlRenderMode->efbHeight;

        switch (pixelFormat)
        {
        case GX_PF_RGB8_Z24:
        case GX_PF_RGBA6_Z24:
            _RwDlVideoModes[3].depth = 24;
            break;
        case GX_PF_RGB565_Z16:
            _RwDlVideoModes[3].depth = 16;
            break;
        default:
            DLDEVICE_ERROR(2, "Invalid pixel format");
            break;
        }
    }
    else
    {
        switch (VIGetTvFormat())
        {
        case VI_NTSC:
            _RwDlRenderMode = &GXNtsc480IntDf;
            _RwGameCubeVideoMode = 0;
            break;
        case VI_PAL:
        case VI_EURGB60:
            _RwDlRenderMode = &GXPal528IntDf;
            _RwGameCubeVideoMode = 1;
            break;
        case VI_MPAL:
            _RwDlRenderMode = &GXMpal480IntDf;
            _RwGameCubeVideoMode = 2;
            break;
        default:
            DLDEVICE_ERROR(2, "Invalid TV format");
            break;
        }
    }
}

// Points every standard slot at the null standard.
// FAKEMATCH: helper exists only to set register order in _rwDlSystem; real form believed to be an inline loop
static inline void DlFillStandards(RwStandardFunc* funcs, S32 numIn)
{
    S32 i;

    for (i = 0; i < numIn; i++)
    {
        funcs[i] = _rwDlNullStandard;
    }
}

// Clears the standard slots, then installs the driver's standards.
// FAKEMATCH: helper exists only to set register order in _rwDlSystem; real form believed to be inline loops
static inline void DlInstallStandards(RwStandardFunc* funcs, DlStandard* standards, S32 count,
                                      S32 numIn)
{
    // FAKEMATCH: the clearing loop goes through DlFillStandards to set register order; real form believed to be the loop written here
    DlFillStandards(funcs, numIn);

    while (count--)
    {
        // The leading standard gates installation of the whole table.
        if (standards[0].index < numIn && standards[0].index >= 0)
        {
            funcs[standards[count].index] = standards[count].func;
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
            switch (_RwGameCubeVideoMode)
            {
            case 0:
                *(RwVideoMode*)out = _RwDlVideoModes[0];
                break;
            case 1:
            case 5:
                *(RwVideoMode*)out = _RwDlVideoModes[1];
                break;
            case 2:
                *(RwVideoMode*)out = _RwDlVideoModes[2];
                break;
            case 0x2A:
                *(RwVideoMode*)out = _RwDlVideoModes[3];
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
        dgGGlobals.openParam = inOut;
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
            _RwDlFifoSize = (U32)params[2];
        }
        else
        {
            _rwDlRenderModeSelect(NULL, _RwDlPixelFormat);
        }

        // The bytes of one external frame buffer (two bytes a pixel); the FIFO and two of them
        // share one allocation.
        frameSize = ((_RwDlRenderMode->fbWidth + 15) & 0xFFF0) * _RwDlRenderMode->xfbHeight * 2;
        _RwDl_FIFO_XFB = RwMalloc(_RwDlFifoSize + frameSize * 2 + 0x1F);
        _RwDlDefaultFifo = (void*)(((U32)_RwDl_FIFO_XFB + 0x1F) & ~0x1F);
        DCInvalidateRange(_RwDlDefaultFifo, _RwDlFifoSize);
        _RwGCXFB1 = (U8*)_RwDlDefaultFifo + _RwDlFifoSize;
        _RwGCXFB2 = (U8*)_RwGCXFB1 + frameSize;
        _RwGCXFBDisp = _RwGCXFB1;
        _RwGCXFBCopy = _RwGCXFB2;
        _RwDlDefaultFifoObj = GXInit(_RwDlDefaultFifo, _RwDlFifoSize);

        while ((U16)GXReadDrawSync() != _RwDlTokenLastSeen)
        {
            GXSetDrawSync(_RwDlTokenLastSeen);
        }

        return TRUE;
    }
    case 1:
        RwFree(_RwDl_FIFO_XFB);
        _RwGCXFB2 = NULL;
        _RwGCXFB1 = NULL;
        _RwDlDefaultFifo = NULL;
        _RwDl_FIFO_XFB = NULL;
        return TRUE;
    case 2:
    {
        S32 pixelFormat = _RwDlPixelFormat;
        GXRenderModeObj* renderMode;

        VIConfigure(renderMode = _RwDlRenderMode);
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
            _RwDlHalfHeight = renderMode->xfbHeight / 2;
            _RwDlFSAA = 1;
            _RwDlFSAATop = 1;
        }
        else
        {
            GXSetDispCopyYScale((F32)renderMode->xfbHeight / (F32)renderMode->efbHeight);
            _RwDlFSAA = 0;
        }

        GXSetFieldMode(renderMode->field_rendering,
                       renderMode->xfbHeight < renderMode->viHeight);
        GXSetPixelFmt(pixelFormat, GX_ZC_LINEAR);
        _RwDlFirstFrame = 1;
        VIWaitForRetrace();

        if (_RwDlRenderMode->viTVmode & 1)
        {
            VIWaitForRetrace();
        }

        GXSetDispCopyGamma(GX_GM_1_0);
        VISetPreRetraceCallback(_rwDlVIPreRetraceCallback);
        VISetPostRetraceCallback(_rwDlVIPostRetraceCallback);
        GXSetBreakPtCallback(_rwDlBreakPtCallback);
        OSInitThreadQueue(&_RwDlWaitingDoneRender);
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
            {7, _rwDlRasterSetFromImage},
            {6, _rwDlImageGetFromRaster},
            {5, (RwStandardFunc)_rwDlRasterDestroy},
            {4, (RwStandardFunc)_rwDlRasterCreate},
            {9, _rwDlImageFindRasterFormat},
            {8, (RwStandardFunc)_rwDlTextureSetRaster},
            {15, _rwDlRasterLock},
            {16, _rwDlRasterUnlock},
            {23, (RwStandardFunc)_rwDlRasterLockPalette},
            {24, (RwStandardFunc)_rwDlRasterUnlockPalette},
            {14, _rwDlRasterClear},
            {13, _rwDlRasterClearRect},
            {17, _rwDlRasterRender},
            {18, _rwDlRasterRenderScaled},
            {19, _rwDlRasterRenderFast},
            {11, _rwDlSetRasterContext},
            {12, (RwStandardFunc)_rwDlRasterSubRaster},
            {25, (RwStandardFunc)_rwDlNativeTextureGetSize},
            {27, (RwStandardFunc)_rwDlNativeTextureWrite},
            {26, (RwStandardFunc)_rwDlNativeTextureRead},
            {28, _rwDlRasterGetNumMipLevels},
        };
        // FAKEMATCH: DlInstallStandards wraps the table loops to set register order; real form believed to be both loops written inline here
        DlInstallStandards(out, standards, sizeof(standards) / sizeof(standards[0]), numIn);

        return TRUE;
    }
    case 19:
        *(S32*)out = 0x400;
        return TRUE;
    }

    return FALSE;
}

S32 _rwDlIm2DRenderLine(RwIm2DVertex* vertices, S32 numVertices, S32 vert1, S32 vert2);
S32 _rwDlIm2DRenderTriangle(RwIm2DVertex* vertices, S32 numVertices, S32 vert1, S32 vert2,
                            S32 vert3);
S32 _rwDlIm2DRenderPrimitive(RwPrimitiveType primType, RwIm2DVertex* vertices, S32 numVertices);
S32 _rwDlIm2DRenderIndexedPrimitive(RwPrimitiveType primType, RwIm2DVertex* vertices,
                                    S32 numVertices, RwImVertexIndex* indices, S32 numIndices);

// Clears the camera: by drawing a quad over its frame buffer's area (twice the height when the
// frame is drawn in halves), or, when the next display copy clears the frame, by setting that
// copy's clear color (and the far depth) if the image is cleared.
S32 _rwDlCameraClear(void* camera, void* color, S32 flags)
{
    RwRect rect;
    RwRaster* raster;

    if (_RwDlCopyClear == 0)
    {
        raster = ((RwCamera*)camera)->frameBuffer;

        if (_RwDlFSAA == 0)
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
    // The GX projection the camera sets.
    static F32 projVector[7] = {0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};

    RwCamera* camera = object;
    RwMatrix offset;
    RwRaster* raster;

    GXSetCurrentGXThread();
    dgGGlobals.camera = camera;

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

    _RwDlInvCamLTM.flags = 0x20003;
    RwMatrixInvert(&_RwDlInvCamLTM, RwFrameGetLTM(RwCameraGetFrame(camera)));
    RwMatrixTransform(&_RwDlInvCamLTM, &offset, rwCOMBINEPOSTCONCAT);

    projVector[1] = camera->recipViewWindow.x;
    projVector[3] = camera->recipViewWindow.y;

    if (camera->projectionType == rwPARALLEL)
    {
        projVector[0] = 1.0f;
        projVector[5] = -1.0f / (camera->farPlane - camera->nearPlane);
        projVector[6] = camera->farPlane * projVector[5];
    }
    else
    {
        projVector[0] = 0.0f;
        projVector[5] = -camera->nearPlane / (camera->farPlane - camera->nearPlane);
        projVector[6] = camera->farPlane * projVector[5];
    }

    GXSetProjectionv(projVector);
    raster = camera->frameBuffer;

    if (raster->cType & (rwRASTERTYPEZBUFFER | rwRASTERTYPETEXTURE))
    {
        if (RWGCRASTEREXT(raster)->flags & RWGCRASTER_ALPHA)
        {
            if (_RwDlCurPixelFormat != GX_PF_RGBA6_Z24)
            {
                GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
                _RwDlCurPixelFormat = GX_PF_RGBA6_Z24;
            }
        }
        else if (_RwDlCurPixelFormat != GX_PF_RGB8_Z24)
        {
            GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
            _RwDlCurPixelFormat = GX_PF_RGB8_Z24;
        }

        GXSetViewport(raster->nOffsetX, raster->nOffsetY, raster->width, raster->height, 0.0f,
                      1.0f);
        GXSetScissor(raster->nOffsetX, raster->nOffsetY, raster->width, raster->height);
        GXSetScissorBoxOffset(0, 0);
    }
    else
    {
        if (_RwDlCurPixelFormat != _RwDlPixelFormat)
        {
            GXSetPixelFmt(_RwDlPixelFormat, GX_ZC_LINEAR);
            _RwDlCurPixelFormat = _RwDlPixelFormat;
        }

        if (_RwDlFSAA == 0)
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

            if (_RwDlFSAATop != 0)
            {
                if ((raster->nOffsetY + raster->height) * 2 <= _RwDlHalfHeight + 2)
                {
                    GXSetScissor(raster->nOffsetX, raster->nOffsetY * 2, _RwDlRenderMode->fbWidth,
                                 raster->height * 2);
                }
                else if (raster->nOffsetY * 2 > _RwDlHalfHeight + 2)
                {
                    GXSetScissor(0, 0, _RwDlRenderMode->fbWidth, _RwDlHalfHeight + 2);
                }
                else
                {
                    GXSetScissor(raster->nOffsetX, raster->nOffsetY * 2, _RwDlRenderMode->fbWidth,
                                 _RwDlHalfHeight + 2);
                }

                GXSetScissorBoxOffset(0, 0);
            }
            else
            {
                if (raster->nOffsetY * 2 >= _RwDlHalfHeight - 2)
                {
                    GXSetScissor(raster->nOffsetX, raster->nOffsetY * 2, _RwDlRenderMode->fbWidth,
                                 raster->height * 2);
                }
                else if ((raster->nOffsetY + raster->height) * 2 < _RwDlHalfHeight + 2)
                {
                    GXSetScissor(0, _RwDlHalfHeight - 2, _RwDlRenderMode->fbWidth,
                                 _RwDlHalfHeight + 2);
                }
                else
                {
                    GXSetScissor(raster->nOffsetX, _RwDlHalfHeight - 2, _RwDlRenderMode->fbWidth,
                                 raster->height * 2);
                }

                GXSetScissorBoxOffset(0, _RwDlHalfHeight - 2);
            }
        }
    }

    return TRUE;
}

S32 _rwDlCameraEndUpdate(void* out, void* camera, S32 flags)
{
    dgGGlobals.camera = NULL;

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
    volatile S32 full;
    U32 stride;

    enabled = OSDisableInterrupts();
    full = (_RwDlFrameNew - _RwDlFrameCurrent == -1 || _RwDlFrameNew - _RwDlFrameCurrent == 2) ? 1 : 0;
    OSRestoreInterrupts(enabled);

    if (full == 1)
    {
        OSSleepThread(&_RwDlWaitingDoneRender);
    }

    if (_RwDlFSAA == 0)
    {
        GXFlush();
        GXGetFifoPtrs(GXGetCPUFifo(), &readPtr, &writePtr);
        enabled = OSDisableInterrupts();
        _RwGCFrameQueue[_RwDlFrameNew].fifoPos = writePtr;
        _RwGCFrameQueue[_RwDlFrameNew].frameBuffer = _RwGCXFBCopy;
        _RwDlFrameSwap[_RwDlFrameTokenNew] = _RwDlTokenCurrent;
        _RwDlFrameNew = (_RwDlFrameNew + 1) % DLDEVICE_RING;
        _RwDlFrameTokenNew = (_RwDlFrameTokenNew + 1) % DLDEVICE_RING;
        OSRestoreInterrupts(enabled);

        if (_RwDlBreakPointEnabled == 0)
        {
            _RwDlBreakPointEnabled = 1;
            GXEnableBreakPt(writePtr);
        }

        GXCopyDisp(_RwGCXFBCopy, _RwDlCopyClear);
        GXSetDrawSync(_RwDlTokenCurrent);
        _RwDlTokenCurrent = DLDEVICE_NEXTTOKEN(_RwDlTokenCurrent);
        GXFlush();

        _RwGCXFBCopy = (_RwGCXFBCopy == _RwGCXFB1) ? _RwGCXFB2 : _RwGCXFB1;
    }
    else if (_RwDlFSAATop != 0)
    {
        GXFlush();
        GXGetFifoPtrs(GXGetCPUFifo(), &readPtr, &writePtr);
        enabled = OSDisableInterrupts();
        _RwGCFrameQueue[_RwDlFrameNew].fifoPos = writePtr;
        _RwGCFrameQueue[_RwDlFrameNew].frameBuffer = _RwGCXFBCopy;
        _RwDlFrameNew = (_RwDlFrameNew + 1) % DLDEVICE_RING;
        OSRestoreInterrupts(enabled);

        if (_RwDlBreakPointEnabled == 0)
        {
            _RwDlBreakPointEnabled = 1;
            GXEnableBreakPt(writePtr);
        }

        GXCopyDisp(_RwGCXFBCopy, _RwDlCopyClear);
        GXSetDrawSync(_RwDlTokenCurrent);
        _RwDlTokenCurrent = DLDEVICE_NEXTTOKEN(_RwDlTokenCurrent);
        GXFlush();
        _RwDlFSAATop = 0;
    }
    else
    {
        GXFlush();
        GXGetFifoPtrs(GXGetCPUFifo(), &readPtr, &writePtr);
        enabled = OSDisableInterrupts();
        _RwGCFrameQueue[_RwDlFrameNew].fifoPos = writePtr;
        _RwGCFrameQueue[_RwDlFrameNew].frameBuffer = _RwGCXFBCopy;
        _RwDlFrameSwap[_RwDlFrameTokenNew] = _RwDlTokenCurrent;
        _RwDlFrameNew = (_RwDlFrameNew + 1) % DLDEVICE_RING;
        _RwDlFrameTokenNew = (_RwDlFrameTokenNew + 1) % DLDEVICE_RING;
        OSRestoreInterrupts(enabled);

        if (_RwDlBreakPointEnabled == 0)
        {
            _RwDlBreakPointEnabled = 1;
            GXEnableBreakPt(writePtr);
        }

        GXSetCopyClamp(GX_CLAMP_BOTTOM);
        GXSetDispCopySrc(0, 2, _RwDlRenderMode->fbWidth, _RwDlRenderMode->efbHeight - 2);
        stride = (_RwDlRenderMode->fbWidth + 15) & 0xFFF0;
        GXCopyDisp((U8*)_RwGCXFBCopy + stride * (_RwDlRenderMode->efbHeight - 2) * 2,
                   _RwDlCopyClear);
        GXSetDrawSync(_RwDlTokenCurrent);
        _RwDlTokenCurrent = DLDEVICE_NEXTTOKEN(_RwDlTokenCurrent);
        GXSetCopyClamp(GX_CLAMP_TOP | GX_CLAMP_BOTTOM);
        GXSetDispCopySrc(0, 0, _RwDlRenderMode->fbWidth, _RwDlRenderMode->efbHeight);
        GXFlush();

        _RwGCXFBCopy = (_RwGCXFBCopy == _RwGCXFB1) ? _RwGCXFB2 : _RwGCXFB1;
        _RwDlFSAATop = 1;
    }

    return TRUE;
}

// Returns the device this driver registers: gamma 1, the system function, the depth buffer's near
// and far values, the render state and Im2D functions.
RwDevice* _rwDeviceGetHandle(void)
{
    static RwDevice rwDlDriverDevice = {
        1.0f,
        _rwDlSystem,
        0.0f,
        0.99999994f,
        _rwDlSetRenderState,
        _rwDlGetRenderState,
        _rwDlIm2DRenderLine,
        _rwDlIm2DRenderTriangle,
        _rwDlIm2DRenderPrimitive,
        _rwDlIm2DRenderIndexedPrimitive,
    };

    return &rwDlDriverDevice;
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
        RwMatrixMultiply(&combined, matrix, &_RwDlInvCamLTM);
        view = &combined;
    }
    else
    {
        view = &_RwDlInvCamLTM;
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
void RwGameCubeCameraTextureFlush(RwRaster* raster)
{
    RwGCRasterExt* ext = RWGCRASTEREXT(raster);

    GXSetCopyFilter(GX_FALSE, NULL, GX_FALSE, NULL);
    GXSetTexCopySrc(0, 0, raster->width, raster->height);
    GXSetTexCopyDst(raster->width, raster->height, ext->format, GX_FALSE);
    GXCopyTex(ext->pixels, _RwDlCopyClear);
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
