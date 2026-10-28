#ifndef ZENTDESTRUCTOBJ_H
#define ZENTDESTRUCTOBJ_H

#include "zEnt.h"

struct _zParEmitter;

struct zEntDestructObjAsset
{
    F32 animSpeed;
    U32 initAnimState;
    U32 health;
    U32 spawnItemID;
    U32 dflags;
    U8 collType;
    U8 fxType;
    U8 pad[2];
    F32 blast_radius;
    F32 blast_strength;
};

struct _zEntDestructObj : _zEnt
{
    zEntDestructObjAsset* dasset;
    U32 state;
    U32 healthCnt;
    F32 fx_timer;
    _zParEmitter* fx_emitter;
};

extern _zParEmitter* sEmitDust;
extern _zParEmitter* sEmitXplo;
extern _zParEmitter* sEmitWeb;

void zEntDestructObj_FindFX();
void zEntDestructObj_Init(void* ent, void* asset);
void zEntDestructObj_Init(_zEntDestructObj* ent, p2EntAsset* asset);
void zEntDextructObj_Move(_zEntDestructObj* ent, _xScene* sc, F32 dt, _xEntFrame* frame);
void zEntDestructObj_Update(_zEntDestructObj* ent, _xScene* sc, F32 dt);
U32 zEntDestructObj_ButtBounce(_zEntDestructObj* ent);
U32 zEntDestructObj_Headbutt(_zEntDestructObj* ent);
void zEntDestructObj_Save(_zEntDestructObj* ent, xSerial* s);
void zEntDestructObj_Load(_zEntDestructObj* ent, xSerial* s);
U32 zEntDestructObj_CheckState(_zEntDestructObj* ent);
void zEntDestructObj_DestroyFX(_zEntDestructObj* o);
S32 zEntDestructObjEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* b3);

#endif
