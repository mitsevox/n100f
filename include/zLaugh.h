#ifndef ZLAUGH_H
#define ZLAUGH_H

#include "types.h"

struct zLaughSituation
{
    U32 countMax;
    F32 delay;
    F32 punchDelay;
    F32 elapsedTime;
    U32 count;
};

void zLaughInit();
void zLaughNotify(S32 situation);
void zLaughUpdate(F32 dt);

#endif
