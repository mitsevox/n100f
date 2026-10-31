#ifndef XFONT_H
#define XFONT_H

#include "types.h"
#include "iColor.h"

struct RwTexture;
struct RwRaster;

struct iFontSpacing
{
    U8 ox;
    U8 dx;
};

struct iFontInfo
{
    RwTexture* texture;
    RwRaster* raster;
};

struct xFontInfo
{
    U16 id;
    U16 flags;
    U16 char_width;
    U16 char_height;
    F32 oo_tex_width;
    F32 oo_tex_height;
    F32 u;
    F32 v;
    U16 layout;
    U16 pad;
    U8 lookup[128];
    iFontSpacing pspc[128];
    char* characters;
    iFontInfo ps;
};

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

// The pen position while a box draws; draw is clear while words are only being measured.
struct xFontGlobals_t
{
    F32 dpx;
    F32 dpy;
    U32 draw;
    F32 aspect_ratio;
};

extern xFontGlobals_t gFontGlobals;

xFontBox* xFontBoxCreate(F32 x, F32 y, F32 w, F32 h, U32 id, iColor_tag color);
void xFontBoxDestroy(xFontBox* box);
void xFontBoxSetText(xFontBox* box, char* text);
void xFontBoxSetJustification(xFontBox* box, S32 mode);
void xFontBoxSetDrawSize(xFontBox* box, F32 draw_width, F32 draw_height);
void xFontBoxSetDrawSpacing(xFontBox* box, F32 x_spacing, F32 y_spacing);
void xFontBoxSetColor(xFontBox* box, iColor_tag color);
char* xFontBoxRender(xFontBox* box);
void xFontBoxSetBackdrop(xFontBox* box, iColor_tag color);
void xFontBoxSetInset(xFontBox* box, F32 inset_left, F32 inset_right, F32 inset_top,
                      F32 inset_bottom);
void xFontBoxSetFont(xFontBox* box, U32 id);
void xFontRender();
void xFontInit();

void iFontInit();

#endif
