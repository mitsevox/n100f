#ifndef ITIME_H
#define ITIME_H

#include "types.h"

typedef S64 iTime;

void iTimeInit();
void iTimeExit();
iTime iTimeGet();
void iTimeGameAdvance(F32 elapsed);
void iTimeSetGame(F32 time);
void iProfileClear(U32 sceneID);
F32 iTimeDiffSec(iTime time);
F32 iTimeDiffSec(iTime t0, iTime t1);

#endif
