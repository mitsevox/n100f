#include "iMath.h"

#include "iTime.h"

static U32 rndseed;

void iMathInit()
{
    isrand(iTimeGet());
    iTimeGet();
    irand();
}

void iMathExit()
{
}

void isrand(U32 seed)
{
    rndseed = seed;
}

U32 irand()
{
    rndseed = rndseed * 1103515245 + 12345;
    return rndseed;
}

F32 iurand()
{
    return irand() * (1.0f / 4294967296.0f);
}
