#ifndef XFONT_H
#define XFONT_H

#include "types.h"
#include "iColor.h"

struct xFontBox
{
    S32 flags;
    F32 x;
    F32 y;
    F32 w;
    F32 h;
    U32 font;
    iColor_tag color;
    iColor_tag saved_color;
    F32 draw_width;
    F32 draw_height;
    F32 x_spacing;
    F32 y_spacing;
    F32 depth;
    F32 inset_left;
    F32 inset_right;
    F32 inset_top;
    F32 inset_bottom;
    char* text;
    iColor_tag backdrop_color;
};

xFontBox* xFontBoxCreate(F32 x, F32 y, F32 w, F32 h, U32 id, iColor_tag color);
void xFontBoxDestroy(xFontBox* box);
void xFontBoxSetText(xFontBox* box, char* text);
void xFontBoxSetJustification(xFontBox* box, S32 mode);
void xFontBoxSetDrawSize(xFontBox* box, F32 draw_width, F32 draw_height);
void xFontBoxSetDrawSpacing(xFontBox* box, F32 x_spacing, F32 y_spacing);
void xFontBoxSetColor(xFontBox* box, iColor_tag color);
char* xFontBoxRender(xFontBox* box);

#endif
