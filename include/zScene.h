#ifndef ZSCENE_H
#define ZSCENE_H

#include "types.h"
#include "xBase.h"
#include "xScene.h"

struct _zEnt;
struct _zPortal;
struct _zEnv;
struct _tagClimate;

// Partial: members up to ents. The _xScene members past gravity are left opaque.
struct zScene : _xScene
{
    U8 opaque_14[0x6C - 0x14];
    U32 sceneID;
    _zPortal* pendingPortal;
    U32 num_ents;
    _zEnt** ents;
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

#endif
