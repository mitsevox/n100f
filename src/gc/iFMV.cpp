#include "iFile.h"
#include "iPad.h"
#include "xPad.h"
#include "xMath.h"
#include "xSnd.h"
#include "zGlobals.h"

#include <bink.h>
#include <rad3d.h>
#include <rwcore.h>

#include <dolphin/ar.h>
#include <dolphin/dvd.h>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>

#include <string.h>

#define FMV_WORK_AREA_SIZE 0x3E0000
#define FMV_WORK_BUFFER_SIZE 0x12C400

extern "C" {
extern GXRenderModeObj* _RwDlRenderMode;

void fn_80014A34(S32);
void fn_800167F0();
void fn_80016600();
S32 fn_800443F8();
void fn_800443AC();
S32 fn_8004436C();
void fn_80021CB0(F32, F32, F32, F32);
}

static U32 Bink_surface_type[5];

static RwResEntry* repEntry;
static U32 Letterbox;
static S32 frame_num;
static Bink* Movie;
static HRAD3DIMAGE Image;
static F32 X_offset;
static F32 Y_offset;
static void* pixels;
static U8* Work_area;
static U8* Work_area_aligned;
static U32 Work_area_used;
U8* FMVWorkBuffer;
static volatile F32 vol;
S32 ip;
U32 oof;
static char* Bink_error;

static F32 Width_scale = 1.0f;
static F32 Height_scale = 1.0f;

static void PlayFMV(char* fname, U32 buttons, F32 time);

U32 iFMVPlay(char* filename, U32 buttons, F32 time)
{
    if (filename == NULL)
    {
        return 1;
    }

    Letterbox = strncmp(filename, "FMV/cin", 7) == 0;
    frame_num = 0;

    while (frame_num >= 0)
    {
        PlayFMV(filename, buttons, time);
    }

    return 0;
}

static void DrawFrame(F32 x, F32 y, F32 x_scale, F32 y_scale)
{
    GXRenderModeObj* rm = _RwDlRenderMode;
    Mtx idt;
    Mtx44 mtx;

    GXSetViewport(0.0f, 0.0f, rm->fbWidth, rm->efbHeight, 0.0f, 1.0f);
    GXSetScissor(0, 0, rm->fbWidth, rm->efbHeight);
    GXSetDispCopySrc(0, 0, rm->fbWidth, rm->efbHeight);
    GXSetDispCopyDst(rm->fbWidth, rm->xfbHeight);
    GXSetDispCopyYScale((F32)rm->xfbHeight / rm->efbHeight);
    GXSetCopyFilter(rm->aa, rm->sample_pattern, GX_TRUE, rm->vfilter);

    if (rm->aa)
    {
        GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
    }
    else
    {
        GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    }

    GXSetDispCopyGamma(GX_GM_1_0);
    C_MTXOrtho(mtx, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, 10000.0f);
    GXSetProjection(mtx, GX_ORTHOGRAPHIC);
    PSMTXIdentity(idt);
    GXLoadPosMtxImm(idt, 0);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    GXSetNumTexGens(1);
    GXSetNumChans(1);
    GXSetNumTevStages(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);

    if (rm->field_rendering)
    {
        GXSetViewportJitter(0.0f, 0.0f, rm->fbWidth, rm->efbHeight, 0.0f, 1.0f, VIGetNextField());
    }
    else
    {
        GXSetViewport(0.0f, 0.0f, rm->fbWidth, rm->efbHeight, 0.0f, 1.0f);
    }

    GXInvalidateVtxCache();
    GXInvalidateTexAll();
    Blit_RAD_3D_image(Image, x, y, x_scale, y_scale, 1.0f);
}

