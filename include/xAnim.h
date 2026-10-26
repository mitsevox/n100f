#ifndef XANIM_H
#define XANIM_H

#include "types.h"
#include "xMemMgr.h"

struct xAnimTransition;
struct xAnimFile;
struct xAnimEffect;
struct xAnimTransitionList;
struct xAnimPlay;
struct xModelInstance;

struct xAnimState
{
    xAnimState* Next;
    char* Name;
    U32 ID;
    U32 Flags;
    U32 UserFlags;
    F32 Speed;
    xAnimFile* Data;
    xAnimEffect* Effects;
    xAnimTransitionList* Default;
    xAnimTransitionList* List;
    F32* BoneBlend;
    F32* TimeSnap;
    F32 FadeRecip;
    U16* FadeOffset;
};

struct xAnimTable
{
    xAnimTable* Next;
    char* Name;
    xAnimTransition* TransitionList;
    xAnimState* StateList;
    U32 AnimIndex;
    U32 MorphIndex;
};

void xAnimInit();
xAnimState* xAnimTableGetStateID(xAnimTable* table, U32 ID);
xAnimState* xAnimTableGetState(xAnimTable* table, char* name);
void xAnimPlaySetup(xAnimPlay* play, void* object, xAnimTable* table, xModelInstance* modelInst);
xAnimPlay* xAnimPoolAlloc(xMemPool* pool, void* object, xAnimTable* table,
                          xModelInstance* modelInst);

#endif
