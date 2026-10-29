#include "xAnim.h"
#include "iAnim.h"
#include "iModel.h"
#include "xMath.h"
#include "xModel.h"
#include "xMorph.h"
#include "xString.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

U32 gxAnimUseGrowAlloc;
static xMemPool sxAnimTempTranPool;

// The heading of a root rotation: the angle of its forward axis in the ground plane.
static inline F32 CalcYaw(xQuat* quat)
{
    xQuat fwd = { { 0.0f, 0.0f, 1.0f }, 0.0f };
    xQuat res;

    xQuatMul(&res, quat, &fwd);

    if (fabsf(res.v.x) + fabsf(res.v.z) < 1e-4f)
    {
        return 0.0f;
    }

    return xAngleClamp(atan2(res.v.x, res.v.z));
}

// Clear the animation system's temporary transition pool.
void xAnimInit()
{
    iAnimInit();
    memset(&sxAnimTempTranPool, 0, sizeof(xMemPool));
}

// Set up the pool of temporary transitions (count of them, growing by half as many).
void xAnimTempTransitionInit(U32 count)
{
    xMemPoolSetup(&sxAnimTempTranPool, xMemAlloc(gActiveHeap, count * sizeof(xAnimTransition)), 0,
                  0, NULL, sizeof(xAnimTransition), count, count / 2);
}

// Take a transition from the temporary pool, marked temporary.
xAnimTransition* xAnimTempTransitionAlloc()
{
    xAnimTransition* tran = (xAnimTransition*)xMemPoolAlloc(&sxAnimTempTranPool);
    tran->Flags = 0x2;
    return tran;
}

// Work out a file's root motion per axis (x, y, z, yaw) from the root positions at its first and
// last frames: held still, moved by the file's own motion, or moved relative to the root.
static void CalcPhysFromKeys(xAnimFile* af, F32* rootStart, F32* rootEnd, F32* physStart,
                             F32* physEnd)
{
    S32 i;

    if (af->FileFlags & 0x1000)
    {
        F32* temp = rootStart;
        rootStart = rootEnd;
        rootEnd = temp;

        temp = physStart;
        physStart = physEnd;
        physEnd = temp;
    }

    if (af->FileFlags & 0x2000)
    {
        rootEnd = rootStart;
        physEnd = physStart;
    }

    for (i = 0; i < 4; i++)
    {
        if (af->FileFlags & (0x10 << i))
        {
            af->PhysBase[i] = physStart[i];
            af->PhysDelta[i] = 0.0f;
        }
        else if (af->FileFlags & (0x100 << i))
        {
            F32 delta;

            af->PhysBase[i] = physStart[i];
            delta = physEnd[i] - physStart[i];

            if (i == 3)
            {
                while (delta > PI)
                {
                    delta -= 2.0f * PI;
                }

                while (delta < -PI)
                {
                    delta += 2.0f * PI;
                }
            }

            af->PhysDelta[i] = delta / af->Duration;
        }
        else
        {
            F32 delta;

            af->PhysBase[i] = physStart[i] - rootStart[i];

            if (i == 3)
            {
                while (af->PhysBase[i] > 2.0f * PI)
                {
                    af->PhysBase[i] -= 2.0f * PI;
                }

                while (af->PhysBase[i] < 0.0f)
                {
                    af->PhysBase[i] += 2.0f * PI;
                }
            }

            delta = (physEnd[i] - physStart[i]) - (rootEnd[i] - rootStart[i]);

            if (i == 3)
            {
                while (delta > PI)
                {
                    delta -= 2.0f * PI;
                }

                while (delta < -PI)
                {
                    delta += 2.0f * PI;
                }
            }

            af->PhysDelta[i] = delta / af->Duration;
        }
    }
}

// Work out a file's root motion from its own root bone, unless it has separate physics data.
static void CalcPhys(xAnimFile* af)
{
    S32 i;
    void* root;
    F32 rootStart[4];
    F32 rootEnd[4];
    F32 rootZero[4];
    F32 physStart[4];
    F32 physEnd[4];
    xQuat tempQ;

    if (af->Physics)
    {
        return;
    }

    if (af->FileFlags & 0x8000)
    {
        for (i = 0; i < 4; i++)
        {
            af->PhysBase[i] = 0.0f;
            af->PhysDelta[i] = 0.0f;
        }

        return;
    }

    root = *af->RawData;

    iAnimEval(root, 0.0f, 1, (xVec3*)rootZero, &tempQ);
    rootZero[3] = CalcYaw(&tempQ);

    iAnimEval(root, af->TimeOffset, 1, (xVec3*)rootStart, &tempQ);
    rootStart[3] = CalcYaw(&tempQ);

    iAnimEval(root, af->TimeOffset + af->Duration, 1, (xVec3*)rootEnd, &tempQ);
    rootEnd[3] = CalcYaw(&tempQ);

    for (i = 0; i < 4; i++)
    {
        if (af->FileFlags & (1 << i))
        {
            physStart[i] = rootStart[i];
        }
        else
        {
            physStart[i] = rootStart[i] - rootZero[i];
        }

        physEnd[i] = physStart[i] + rootEnd[i] - rootStart[i];

        if (i == 3)
        {
            while (physEnd[i] > 2.0f * PI)
            {
                physEnd[i] -= 2.0f * PI;
            }

            while (physEnd[i] < 0.0f)
            {
                physEnd[i] += 2.0f * PI;
            }
        }
    }

    CalcPhysFromKeys(af, rootStart, rootEnd, physStart, physEnd);
}

// Create an animation file over a numX by numY grid of raw animations, blended bilinearly when
// there is more than one.
xAnimFile* xAnimFileNewBilinear(void** rawData, char* name, U32 flags, xAnimFile** linkedList,
                                U32 numX, U32 numY)
{
    S32 i;
    xAnimFile* afile;

    if (gxAnimUseGrowAlloc)
    {
        afile = (xAnimFile*)xMemGrowAlloc(gActiveHeap, numX * numY * sizeof(void*) +
                                                           sizeof(xAnimFile));
    }
    else
    {
        afile =
            (xAnimFile*)xMemAlloc(gActiveHeap, numX * numY * sizeof(void*) + sizeof(xAnimFile));
    }

    if (numX > 1 || numY > 1)
    {
        flags |= 0x4000;
    }

    if (**(U32**)rawData == 'QSPM')
    {
        flags |= 0x8000;
    }

    if (linkedList)
    {
        afile->Next = *linkedList;
        *linkedList = afile;
    }
    else
    {
        afile->Next = NULL;
    }

    afile->RawData = (void**)(afile + 1);

    for (i = 0; i < (S32)(numX * numY); i++)
    {
        afile->RawData[i] = rawData[i];
    }

    afile->Name = name;
    afile->ID = xStrHash(name);
    afile->FileFlags = flags;

    if (afile->FileFlags & 0x2000)
    {
        afile->Duration = 2.0f * ((afile->FileFlags & 0x8000) ?
                                      xMorphSeqDuration(*(xMorphSeqFile**)afile->RawData) :
                                      iAnimDuration(*afile->RawData));
    }
    else
    {
        afile->Duration = (afile->FileFlags & 0x8000) ?
                              xMorphSeqDuration(*(xMorphSeqFile**)afile->RawData) :
                              iAnimDuration(*afile->RawData);
    }

    afile->TimeOffset = 0.0f;
    afile->BoneCount = (flags & 0x8000) ? 0 : iAnimBoneCount(*rawData);
    afile->NumAnims[0] = numX;
    afile->NumAnims[1] = numY;
    afile->Physics = NULL;

    CalcPhys(afile);

    return afile;
}

