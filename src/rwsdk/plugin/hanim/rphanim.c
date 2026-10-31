#include <rphanim.h>
#include <string.h>

#define HANIM_PLUGIN_ID 0x11E

// The version stamped on a frame's hierarchy chunk.
#define HANIM_STREAM_VERSION 0x100

#define HANIM_MAX_INTERPOLATORS 16

// The type of the standard keyframe scheme.
#define HANIM_STD_KEYFRAME_TYPE 1

// The plugin's own error codes.
#define HANIM_ERR_INTERPOLATOREXISTS 0
#define HANIM_ERR_TOOMANYINTERPOLATORS 1

// A keyframe scheme: the size of its keyframes and the callbacks that work on them.
typedef struct RpHAnimInterpolatorInfo RpHAnimInterpolatorInfo;
struct RpHAnimInterpolatorInfo
{
    S32 typeID;
    S32 keyFrameSize;
    void (*keyFrameToMatrixCB)(void* matrix, void* voidIFrame);
    void (*keyFrameBlendCB)(void* voidOut, void* voidIn1, void* voidIn2, F32 alpha);
    void (*keyFrameInterpolateCB)(void* voidOut, void* voidIn1, void* voidIn2, F32 time);
    void (*keyFrameAddCB)(void* voidOut, void* voidIn1, void* voidIn2);
    void (*keyFrameMulRecipCB)(void* voidFrame, void* voidStart);
    RpHAnimAnimation* (*keyFrameStreamReadCB)(RwStream* stream, RpHAnimAnimation* animation);
    S32 (*keyFrameStreamWriteCB)(RpHAnimAnimation* animation, RwStream* stream);
    S32 (*keyFrameStreamGetSizeCB)(RpHAnimAnimation* animation);
};

typedef struct RpHAnimAtomicGlobalVars RpHAnimAtomicGlobalVars;
struct RpHAnimAtomicGlobalVars
{
    S32 engineOffset;
    RwFreeList* HAnimFreeList;
    RwFreeList* HAnimAnimationFreeList;
};

// What the plugin adds to every frame: its node ID and the hierarchy it heads.
typedef struct HAnimFrameData HAnimFrameData;
struct HAnimFrameData
{
    S32 id;
    RpHAnimHierarchy* hierarchy;
};

#define HANIM_FRAMEDATA(frame)                                                                     \
    ((HAnimFrameData*)((U8*)(frame) + RpHAnimAtomicGlobals.engineOffset))

RpHAnimAtomicGlobalVars RpHAnimAtomicGlobals;
RpHAnimInterpolatorInfo RpHAnimInterpolatorInfoBlock[HANIM_MAX_INTERPOLATORS];
S32 RpHAnimInterpolatorInfoBlockNumEntries;

static S32 fn_8013D4B8(RpHAnimInterpolatorInfo* interpolatorInfo);

static void* HAnimOpen(void* instance, S32 offset, S32 size)
{
    RpHAnimInterpolatorInfo interpolatorInfo;

    RpHAnimAtomicGlobals.HAnimFreeList = NULL;
    RpHAnimAtomicGlobals.HAnimAnimationFreeList = RwFreeListCreate(sizeof(RpHAnimAnimation), 20, 0);

    if (!RpHAnimAtomicGlobals.HAnimAnimationFreeList)
    {
        instance = NULL;
    }

    interpolatorInfo.typeID = HANIM_STD_KEYFRAME_TYPE;
    interpolatorInfo.keyFrameSize = sizeof(HAnimStdKey);
    interpolatorInfo.keyFrameToMatrixCB = RpHAnimStdKeyFrameToMatrix;
    interpolatorInfo.keyFrameBlendCB = RpHAnimStdKeyFrameBlend;
    interpolatorInfo.keyFrameInterpolateCB = RpHAnimStdKeyFrameInterpolate;
    interpolatorInfo.keyFrameAddCB = RpHAnimStdKeyFrameAdd;
    interpolatorInfo.keyFrameMulRecipCB = RpHAnimStdKeyFrameMulRecip;
    interpolatorInfo.keyFrameStreamReadCB = RpHAnimStdKeyFrameStreamRead;
    interpolatorInfo.keyFrameStreamWriteCB = RpHAnimStdKeyFrameStreamWrite;
    interpolatorInfo.keyFrameStreamGetSizeCB = RpHAnimStdKeyFrameStreamGetSize;
    fn_8013D4B8(&interpolatorInfo);

    return instance;
}