static void Show_frame()
{
    RwRGBA color = { 0 };
    RwCamera* cam = globals.camera.lo_cam;
    F32 x;
    F32 y;

    RwCameraClear(cam, &color, rwCAMERACLEARIMAGE);
    RwCameraBeginUpdate(cam);

    x = Movie->width * (X_offset * Width_scale) + (640.0f - Width_scale * Movie->width) / 2;
    y = Movie->height * (Y_offset * Height_scale) + (480.0f - Height_scale * Movie->height) / 2;

    fn_80021CB0(0.0f, 0.0f, 0.0f, 0.0f);
    RwResourcesUseResEntry(repEntry);
    DrawFrame(x, y, Width_scale, Height_scale);

    if (Letterbox)
    {
        RwImVertexIndex indices[4] = { 0, 1, 2, 3 };
        RwIm2DVertex verts[4];

        RwIm2DVertexSetScreenX(&verts[0], 0.0f);
        RwIm2DVertexSetScreenY(&verts[0], 0.0f);
        RwIm2DVertexSetScreenX(&verts[1], 640.0f);
        RwIm2DVertexSetScreenY(&verts[1], 0.0f);
        RwIm2DVertexSetScreenX(&verts[2], 0.0f);
        RwIm2DVertexSetScreenY(&verts[2], 48.0f);
        RwIm2DVertexSetScreenX(&verts[3], 640.0f);
        RwIm2DVertexSetScreenY(&verts[3], 48.0f);
        RwIm2DVertexSetIntRGBA(&verts[0], 0, 0, 0, 255);
        RwIm2DVertexSetIntRGBA(&verts[1], 0, 0, 0, 255);
        RwIm2DVertexSetIntRGBA(&verts[2], 0, 0, 0, 255);
        RwIm2DVertexSetIntRGBA(&verts[3], 0, 0, 0, 255);
        RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRISTRIP, verts, 4, indices, 4);

        RwIm2DVertexSetScreenX(&verts[0], 0.0f);
        RwIm2DVertexSetScreenY(&verts[0], 432.0f);
        RwIm2DVertexSetScreenX(&verts[1], 640.0f);
        RwIm2DVertexSetScreenY(&verts[1], 432.0f);
        RwIm2DVertexSetScreenX(&verts[2], 0.0f);
        RwIm2DVertexSetScreenY(&verts[2], 480.0f);
        RwIm2DVertexSetScreenX(&verts[3], 640.0f);
        RwIm2DVertexSetScreenY(&verts[3], 480.0f);
        RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRISTRIP, verts, 4, indices, 4);
    }

    fn_8004436C();
    RwCameraEndUpdate(cam);
    RwCameraShowRaster(cam, NULL, 0);
}

static void* iFMVmalloc(U32 size)
{
    U8* mem;

    size = (size + 31) & ~31;
    mem = Work_area_aligned;
    if (Work_area_used + size > FMV_WORK_AREA_SIZE)
    {
        return NULL;
    }

    mem += Work_area_used;
    Work_area_used += size;
    return mem;
}

static void iFMVfree(void* mem)
{
}

static void* arammalloc(U32 size)
{
    return (void*)ARAlloc(size);
}

static void aramfree(void* mem)
{
    u32 length;
    ARFree(&length);
}

static RwResEntry* Rep_entry_destroy(RwResEntry* entry)
{
    return NULL;
}

static void Setup_surface_array()
{
    Bink_surface_type[0] = BINKSURFACE32;
    Bink_surface_type[1] = BINKSURFACE32A;
    Bink_surface_type[2] = BINKSURFACE565;
    Bink_surface_type[3] = BINKSURFACE4444;
    Bink_surface_type[4] = BINKSURFACEYUY2;
}

static void Decompress_frame(struct Bink* bnk, HRAD3DIMAGE rad_image)
{
    U32 pitch;
    U32 type;

    BinkDoFrame(bnk);
    if (Lock_RAD_3D_image(rad_image, &pixels, &pitch, &type))
    {
        BinkCopyToBuffer(bnk, pixels, pitch, bnk->height, 0, 0, Bink_surface_type[type]);
        Unlock_RAD_3D_image(rad_image);
    }
}

