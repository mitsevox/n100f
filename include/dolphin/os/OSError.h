#ifndef _DOLPHIN_OSERROR_H_
#define _DOLPHIN_OSERROR_H_

#include <dolphin/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef u16 OSError;
typedef void (*OSErrorHandler)(OSError error, OSContext* context, ...);

#define OS_ERROR_MACHINE_CHECK 1
#define OS_ERROR_PROTECTION 15
#define OS_ERROR_MAX (OS_ERROR_PROTECTION + 1)

OSErrorHandler OSSetErrorHandler(OSError error, OSErrorHandler handler);

extern OSErrorHandler __OSErrorTable[OS_ERROR_MAX];

#ifdef __cplusplus
}
#endif

#endif