static void* HAnimClose(void* instance, S32 offset, S32 size)
{
    RpHAnimInterpolatorInfoBlockNumEntries = 0;

    if (RpHAnimAtomicGlobals.HAnimAnimationFreeList)
    {
        RwFreeListDestroy(RpHAnimAtomicGlobals.HAnimAnimationFreeList);
        RpHAnimAtomicGlobals.HAnimAnimationFreeList = NULL;
    }

    if (RpHAnimAtomicGlobals.HAnimFreeList)
    {
        RwFreeListDestroy(RpHAnimAtomicGlobals.HAnimFreeList);
        RpHAnimAtomicGlobals.HAnimFreeList = NULL;
    }

    return instance;
}

static void* HAnimConstructor(void* object, S32 offset, S32 size)
{
    HAnimFrameData* frameExt = HANIM_FRAMEDATA(object);

    frameExt->hierarchy = NULL;
    frameExt->id = -1;

    return object;
}

// Unlinks the frame's hierarchy from its frames and destroys it when this frame heads it.
static void* HAnimDestructor(void* object, S32 offset, S32 size)
{
    HAnimFrameData* frameExt = HANIM_FRAMEDATA(object);
    RpHAnimHierarchy* hierarchy = frameExt->hierarchy;

    if (hierarchy)
    {
        S32 i;

        for (i = 0; i < hierarchy->numNodes; i++)
        {
            hierarchy->pNodeInfo[i].pFrame = NULL;
        }

        if (hierarchy->parentFrame == object)
        {
            RpHAnimHierarchyDestroy(hierarchy);
        }

        frameExt->hierarchy = NULL;
    }

    frameExt->id = -1;

    return object;
}

static void* HAnimCopy(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    HAnimFrameData* srcFrameExt = HANIM_FRAMEDATA(srcObject);
    HAnimFrameData* dstFrameExt = HANIM_FRAMEDATA(dstObject);
    RpHAnimHierarchy* srcHierarchy;

    dstFrameExt->id = srcFrameExt->id;
    srcHierarchy = srcFrameExt->hierarchy;

    if (srcHierarchy && !(srcHierarchy->flags & rpHANIMHIERARCHYSUBHIERARCHY))
    {
        RpHAnimHierarchy* dstHierarchy =
            RpHAnimHierarchyCreate(srcHierarchy->numNodes, NULL, NULL,
                                   (RpHAnimHierarchyFlag)srcHierarchy->flags,
                                   srcHierarchy->maxKeyFrameSize);
        S32 i;

        for (i = 0; i < dstHierarchy->numNodes; i++)
        {
            dstHierarchy->pNodeInfo[i].pFrame = NULL;
            dstHierarchy->pNodeInfo[i].flags = srcHierarchy->pNodeInfo[i].flags;
            dstHierarchy->pNodeInfo[i].nodeIndex = srcHierarchy->pNodeInfo[i].nodeIndex;
            dstHierarchy->pNodeInfo[i].nodeID = srcHierarchy->pNodeInfo[i].nodeID;
        }

        dstFrameExt->hierarchy = dstHierarchy;
        dstHierarchy->parentFrame = (RwFrame*)dstObject;
    }

    return dstObject;
}

