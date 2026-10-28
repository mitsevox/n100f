#ifndef XANIM_H
#define XANIM_H

#include "types.h"
#include "xMemMgr.h"
#include "xMath3.h"

struct xAnimTransition;
struct xAnimTable;
struct xAnimEffect;
struct xAnimTransitionList;
struct xModelInstance;

struct xAnimFile
{
    xAnimFile* Next;
    char* Name;
    U32 ID;
    U32 FileFlags;
    F32 Duration;
    F32 TimeOffset;
    F32 PhysBase[4];
    F32 PhysDelta[4];
    U16 BoneCount;
    U8 NumAnims[2];
    void** RawData;
    void* Physics;
};

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

struct xAnimActiveEffect;
struct xAnimPlay;

struct xAnimSingle
{
    U32 SingleFlags;
    xAnimState* State;
    F32 Time;
    F32 CurrentSpeed;
    F32 BilinearLerp[2];
    xAnimEffect* Effect;
    U32 ActiveCount;
    xAnimActiveEffect* ActiveList;
    xAnimPlay* Play;
    F32 LastTime;
    xVec3 LastTranslation;
    F32 LastYaw;
    U32 pad;
    xQuat LastQuat;
    xAnimTransition* Sync;
    xAnimTransition* Tran;
    xAnimSingle* Blend;
    F32 BlendFactor;
};

struct xAnimTransitionList
{
    xAnimTransitionList* Next;
    xAnimTransition* T;
};

struct xAnimTransition
{
    xAnimTransition* Next;
    xAnimState* Dest;
    U32 (*Conditional)(xAnimTransition*, xAnimSingle*, void*);
    U32 (*Callback)(xAnimTransition*, xAnimSingle*, void*);
    U32 Flags;
    U32 UserFlags;
    F32 SrcTime;
    F32 DestTime;
    U16 Priority;
    U16 QueuePriority;
    F32 BlendRecip;
    U16* BlendOffset;
};

// Partial: members up to ModelInst.
struct xAnimPlay
{
    xAnimPlay* Next;
    U16 NumSingle;
    U16 BoneCount;
    xAnimSingle* Single;
    void* Object;
    xAnimTable* Table;
    xVec3 Translate[2];
    U32 pad1;
    xQuat Quat[2];
    F32 Yaw;
    xMemPool* Pool;
    xModelInstance* ModelInst;
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
                                         xAnimTransitionCallback callback, U32 flags, U32 userFlags,
                                         F32 srcTime, F32 destTime, U16 priority, U16 queuePriority,
                                         F32 blendRecip, U16* blendOffset);
void xAnimTableAddTransition(xAnimTable* table, xAnimTransition* tran, char* source);
xAnimState* xAnimTableGetStateID(xAnimTable* table, U32 ID);
void xAnimTableAddFile(xAnimTable* table, xAnimFile* file, char* states);
xAnimState* xAnimTableAddFileID(xAnimTable* table, xAnimFile* file, U32 stateID);
xAnimState* xAnimTableGetState(xAnimTable* table, char* name);
xAnimEffect* xAnimStateNewEffect(xAnimState* state, U32 flags, F32 startTime, F32 endTime,
                                 U32 (*callback)(U32, xAnimActiveEffect*, xAnimSingle*, void*),
                                 U32 userDataSize);
void xAnimPlaySetState(xAnimSingle* single, xAnimState* state, F32 startTime);
void xAnimPlaySetup(xAnimPlay* play, void* object, xAnimTable* table, xModelInstance* modelInst);
xAnimPlay* xAnimPoolAlloc(xMemPool* pool, void* object, xAnimTable* table,
                          xModelInstance* modelInst);
void xAnimPlayStartTransition(xAnimPlay* play, xAnimTransition* transition);
xAnimTransition* xAnimTempTransitionAlloc();

#endif
