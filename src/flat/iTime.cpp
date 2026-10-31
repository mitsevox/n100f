#include "iTime.h"

#include "xTRC.h"

#include <dolphin/os.h>

static iTime sStartupTime;
static F32 sGameTime;

void iTimeInit()
{
    sStartupTime = OSGetTime();
}

void iTimeExit()
{
}

// Ticks since iTimeInit.
iTime iTimeGet()
{
    return OSGetTime() - sStartupTime;
}

F32 iTimeDiffSec(iTime time)
{
    return (F32)time / (OS_BUS_CLOCK / 4);
}

F32 iTimeDiffSec(iTime t0, iTime t1)
{
    return iTimeDiffSec(t1 - t0);
}

void iTimeGameAdvance(F32 elapsed)
{
    sGameTime += elapsed;
}

void iTimeSetGame(F32 time)
{
    sGameTime = time;
}

// The profiler does nothing on this platform.
void iProfileClear(U32 sceneID)
{
}

// The last disc state the disc layer reported (see xTRCDisk).
_tagTRCState iTRCGetDiskState()
{
    return gTrcDisk[0];
}

// Whether the disc has been identified since the last check started.
U8 iTRCDiskIdentified()
{
    return gTrcDisk[1] == TRC_DiskIdentified;
}
