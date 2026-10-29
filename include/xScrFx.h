#ifndef XSCRFX_H
#define XSCRFX_H

#include "types.h"
#include "iColor.h"

S32 xScrFxIsLetterbox();
void xScrFxLetterbox(S32 enable);
void xScrFxFade(iColor_tag* base, iColor_tag* dest, F32 seconds, void (*callback)(), S32 hold, S32);
S32 xScrFxIsFading();

#endif
