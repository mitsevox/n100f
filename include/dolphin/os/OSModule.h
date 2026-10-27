#ifndef _DOLPHIN_OSMODULE_H_
#define _DOLPHIN_OSMODULE_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSModuleInfo OSModuleInfo;

typedef struct OSModuleQueue {
    OSModuleInfo* head;
    OSModuleInfo* tail;
} OSModuleQueue;

#ifdef __cplusplus
}
#endif

#endif