static RwStream* HAnimWrite(RwStream* stream, S32 binaryLength, const void* object, S32 offset,
                            S32 size)
{
    S32 version = HANIM_STREAM_VERSION;
    HAnimFrameData* frameExt;
    RpHAnimHierarchy* hierarchy;

    if (!RwStreamWriteInt32(stream, &version, sizeof(S32)))
    {
        return NULL;
    }

    frameExt = HANIM_FRAMEDATA(object);

    if (!RwStreamWriteInt32(stream, &frameExt->id, sizeof(S32)))
    {
        return NULL;
    }

    hierarchy = frameExt->hierarchy;

    if (hierarchy && !(hierarchy->flags & rpHANIMHIERARCHYSUBHIERARCHY))
    {
        RpHAnimNodeInfo* nodeInfo;
        S32 i;

        if (!RwStreamWriteInt32(stream, &hierarchy->numNodes, sizeof(S32)))
        {
            return NULL;
        }

        if (!RwStreamWriteInt32(stream, &hierarchy->flags, sizeof(S32)))
        {
            return NULL;
        }

        if (!RwStreamWriteInt32(stream, &hierarchy->maxKeyFrameSize, sizeof(S32)))
        {
            return NULL;
        }

        nodeInfo = hierarchy->pNodeInfo;

        for (i = 0; i < hierarchy->numNodes; i++)
        {
            if (!RwStreamWriteInt32(stream, &nodeInfo->nodeID, sizeof(S32)))
            {
                return NULL;
            }

            if (!RwStreamWriteInt32(stream, &nodeInfo->nodeIndex, sizeof(S32)))
            {
                return NULL;
            }

            if (!RwStreamWriteInt32(stream, &nodeInfo->flags, sizeof(S32)))
            {
                return NULL;
            }

            nodeInfo++;
        }
    }
    else
    {
        S32 numNodes = 0;

        if (!RwStreamWriteInt32(stream, &numNodes, sizeof(S32)))
        {
            return NULL;
        }
    }

    return stream;
}

static RwStream* HAnimRead(RwStream* stream, S32 binaryLength, void* object, S32 offset, S32 size)
{
    HAnimFrameData* frameExt = HANIM_FRAMEDATA(object);
    S32 version;
    S32 numNodes;
    S32 flags;
    S32 maxKeyFrameSize;

    if (!RwStreamReadInt32(stream, &version, sizeof(S32)))
    {
        return NULL;
    }

    if (version != HANIM_STREAM_VERSION)
    {
        return NULL;
    }

    if (!RwStreamReadInt32(stream, &frameExt->id, sizeof(S32)))
    {
        return NULL;
    }

    if (!RwStreamReadInt32(stream, &numNodes, sizeof(S32)))
    {
        return NULL;
    }

    if (numNodes > 0)
    {
        RpHAnimHierarchy* hierarchy;
        RpHAnimNodeInfo* nodeInfo;
        S32 i;

        if (!RwStreamReadInt32(stream, &flags, sizeof(S32)))
        {
            return NULL;
        }

        if (!RwStreamReadInt32(stream, &maxKeyFrameSize, sizeof(S32)))
        {
            return NULL;
        }

        hierarchy = (RpHAnimHierarchy*)RwMalloc(sizeof(RpHAnimHierarchy) +
                                                numNodes * maxKeyFrameSize * 3);
        memset(hierarchy, 0, sizeof(RpHAnimHierarchy) + numNodes * maxKeyFrameSize * 3);

        if (hierarchy)
        {
            hierarchy->flags = flags;
            hierarchy->parentFrame = (RwFrame*)object;
            hierarchy->numNodes = numNodes;
            hierarchy->currentKeyFrameSize = maxKeyFrameSize;
            hierarchy->maxKeyFrameSize = maxKeyFrameSize;
            hierarchy->parentHierarchy = hierarchy;
            hierarchy->offsetInParent = 0;

            if (hierarchy->flags & rpHANIMHIERARCHYNOMATRICES)
            {
                hierarchy->pMatrixArray = NULL;
                hierarchy->pMatrixArrayUnaligned = NULL;
            }
            else
            {
                void* matrices = RwMalloc(sizeof(RwMatrix) * numNodes + 15);

                hierarchy->pMatrixArray = (RwMatrix*)(((U32)matrices + 15) & ~15);
                hierarchy->pMatrixArrayUnaligned = matrices;
            }

            hierarchy->pNodeInfo =
                (RpHAnimNodeInfo*)RwMalloc(sizeof(RpHAnimNodeInfo) * numNodes);
            nodeInfo = hierarchy->pNodeInfo;

            for (i = 0; i < hierarchy->numNodes; i++)
            {
                if (!RwStreamReadInt32(stream, &nodeInfo->nodeID, sizeof(S32)))
                {
                    return NULL;
                }

                if (!RwStreamReadInt32(stream, &nodeInfo->nodeIndex, sizeof(S32)))
                {
                    return NULL;
                }

                if (!RwStreamReadInt32(stream, &nodeInfo->flags, sizeof(S32)))
                {
                    return NULL;
                }

                nodeInfo->pFrame = NULL;
                nodeInfo++;
            }
        }

        frameExt->hierarchy = hierarchy;
    }

    return stream;
}

