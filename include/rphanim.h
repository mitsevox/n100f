#ifndef RPHANIM_H
#define RPHANIM_H

#include <rpworld.h>

typedef struct RpHAnimAnimation RpHAnimAnimation;
typedef struct RpHAnimHierarchy RpHAnimHierarchy;

typedef struct RpHAnimNodeInfo RpHAnimNodeInfo;
struct RpHAnimNodeInfo
{
    S32 nodeID;
    S32 nodeIndex;
    S32 flags;
    RwFrame* pFrame;
};

typedef RpHAnimHierarchy* (*RpHAnimHierarchyCallBack)(RpHAnimHierarchy* hierarchy, void* data);

struct RpHAnimHierarchy
{
    S32 flags;
    S32 numNodes;
    RpHAnimAnimation* pCurrentAnim;
    F32 currentTime;
    void* pNextFrame;
    RpHAnimHierarchyCallBack pAnimCallBack;
    void* pAnimCallBackData;
    F32 animCallBackTime;
    RpHAnimHierarchyCallBack pAnimLoopCallBack;
    void* pAnimLoopCallBackData;
    RwMatrix* pMatrixArray;
    void* pMatrixArrayUnaligned;
    RpHAnimNodeInfo* pNodeInfo;
    RwFrame* parentFrame;
    S32 maxKeyFrameSize;
    S32 currentKeyFrameSize;
    void (*keyFrameToMatrixCB)(RwMatrix* matrix, void* voidIFrame);
    void (*keyFrameBlendCB)(void* voidOut, void* voidIn1, void* voidIn2, F32 alpha);
    void (*keyFrameInterpolateCB)(void* voidOut, void* voidIn1, void* voidIn2, F32 time);
    void (*keyFrameAddCB)(void* voidOut, void* voidIn1, void* voidIn2);
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

RpHAnimHierarchy* RpHAnimHierarchyCreate(S32 numNodes, U32* nodeFlags, S32* nodeIDs,
                                         RpHAnimHierarchyFlag flags, S32 maxKeyFrameSize);
S32 RpHAnimFrameSetHierarchy(RwFrame* frame, RpHAnimHierarchy* hierarchy);
RpHAnimHierarchy* RpHAnimFrameGetHierarchy(RwFrame* frame);

#ifdef __cplusplus
}
#endif

#endif
