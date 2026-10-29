#ifndef XCOLOR_H
#define XCOLOR_H

#include "iColor.h"

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
