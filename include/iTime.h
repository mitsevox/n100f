#ifndef ITIME_H
#define ITIME_H

#include "types.h"

typedef S64 iTime;

iTime iTimeGet();
F32 iTimeDiffSec(iTime t0, iTime t1);

#endif
