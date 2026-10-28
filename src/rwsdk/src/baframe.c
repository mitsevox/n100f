#include <rwcore.h>

// Private frame flags: the hierarchy root is on the dirty list, and this frame's subtree needs
// its matrices and objects resynchronised.
#define FRAME_HIERARCHYDIRTY 0x03
#define FRAME_SUBTREEDIRTY 0x0C

// The frame module's data in the engine instance.
typedef struct FrameModuleData FrameModuleData;
struct FrameModuleData
{
    RwFreeList* freeList;
};

#define FRAME_DATA(var) \
    (((FrameModuleData*)((U8*)RwEngineInstance + frameModule.globalsOffset))->var)

RwPluginRegistry frameTKList = { sizeof(RwFrame), sizeof(RwFrame) };
static RwModuleInfo frameModule;

void rwFrameDestroyRecurse(RwFrame* frame);
void _rwFrameSyncHierarchyLTM(RwFrame* frame);

void* _rwFrameClose(void* instance, S32 offset, S32 size)
{
    if (FRAME_DATA(freeList))
    {
        RwFreeListDestroy(FRAME_DATA(freeList));
        FRAME_DATA(freeList) = NULL;
    }

    frameModule.numInstances--;

    return instance;
}

S32 RwFrameDirty(const RwFrame* frame)
{
    return frame->root->object.privateFlags & 3;
}

static void FrameDestroyRecurseDeInitLeaf(RwFrame* frame)
{
    _rwPluginRegistryDeInitObject(&frameTKList, frame);

    if (frame->object.privateFlags & 3)
    {
        frame->inDirtyListLink.prev->next = frame->inDirtyListLink.next;
        frame->inDirtyListLink.next->prev = frame->inDirtyListLink.prev;
    }
}

static void rwFrameDestroyRecurseDestroyLeaf(RwFrame* frame)
{
    _rwPluginRegistryDeInitObject(&frameTKList, frame);

    if (frame->object.privateFlags & 3)
    {
        frame->inDirtyListLink.prev->next = frame->inDirtyListLink.next;
        frame->inDirtyListLink.next->prev = frame->inDirtyListLink.prev;
    }

    RWSRCGLOBAL(memoryFree)(FRAME_DATA(freeList), frame);
}

S32 RwFrameDestroyHierarchy(RwFrame* frame)
{
    rwFrameDestroyRecurse(frame);

    return 1;
}

RwFrame* RwFrameUpdateObjects(RwFrame* frame)
{
    U8 oldFlags = frame->root->object.privateFlags;

    if (!(oldFlags & FRAME_HIERARCHYDIRTY))
    {
        frame->root->inDirtyListLink.next = RWSRCGLOBAL(dirtyFrameList).link.next;
        frame->root->inDirtyListLink.prev = &RWSRCGLOBAL(dirtyFrameList).link;
        RWSRCGLOBAL(dirtyFrameList).link.next->prev = &frame->root->inDirtyListLink;
        RWSRCGLOBAL(dirtyFrameList).link.next = &frame->root->inDirtyListLink;
    }

    frame->root->object.privateFlags = oldFlags | FRAME_HIERARCHYDIRTY;
    frame->object.privateFlags |= FRAME_SUBTREEDIRTY;

    return frame;
}

RwMatrix* RwFrameGetLTM(RwFrame* frame)
{
    if (frame->root->object.privateFlags & 1)
    {
        _rwFrameSyncHierarchyLTM(frame->root);
    }

    return &frame->ltm;
}

RwFrame* RwFrameGetRoot(const RwFrame* frame)
{
    return frame->root;
}

RwFrame* RwFrameForAllChildren(RwFrame* frame, RwFrameCallBack callBack, void* data)
{
    RwFrame* child = frame->child;

    while (child)
    {
        RwFrame* next = child->next;

        if (!callBack(child, data))
        {
            return frame;
        }

        child = next;
    }

    return frame;
}

S32 RwFrameRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&frameTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}
