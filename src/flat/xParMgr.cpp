#include "xParMgr.h"
#include "iParMgr.h"

void xParMemInit();

static U32 sFrameCount;
static S32 sParMgrInit;

// The particle memory is set up once; the platform manager is reset on every call.
void xParMgrInit()
{
    if (!sParMgrInit)
    {
        xParMemInit();
        iParMgrInit();
        sParMgrInit = 1;
    }
    else
    {
        iParMgrInit();
    }
}

// The frame counter wraps from 10 back to 1 and is then drained to zero by an empty loop, so it
// always ends the frame at zero.
void xParMgrUpdate(F32 elapsedTime)
{
    iParMgrUpdate(elapsedTime);
    sFrameCount++;
    if (sFrameCount > 10)
    {
        sFrameCount = 1;
    }
    while (sFrameCount >= 1)
    {
        sFrameCount--;
    }
}

void xParMgrRender()
{
    iParMgrRender();
}