static void PlayFMV(char* fname, U32 buttons, F32 time)
{
    RwCamera* cam;
    RwResEntry* entry;
    U8* buffer;
    S32 i;
    U32 err;
    F32 t;
    tag_xFile file;

    cam = globals.camera.lo_cam;
    i = 3;
    while (i-- > 0)
    {
        RwCameraBeginUpdate(cam);
        RwCameraEndUpdate(cam);
        RwCameraShowRaster(cam, NULL, 1);
    }

    fn_80014A34(0);
    iPadStopRumble(globals.pad0);
    fn_800167F0();

    entry = RwResourcesAllocateResEntry(NULL, NULL, FMV_WORK_AREA_SIZE,
                                        (RwResEntryDestroyNotify)Rep_entry_destroy);
    if (entry == NULL)
    {
        OSReport("### Error allocating Bink work buffer\n");
        return;
    }

    repEntry = entry;
    OSReport("RW resource arena size:  %d bytes\n", RwResourcesGetArenaSize());
    OSReport("RW rep entry:        %08X-%08X\n", entry + 1,
             (U8*)(entry + 1) + FMV_WORK_AREA_SIZE - 1);

    Work_area = (U8*)(entry + 1);
    Work_area_aligned = (U8*)(((U32)Work_area + 31) & ~31);
    buffer = (U8*)iFMVmalloc(FMV_WORK_BUFFER_SIZE);
    OSReport("RW rep entry round:  %08X-%08X\n", buffer, buffer + FMV_WORK_BUFFER_SIZE - 1);
    FMVWorkBuffer = buffer;
    OSReport("### Allocated %d bytes at %08X for Bink work buffer\n", FMV_WORK_BUFFER_SIZE,
             buffer);
    OSReport("FMV work area:  %08X - %08X\n", FMVWorkBuffer,
             FMVWorkBuffer + FMV_WORK_BUFFER_SIZE - 1);

    RADSetAudioMemory(arammalloc, aramfree);
    RADSetMemory(iFMVmalloc, iFMVfree);
    Setup_surface_array();

    do
    {
        Movie = BinkOpen(fname, 0);
        if (Movie == NULL)
        {
            iFileOpen(fname, 0, &file);
            DVDSeekAsyncPrio(&file.ps.fileInfo, 0, NULL, 2);
            fn_80016600();
            DVDCancelAsync(&file.ps.fileInfo.cb, NULL);
        }

        err = fn_800443F8();
        if (err)
        {
            fn_800443AC();
        }
        oof = err;
        if (Movie != NULL)
        {
            break;
        }
    } while (oof);

    if (Movie != NULL)
    {
        if (Movie->num_tracks != 0)
        {
            for (ip = 0; ip <= Movie->num_tracks; ip++)
            {
                vol = xabs(MAX(gSnd.mastervol.volL, gSnd.mastervol.volR)) *
                      xabs(MAX(gSnd.streamvol.volL, gSnd.streamvol.volR));
                vol *= vol;
                vol *= 32768.0f;
                BinkSetVolume(Movie, ip, vol);
            }
        }

        Image = Open_RAD_3D_image(0, Movie->width, Movie->height, 0);
        if (Image != NULL)
        {
            if (frame_num != 0)
            {
                BinkGoto(Movie, frame_num, 0);
            }

            oof = 0;
            do
            {
                fn_80016600();
                err = fn_800443F8();
                if (err)
                {
                    fn_800443AC();
                }
                if (err)
                {
                    goto superbreak;
                }

                if (BinkWait(Movie) == 0)
                {
                    Decompress_frame(Movie, Image);
                    Show_frame();
                    BinkNextFrame(Movie);
                }

                xPadUpdate(0, 0.0f);

                t = (F32)Movie->frame_num / (Movie->frame_rate / Movie->frame_rate_div);
                if (buttons && t >= time && (mPad[0].pressed & buttons))
                {
                    frame_num = -1;
                    goto superbreak;
                }
            } while (Movie->frame_num != Movie->frames - 1);

            frame_num = -1;
        }

    superbreak:
        if (frame_num != -1)
        {
            frame_num = Movie->frame_num;
        }

        Close_RAD_3D_image(Image);
        Image = NULL;
        BinkClose(Movie);
        Movie = NULL;
    }
    else
    {
        Bink_error = BinkGetError();
    }

    cam = globals.camera.lo_cam;
    i = 3;
    while (i-- > 0)
    {
        RwCameraBeginUpdate(cam);
        RwCameraEndUpdate(cam);
        RwCameraShowRaster(cam, NULL, 1);
    }

    RwResourcesFreeResEntry(entry);
    Work_area = NULL;
    Work_area_aligned = NULL;
    Work_area_used = 0;
}
