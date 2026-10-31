#ifndef RPHANIM_H
#define RPHANIM_H

#include <rpworld.h>
#include <rtquat.h>

typedef struct RpHAnimAnimation RpHAnimAnimation;
typedef struct RpHAnimHierarchy RpHAnimHierarchy;

struct RpHAnimAnimation
{
    void* interpInfo;
    S32 numFrames;
    S32 flags;
    F32 duration;
    void* pFrames;
};

// The standard keyframe: a rotation and a translation at a time, linked to the previous
// keyframe of the same node.
typedef struct HAnimStdKey HAnimStdKey;
struct HAnimStdKey
{
    HAnimStdKey* prev;
    F32 time;
    RtQuat rotation;
    RwV3d translation;
};

typedef struct RpHAnimNodeInfo RpHAnimNodeInfo;
struct RpHAnimNodeInfo
{
    S32 nodeID;
    S32 nodeIndex;
    S32 flags;
    RwFrame* pFrame;
};

struct RpHAnimHierarchy
{
    S32 flags;
    S32 numNodes;
    RpHAnimAnimation* pCurrentAnim;
    F32 currentTime;
    void* pNextFrame;
    RpHAnimHierarchy* (*pAnimCallBack)(RpHAnimHierarchy*, void*);
    void* pAnimCallBackData;
    F32 animCallBackTime;
    RpHAnimHierarchy* (*pAnimLoopCallBack)(RpHAnimHierarchy*, void*);
    void* pAnimLoopCallBackData;
    RwMatrix* pMatrixArray;
    void* pMatrixArrayUnaligned;
    RpHAnimNodeInfo* pNodeInfo;
    RwFrame* parentFrame;
    S32 maxKeyFrameSize;
    S32 currentKeyFrameSize;
    void (*keyFrameToMatrixCB)(RwMatrix*, void*);
    void (*keyFrameBlendCB)(void*, void*, void*, F32);
    void (*keyFrameInterpolateCB)(void*, void*, void*, F32);
    void (*keyFrameAddCB)(void*, void*, void*);
    RpHAnimHierarchy* parentHierarchy;
    S32 offsetInParent;
    S32 rootParentOffset;
};

typedef enum RpHAnimHierarchyFlag
{
    rpHANIMHIERARCHYSUBHIERARCHY = 0x01,
    rpHANIMHIERARCHYNOMATRICES = 0x02,
    rpHANIMHIERARCHYUPDATEMODELLINGMATRICES = 0x1000,
    rpHANIMHIERARCHYUPDATELTMS = 0x2000,
    rpHANIMHIERARCHYLOCALSPACEMATRICES = 0x4000,
    rpHANIMHIERARCHYFLAGFORCEENUMSIZEINT = 0x7FFFFFFF
} RpHAnimHierarchyFlag;

#ifdef __cplusplus
extern "C" {
#endif

void RpHAnimStdKeyFrameToMatrix(void* matrix, void* voidIFrame);
void RpHAnimStdKeyFrameInterpolate(void* voidOut, void* voidIn1, void* voidIn2, F32 time);
void RpHAnimStdKeyFrameBlend(void* voidOut, void* voidIn1, void* voidIn2, F32 alpha);
RpHAnimAnimation* RpHAnimStdKeyFrameStreamRead(RwStream* stream, RpHAnimAnimation* animation);
S32 RpHAnimStdKeyFrameStreamWrite(RpHAnimAnimation* animation, RwStream* stream);
S32 RpHAnimStdKeyFrameStreamGetSize(RpHAnimAnimation* animation);
void RpHAnimStdKeyFrameMulRecip(void* voidFrame, void* voidStart);
void RpHAnimStdKeyFrameAdd(void* voidOut, void* voidIn1, void* voidIn2);
RpHAnimHierarchy* RpHAnimHierarchyCreate(S32 numNodes, U32* nodeFlags, S32* nodeIDs,
                                         RpHAnimHierarchyFlag flags, S32 maxKeyFrameSize);
RpHAnimHierarchy* RpHAnimHierarchyDestroy(RpHAnimHierarchy* hierarchy);
S32 RpHAnimFrameSetHierarchy(RwFrame* frame, RpHAnimHierarchy* hierarchy);
RpHAnimHierarchy* RpHAnimFrameGetHierarchy(RwFrame* frame);
S32 RpHAnimPluginAttach(void);

#ifdef __cplusplus
}
#endif

#endif
