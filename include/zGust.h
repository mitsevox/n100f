#ifndef ZGUST_H
#define ZGUST_H

#include "xBase.h"
#include "xVec3.h"

struct _xEnt;
struct _xScene;

struct _zVolume;

struct zGustAsset : p2BaseAsset
{
    U32 flags;
    U32 volumeID;
    U32 effectID;
    xVec3 vel;
    F32 fade;
    F32 partMod;
};

struct _zGust : xBase
{
    U32 flags;
    zGustAsset* asset;
    _zVolume* volume;
    _zVolume* fx_volume;
    F32 debris_timer;
};

struct zGustData
{
    U32 gust_on;
    _zGust* g[4];
    F32 lerp[4];
};

_zGust* zGustGetGust(U16 n);
S32 zGustEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* b);
void zGustUpdateFX(F32 seconds);
void zGustUpdateEnt(_xEnt* ent, _xScene* sc, F32 dt, void* gdata);
void zGustLoad(_zGust* ent, xSerial* s);
void zGustSave(_zGust* ent, xSerial* s);
void zGustSetup();
void zGustInit();

#endif
