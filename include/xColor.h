#ifndef XCOLOR_H
#define XCOLOR_H

#include "iColor.h"

// The named colors xColor.cpp defines in .sdata2 (names and order from the SLUS-20349 DWARF).
extern const iColor_tag g_BLACK;
extern const iColor_tag g_NEON_RED;
extern const iColor_tag g_NEON_GREEN;
extern const iColor_tag g_MAROON;
extern const iColor_tag g_PIMP_GOLD;

inline iColor_tag xColorFromRGBA(U8 r, U8 g, U8 b, U8 a)
{
    iColor_tag c;

    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;

    return c;
}

#endif
