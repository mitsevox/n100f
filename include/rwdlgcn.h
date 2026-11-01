#ifndef RWDLGCN_H
#define RWDLGCN_H

#include <rwcore.h>
#include <dolphin/gx.h>

// The GameCube device driver's private data (src/rwsdk/driver/gcn/). Type, member and macro names
// are written for this project from the target's accesses.

// What the driver keeps after each raster (the raster plugin registered in
// _rwDlRasterPluginAttach, 0x34 bytes): the palette's GX lookup table object, the GX texel and
// palette formats, flags, the allocation and the 32-byte aligned texels inside it, the palette
// (after the texels), the buffers a lock hands out, the highest mipmap level (0xFF when it follows
// the raster's size) and the level that is locked. region is the texture memory a preloaded
// texture sits in; syncToken the draw sync token of the last frame that used the raster.
typedef struct RwGCRasterExt RwGCRasterExt;
struct RwGCRasterExt
{
    GXTlutObj tlut;
    U32 format;
    S32 tlutFormat;
    U32 flags;
    void* unaligned;
    void* pixels;
    void* palette;
    void* lockPixels;
    void* lockBuffer;
    GXTexRegion* region;
    U16 syncToken;
    U8 maxLevel;
    U8 lockedLevel;
};

// RwGCRasterExt flags: the texels carry alpha.
#define RWGCRASTER_ALPHA 0x01

// What the driver keeps after each texture (0x24 bytes): its GX texture object and a word of
// flags.
typedef struct RwGCTextureExt RwGCTextureExt;
struct RwGCTextureExt
{
    GXTexObj texObj;
    U32 flags;
};

// RwGCTextureExt flags: the texture object still has the default LOD settings; the texture and
// its palette are preloaded into texture memory. The low 16 bits keep the filter and addressing
// modes the texture object was built with.
#define RWGCTEXTURE_DEFAULTLOD 0x01000000
#define RWGCTEXTURE_PRELOADED 0x02000000

// The render states this driver keeps (0x40 bytes). The type name is the one src/flat/iEnv.cpp and
// iShadow.cpp give their partial view of it; the member names are written for this project.
typedef struct RwGcnRenderStateCache RwGcnRenderStateCache;
struct RwGcnRenderStateCache
{
    S32 zWriteEnable;
    S32 zTestEnable;
    S32 zFunc;
    S32 unkC;
    S32 cullMode;
    S32 fogEnable;
    S32 fogType;
    U32 fogColor;
    GXColor gxFogColor;
    F32 fogEnd;
    S32 srcBlend;
    S32 destBlend;
    S32 gxSrcBlend;
    S32 gxDestBlend;
    S32 alphaTest;
    U8 alphaRef0;
    U8 alphaRef1;
};

#define RWGCRASTEREXT(_raster) ((RwGCRasterExt*)((U8*)(_raster) + lbl_8025F870))
#define RWGCTEXTUREEXT(_texture) ((RwGCTextureExt*)((U8*)(_texture) + lbl_8025F888))

#ifdef __cplusplus
extern "C" {
#endif

// The raster and texture plugin offsets.
extern S32 lbl_8025F870;
extern S32 lbl_8025F888;

// The render mode the device was opened with (dldevice.c).
extern GXRenderModeObj* _RwDlRenderMode;

// The render states the driver last set (dlrendst.c).
extern RwGcnRenderStateCache lbl_8025E348;

S32 _rwDlFindMSB(S32 num);
S32 fn_80179064(RwRaster* raster, U8 level);
S32 fn_80179C7C(RwRaster* raster);
S32 fn_80179D54(RwRaster* raster, S32 format, S32 tlutFormat, S32 flags, U8 depth, S32 numLevels);
F32 fn_8017CF40(const GXTexObj* texObj);
S32 _rwDlTextureSetRaster(RwTexture* texture, RwRaster* raster, S32 flags);
S32 _rwDlGetRenderState(RwRenderState state, void* param);
S32 _rwDlSetRenderState(RwRenderState state, void* param);
S32 _rwDlRenderStateFogEnable(S32 enable);
void fn_8017ABB4(void);
void RwGameCubeSetAlphaCompare(S32 mode);
void RwGameCubeTextureSetLOD(RwTexture* texture, S32 biasClamp, S32 edgeLOD,
                             GXAnisotropy maxAniso, F32 lodBias);

#ifdef __cplusplus
}
#endif

#endif
