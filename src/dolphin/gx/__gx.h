#ifndef _DOLPHIN_GX_INTERNAL_H_
#define _DOLPHIN_GX_INTERNAL_H_

#include <dolphin/gx.h>
#include <dolphin/os.h>

#define GX_WRITE_U8(ub)     \
    GXWGFifo.u8 = (u8)(ub)

#define GX_WRITE_U16(us)   \
   GXWGFifo.u16 = (u16)(us)

#define GX_WRITE_U32(ui)   \
   GXWGFifo.u32 = (u32)(ui)

#define GX_WRITE_F32(f)     \
   GXWGFifo.f32 = (f32)(f)

#define VERIF_XF_REG(addr, value) ((void)0)
#define VERIF_RAS_REG(value) ((void)0)

#define GX_WRITE_XF_REG(addr, value) \
do { \
    GX_WRITE_U8(0x10); \
    GX_WRITE_U32(0x1000 + (addr)); \
    GX_WRITE_U32(value); \
    VERIF_XF_REG(addr, value); \
} while (0)

#define GX_WRITE_XF_REG_2(addr, value) \
do { \
    GX_WRITE_U32(value); \
} while (0)
#define GX_WRITE_XF_REG_F(addr, value) \
do { \
    GX_WRITE_F32(value); \
} while (0)

#define GX_WRITE_RAS_REG(value) \
do { \
    GX_WRITE_U8(0x61); \
    GX_WRITE_U32(value); \
    VERIF_RAS_REG(value); \
} while (0)

#define GX_WRITE_SOME_REG2(a, b, c, addr) \
do { \
    long regAddr; \
    GX_WRITE_U8(a); \
    GX_WRITE_U8(b); \
    GX_WRITE_U32(c); \
    regAddr = addr; \
    if (regAddr >= 0 && regAddr < 4) { \
        gx->indexBase[regAddr] = c; \
    } \
} while (0)
#define GX_WRITE_SOME_REG3(a, b, c, addr) \
do { \
    long regAddr; \
    GX_WRITE_U8(a); \
    GX_WRITE_U8(b); \
    GX_WRITE_U32(c); \
    regAddr = addr; \
    if (regAddr >= 0 && regAddr < 4) { \
        gx->indexStride[regAddr] = c; \
    } \
} while (0)
#define GX_WRITE_SOME_REG4(a, b, c, addr) \
do { \
    long regAddr; \
    GX_WRITE_U8(a); \
    GX_WRITE_U8(b); \
    GX_WRITE_U32(c); \
    regAddr = addr; \
} while (0)

#define GET_REG_FIELD(reg, size, shift) ((int)((reg) >> (shift)) & ((1 << (size)) - 1))

#define SET_REG_FIELD(line, reg, size, shift, val) \
do { \
    ASSERTMSGLINE(line, ((u32)(val) & ~((1 << (size)) - 1)) == 0, "GX Internal: Register field out of range"); \
    (reg) = ((u32)(reg) & ~(((1 << (size)) - 1) << (shift))) | ((u32)(val) << (shift)); \
} while (0)

#define CHECK_GXBEGIN(line, name) ASSERTMSGLINE(line, !__GXinBegin, "'" name "' is not allowed between GXBegin/GXEnd")

/* GXAttr.c */

void __GXSetVCD(void);
void __GXSetVAT(void);

/* GXBump.c */

void __GXUpdateBPMask(void);
void __GXFlushTextureState(void);

/* GXFifo.c */

struct __GXFifoObj {
    u8 *base;
    u8 *top;
    u32 size;
    u32 hiWatermark;
    u32 loWatermark;
    void *rdPtr;
    void *wrPtr;
    s32 count;
    u8 bind_cpu;
    u8 bind_gp;
};

void __GXSaveCPUFifoAux(struct __GXFifoObj *realFifo);
void __GXFifoInit(void);
void __GXCleanGPFifo(void);

/* GXGeometry.c */

void __GXSetDirtyState(void);
void __GXSendFlushPrim(void);
void __GXSetGenMode(void);

/* GXInit.c */

void __GXInitGX(void);

struct __GXData_struct {
    unsigned short vNumNot;
    unsigned short bpSentNot;
    unsigned short vNum;
    unsigned short vLim;
    unsigned long cpEnable;
    unsigned long cpStatus;
    unsigned long cpClr;
    unsigned long vcdLo;
    unsigned long vcdHi;
    unsigned long vatA[8];
    unsigned long vatB[8];
    unsigned long vatC[8];
    unsigned long lpSize;
    unsigned long matIdxA;
    unsigned long matIdxB;
    unsigned long indexBase[4];
    unsigned long indexStride[4];
    unsigned long ambColor[2];
    unsigned long matColor[2];
    unsigned long suTs0[8];
    unsigned long suTs1[8];
    unsigned long suScis0;
    unsigned long suScis1;
    unsigned long tref[8];
    unsigned long iref;
    unsigned long bpMask;
    unsigned long IndTexScale0;
    unsigned long IndTexScale1;
    unsigned long tevc[16];
    unsigned long teva[16];
    unsigned long tevKsel[8];
    unsigned long cmode0;
    unsigned long cmode1;
    unsigned long zmode;
    unsigned long peCtrl;
    unsigned long cpDispSrc;
    unsigned long cpDispSize;
    unsigned long cpDispStride;
    unsigned long cpDisp;
    unsigned long cpTexSrc;
    unsigned long cpTexSize;
    unsigned long cpTexStride;
    unsigned long cpTex;
    unsigned char cpTexZ;
    unsigned long genMode;
    GXTexRegion TexRegions[8];
    GXTexRegion TexRegionsCI[4];
    unsigned long nextTexRgn;
    unsigned long nextTexRgnCI;
    GXTlutRegion TlutRegions[20];
    GXTexRegionCallback texRegionCallback;
    GXTlutRegion * (* tlutRegionCallback)(unsigned long);
    GXAttrType nrmType;
    unsigned char hasNrms;
    unsigned char hasBiNrms;
    unsigned long projType;
    float projMtx[6];
    float vpLeft;
    float vpTop;
    float vpWd;
    float vpHt;
    float vpNearz;
    float vpFarz;
    unsigned char fgRange;
    float fgSideX;
    unsigned long tImage0[8];
    unsigned long tMode0[8];
    unsigned long texmapId[16];
    unsigned long tcsManEnab;
    unsigned long tevTcEnab;
    GXPerf0 perf0;
    GXPerf1 perf1;
    unsigned long perfSel;
    unsigned char inDispList;
    unsigned char dlSaveContext;
    unsigned char dirtyVAT;
    unsigned long dirtyState;
};

extern struct __GXData_struct *gx;

/* GXMisc.c */

void __GXPEInit(void);

/* GXStubs.c */

void __GXSetRange(float nearz, float fgSideX);

/* GXTexture.c */

void __GetImageTileCount(GXTexFmt fmt, u16 wd, u16 ht, u32 *rowTiles, u32 *colTiles, u32 *cmpTiles);
void __GXSetSUTexRegs(void);
void __GXSetTmemConfig(u32 config);

/* GXTransform.c */

void __GXSetMatrixIndex(GXAttr matIdxAttr);

#endif
