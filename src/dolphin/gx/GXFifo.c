#include <dolphin/base/PPCArch.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>

#include "dolphin/gx/__gx.h"

struct __GXFifoObj *CPUFifo = NULL;
struct __GXFifoObj *GPFifo = NULL;

static OSThread *__GXCurrentThread;
static GXBool CPGPLinked;
static BOOL GXOverflowSuspendInProgress;
static GXBreakPtCallback BreakPointCB;
static u32 __GXOverflowCount;

void *__GXCurrentBP;

static void __GXFifoReadEnable(void);
static void __GXFifoReadDisable(void);
static void __GXFifoLink(u8 en);
static void __GXWriteFifoIntEnable(u8 hiWatermarkEn, u8 loWatermarkEn);
static void __GXWriteFifoIntReset(u8 hiWatermarkClr, u8 loWatermarkClr);

static void GXOverflowHandler(s16 interrupt, OSContext *context)
{
    ASSERTLINE(359, !GXOverflowSuspendInProgress);

    __GXOverflowCount++;
    __GXWriteFifoIntEnable(0, 1);
    __GXWriteFifoIntReset(1, 0);
    GXOverflowSuspendInProgress = TRUE;

    OSSuspendThread(__GXCurrentThread);
}

static void GXUnderflowHandler(s16 interrupt, OSContext *context)
{
    ASSERTLINE(401, GXOverflowSuspendInProgress);

    OSResumeThread(__GXCurrentThread);
    GXOverflowSuspendInProgress = FALSE;
    __GXWriteFifoIntReset(1U, 1U);
    __GXWriteFifoIntEnable(1U, 0U);
}

static void GXBreakPointHandler(s16 interrupt, OSContext *context)
{
    OSContext exceptionContext;

    gx->cpEnable = gx->cpEnable & 0xFFFFFFDF;
    __cpReg[1] = gx->cpEnable;
    if (BreakPointCB != NULL) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        BreakPointCB();
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
}

static void GXCPInterruptHandler(s16 interrupt, OSContext *context)
{
    gx->cpStatus = __cpReg[0];
    if (GET_REG_FIELD(gx->cpEnable, 1, 3) && GET_REG_FIELD(gx->cpStatus, 1, 1)) {
        GXUnderflowHandler(interrupt, context);
    }
    if (GET_REG_FIELD(gx->cpEnable, 1, 2) && GET_REG_FIELD(gx->cpStatus, 1, 0)) {
        GXOverflowHandler(interrupt, context);
    }
    if (GET_REG_FIELD(gx->cpEnable, 1, 5) && GET_REG_FIELD(gx->cpStatus, 1, 4)) {
        GXBreakPointHandler(interrupt, context);
    }
}

void GXInitFifoBase(GXFifoObj *fifo, void *base, u32 size)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;

    ASSERTMSGLINE(524, realFifo != CPUFifo,     "GXInitFifoBase: fifo is attached to CPU");
    ASSERTMSGLINE(526, realFifo != GPFifo,      "GXInitFifoBase: fifo is attached to GP");
    ASSERTMSGLINE(528, ((u32)base & 0x1F) == 0, "GXInitFifoBase: base must be 32B aligned");
    ASSERTMSGLINE(530, base != NULL,            "GXInitFifoBase: base pointer is NULL");
    ASSERTMSGLINE(532, (size & 0x1F) == 0,      "GXInitFifoBase: size must be 32B aligned");
    ASSERTMSGLINE(534, size >= 0x10000,         "GXInitFifoBase: fifo is not large enough");

    realFifo->base = base;
    realFifo->top = (u8 *)base + size - 4;
    realFifo->size = size;
    realFifo->count = 0;
    GXInitFifoLimits(fifo, size - 0x4000, (size >> 1) & ~0x1F);
    GXInitFifoPtrs(fifo, base, base);
}

