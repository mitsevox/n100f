#ifndef ZSCENE_H
#define ZSCENE_H

#include "types.h"
#include "xBase.h"
#include "xScene.h"

struct _zEnt;
struct _zPortal;
struct _zEnv;
struct _tagClimate;

// Partial: the _xScene members past env are left opaque.
struct zScene : _xScene
{
    U8 opaque_44[0x6C - 0x44];
    U32 sceneID;
    _zPortal* pendingPortal;
    U32 num_ents;
    _zEnt** ents;
    U32 baseCount[43];
    xBase* baseList[43];
    U32 num_bubbles;
    _zEnt** bubbles;
};

struct IDBasePair
{
    U32 id;
    xBase* base;
};

extern _tagClimate gClimate;
extern _zEnv* gCurEnv;

xBase* zSceneFindObject(U32 gameID);
xBase* zSceneGetObject(S32 type, S32 idx);
const char* zSceneGetName(U32 gameID);
const char* zSceneGetName(xBase* b);
S32 zSceneSetup_serialTraverseCB(U32 clientID, xSerial* xser);

inline U32 zSceneIDFromName(const char* name)
{
    return ((name[0] & ~0x20) << 24) | (name[1] << 16) | (name[2] << 8) | name[3];
}

extern U32 gTransitionSceneID;

#endif