// Create an animation file over one raw animation.
xAnimFile* xAnimFileNew(void* rawData, char* name, U32 flags, xAnimFile** linkedList)
{
    xAnimFile* afile;

    if (gxAnimUseGrowAlloc)
    {
        afile = (xAnimFile*)xMemGrowAlloc(gActiveHeap, sizeof(void*) + sizeof(xAnimFile));
    }
    else
    {
        afile = (xAnimFile*)xMemAlloc(gActiveHeap, sizeof(void*) + sizeof(xAnimFile));
    }

    if (*(U32*)rawData == 'QSPM')
    {
        flags |= 0x8000;
    }

    if (linkedList)
    {
        afile->Next = *linkedList;
        *linkedList = afile;
    }
    else
    {
        afile->Next = NULL;
    }

    afile->RawData = (void**)(afile + 1);
    afile->RawData[0] = rawData;
    afile->Name = name;
    afile->ID = xStrHash(name);
    afile->FileFlags = flags;

    if (afile->FileFlags & 0x2000)
    {
        afile->Duration = 2.0f * ((afile->FileFlags & 0x8000) ?
                                      xMorphSeqDuration(*(xMorphSeqFile**)afile->RawData) :
                                      iAnimDuration(*afile->RawData));
    }
    else
    {
        afile->Duration = (afile->FileFlags & 0x8000) ?
                              xMorphSeqDuration(*(xMorphSeqFile**)afile->RawData) :
                              iAnimDuration(*afile->RawData);
    }

    afile->TimeOffset = 0.0f;
    afile->BoneCount = (flags & 0x8000) ? 0 : iAnimBoneCount(rawData);
    afile->NumAnims[0] = 1;
    afile->NumAnims[1] = 1;
    afile->Physics = NULL;

    CalcPhys(afile);

    return afile;
}

// Give a file separate root-motion data.
void xAnimFileSetPhysics(xAnimFile* data, void* rawData)
{
    data->Physics = rawData;
}

// Play only part of a file: from timeOffset, for duration seconds (doubled for ping-pong files).
void xAnimFileSetTime(xAnimFile* data, F32 duration, F32 timeOffset)
{
    F32 rawDuration;

    if (data->FileFlags & 0x8000)
    {
        rawDuration = xMorphSeqDuration(*(xMorphSeqFile**)data->RawData);
    }
    else
    {
        rawDuration = iAnimDuration(*data->RawData);
    }

    if (timeOffset > rawDuration - 0.1f)
    {
        timeOffset = rawDuration - 0.1f;
    }

    data->TimeOffset = timeOffset;

    if (duration > rawDuration - timeOffset)
    {
        duration = rawDuration - timeOffset;
    }

    data->Duration = (data->FileFlags & 0x2000) ? 2.0f * duration : duration;

    CalcPhys(data);
}

// Work out a file's root motion as it moves from one pose (at startTime of startPose) to another.
void xAnimFileSetPoses(xAnimFile* data, void* startPose, F32 startTime, void* endPose,
                       F32 endTime)
{
    S32 i;
    void* root;
    F32 rootStart[4];
    F32 rootEnd[4];
    F32 poseStart[4];
    F32 poseEnd[4];
    F32 physStart[4];
    F32 physEnd[4];
    xQuat tempQ;

    if (data->Physics)
    {
        return;
    }

    if (data->FileFlags & 0x8000)
    {
        return;
    }

    root = *data->RawData;

    iAnimEval(root, data->TimeOffset, 1, (xVec3*)rootStart, &tempQ);
    rootStart[3] = CalcYaw(&tempQ);

    iAnimEval(root, data->TimeOffset + data->Duration, 1, (xVec3*)rootEnd, &tempQ);
    rootEnd[3] = CalcYaw(&tempQ);

    iAnimEval(startPose, startTime, 1, (xVec3*)poseStart, &tempQ);
    poseStart[3] = CalcYaw(&tempQ);

    iAnimEval(endPose, endTime, 1, (xVec3*)poseEnd, &tempQ);
    poseEnd[3] = CalcYaw(&tempQ);

    for (i = 0; i < 4; i++)
    {
        physStart[i] = rootStart[i] - poseStart[i];
        physEnd[i] = rootEnd[i] - poseEnd[i];

        if (data->FileFlags & (1 << i))
        {
            physStart[i] += poseStart[i];
            physEnd[i] += poseStart[i];
        }
    }

    CalcPhysFromKeys(data, rootStart, rootEnd, physStart, physEnd);
}

// Evaluate a file at a time into bone translations and rotations. With flag 4 the first entry
// is the root motion, taken from the physics data or the file's root motion, and the root bone is
// made relative to it.
void xAnimFileEval(xAnimFile* data, F32 time, F32* bilinear, U32 flags, xVec3* tran, xQuat* quat,
                   F32* yaw)
{
    S32 i;
    U32 numBones;
    U32 fileflags;
    F32 tmpyaw;
    xQuat tempQ;
    F32 bilerp[2];
    U32 biindex[2];
    U32 biplus[2];
    F32 tmpf;
    xQuat* q0;
    xVec3* t0;
    xQuat* q1;
    xVec3* t1;
    F32 rawTime;

    fileflags = data->FileFlags;

    time = (time < 0.0f) ? 0.0f : ((time < data->Duration) ? time : data->Duration);
    rawTime = xAnimFileRawTime(data, time);

    if (fileflags & 0x8000)
    {
        if (flags & 0x4)
        {
            if (data->Physics)
            {
                iAnimEval(data->Physics, rawTime, 0, tran, quat);
                *yaw = CalcYaw(quat);
            }
            else
            {
                tran->x = 0.0f;
                tran->y = 0.0f;
                tran->z = 0.0f;
                quat->v.x = 0.0f;
                quat->v.y = 0.0f;
                quat->v.z = 0.0f;
                quat->s = 1.0f;
                *yaw = 0.0f;
            }
        }

        return;
    }

    if (flags & 0x1)
    {
        numBones = 1;
    }
    else
    {
        numBones = data->BoneCount;
    }

    if (flags & 0x2)
    {
        numBones--;
    }

    if (numBones == 0)
    {
        return;
    }

    if (flags & 0x4)
    {
        tran++;
        quat++;
    }

    if (bilinear && (fileflags & 0x4000))
    {
        for (i = 0; i < 2; i++)
        {
            tmpf = bilinear[i];
            tmpf = (tmpf < 0.0f) ? 0.0f : ((tmpf < data->NumAnims[i] - 1) ? tmpf : data->NumAnims[i] - 1);
            F32 t = floor(tmpf);
            bilerp[i] = tmpf - t;
            biindex[i] = t;
            biplus[i] = (biindex[i] + 1 < data->NumAnims[i]) ? biindex[i] + 1 : biindex[i];
        }

        q0 = (xQuat*)(giAnimScratch + 0x1560);
        t0 = (xVec3*)((U8*)q0 + 0x410);

        if (bilerp[0] && bilerp[1])
        {
            q1 = (xQuat*)(giAnimScratch + 0x1C80);
            t1 = (xVec3*)((U8*)q1 + 0x410);

            iAnimEval(data->RawData[biindex[0] + biindex[1] * data->NumAnims[0]], rawTime, flags,
                      tran, quat);
            iAnimEval(data->RawData[biplus[0] + biindex[1] * data->NumAnims[0]], rawTime, flags, t0,
                      q0);
            iAnimBlend(bilerp[0], 1.0f, NULL, NULL, numBones, tran, quat, t0, q0, tran, quat);

            iAnimEval(data->RawData[biindex[0] + biplus[1] * data->NumAnims[0]], rawTime, flags, t0,
                      q0);
            iAnimEval(data->RawData[biplus[0] + biplus[1] * data->NumAnims[0]], rawTime, flags, t1,
                      q1);
            iAnimBlend(bilerp[0], 1.0f, NULL, NULL, numBones, t0, q0, t1, q1, t0, q0);
            iAnimBlend(bilerp[1], 1.0f, NULL, NULL, numBones, tran, quat, t0, q0, tran, quat);
        }
        else if (bilerp[0])
        {
            iAnimEval(data->RawData[biindex[0] + biindex[1] * data->NumAnims[0]], rawTime, flags,
                      tran, quat);
            iAnimEval(data->RawData[biplus[0] + biindex[1] * data->NumAnims[0]], rawTime, flags, t0,
                      q0);
            iAnimBlend(bilerp[0], 1.0f, NULL, NULL, numBones, tran, quat, t0, q0, tran, quat);
        }
        else if (bilerp[1])
        {
            iAnimEval(data->RawData[biindex[0] + biindex[1] * data->NumAnims[0]], rawTime, flags,
                      tran, quat);
            iAnimEval(data->RawData[biindex[0] + biplus[1] * data->NumAnims[0]], rawTime, flags, t0,
                      q0);
            iAnimBlend(bilerp[0], 1.0f, NULL, NULL, numBones, tran, quat, t0, q0, tran, quat);
        }
        else
        {
            iAnimEval(data->RawData[biindex[0] + biindex[1] * data->NumAnims[0]], rawTime, flags,
                      tran, quat);
        }
    }
    else
    {
        iAnimEval(data->RawData[0], rawTime, flags, tran, quat);
    }

    if (flags & 0x4)
    {
        if (data->Physics)
        {
            iAnimEval(data->Physics, rawTime, 0, tran - 1, quat - 1);
            tmpyaw = CalcYaw(quat - 1);
        }
        else
        {
            for (i = 0; i < 3; i++)
            {
                if (fileflags & (0x10 << i))
                {
                    (&tran[-1].x)[i] = data->PhysBase[i];
                }
                else if (fileflags & (0x100 << i))
                {
                    (&tran[-1].x)[i] = time * data->PhysDelta[i] + data->PhysBase[i];
                }
                else
                {
                    (&tran[-1].x)[i] = time * data->PhysDelta[i] + data->PhysBase[i] + (&tran->x)[i];
                }
            }

            if (fileflags & 0x80)
            {
                tmpyaw = data->PhysBase[3];
            }
            else if (fileflags & 0x800)
            {
                tmpyaw = time * data->PhysDelta[3] + data->PhysBase[3];
            }
            else
            {
                tmpyaw = time * data->PhysDelta[3] + data->PhysBase[3] + CalcYaw(quat);
            }

            quat[-1].v.x = 0.0f;
            quat[-1].v.y = sin(tmpyaw);
            quat[-1].v.z = 0.0f;
            quat[-1].s = cos(tmpyaw);
        }

        tran->x -= tran[-1].x;
        tran->y -= tran[-1].y;
        tran->z -= tran[-1].z;

        tempQ.v.x = -quat[-1].v.x;
        tempQ.v.y = -quat[-1].v.y;
        tempQ.v.z = -quat[-1].v.z;
        tempQ.s = quat[-1].s;
        xQuatMul(quat, quat, &tempQ);

        if (yaw)
        {
            *yaw = tmpyaw;
        }
    }
}

