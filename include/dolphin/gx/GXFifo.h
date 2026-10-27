#ifndef _DOLPHIN_GX_GXFIFO_H_
#define _DOLPHIN_GX_GXFIFO_H_

#include <dolphin/gx/GXEnum.h>
#include <dolphin/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    u8 pad[128];
} GXFifoObj;

typedef void (*GXBreakPtCallback)(void);

void GXInitFifoBase(GXFifoObj *fifo, void *base, u32 size);
void GXInitFifoPtrs(GXFifoObj *fifo, void *readPtr, void *writePtr);
void GXInitFifoLimits(GXFifoObj *fifo, u32 hiWatermark, u32 loWatermark);
void GXSetCPUFifo(GXFifoObj *fifo);
void GXSetGPFifo(GXFifoObj *fifo);
void GXSaveCPUFifo(GXFifoObj *fifo);
void GXGetFifoPtrs(GXFifoObj *fifo, void **readPtr, void **writePtr);
void *GXGetFifoBase(GXFifoObj *fifo);
GXBreakPtCallback GXSetBreakPtCallback(GXBreakPtCallback cb);
void GXEnableBreakPt(void *break_pt);
void GXDisableBreakPt(void);
OSThread *GXSetCurrentGXThread(void);
GXFifoObj *GXGetCPUFifo(void);
GXFifoObj *GXGetGPFifo(void);

#ifdef __cplusplus
}
#endif

#endif
