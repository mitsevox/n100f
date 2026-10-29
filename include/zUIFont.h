#ifndef ZUIFONT_H
#define ZUIFONT_H

#include "zUI.h"

struct zUIFontAsset : zUIAsset
{
    U16 uiFontFlags;
    U8 mode;
    U8 fontID;
    U32 textAssetID;
    U8 bcolor[4];
    U8 color[4];
    U16 inset[4];
    U16 space[2];
    U16 cdim[2];
};

struct _zUIFont : _zUI
{
    zUIFontAsset* fasset;
    U16 uiFontFlags;
    U16 uiFontHackFlags;
};

void zUIFont_Init(void* ent, void* asset);
void zUIFont_PreUpdate(_zUIFont* ent, _xScene* sc, F32 dt);
void zUIFont_Save(_zUIFont* ent, xSerial* s);
void zUIFont_Load(_zUIFont* ent, xSerial* s);

#endif