// The reciprocal of the longest blend in a blend-offset table (pairs of start time in ms and
// 1024ths of a second, ending with 0xFFFF).
static F32 CalcRecipBlendMax(U16* offset)
{
    F32 max = 0.0f;

    while (offset[0] != 0xFFFF)
    {
        F32 blend;

        if (offset[1] == 0)
        {
            blend = 0.0f;
        }
        else
        {
            blend = 1.0f / (offset[1] * (1.0f / 1024.0f));
        }

        blend = offset[0] * 0.001f + blend;

        if (blend > max)
        {
            max = blend;
        }

        offset += 2;
    }

    if (max == 0.0f)
    {
        return 0.0f;
    }

    return 1.0f / max;
}

// Add an effect to a state, keeping the state's effects sorted by start time.
xAnimEffect* xAnimStateNewEffect(xAnimState* state, U32 flags, F32 startTime, F32 endTime,
                                 U32 (*callback)(U32, xAnimActiveEffect*, xAnimSingle*, void*),
                                 U32 userDataSize)
{
    xAnimEffect* curr;
    xAnimEffect** prev;
    xAnimEffect* effect;

    if (gxAnimUseGrowAlloc)
    {
        effect = (xAnimEffect*)xMemGrowAlloc(gActiveHeap, userDataSize + sizeof(xAnimEffect));
    }
    else
    {
        effect = (xAnimEffect*)xMemAlloc(gActiveHeap, userDataSize + sizeof(xAnimEffect));
    }

    effect->Flags = flags;
    effect->StartTime = startTime;
    effect->EndTime = endTime;
    effect->Callback = callback;

    prev = &state->Effects;
    curr = state->Effects;

    while (curr && startTime > curr->StartTime)
    {
        prev = &curr->Next;
        curr = curr->Next;
    }

    effect->Next = curr;
    *prev = effect;

    return effect;
}

// Create an empty animation table and link it into a list.
xAnimTable* xAnimTableNew(char* name, xAnimTable** linkedList)
{
    xAnimTable* table;

    table = (xAnimTable*)xMemAlloc(gActiveHeap, sizeof(xAnimTable));

    if (linkedList)
    {
        table->Next = *linkedList;
        *linkedList = table;
    }
    else
    {
        table->Next = NULL;
    }

    table->Name = name;
    table->TransitionList = NULL;
    table->StateList = NULL;
    table->AnimIndex = 0;
    table->MorphIndex = 0;

    return table;
}

// Add a state to a table. The first state stays first; later ones go in after it.
xAnimState* xAnimTableNewState(xAnimTable* table, char* name, U32 flags, U32 userFlags, F32 speed,
                               F32* boneBlend, F32* timeSnap, F32 fadeRecip, U16* fadeOffset)
{
    xAnimState* state;

    if (gxAnimUseGrowAlloc)
    {
        state = (xAnimState*)xMemGrowAlloc(gActiveHeap, sizeof(xAnimState));
    }
    else
    {
        state = (xAnimState*)xMemAlloc(gActiveHeap, sizeof(xAnimState));
    }

    if (!table->StateList)
    {
        state->Next = NULL;
        table->StateList = state;
    }
    else
    {
        state->Next = table->StateList->Next;
        table->StateList->Next = state;
    }

    state->Name = name;
    state->ID = xStrHash(name);
    state->Flags = flags;
    state->UserFlags = userFlags;
    state->Speed = speed;
    state->Data = NULL;
    state->Effects = NULL;
    state->Default = NULL;
    state->List = NULL;
    state->BoneBlend = boneBlend;
    state->TimeSnap = timeSnap;
    state->FadeRecip = fadeOffset ? CalcRecipBlendMax(fadeOffset) : fadeRecip;
    state->FadeOffset = fadeOffset;

    return state;
}

// Whether the state already lists this transition.
static U32 StateHasTransition(xAnimState* state, xAnimTransition* tran)
{
    xAnimTransitionList* curr = state->List;

    while (curr)
    {
        if (curr->T == tran)
        {
            return TRUE;
        }

        curr = curr->Next;
    }

    return FALSE;
}

// Whether the state already has this default transition; counts the state's defaults.
static U32 DefaultHasTransition(xAnimState* state, xAnimTransition* tran, U32* stateCount)
{
    U32 add = 1;
    xAnimTransitionList* curr = state->Default;

    while (curr)
    {
        if (curr->T == tran)
        {
            return TRUE;
        }

        if (curr->T->Conditional == tran->Conditional && curr->T->UserFlags == tran->UserFlags)
        {
            add = 0;
        }

        curr = curr->Next;
    }

    *stateCount += add;
    return FALSE;
}

// Replace a default of the state that goes to the same destination with this transition.
static U32 DefaultOverride(xAnimState* state, xAnimTransition* tran)
{
    xAnimTransitionList* curr = state->Default;

    while (curr)
    {
        if (curr->T->Conditional == tran->Conditional && curr->T->UserFlags == tran->UserFlags)
        {
            curr->T = tran;
            return TRUE;
        }

        curr = curr->Next;
    }

    return FALSE;
}

