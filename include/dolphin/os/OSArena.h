#ifndef _DOLPHIN_OSARENA_H_
#define _DOLPHIN_OSARENA_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void* OSGetArenaHi(void);
void OSSetArenaHi(void* newHi);

#ifdef __cplusplus
}
#endif

#endif
