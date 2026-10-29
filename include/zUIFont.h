#ifndef ZUIFONT_H
#define ZUIFONT_H

#include "zUI.h"

struct zUIFontAsset;

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