// Add a transition to every listed source state. A default transition replaces the state's
// default with the same condition; an unconditional one goes last.
void xAnimTableAddTransition(xAnimTable* table, xAnimTransition* tran, char* source)
{
    U8* buffer = giAnimScratch;
    xAnimState** stateList = (xAnimState**)(giAnimScratch + 0x400);
    U32 i;
    U32 stateCount = 0;
    U32 allocCount = 0;
    U32 flags = tran->Flags;
    char* stateName;
    xAnimTransitionList* tlist;
    xAnimTransitionList* curr;

    stateName = xStrTokBuffer(source, " ,\t\n\r", buffer);

    while (stateName)
    {
        xAnimState* state = xAnimTableGetState(table, stateName);

        if (state)
        {
            if (flags & 0x10)
            {
                if (!DefaultHasTransition(state, tran, &stateCount))
                {
                    stateList[allocCount] = state;
                    allocCount++;
                }
            }
            else if (!StateHasTransition(state, tran))
            {
                stateList[allocCount] = state;
                allocCount++;
                stateCount++;
            }
        }

        stateName = xStrTokBuffer(NULL, " ,\t\n\r", buffer);
    }

    if (stateCount)
    {
        if (gxAnimUseGrowAlloc)
        {
            tlist = (xAnimTransitionList*)xMemGrowAlloc(gActiveHeap,
                                                         stateCount * sizeof(xAnimTransitionList));
        }
        else
        {
            tlist = (xAnimTransitionList*)xMemAlloc(gActiveHeap,
                                                     stateCount * sizeof(xAnimTransitionList));
        }
    }

    if (flags & 0x10)
    {
        for (i = 0; i < allocCount; i++)
        {
            if (!DefaultOverride(stateList[i], tran))
            {
                if (!tran->Conditional && stateList[i]->Default)
                {
                    curr = stateList[i]->Default;

                    while (curr->Next)
                    {
                        curr = curr->Next;
                    }

                    tlist->T = tran;
                    tlist->Next = NULL;
                    curr->Next = tlist;
                    tlist++;
                }
                else
                {
                    tlist->T = tran;
                    tlist->Next = stateList[i]->Default;
                    stateList[i]->Default = tlist;
                    tlist++;
                }
            }
        }
    }
    else
    {
        for (i = 0; i < allocCount; i++)
        {
            tlist->T = tran;
            tlist->Next = stateList[i]->List;
            stateList[i]->List = tlist;
            tlist++;
        }
    }
}

// Create a transition from the source states to dest and add it to the table.
xAnimTransition* xAnimTableNewTransition(xAnimTable* table, char* source, char* dest,
                                         xAnimTransitionConditionalCallback conditional,
                                         xAnimTransitionCallback callback, U32 flags, U32 userFlags,
                                         F32 srcTime, F32 destTime, U16 priority, U16 queuePriority,
                                         F32 blendRecip, U16* blendOffset)
{
    xAnimTransition* tran;

    if (gxAnimUseGrowAlloc)
    {
        tran = (xAnimTransition*)xMemGrowAlloc(gActiveHeap, sizeof(xAnimTransition));
    }
    else
    {
        tran = (xAnimTransition*)xMemAlloc(gActiveHeap, sizeof(xAnimTransition));
    }

    if (blendRecip != 0.0f)
    {
        blendRecip = 1.0f / blendRecip;
    }

    tran->Next = table->TransitionList;
    table->TransitionList = tran;
    tran->Dest = xAnimTableGetState(table, dest);
    tran->Conditional = conditional;
    tran->Callback = callback;
    tran->Flags = flags;
    tran->UserFlags = userFlags;
    tran->SrcTime = srcTime;
    tran->DestTime = destTime;
    tran->Priority = priority;
    tran->QueuePriority = queuePriority;
    tran->BlendRecip = blendOffset ? CalcRecipBlendMax(blendOffset) : blendRecip;
    tran->BlendOffset = blendOffset;

    xAnimTableAddTransition(table, tran, source);

    return tran;
}

// Give every state named in a list (separated by spaces, commas or line breaks) the file.
void xAnimTableAddFile(xAnimTable* table, xAnimFile* file, char* states)
{
    U8* buffer = giAnimScratch;
    char* stateName = xStrTokBuffer(states, " ,\t\n\r", buffer);

    while (stateName != NULL)
    {
        xAnimTableAddFileID(table, file, xStrHash(stateName));
        stateName = xStrTokBuffer(NULL, " ,\t\n\r", buffer);
    }
}

// Give the state with this ID the file, and mark its single in the table's masks.
xAnimState* xAnimTableAddFileID(xAnimTable* table, xAnimFile* file, U32 stateID)
{
    xAnimState* state = xAnimTableGetStateID(table, stateID);

    if (state)
    {
        state->Data = file;

        if (file->FileFlags & 0x8000)
        {
            table->MorphIndex |= 1 << (state->Flags & 0xF);
        }
        else
        {
            table->AnimIndex |= 1 << (state->Flags & 0xF);
        }
    }

    return state;
}

// Find a state by its name hash.
xAnimState* xAnimTableGetStateID(xAnimTable* table, U32 ID)
{
    xAnimState* curr;

    curr = table->StateList;

    while (curr)
    {
        if (curr->ID == ID)
        {
            return curr;
        }

        curr = curr->Next;
    }

    return NULL;
}

// Unlink every effect, in every state, that uses the given callback.
void xAnimTableRemoveEffectByCB(xAnimTable* table,
                                U32 (*callback)(U32, xAnimActiveEffect*, xAnimSingle*, void*))
{
    xAnimState* curr;

    for (curr = table->StateList; curr; curr = curr->Next)
    {
        xAnimEffect* prev = NULL;
        xAnimEffect* effect = curr->Effects;

        while (effect)
        {
            if (effect->Callback == callback)
            {
                if (prev)
                {
                    prev->Next = effect->Next;
                }
                else
                {
                    curr->Effects = effect->Next;
                }
            }
            else
            {
                prev = effect;
            }

            effect = effect->Next;
        }
    }
}

// Find a state by name.
xAnimState* xAnimTableGetState(xAnimTable* table, char* name)
{
    return xAnimTableGetStateID(table, xStrHash(name));
}

// Put an effect in the first free slot of the single's active list.
static inline void EffectActiveInsert(xAnimSingle* single, xAnimActiveEffect* active)
{
    U32 index = 0;
    U32 count = single->ActiveCount;
    xAnimActiveEffect* curr = &single->ActiveList[0];

    while (index < count && curr->Effect)
    {
        curr++;
        index++;
    }

    *curr = *active;

    if (index < count - 1)
    {
        (curr + 1)->Effect = NULL;
    }
}

// Take an effect out of the active list by moving the last active effect into its slot.
static inline void EffectActiveRemove(xAnimActiveEffect* active, U32 index, U32 count)
{
    count--;

    if (index == count || !(active + 1)->Effect)
    {
        active->Effect = NULL;
    }
    else
    {
        xAnimActiveEffect* curr = active;

        while (index < count && (curr + 1)->Effect)
        {
            curr++;
            index++;
        }

        *active = *curr;
        curr->Effect = NULL;
    }
}

// Whether the effect is in the single's active list.
static inline U32 EffectPlaying(xAnimSingle* single, xAnimEffect* effect)
{
    U32 index = 0;

    while (index < single->ActiveCount && single->ActiveList[index].Effect)
    {
        if (single->ActiveList[index].Effect == effect)
        {
            return TRUE;
        }

        index++;
    }

    return FALSE;
}

// Start the effects a state begins with when it starts partway through.
static void EffectSingleStart(xAnimSingle* single)
{
    xAnimEffect* effect = single->State->Effects;
    xAnimActiveEffect tempActive;
    F32 time = single->Time;
    U32 flags;

    while (effect && effect->StartTime < time)
    {
        flags = effect->Flags;

        if (flags & 0x1)
        {
            if (flags & 0x4)
            {
                tempActive.Effect = effect;
                tempActive.Handle = effect->Callback(1, &tempActive, single, single->Play->Object);

                flags = effect->Flags;

                if (flags & 0x2)
                {
                    if (effect->EndTime <= time)
                    {
                        effect->Callback(3, &tempActive, single, single->Play->Object);
                    }
                    else
                    {
                        EffectActiveInsert(single, &tempActive);
                    }
                }
                else if (flags & 0x20)
                {
                    EffectActiveInsert(single, &tempActive);
                }
            }
            else if ((flags & 0x2) && !(flags & 0x10) && effect->EndTime > time)
            {
                tempActive.Effect = effect;
                tempActive.Handle = effect->Callback(1, &tempActive, single, single->Play->Object);

                EffectActiveInsert(single, &tempActive);
            }
        }

        effect = effect->Next;
    }

    single->Effect = effect;
}