void GXInitFifoPtrs(GXFifoObj *fifo, void *readPtr, void *writePtr)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;
    BOOL enabled;

    ASSERTMSGLINE(574, realFifo != CPUFifo,         "GXInitFifoPtrs: fifo is attached to CPU");
    ASSERTMSGLINE(576, realFifo != GPFifo,          "GXInitFifoPtrs: fifo is attached to GP");
    ASSERTMSGLINE(578, ((u32)readPtr & 0x1F) == 0,  "GXInitFifoPtrs: readPtr not 32B aligned");
    ASSERTMSGLINE(580, ((u32)writePtr & 0x1F) == 0, "GXInitFifoPtrs: writePtr not 32B aligned");
    ASSERTMSGLINE(583, realFifo->base <= readPtr && readPtr < realFifo->top,   "GXInitFifoPtrs: readPtr not in fifo range");
    ASSERTMSGLINE(586, realFifo->base <= writePtr && writePtr < realFifo->top, "GXInitFifoPtrs: writePtr not in fifo range");

    enabled = OSDisableInterrupts();
    realFifo->rdPtr = readPtr;
    realFifo->wrPtr = writePtr;
    realFifo->count = (u8 *)writePtr - (u8 *)readPtr;
    if (realFifo->count < 0) {
        realFifo->count += realFifo->size;
    }
    OSRestoreInterrupts(enabled);
}

void GXInitFifoLimits(GXFifoObj *fifo, u32 hiWatermark, u32 loWatermark)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;

    ASSERTMSGLINE(623, realFifo != GPFifo,        "GXInitFifoLimits: fifo is attached to GP");
    ASSERTMSGLINE(625, (hiWatermark & 0x1F) == 0, "GXInitFifoLimits: hiWatermark not 32B aligned");
    ASSERTMSGLINE(627, (loWatermark & 0x1F) == 0, "GXInitFifoLimits: loWatermark not 32B aligned");
    ASSERTMSGLINE(629, hiWatermark < realFifo->top - realFifo->base, "GXInitFifoLimits: hiWatermark too large");
    ASSERTMSGLINE(631, loWatermark < hiWatermark, "GXInitFifoLimits: hiWatermark below lo watermark");

    realFifo->hiWatermark = hiWatermark;
    realFifo->loWatermark = loWatermark;
}

void GXSetCPUFifo(GXFifoObj *fifo)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;
    BOOL enabled = OSDisableInterrupts();

    CPUFifo = realFifo;
    if (CPUFifo == GPFifo)
    {
        u32 reg = 0;

        __piReg[3] = (u32)realFifo->base & 0x3FFFFFFF;
        __piReg[4] = (u32)realFifo->top & 0x3FFFFFFF;
        SET_REG_FIELD(673, reg, 21, 5, ((u32)realFifo->wrPtr & 0x3FFFFFFF) >> 5);
        SET_REG_FIELD(674, reg, 1, 26, 0);
        __piReg[5] = reg;
        CPGPLinked = GX_TRUE;
        __GXWriteFifoIntReset(1, 1);
        __GXWriteFifoIntEnable(1, 0);
        __GXFifoLink(1);
    }
    else
    {
        u32 reg;

        if (CPGPLinked)
        {
            __GXFifoLink(0);
            CPGPLinked = GX_FALSE;
        }
        __GXWriteFifoIntEnable(0, 0);
        reg = 0;
        __piReg[3] = (u32)realFifo->base & 0x3FFFFFFF;
        __piReg[4] = (u32)realFifo->top & 0x3FFFFFFF;
        SET_REG_FIELD(708, reg, 21, 5, ((u32)realFifo->wrPtr & 0x3FFFFFFF) >> 5);
        SET_REG_FIELD(709, reg, 1, 26, 0);
        __piReg[5] = reg;
    }

    __sync();

    OSRestoreInterrupts(enabled);
}

