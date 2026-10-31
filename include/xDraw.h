#ifndef XDRAW_H
#define XDRAW_H

#include "iColor.h"
#include "xMath3.h"

void xDrawBegin();
void xDrawEnd();
void xDrawSetColor(iColor_tag color);
void xDrawSetColor(U8 r, U8 g, U8 b, U8 a);
void xDrawLine(const xVec3* a, const xVec3* b);

#endif