// Update the running effects and end the ones whose time is up.
static void EffectSingleDuration(xAnimSingle* single)
{
    F32 time = single->Time;
    xAnimActiveEffect* alist = single->ActiveList;
    U32 index = 0;
    U32 count = single->ActiveCount;

    while (index < count && alist->Effect)
    {
        if (!(alist->Effect->Flags & 0x20) && alist->Effect->EndTime <= time)
        {
            alist->Effect->Callback(3, alist, single, single->Play->Object);

            EffectActiveRemove(alist, index, count);
        }
        else
        {
            alist->Effect->Callback(2, alist, single, single->Play->Object);

            alist++;
            index++;
        }
    }
}

// Start the effects whose start time the animation has reached.
static void EffectSingleRun(xAnimSingle* single)
{
    xAnimEffect* effect = single->Effect;
    xAnimActiveEffect tempActive;
    F32 time = single->Time;
    U32 flags;

    while (effect && effect->StartTime <= time)
    {
        flags = effect->Flags;

        if ((flags & 0x1) && (!(flags & 0x20) || !EffectPlaying(single, effect)))
        {
            tempActive.Effect = effect;
            tempActive.Handle = effect->Callback(1, &tempActive, single, single->Play->Object);

            if (flags & 0x2)
            {
                if (effect->EndTime <= time)
                {
                    effect->Callback(3, &tempActive, single, single->Play->Object);
                }
                else
                {
                    EffectActiveInsert(single, &tempActive);
                }
            }
            else if (flags & 0x20)
            {
                EffectActiveInsert(single, &tempActive);
            }
        }

        effect = effect->Next;
    }

    single->Effect = effect;
}

// At the loop point: finish this pass's effects, end the non-looping ones and rewind.
static void EffectSingleLoop(xAnimSingle* single)
{
    EffectSingleRun(single);

    xAnimActiveEffect* alist = single->ActiveList;
    U32 index = 0;
    U32 count = single->ActiveCount;

    while (index < count && alist[index].Effect)
    {
        if (!(alist[index].Effect->Flags & 0x20))
        {
            alist[index].Effect->Callback(3, &alist[index], single, single->Play->Object);

            EffectActiveRemove(&alist[index], index, count);
        }
        else
        {
            index++;
        }
    }

    xAnimEffect* effect = single->State->Effects;

    while (effect && effect->StartTime < 0.0f)
    {
        effect = effect->Next;
    }

    single->Effect = effect;
}

// End every running effect, and fire the ones flagged to run even when the state is cut short.
static void EffectSingleStop(xAnimSingle* single)
{
    if (!single->State || single->LastTime == -1.0f)
    {
        return;
    }

    for (U32 i = 0; i < single->ActiveCount && single->ActiveList[i].Effect; i++)
    {
        if (single->ActiveList[i].Effect->Flags & 0x2)
        {
            single->ActiveList[i].Effect->Callback(3, &single->ActiveList[i], single,
                                                   single->Play->Object);
        }
    }

    single->ActiveList->Effect = NULL;

    for (xAnimEffect* effect = single->Effect; effect; effect = effect->Next)
    {
        xAnimActiveEffect tempActive;

        if ((effect->Flags & 0x9) == 0x9)
        {
            tempActive.Effect = effect;
            tempActive.Handle = effect->Callback(1, &tempActive, single, single->Play->Object);

            if (effect->Flags & 0x2)
            {
                effect->Callback(3, &tempActive, single, single->Play->Object);
            }
        }
    }

    single->Effect = NULL;
}

// Sample the single at its current time and store the root motion since the last sample in
// tran/quat/yaw (unless lastOnly), then remember this sample as the last one.
static void CalcLastAndDeltas(xAnimSingle* single, xVec3* tran, xQuat* quat, F32* yaw,
                              U32 lastOnly)
{
    xQuat lastQ;
    xVec3 tempV[2];
    xQuat tempQ[2];
    F32 tempYaw[2];

    xAnimFileEval(single->State->Data, single->Time, single->BilinearLerp, 5, tempV, tempQ,
                  tempYaw);

    if (!lastOnly)
    {
        xVec3Sub(tran, &tempV[0], &single->LastTranslation);

        lastQ.v.x = -single->LastQuat.v.x;
        lastQ.v.y = -single->LastQuat.v.y;
        lastQ.v.z = -single->LastQuat.v.z;
        lastQ.s = single->LastQuat.s;
        xQuatMul(quat, &lastQ, &tempQ[0]);

        *yaw = tempYaw[0] - single->LastYaw;

        while (*yaw > 3.1415927f)
        {
            *yaw -= 6.2831855f;
        }

        while (*yaw < -3.1415927f)
        {
            *yaw += 6.2831855f;
        }

        if (!(single->SingleFlags & 0x8000))
        {
            tran[1] = tempV[1];
            quat[1] = tempQ[1];
            yaw[1] = tempYaw[1];
        }
    }

    single->LastTranslation = tempV[0];
    single->LastQuat = tempQ[0];
    single->LastYaw = tempYaw[0];
}

// Pick the time a transition starts its destination at: mirrored for a reversed destination
// when the transition keeps the time, else its DestTime (a random time for random-start states).
static inline void TransitionTimeInit(xAnimSingle* single, xAnimTransition* tran)
{
    if (tran->Flags & 0x20)
    {
        if ((tran->Dest->Data->FileFlags ^ single->State->Data->FileFlags) & 0x1000)
        {
            single->Time = tran->Dest->Data->Duration - single->Time;
        }
    }
    else
    {
        single->Time = ((tran->Dest->Flags & 0x100) && tran->DestTime == 0.0f) ?
                           single->State->Data->Duration * iurand() :
                           tran->DestTime;
    }

    single->LastTime = single->Time;
}

// Hold a non-looping single at its end, then take the root motion since the last sample.
static void StopUpdate(xAnimSingle* single, xVec3* tran, xQuat* quat, F32* yaw)
{
    if (single->Time > single->State->Data->Duration)
    {
        single->Time = single->State->Data->Duration;
        single->CurrentSpeed = 0.0f;
    }

    CalcLastAndDeltas(single, tran, quat, yaw, FALSE);
}

// Wrap a looping single past its end: take the motion up to the end, run the loop point's
// effects, restart from zero and add the motion of the time left over.
static void LoopUpdate(xAnimSingle* single, xVec3* tran, xQuat* quat, F32* yaw)
{
    F32 partialYaw[2];
    xQuat partialQuat[2];
    xVec3 partialTran[2];
    F32 duration = single->State->Data->Duration;

    if (single->Time > duration)
    {
        F32 remaining = single->Time - duration;

        single->Time = duration;
        CalcLastAndDeltas(single, partialTran, partialQuat, partialYaw, FALSE);
        EffectSingleLoop(single);

        single->Time = 0.0f;
        CalcLastAndDeltas(single, NULL, NULL, NULL, TRUE);

        single->Time = remaining;
        CalcLastAndDeltas(single, tran, quat, yaw, FALSE);

        xVec3AddTo(tran, &partialTran[0]);
        xQuatMul(quat, quat, &partialQuat[0]);
        *yaw += partialYaw[0];
    }
    else
    {
        CalcLastAndDeltas(single, tran, quat, yaw, FALSE);
    }
}

// Switch a single (and drop its blend) to a state, ending the old state's effects.
void xAnimPlaySetState(xAnimSingle* single, xAnimState* state, F32 startTime)
{
    EffectSingleStop(single);

    if (single->Blend)
    {
        EffectSingleStop(single->Blend);
        single->Blend->State = NULL;
    }

    single->State = state;

    if (state)
    {
        single->Time = (state->Flags & 0x100 && startTime == 0.0f) ?
                           state->Data->Duration * iurand() :
                           startTime;
        single->CurrentSpeed = state->Speed;
        single->BilinearLerp[0] = 0.0f;
        single->BilinearLerp[1] = 0.0f;
        single->Effect = NULL;
        memset(single->ActiveList, 0, single->ActiveCount * sizeof(xAnimActiveEffect));
        single->LastTime = -1.0f;
        memset(&single->LastTranslation, 0, sizeof(xVec3));
        memset(&single->LastQuat, 0, sizeof(xQuat));
        single->LastYaw = 0.0f;
        single->Sync = NULL;

        if (single->Tran && single->Tran->Flags & 0x2)
        {
            xMemPoolFree(&sxAnimTempTranPool, single->Tran);
        }

        single->Tran = NULL;
        single->BlendFactor = 0.0f;
    }
}