void GXSetGPFifo(GXFifoObj *fifo)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;
    BOOL enabled = OSDisableInterrupts();

    __GXFifoReadDisable();
    __GXWriteFifoIntEnable(0, 0);
    GPFifo = realFifo;

    __cpReg[16] = (u32)realFifo->base & 0xFFFF;
    __cpReg[18] = (u32)realFifo->top & 0xFFFF;
    __cpReg[24] = realFifo->count & 0xFFFF;
    __cpReg[26] = (u32)realFifo->wrPtr & 0xFFFF;
    __cpReg[28] = (u32)realFifo->rdPtr & 0xFFFF;
    __cpReg[20] = (u32)realFifo->hiWatermark & 0xFFFF;
    __cpReg[22] = (u32)realFifo->loWatermark & 0xFFFF;
    __cpReg[17] = ((u32)realFifo->base & 0x3FFFFFFF) >> 16;
    __cpReg[19] = ((u32)realFifo->top & 0x3FFFFFFF) >> 16;
    __cpReg[25] = realFifo->count >> 16;
    __cpReg[27] = ((u32)realFifo->wrPtr & 0x3FFFFFFF) >> 16;
    __cpReg[29] = ((u32)realFifo->rdPtr & 0x3FFFFFFF) >> 16;
    __cpReg[21] = (u32)realFifo->hiWatermark >> 16;
    __cpReg[23] = (u32)realFifo->loWatermark >> 16;

    __sync();

    if (CPUFifo == GPFifo) {
        CPGPLinked = GX_TRUE;
        __GXWriteFifoIntEnable(1, 0);
        __GXFifoLink(1);
    }
    else {
        CPGPLinked = GX_FALSE;
        __GXWriteFifoIntEnable(0, 0);
        __GXFifoLink(0);
    }
    __GXWriteFifoIntReset(1, 1);
    __GXFifoReadEnable();
    OSRestoreInterrupts(enabled);
}

void GXSaveCPUFifo(GXFifoObj *fifo)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;
    ASSERTMSGLINE(814, realFifo == CPUFifo, "GXSaveCPUFifo: fifo is not attached to CPU");
    __GXSaveCPUFifoAux(realFifo);
}

#define SOME_MACRO1(fifo) \
do { \
    u32 temp = __cpReg[29] << 16; \
    temp |= __cpReg[28]; \
    fifo->rdPtr = OSPhysicalToCached(temp); \
} while (0)

#define SOME_MACRO2(fifo) \
do { \
    u32 temp = __cpReg[25] << 16; \
    temp |= __cpReg[24]; \
    fifo->count = temp; \
} while (0)

void __GXSaveCPUFifoAux(struct __GXFifoObj *realFifo)
{
    BOOL enabled = OSDisableInterrupts();

    GXFlush();
    realFifo->base = OSPhysicalToCached(__piReg[3]);
    realFifo->top = OSPhysicalToCached(__piReg[4]);
    realFifo->wrPtr = OSPhysicalToCached(__piReg[5] & 0xFBFFFFFF);
    if (CPGPLinked) {
        SOME_MACRO1(realFifo);
        SOME_MACRO2(realFifo);
    } else {
        realFifo->count = (u8 *)realFifo->wrPtr - (u8 *)realFifo->rdPtr;
        if (realFifo->count < 0)
            realFifo->count += realFifo->size;
    }
    OSRestoreInterrupts(enabled);
}

void GXGetFifoPtrs(GXFifoObj *fifo, void **readPtr, void **writePtr)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;

    if (realFifo == CPUFifo) {
        realFifo->wrPtr = OSPhysicalToCached(__piReg[5] & 0xFBFFFFFF);
    }
    if (realFifo == GPFifo) {
        SOME_MACRO1(realFifo);
        SOME_MACRO2(realFifo);
    } else {
        realFifo->count = (u8 *)realFifo->wrPtr - (u8 *)realFifo->rdPtr;
        if (realFifo->count < 0) {
            realFifo->count += realFifo->size;
        }
    }
    *readPtr = realFifo->rdPtr;
    *writePtr = realFifo->wrPtr;
}

void *GXGetFifoBase(GXFifoObj *fifo)
{
    struct __GXFifoObj *realFifo = (struct __GXFifoObj *)fifo;

    return realFifo->base;
}