static S32 HAnimSize(const void* object, S32 offset, S32 size)
{
    S32 streamSize = sizeof(S32) * 3;
    RpHAnimHierarchy* hierarchy = HANIM_FRAMEDATA(object)->hierarchy;

    if (hierarchy && !(hierarchy->flags & rpHANIMHIERARCHYSUBHIERARCHY))
    {
        streamSize = hierarchy->numNodes * (sizeof(S32) * 3) + sizeof(S32) * 5;
    }

    return streamSize;
}

// Adds a keyframe scheme to the table; a type already there, or a full table, is an error.
static S32 fn_8013D4B8(RpHAnimInterpolatorInfo* interpolatorInfo)
{
    if (RpHAnimInterpolatorInfoBlockNumEntries < HANIM_MAX_INTERPOLATORS)
    {
        S32 i;

        for (i = 0; i < RpHAnimInterpolatorInfoBlockNumEntries; i++)
        {
            if (interpolatorInfo->typeID == RpHAnimInterpolatorInfoBlock[i].typeID)
            {
                RwError err;

                err.pluginID = HANIM_PLUGIN_ID;
                err.errorCode = _rwerror(HANIM_ERR_INTERPOLATOREXISTS);
                RwErrorSet(&err);
                return FALSE;
            }
        }

        RpHAnimInterpolatorInfoBlock[RpHAnimInterpolatorInfoBlockNumEntries] = *interpolatorInfo;
        RpHAnimInterpolatorInfoBlockNumEntries++;
        return TRUE;
    }
    else
    {
        RwError err;

        err.pluginID = HANIM_PLUGIN_ID;
        err.errorCode = _rwerror(HANIM_ERR_TOOMANYINTERPOLATORS);
        RwErrorSet(&err);
        return FALSE;
    }
}

