#ifndef _DOLPHIN_OSCONTEXT_H_
#define _DOLPHIN_OSCONTEXT_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSContext OSContext;

void OSDumpContext(OSContext* context);

#ifdef __cplusplus
}
#endif

#endif