// Advance one single by timeDelta: take a queued sync or default transition when its time
// comes, finish a blend when the blend time runs out, step the time, effects and root motion,
// and return the frame's root motion in tran/quat/yaw (the root [0] and its child [1]).
static void SingleUpdate(xAnimSingle* single, F32 timeDelta, xVec3* tran, xQuat* quat, F32* yaw,
                         U32)
{
    U32 foundBlendstop = 0;
    U32 firstStep = 1;
    xAnimTransition* foundTransition = NULL;
    xAnimSingle* bl = NULL;
    F32 tranDelta = 0.0f;
    F32 blendDelta = 0.0f;
    xVec3 blendTran[2];
    xVec3 partialTran[2];
    xQuat blendQuat[2];
    xQuat partialQuat[2];
    F32 blendYaw[2];
    F32 partialYaw[2];
    F32 singleTime;
    F32 duration;
    void* object;

    if (!single->State)
    {
        return;
    }

    object = single->Play->Object;

    if (single->LastTime == -1.0f)
    {
        CalcLastAndDeltas(single, NULL, NULL, NULL, TRUE);
        EffectSingleStart(single);
    }

    single->LastTime = single->Time;
    singleTime = timeDelta * single->CurrentSpeed + single->Time;

    if (single->Blend && single->Blend->State)
    {
        bl = single->Blend;

        if (bl->LastTime == -1.0f)
        {
            CalcLastAndDeltas(bl, NULL, NULL, NULL, TRUE);
            EffectSingleStart(bl);
        }

        bl->LastTime = bl->Time;
    }

    duration = single->State->Data->Duration;

    if (single->Sync)
    {
        F32 timeCmp;

        if ((timeCmp = single->Sync->SrcTime) != 0.0f)
        {
            if (timeCmp > duration)
            {
                timeCmp = duration;
            }

            if (single->LastTime <= timeCmp && singleTime >= timeCmp)
            {
                foundTransition = single->Sync;
                tranDelta = (singleTime - timeCmp) / single->CurrentSpeed;
                timeDelta = timeDelta - tranDelta;

                if (timeDelta < 0.0f)
                {
                    timeDelta = 0.0f;
                }
            }
            else
            {
                timeCmp += duration;

                if (single->LastTime <= timeCmp && singleTime >= timeCmp)
                {
                    foundTransition = single->Sync;
                    tranDelta = (singleTime - timeCmp) / single->CurrentSpeed;
                    timeDelta = timeDelta - tranDelta;

                    if (timeDelta < 0.0f)
                    {
                        timeDelta = 0.0f;
                    }
                }
            }
        }
        else if (!bl)
        {
            foundTransition = single->Sync;
            tranDelta = timeDelta;
            timeDelta = 0.0f;
        }
    }
    else if ((single->State->Flags & 0x30) == 0x20)
    {
        F32 timeCmp = single->State->Default->T->SrcTime;

        if (timeCmp == 0.0f || timeCmp > duration)
        {
            timeCmp = duration;
        }

        if (singleTime >= timeCmp && (!(single->State->Default->T->Flags & 0x4) || !bl))
        {
            xAnimTransitionList* curr = single->State->Default;

            while (curr && curr->T->Conditional &&
                   !curr->T->Conditional(curr->T, single, object))
            {
                curr = curr->Next;
            }

            if (!curr)
            {
                fprintf(stderr, "State \"%s\" no default conditionals true!\n",
                        single->State->Name);
                curr = single->State->Default;
            }

            foundTransition = curr->T;

            if (single->LastTime < timeCmp)
            {
                tranDelta = (singleTime - timeCmp) / single->CurrentSpeed;
                timeDelta = timeDelta - tranDelta;

                if (timeDelta < 0.0f)
                {
                    timeDelta = 0.0f;
                }
            }
            else
            {
                tranDelta = timeDelta;
                timeDelta = 0.0f;
            }
        }
    }

    if (single->BlendFactor != 0.0f)
    {
        F32 recip;

        if (single->Tran)
        {
            recip = single->Tran->BlendRecip;
        }
        else
        {
            recip = single->State->FadeRecip;
        }

        if (recip * (single->BlendFactor + timeDelta) > 1.0f)
        {
            foundBlendstop = 1;
            blendDelta = (single->BlendFactor + timeDelta) - (1.0f / recip);
            timeDelta = timeDelta - blendDelta;

            if (timeDelta < 0.0f)
            {
                timeDelta = 0.0f;
            }

            if (blendDelta < 0.0f)
            {
                blendDelta = 0.01f;
            }
        }
    }

    do
    {
        if (!firstStep)
        {
            if (foundBlendstop)
            {
                single->BlendFactor = 0.0f;

                if (bl)
                {
                    EffectSingleStop(bl);
                    bl->State = NULL;
                    bl = NULL;

                    if (single->Tran && single->Tran->Flags & 0x2)
                    {
                        xMemPoolFree(&sxAnimTempTranPool, single->Tran);
                    }

                    single->Tran = NULL;
                }
                else if (single->Tran)
                {
                    if (single->Tran && single->Tran->Flags & 0x2)
                    {
                        xMemPoolFree(&sxAnimTempTranPool, single->Tran);
                    }

                    single->Tran = NULL;
                }
                else
                {
                    EffectSingleStop(single);
                    single->State = NULL;
                    return;
                }

                timeDelta = blendDelta;
                foundBlendstop = FALSE;
            }
            else
            {
                if (bl)
                {
                    EffectSingleStop(bl);
                    bl->State = NULL;
                    bl = NULL;
                    single->BlendFactor = 0.0f;
                }

                if (foundTransition->BlendRecip == 0.0f || !single->Blend)
                {
                    EffectSingleStop(single);

                    if (single->Tran && single->Tran->Flags & 0x2)
                    {
                        xMemPoolFree(&sxAnimTempTranPool, single->Tran);
                    }

                    single->Tran = NULL;
                }
                else
                {
                    bl = single->Blend;
                    bl->State = single->State;
                    bl->Time = single->Time;
                    bl->CurrentSpeed = single->CurrentSpeed;
                    bl->BilinearLerp[0] = single->BilinearLerp[0];
                    bl->BilinearLerp[1] = single->BilinearLerp[1];
                    bl->Effect = single->Effect;
                    bl->LastTime = single->LastTime;
                    bl->LastTranslation = single->LastTranslation;
                    bl->LastQuat = single->LastQuat;
                    bl->LastYaw = single->LastYaw;

                    memcpy(bl->ActiveList, single->ActiveList,
                           single->ActiveCount * sizeof(xAnimActiveEffect));
                    single->ActiveList->Effect = NULL;

                    if (single->Tran && single->Tran->Flags & 0x2)
                    {
                        xMemPoolFree(&sxAnimTempTranPool, single->Tran);
                    }

                    single->Tran = foundTransition;
                    single->BlendFactor = 0.0000001f;
                }

                TransitionTimeInit(single, foundTransition);
                single->State = foundTransition->Dest;
                single->CurrentSpeed = single->State->Speed;
                single->BilinearLerp[0] = 0.0f;
                single->BilinearLerp[1] = 0.0f;
                single->Sync = NULL;
                CalcLastAndDeltas(single, NULL, NULL, NULL, TRUE);
                EffectSingleStart(single);

                if (foundTransition->Callback)
                {
                    foundTransition->Callback(foundTransition, single, single->Play->Object);
                }

                timeDelta = tranDelta;
                foundTransition = NULL;
            }
        }

        single->Time = timeDelta * single->CurrentSpeed + single->Time;

        if (single->BlendFactor != 0.0f)
        {
            single->BlendFactor += timeDelta;
        }

        if ((single->State->Flags & 0x30) == 0x10)
        {
            LoopUpdate(single, partialTran, partialQuat, partialYaw);
        }
        else
        {
            StopUpdate(single, partialTran, partialQuat, partialYaw);
        }

        EffectSingleRun(single);

        if (bl)
        {
            if ((bl->State->Flags & 0x30) == 0x10)
            {
                LoopUpdate(bl, blendTran, blendQuat, blendYaw);
            }
            else
            {
                StopUpdate(bl, blendTran, blendQuat, blendYaw);
            }

            EffectSingleRun(bl);

            // Keep the larger root motion of the two singles.
            if (fabsf(blendTran[0].x) > fabsf(partialTran[0].x))
            {
                partialTran[0].x = blendTran[0].x;
            }

            if (fabsf(blendTran[0].y) > fabsf(partialTran[0].y))
            {
                partialTran[0].y = blendTran[0].y;
            }

            if (fabsf(blendTran[0].z) > fabsf(partialTran[0].z))
            {
                partialTran[0].z = blendTran[0].z;
            }

            if (fabsf(blendYaw[0]) > fabsf(partialYaw[0]))
            {
                partialYaw[0] = blendYaw[0];
            }

            if (xQuatLength2(&blendQuat[0]) > xQuatLength2(&partialQuat[0]))
            {
                partialQuat[0] = blendQuat[0];
            }
        }

        if (firstStep)
        {
            firstStep = FALSE;
            tran[0] = partialTran[0];
            quat[0] = partialQuat[0];
            *yaw = partialYaw[0];
        }
        else
        {
            xVec3AddTo(&tran[0], &partialTran[0]);
            xQuatMul(&quat[0], &quat[0], &partialQuat[0]);
            *yaw += partialYaw[0];
        }
    } while (foundBlendstop || foundTransition);

    if (!single->Tran && single->BlendFactor == 0.0f)
    {
        if ((single->State->Flags & 0x30) == 0x30)
        {
            if (single->State->Flags & 0x200)
            {
                if (single->Time >= duration)
                {
                    single->BlendFactor = 0.0000001f;
                }
            }
            else if (single->State->FadeRecip * (duration - single->Time) < 1.0f)
            {
                single->BlendFactor = 0.0000001f;
            }
        }
    }

    EffectSingleDuration(single);

    if (bl)
    {
        EffectSingleDuration(bl);
    }

    if (bl)
    {
        F32 lerp = single->BlendFactor * single->Tran->BlendRecip;

        if (lerp > 1.0f)
        {
            lerp = 1.0f;
        }

        tran[1].x = lerp * (partialTran[1].x - blendTran[1].x) + blendTran[1].x;
        tran[1].y = lerp * (partialTran[1].y - blendTran[1].y) + blendTran[1].y;
        tran[1].z = lerp * (partialTran[1].z - blendTran[1].z) + blendTran[1].z;
        xQuatSlerp(&quat[1], &blendQuat[1], &partialQuat[1], lerp);
    }
    else
    {
        tran[1] = partialTran[1];
        quat[1] = partialQuat[1];
    }
}

