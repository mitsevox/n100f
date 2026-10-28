#include <rwcore.h>

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

S32 RwFrameRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&frameTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}