GXBreakPtCallback GXSetBreakPtCallback(GXBreakPtCallback cb)
{
    GXBreakPtCallback oldcb = BreakPointCB;
    BOOL enabled = OSDisableInterrupts();

    BreakPointCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

void *__GXCurrentBP;

void GXEnableBreakPt(void *break_pt)
{
    BOOL enabled = OSDisableInterrupts();

    __GXFifoReadDisable();
    __cpReg[30] = (u32)break_pt;
    __cpReg[31] = ((u32)break_pt >> 16) & 0x3FFF;
    gx->cpEnable = (gx->cpEnable & 0xFFFFFFFD) | 2;
    gx->cpEnable = (gx->cpEnable & 0xFFFFFFDF) | 0x20;
    __cpReg[1] = gx->cpEnable;
    __GXCurrentBP = break_pt;
    __GXFifoReadEnable();
    OSRestoreInterrupts(enabled);
}

void GXDisableBreakPt(void)
{
    BOOL enabled = OSDisableInterrupts();

    gx->cpEnable = gx->cpEnable & 0xFFFFFFFD;
    gx->cpEnable = gx->cpEnable & 0xFFFFFFDF;
    __cpReg[1] = gx->cpEnable;
    __GXCurrentBP = NULL;
    OSRestoreInterrupts(enabled);
}

void __GXFifoInit(void)
{
    __OSSetInterruptHandler(0x11, GXCPInterruptHandler);
    __OSUnmaskInterrupts(0x4000);
    __GXCurrentThread = OSGetCurrentThread();
    GXOverflowSuspendInProgress = FALSE;
    CPUFifo = NULL;
    GPFifo = NULL;
}

static void __GXFifoReadEnable(void)
{
    SET_REG_FIELD(0, gx->cpEnable, 1, 0, 1);
    __cpReg[1] = gx->cpEnable;
}

static void __GXFifoReadDisable(void)
{
    SET_REG_FIELD(0, gx->cpEnable, 1, 0, 0);
    __cpReg[1] = gx->cpEnable;
}

static void __GXFifoLink(u8 en)
{
    SET_REG_FIELD(1220, gx->cpEnable, 1, 4, (en != 0) ? 1 : 0);
    __cpReg[1] = gx->cpEnable;
}

static void __GXWriteFifoIntEnable(u8 hiWatermarkEn, u8 loWatermarkEn)
{
    SET_REG_FIELD(1242, gx->cpEnable, 1, 2, hiWatermarkEn);
    SET_REG_FIELD(1243, gx->cpEnable, 1, 3, loWatermarkEn);
    __cpReg[1] = gx->cpEnable;
}

static void __GXWriteFifoIntReset(u8 hiWatermarkClr, u8 loWatermarkClr)
{
    SET_REG_FIELD(1266, gx->cpClr, 1, 0, hiWatermarkClr);
    SET_REG_FIELD(1267, gx->cpClr, 1, 1, loWatermarkClr);
    __cpReg[2] = gx->cpClr;
}

void __GXCleanGPFifo(void)
{
    GXFifoObj dummyFifo;
    GXFifoObj *gpFifo = GXGetGPFifo();
    GXFifoObj *cpuFifo = GXGetCPUFifo();
    void *base = GXGetFifoBase(gpFifo);

    dummyFifo = *gpFifo;
    GXInitFifoPtrs(&dummyFifo, base, base);
    GXSetGPFifo(&dummyFifo);
    if (cpuFifo == gpFifo) {
        GXSetCPUFifo(&dummyFifo);
    }
    GXInitFifoPtrs(gpFifo, base, base);
    GXSetGPFifo(gpFifo);
    if (cpuFifo == gpFifo) {
        GXSetCPUFifo(cpuFifo);
    }
}

OSThread *GXSetCurrentGXThread(void)
{
    BOOL enabled;
    struct OSThread *prev;

    enabled = OSDisableInterrupts();
    prev = __GXCurrentThread;
    ASSERTMSGLINE(1350, !GXOverflowSuspendInProgress, "GXSetCurrentGXThread: Two threads cannot generate GX commands at the same time!");
    __GXCurrentThread = OSGetCurrentThread();
    OSRestoreInterrupts(enabled);
    return prev;
}

GXFifoObj *GXGetCPUFifo(void)
{
    return (GXFifoObj *)CPUFifo;
}

GXFifoObj *GXGetGPFifo(void)
{
    return (GXFifoObj *)GPFifo;
}

#define SET_REG_FIELD2(line, reg, mask, val) \
do { \
    ASSERTMSGLINE(line, ((val) & ~(mask)) == 0, "GX Internal: Register field out of range"); \
    (reg) = ((u32)(reg) & ~(mask)) | ((u32)(val)); \
} while (0)
