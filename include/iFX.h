#ifndef IFX_H
#define IFX_H

#include "types.h"

#include <rpworld.h>

enum iFXWarbModes
{
    IFX_WARB_NONE,
    IFX_WARB_WATER_WAVE,
    IFX_WARB_WATER_RIPPLE,
    IFX_WARB_DAN1,
    IFX_WARB_DAN2,
    IFX_WARB_TREE,
    IFX_WARB_WEB
};

void iFXWarbUpdate(F32 dt);
void iFXWarbSetParams(U32 mode, F32* val);
void iFXWarbInit(void* vp_ent, iFXWarbModes warb_mode);
RxPipeline* iFXanimUVCreatePipe();

#endif
