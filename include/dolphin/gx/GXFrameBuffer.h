#ifndef _DOLPHIN_GXFRAMEBUFFER
#define _DOLPHIN_GXFRAMEBUFFER

#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

extern GXRenderModeObj GXNtsc480IntDf;
extern GXRenderModeObj GXMpal480IntDf;
extern GXRenderModeObj GXPal528IntDf;
extern GXRenderModeObj GXEurgb60Hz480IntDf;

void GXSetCopyClear(GXColor clear_clr, u32 clear_z);
void GXCopyDisp(void* dest, GXBool clear);
void GXSetDispCopyGamma(GXGamma gamma);
void GXSetDispCopySrc(u16 left, u16 top, u16 wd, u16 ht);
void GXSetDispCopyDst(u16 wd, u16 ht);
u32 GXSetDispCopyYScale(f32 vscale);
void GXSetCopyFilter(GXBool aa, const u8 sample_pattern[12][2], GXBool vf, const u8 vfilter[7]);
void GXSetPixelFmt(GXPixelFmt pix_fmt, GXZFmt16 z_fmt);
void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht);
void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap);
void GXCopyTex(void* dest, GXBool clear);

void GXSetCopyClamp(GXFBClamp clamp);
void GXSetDispCopyFrame2Field(GXCopyMode mode);
void GXClearBoundingBox(void);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_GXFRAMEBUFFER