// Attach a play to an object, a table and a model, and start the table's first state.
void xAnimPlaySetup(xAnimPlay* play, void* object, xAnimTable* table, xModelInstance* modelInst)
{
    play->BoneCount = modelInst->BoneCount;
    play->Object = object;
    play->Table = table;
    memset(play->Translate, 0, sizeof(play->Translate));
    memset(play->Quat, 0, sizeof(play->Quat));
    play->Yaw = 0.0f;
    play->ModelInst = modelInst;

    modelInst->Anim = play;
    modelInst->Flags |= 0x104;

    if (table->MorphIndex)
    {
        modelInst->Flags |= 0x80;
    }

    for (S32 i = 0; i < play->NumSingle; i++)
    {
        play->Single[i].SingleFlags = (1 << i & table->MorphIndex) ? 0x8000 : 0x1;
        play->Single[i].State = NULL;
        play->Single[i].Tran = NULL;

        if (play->Single[i].Blend)
        {
            play->Single[i].Blend->State = NULL;
        }
    }

    xAnimPlaySetState(play->Single, table->StateList, 0.0f);
}

// Run every state's transition conditionals and start the highest-priority transition found
// for each single.
void xAnimPlayChooseTransition(xAnimPlay* play)
{
    U32 i;
    void* object = play->Object;
    xAnimTransition** list = (xAnimTransition**)giAnimScratch;
    xAnimTransition** found;
    xAnimTransitionList* curr;

    memset(list, 0, play->NumSingle * sizeof(xAnimTransition*));

    for (i = 0; i < play->NumSingle; i++)
    {
        if (play->Single[i].State)
        {
            curr = play->Single[i].State->List;

            while (curr)
            {
                if (curr->T->Conditional(curr->T, &play->Single[i], object))
                {
                    found = &list[curr->T->Dest->Flags & 0xF];

                    if (!*found || curr->T->Priority > (*found)->Priority)
                    {
                        *found = curr->T;
                    }
                }

                curr = curr->Next;
            }
        }
    }

    for (i = 0; i < play->NumSingle; i++)
    {
        if (list[i] &&
            (!play->Single[i].Sync || list[i]->Priority > play->Single[i].Sync->QueuePriority))
        {
            xAnimPlayStartTransition(play, list[i]);
        }
    }
}

// Start a transition now, or queue it as the single's sync transition when it waits for a
// source time. A transition with a blend time hands the old state to the blend single.
void xAnimPlayStartTransition(xAnimPlay* play, xAnimTransition* transition)
{
    xAnimSingle* single = &play->Single[transition->Dest->Flags & 0xF];
    xAnimSingle* bl = single->Blend;

    if (transition->SrcTime != 0.0f || (transition->Flags & 0x4 && bl && bl->State))
    {
        single->Sync = transition;
        return;
    }

    if (bl && bl->State)
    {
        EffectSingleStop(bl);
        bl->State = NULL;
        single->BlendFactor = 0.0f;
    }

    if (transition->BlendRecip == 0.0f || !single->Blend)
    {
        EffectSingleStop(single);

        if (single->Tran && single->Tran->Flags & 0x2)
        {
            xMemPoolFree(&sxAnimTempTranPool, single->Tran);
        }

        single->Tran = NULL;
    }
    else
    {
        if (single->State)
        {
            bl->State = single->State;
            bl->Time = single->Time;
            bl->CurrentSpeed = single->CurrentSpeed;
            bl->BilinearLerp[0] = single->BilinearLerp[0];
            bl->BilinearLerp[1] = single->BilinearLerp[1];
            bl->Effect = single->Effect;
            bl->LastTime = single->LastTime;
            bl->LastTranslation = single->LastTranslation;
            bl->LastQuat = single->LastQuat;
            bl->LastYaw = single->LastYaw;

            memcpy(bl->ActiveList, single->ActiveList,
                   single->ActiveCount * sizeof(xAnimActiveEffect));
            single->ActiveList[0].Effect = NULL;
        }

        if (single->Tran && single->Tran->Flags & 0x2)
        {
            xMemPoolFree(&sxAnimTempTranPool, single->Tran);
        }

        single->Tran = transition;
        single->BlendFactor = 0.0000001f;
    }

    TransitionTimeInit(single, transition);
    single->State = transition->Dest;
    single->CurrentSpeed = single->State->Speed;
    single->BilinearLerp[0] = 0.0f;
    single->BilinearLerp[1] = 0.0f;
    single->Sync = NULL;
    CalcLastAndDeltas(single, NULL, NULL, NULL, TRUE);
    EffectSingleStart(single);

    if (transition->Callback)
    {
        transition->Callback(transition, single, single->Play->Object);
    }
}

// Update every single; the first single's root motion becomes the play's.
void xAnimPlayUpdate(xAnimPlay* play, F32 timeDelta)
{
    F32 tempYaw[2];
    xVec3 tempTranslate[2];
    xQuat tempQuat[2];

    for (U32 i = 0; i < play->NumSingle; i++)
    {
        SingleUpdate(&play->Single[i], timeDelta, tempTranslate, tempQuat, tempYaw, TRUE);

        if (i == 0)
        {
            play->Translate[0] = tempTranslate[0];
            play->Translate[1] = tempTranslate[1];
            play->Quat[0] = tempQuat[0];
            play->Quat[1] = tempQuat[1];
            play->Yaw = tempYaw[0];
        }
    }
}

