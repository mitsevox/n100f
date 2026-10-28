#include "iTime.h"

#include <dolphin/os.h>

F32 iTimeDiffSec(iTime time)
{
    return (F32)time / (OS_BUS_CLOCK / 4);
}

F32 iTimeDiffSec(iTime t0, iTime t1)
{
    return iTimeDiffSec(t1 - t0);
}
