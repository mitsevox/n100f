#ifndef BINK_YUV_H
#define BINK_YUV_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void YUV_init(U32 type);
void YUV_blit_32bpp(void* dest, U32 destx, U32 desty, S32 pitch, U8* src, U32 srcx, U32 srcy,
                    U32 srcw, U32 srch, U32 width, U32 height, U32 flags);
void YUV_blit_32abpp(void* dest, U32 destx, U32 desty, S32 pitch, U8* src, U32 srcx, U32 srcy,
                     U32 srcw, U32 srch, U32 width, U32 height, U8* alpha, U32 flags);
void YUV_blit_16bpp(void* dest, U32 destx, U32 desty, S32 pitch, U8* src, U32 srcx, U32 srcy,
                    U32 srcw, U32 srch, U32 width, U32 height, U32 flags);
void YUV_blit_16a4bpp(void* dest, U32 destx, U32 desty, S32 pitch, U8* src, U32 srcx, U32 srcy,
                      U32 srcw, U32 srch, U32 width, U32 height, U8* alpha, U32 flags);
void YUV_blit_YUY2(void* dest, U32 destx, U32 desty, S32 pitch, U8* src, U32 srcx, U32 srcy,
                   U32 srcw, U32 srch, U32 width, U32 height, U32 flags);
void YUV_blit_32bpp_mask(void* dest, U32 destx, U32 desty, S32 pitch, U8* mask, U32 mpitch,
                         U8* src, U32 srcx, U32 srcy, U32 srcw, U32 srch, U32 width, U32 height,
                         U32 flags);
void YUV_blit_32abpp_mask(void* dest, U32 destx, U32 desty, S32 pitch, U8* mask, U32 mpitch,
                          U8* src, U32 srcx, U32 srcy, U32 srcw, U32 srch, U32 width, U32 height,
                          U8* alpha, U32 flags);
void YUV_blit_16bpp_mask(void* dest, U32 destx, U32 desty, S32 pitch, U8* mask, U32 mpitch,
                         U8* src, U32 srcx, U32 srcy, U32 srcw, U32 srch, U32 width, U32 height,
                         U32 flags);
void YUV_blit_16a4bpp_mask(void* dest, U32 destx, U32 desty, S32 pitch, U8* mask, U32 mpitch,
                           U8* src, U32 srcx, U32 srcy, U32 srcw, U32 srch, U32 width,
                           U32 height, U8* alpha, U32 flags);
void YUV_blit_YUY2_mask(void* dest, U32 destx, U32 desty, S32 pitch, U8* mask, U32 mpitch,
                        U8* src, U32 srcx, U32 srcy, U32 srcw, U32 srch, U32 width, U32 height,
                        U32 flags);

#ifdef __cplusplus
}
#endif

#endif
