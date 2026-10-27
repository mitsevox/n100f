#ifndef _DOLPHIN_OSEXCEPTION_H_
#define _DOLPHIN_OSEXCEPTION_H_

#include <dolphin/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef u8 __OSException;
typedef void (*__OSExceptionHandler)(__OSException exception, OSContext* context);

__OSExceptionHandler __OSGetExceptionHandler(__OSException exception);
__OSExceptionHandler __OSSetExceptionHandler(__OSException exception, __OSExceptionHandler handler);

#ifdef __cplusplus
}
#endif

#endif
