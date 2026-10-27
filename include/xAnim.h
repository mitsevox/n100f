#ifndef XANIM_H
#define XANIM_H

#include "types.h"
#include "xMemMgr.h"

struct xAnimTransition;
struct xAnimFile;
struct xAnimEffect;
struct xAnimTransitionList;
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

// Partial: members up to Time.
struct xAnimSingle
{
    U32 SingleFlags;
    xAnimState* State;
    F32 Time;
};

// Partial: members up to Single.
struct xAnimPlay
{
    xAnimPlay* Next;
    U16 NumSingle;
    U16 BoneCount;
    xAnimSingle* Single;
};

typedef U32 (*xAnimTransitionConditionalCallback)(xAnimTransition*, xAnimSingle*, void*);
typedef U32 (*xAnimTransitionCallback)(xAnimTransition*, xAnimSingle*, void*);

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
xAnimTable* xAnimTableNew(char* name, xAnimTable** linkedList);
xAnimState* xAnimTableNewState(xAnimTable* table, char* name, U32 flags, U32 userFlags, F32 speed,
                               F32* boneBlend, F32* timeSnap, F32 fadeRecip, U16* fadeOffset);
xAnimTransition* xAnimTableNewTransition(xAnimTable* table, char* source, char* dest,
                                         xAnimTransitionConditionalCallback conditional,
                                         xAnimTransitionCallback callback, U32 flags,
                                         U32 userFlags, F32 srcTime, F32 destTime, U16 priority,
                                         U16 queuePriority, F32 blendRecip, U16* blendOffset);
void xAnimTableAddTransition(xAnimTable* table, xAnimTransition* tran, char* source);
xAnimState* xAnimTableGetStateID(xAnimTable* table, U32 ID);
xAnimState* xAnimTableGetState(xAnimTable* table, char* name);
void xAnimPlaySetup(xAnimPlay* play, void* object, xAnimTable* table, xModelInstance* modelInst);
xAnimPlay* xAnimPoolAlloc(xMemPool* pool, void* object, xAnimTable* table,
                          xModelInstance* modelInst);

#endif