RpHAnimHierarchy* RpHAnimHierarchyCreate(S32 numNodes, U32* nodeFlags, S32* nodeIDs,
                                         RpHAnimHierarchyFlag flags, S32 maxKeyFrameSize)
{
    RpHAnimHierarchy* hierarchy =
        (RpHAnimHierarchy*)RwMalloc(sizeof(RpHAnimHierarchy) + numNodes * maxKeyFrameSize * 3);
    S32 i;

    hierarchy->flags = flags;
    hierarchy->numNodes = numNodes;
    hierarchy->pCurrentAnim = NULL;
    hierarchy->pNextFrame = NULL;
    hierarchy->currentTime = 0.0f;
    hierarchy->parentFrame = NULL;
    hierarchy->pAnimCallBack = NULL;
    hierarchy->animCallBackTime = -1.0f;
    hierarchy->pAnimCallBackData = NULL;
    hierarchy->pAnimLoopCallBack = NULL;
    hierarchy->pAnimLoopCallBackData = NULL;

    if (!(flags & rpHANIMHIERARCHYNOMATRICES))
    {
        void* matrices = RwMalloc(sizeof(RwMatrix) * numNodes + 15);

        hierarchy->pMatrixArray = (RwMatrix*)(((U32)matrices + 15) & ~15);
        hierarchy->pMatrixArrayUnaligned = matrices;
    }

    hierarchy->pNodeInfo = (RpHAnimNodeInfo*)RwMalloc(sizeof(RpHAnimNodeInfo) * numNodes);

    for (i = 0; i < numNodes; i++)
    {
        hierarchy->pNodeInfo[i].pFrame = NULL;

        if (nodeIDs)
        {
            hierarchy->pNodeInfo[i].nodeID = nodeIDs[i];
        }

        hierarchy->pNodeInfo[i].nodeIndex = i;

        if (nodeFlags)
        {
            hierarchy->pNodeInfo[i].flags = nodeFlags[i];
        }
    }

    hierarchy->currentKeyFrameSize = maxKeyFrameSize;
    hierarchy->maxKeyFrameSize = maxKeyFrameSize;
    hierarchy->keyFrameToMatrixCB = NULL;
    hierarchy->keyFrameInterpolateCB = NULL;
    hierarchy->keyFrameBlendCB = NULL;
    hierarchy->keyFrameAddCB = NULL;
    hierarchy->parentHierarchy = hierarchy;
    hierarchy->offsetInParent = 0;

    return hierarchy;
}

RpHAnimHierarchy* RpHAnimHierarchyDestroy(RpHAnimHierarchy* hierarchy)
{
    RwFrame* parentFrame = hierarchy->parentFrame;

    if (!(hierarchy->flags & rpHANIMHIERARCHYSUBHIERARCHY))
    {
        if (!(hierarchy->flags & rpHANIMHIERARCHYNOMATRICES))
        {
            RwFree(hierarchy->pMatrixArrayUnaligned);
        }

        RwFree(hierarchy->pNodeInfo);
    }

    hierarchy->pMatrixArrayUnaligned = NULL;
    hierarchy->pMatrixArray = NULL;
    hierarchy->pNodeInfo = NULL;
    RwFree(hierarchy);

    if (parentFrame)
    {
        HANIM_FRAMEDATA(parentFrame)->hierarchy = NULL;
    }

    return NULL;
}

S32 RpHAnimFrameSetHierarchy(RwFrame* frame, RpHAnimHierarchy* hierarchy)
{
    HAnimFrameData* frameExt = HANIM_FRAMEDATA(frame);

    if (frameExt->hierarchy)
    {
        frameExt->hierarchy->parentFrame = NULL;
    }

    frameExt->hierarchy = hierarchy;

    if (hierarchy)
    {
        hierarchy->parentFrame = frame;
    }

    return TRUE;
}

RpHAnimHierarchy* RpHAnimFrameGetHierarchy(RwFrame* frame)
{
    return HANIM_FRAMEDATA(frame)->hierarchy;
}

S32 RpHAnimPluginAttach(void)
{
    S32 offset;

    if (RwEngineRegisterPlugin(0, HANIM_PLUGIN_ID, HAnimOpen, HAnimClose) < 0)
    {
        return FALSE;
    }

    RpHAnimAtomicGlobals.engineOffset =
        RwFrameRegisterPlugin(sizeof(HAnimFrameData), HANIM_PLUGIN_ID, HAnimConstructor,
                              HAnimDestructor, HAnimCopy);
    offset = RwFrameRegisterPluginStream(HANIM_PLUGIN_ID, HAnimRead, HAnimWrite, HAnimSize);

    return offset >= 0 && RpHAnimAtomicGlobals.engineOffset >= 0;
}
