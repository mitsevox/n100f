#ifndef ZUI_H
#define ZUI_H

#include "zEnt.h"

struct xSerial;

struct zUIAsset : p2EntAsset
{
    U32 uiFlags;
    U16 dim[2];
    U32 textureID;
    F32 uva[2];
    F32 uvb[2];
    F32 uvc[2];
    F32 uvd[2];
};

struct _zUI : _zEnt
{
    zUIAsset* sasset;
    U32 uiFlags;
    U32 uiButton;
};

void zUI_Init(void* ent, void* asset);
void zUI_PreUpdate(_zUI* ent, _xScene* sc, F32 dt);
void zUI_Save(_zUI* ent, xSerial* s);
void zUI_Load(_zUI* ent, xSerial* s);
void zUIRenderAll();

#endif
