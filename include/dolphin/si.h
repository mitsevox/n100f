#ifndef _DOLPHIN_SI_H_
#define _DOLPHIN_SI_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void SIInit(void);
void SISetXY(u32 x, u32 y);
void SISetSamplingRate(u32 msec);
void SIRefreshSamplingRate(void);

#ifdef __cplusplus
}
#endif

#endif
