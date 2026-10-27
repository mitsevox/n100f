#ifndef _DOLPHIN_OSINTERRUPT_H_
#define _DOLPHIN_OSINTERRUPT_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef s16 __OSInterrupt;
typedef u32 OSInterruptMask;

#define OS_INTERRUPTMASK_PI_RSW 0x00000200

BOOL OSDisableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL level);
OSInterruptMask __OSMaskInterrupts(OSInterruptMask mask);

#ifdef __cplusplus
}
#endif

#endif
