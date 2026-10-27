#ifndef _DOLPHIN_OSINTERRUPT_H_
#define _DOLPHIN_OSINTERRUPT_H_

#include <dolphin/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef s16 __OSInterrupt;
typedef u32 OSInterruptMask;
typedef void (*__OSInterruptHandler)(__OSInterrupt interrupt, OSContext* context);

#define OS_INTERRUPTMASK_PI_RSW 0x00000200

BOOL OSDisableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL level);
__OSInterruptHandler __OSSetInterruptHandler(__OSInterrupt interrupt, __OSInterruptHandler handler);
OSInterruptMask __OSMaskInterrupts(OSInterruptMask mask);
OSInterruptMask __OSUnmaskInterrupts(OSInterruptMask mask);

#ifdef __cplusplus
}
#endif

#endif
