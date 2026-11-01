#ifndef _DOLPHIN_GXVERT
#define _DOLPHIN_GXVERT

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GXFIFO_ADDR 0xCC008000

typedef union {
  u8 u8;
  u16 u16;
  u32 u32;
  u64 u64;
  s8 s8;
  s16 s16;
  s32 s32;
  s64 s64;
  f32 f32;
  f64 f64;
} PPCWGPipe;

volatile PPCWGPipe GXWGFifo AT_ADDRESS(GXFIFO_ADDR);

static inline void GXPosition3s16(const s16 x, const s16 y, const s16 z)
{
    GXWGFifo.s16 = x;
    GXWGFifo.s16 = y;
    GXWGFifo.s16 = z;
}

static inline void GXPosition3f32(const f32 x, const f32 y, const f32 z)
{
    GXWGFifo.f32 = x;
    GXWGFifo.f32 = y;
    GXWGFifo.f32 = z;
}

static inline void GXColor4u8(const u8 r, const u8 g, const u8 b, const u8 a)
{
    GXWGFifo.u8 = r;
    GXWGFifo.u8 = g;
    GXWGFifo.u8 = b;
    GXWGFifo.u8 = a;
}

static inline void GXTexCoord2f32(const f32 s, const f32 t)
{
    GXWGFifo.f32 = s;
    GXWGFifo.f32 = t;
}

static inline void GXEnd(void)
{
}

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_GXVERT
