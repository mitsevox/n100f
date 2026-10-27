#ifndef _DOLPHIN_OSCACHE_H_
#define _DOLPHIN_OSCACHE_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void DCFlushRange(void* addr, u32 nBytes);
void DCFlushRangeNoSync(void* addr, u32 nBytes);
void ICInvalidateRange(void* addr, u32 nBytes);

#ifdef __cplusplus
}
#endif

#endif
