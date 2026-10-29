#ifndef XSCRFX_H
#define XSCRFX_H

#include "types.h"
#include "iColor.h"

S32 xScrFxIsLetterbox();
void xScrFxLetterbox(S32 enable);
// The DWARF lists five parameters, but every caller passes a sixth int and the
// mangled name carries it.
void xScrFxFade(iColor_tag* base, iColor_tag* dest, F32 seconds, void (*callback)(), S32 hold, S32);

#endif
