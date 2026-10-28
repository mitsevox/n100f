#ifndef ZFOG_H
#define ZFOG_H

#include "xBase.h"

struct zFogAsset : p2BaseAsset
{
    U8 bkgndColor[4];
    U8 fogColor[4];
    F32 fogDensity;
    F32 fogStart;
    F32 fogStop;
    F32 transitionTime;
    U8 fogType;
    U8 padFog[3];
};

struct _zFog : xBase
{
    zFogAsset* tasset;
};

struct _xScene;

S32 zFogEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*);
void zFogInit(void* b, void* tasset);
void zFogSave(_zFog* ent, xSerial* s);
void zFogLoad(_zFog* ent, xSerial* s);
void zFogUpdate(xBase*, _xScene*, F32);

#endif