// Evaluate a single's bones (not the root) into tran/quat, blended with its blend single.
static inline void SingleEval(xAnimSingle* single, xVec3* tran, xQuat* quat)
{
    if (single->Play->BoneCount > 1)
    {
        xAnimFileEval(single->State->Data, single->Time, single->BilinearLerp, 0x2, tran + 1,
                      quat + 1, NULL);

        if (single->Blend && single->Blend->State)
        {
            xQuat* qbuf = (xQuat*)(giAnimScratch + 0xE40);
            xVec3* vbuf = (xVec3*)(qbuf + 0x41);

            xAnimFileEval(single->Blend->State->Data, single->Blend->Time,
                          single->Blend->BilinearLerp, 0x2, vbuf, qbuf, NULL);

            iAnimBlend(single->BlendFactor, single->Tran->BlendRecip, single->Tran->BlendOffset,
                       NULL, single->Play->BoneCount - 1, vbuf, qbuf, tran + 1, quat + 1, tran + 1,
                       quat + 1);
        }
    }
}

// Evaluate and blend all singles into the bone pose, put the play's root motion in the root
// bone and build the model's bone matrices.
void xAnimPlayEval(xAnimPlay* play)
{
    U32 i;
    U32 bone;
    xQuat* quatresult = (xQuat*)giAnimScratch;
    xVec3* tranresult = (xVec3*)((U8*)quatresult + 0x410);

    if (play->BoneCount > 1)
    {
        xQuat* quatblend = (xQuat*)((U8*)quatresult + 0x720);
        xVec3* tranblend = (xVec3*)((U8*)quatblend + 0x410);

        SingleEval(play->Single, tranresult, (xQuat*)giAnimScratch);

        for (i = 1; i < play->NumSingle; i++)
        {
            xAnimSingle* si = &play->Single[i];

            if (!si->State || si->SingleFlags & 0x8000)
            {
                continue;
            }

            F32 blendF = 1.0f;
            F32 blendR = 1.0f;
            U16* blendO = NULL;

            SingleEval(si, tranblend, quatblend);

            if ((!si->Blend || !si->Blend->State) && si->BlendFactor)
            {
                if (si->Tran)
                {
                    blendF = si->BlendFactor;
                    blendR = si->Tran->BlendRecip;
                    blendO = si->Tran->BlendOffset;
                }
                else
                {
                    blendF = -si->BlendFactor;
                    blendR = si->State->FadeRecip;
                    blendO = si->State->FadeOffset;
                }
            }

            if ((si->SingleFlags & 0x3) == 2)
            {
                iAnimBlend(blendF, blendR, blendO, si->State->BoneBlend, play->BoneCount - 1,
                           tranblend + 1, quatblend + 1, NULL, NULL, tranblend + 1, quatblend + 1);

                for (bone = 1; bone < play->BoneCount; bone++)
                {
                    tranresult[bone].x += tranblend[bone].x;
                    tranresult[bone].y += tranblend[bone].y;
                    tranresult[bone].z += tranblend[bone].z;
                    xQuatMul(&quatresult[bone], &quatresult[bone], &quatblend[bone]);
                }
            }
            else
            {
                iAnimBlend(blendF, blendR, blendO, si->State->BoneBlend, play->BoneCount - 1,
                           tranresult + 1, quatresult + 1, tranblend + 1, quatblend + 1,
                           tranresult + 1, quatresult + 1);
            }
        }
    }

    xVec3Copy(tranresult, &play->Translate[1]);
    *quatresult = play->Quat[1];

    iModelAnimMatrices(play->ModelInst->Data, quatresult, tranresult, &play->ModelInst->Mat[1]);
}

// Pool callback: lay out a newly allocated play like the pool's template play (its singles,
// their blend singles and their active effect lists all follow the play in one block).
void xAnimPoolCB(xMemPool* pool, void* data)
{
    S32 i;
    xAnimPlay* clone = (xAnimPlay*)data;
    xAnimPlay* play = (xAnimPlay*)pool->Buffer;
    xAnimSingle* clonesingle = (xAnimSingle*)(clone + 1);
    xAnimSingle* currsingle;
    xAnimActiveEffect* curract;

    clone->NumSingle = play->NumSingle;
    clone->Single = clonesingle;

    clonesingle += play->NumSingle;

    for (i = 0; i < play->NumSingle; i++)
    {
        clone->Single[i].Play = clone;

        if (play->Single[i].Blend)
        {
            clone->Single[i].Blend = clonesingle;

            clonesingle->Blend = NULL;
            clonesingle->Play = clone;

            clonesingle++;
        }
        else
        {
            clone->Single[i].Blend = NULL;
        }
    }

    curract = (xAnimActiveEffect*)clonesingle;

    for (i = 0; i < clone->NumSingle; i++)
    {
        clonesingle = &clone->Single[i];
        currsingle = &play->Single[i];

        while (clonesingle)
        {
            clonesingle->ActiveCount = currsingle->ActiveCount;

            if (currsingle->ActiveCount)
            {
                clonesingle->ActiveList = curract;

                curract += currsingle->ActiveCount;
            }
            else
            {
                clonesingle->ActiveList = NULL;
            }

            currsingle = currsingle->Blend;
            clonesingle = clonesingle->Blend;
        }
    }

    clone->Pool = pool;
}

#define ADD_4_BITS(x) (((x) & 1) + (((x) >> 1) & 1) + (((x) >> 2) & 1) + (((x) >> 3) & 1))

// Make a pool of count plays with the given number of singles; blendFlags marks the singles
// that get a blend single, and every single gets room for effectMax active effects.
void xAnimPoolInit(xMemPool* pool, U32 count, U32 singles, U32 blendFlags, U32 effectMax)
{
    effectMax += effectMax & 1;

    U32 size =
        (effectMax * sizeof(xAnimActiveEffect) + sizeof(xAnimSingle)) *
            (ADD_4_BITS((blendFlags & 0xffff) & ((int)(1 << singles) - 1 >> 0x0)) +
             ADD_4_BITS((blendFlags & 0xffff) & ((int)(1 << singles) - 1 >> 0x4)) +
             ADD_4_BITS((blendFlags & 0xffff) & ((int)(1 << singles) - 1 >> 0x8)) +
             ADD_4_BITS((blendFlags & 0xffff) & ((int)(1 << singles) - 1 >> 0xC)) + singles) +
        sizeof(xAnimPlay);

    U32 i;
    void* buffer = xMemAlloc(gActiveHeap, count * size);

    xAnimPlay* play = (xAnimPlay*)buffer;
    play->NumSingle = singles;

    xAnimSingle* currsingle;
    play->Single = currsingle = (xAnimSingle*)((U32)play + sizeof(xAnimPlay));
    currsingle += singles;

    for (i = 0; i < singles; i++)
    {
        if (blendFlags & (1 << i))
        {
            play->Single[i].Blend = currsingle;
            currsingle->Blend = NULL;
            currsingle++;
        }
        else
        {
            play->Single[i].Blend = NULL;
        }
    }

    xAnimActiveEffect* curract = (xAnimActiveEffect*)currsingle;

    for (i = 0; i < play->NumSingle; i++)
    {
        currsingle = &play->Single[i];

        while (currsingle)
        {
            currsingle->ActiveCount = effectMax;

            if (effectMax)
            {
                currsingle->ActiveList = curract;
                curract += effectMax;
            }
            else
            {
                currsingle->ActiveList = NULL;
            }

            currsingle = currsingle->Blend;
        }
    }

    play->Pool = pool;
    xMemPoolSetup(pool, buffer, 0, 1, xAnimPoolCB, size, count, count / 2);
}

// Take a play from the pool and set it up.
xAnimPlay* xAnimPoolAlloc(xMemPool* pool, void* object, xAnimTable* table,
                          xModelInstance* modelInst)
{
    xAnimPlay* play;

    play = (xAnimPlay*)xMemPoolAlloc(pool);

    xAnimPlaySetup(play, object, table, modelInst);

    return play;
}

// End every single's effects and give the play back to its pool.
void xAnimPoolFree(xAnimPlay* play)
{
    U32 i;

    for (i = 0; i < play->NumSingle; i++)
    {
        EffectSingleStop(&play->Single[i]);

        if (play->Single[i].Blend)
        {
            EffectSingleStop(play->Single[i].Blend);
        }
    }

    xMemPoolFree(play->Pool, play);
}
