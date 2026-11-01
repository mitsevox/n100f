#include <rwcore.h>

// Private frame flags: the hierarchy root is on the dirty list, and this frame's subtree needs
// its matrices and objects resynchronised.
#define FRAME_HIERARCHYDIRTY 0x03
#define FRAME_SUBTREEDIRTY 0x0C

// The object type of a frame.
#define FRAME_OBJECTTYPE 0

// The frame module's data in the engine instance.
typedef struct FrameModuleData FrameModuleData;
struct FrameModuleData
{
    RwFreeList* freeList;
};

#define FRAME_DATA(var) \
    (((FrameModuleData*)((U8*)RwEngineInstance + frameModule.globalsOffset))->var)

// The object that owns a link in a frame's object list.
#define OBJECTFROMFRAMELINK(link) \
    ((RwObject*)((U8*)(link) - (S32)&((RwObjectHasFrame*)0)->lFrame))

RwPluginRegistry frameTKList = { sizeof(RwFrame), sizeof(RwFrame) };
static RwModuleInfo frameModule;

void rwFrameDestroyRecurse(RwFrame* frame);
void _rwFrameSyncHierarchyLTM(RwFrame* frame);

// Creates the frame free list (fifty per block) and empties the engine's dirty frame list.
void* _rwFrameOpen(void* instance, S32 offset, S32 size)
{
    frameModule.globalsOffset = offset;

    FRAME_DATA(freeList) = RwFreeListCreate(frameTKList.sizeOfStruct, 50, 4);
    if (!FRAME_DATA(freeList))
    {
        return NULL;
    }

    rwLinkListInitialize(&RWSRCGLOBAL(dirtyFrameList));

    frameModule.numInstances++;

    return instance;
}

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

static void rwSetHierarchyRoot(RwFrame* frame, RwFrame* root)
{
    frame->root = root;
    frame = frame->child;

    while (frame)
    {
        rwSetHierarchyRoot(frame, root);
        frame = frame->next;
    }
}

S32 RwFrameDirty(const RwFrame* frame)
{
    return frame->root->object.privateFlags & 3;
}

RwFrame* RwFrameCreate()
{
    RwFrame* frame = (RwFrame*)RWSRCGLOBAL(memoryAlloc)(FRAME_DATA(freeList));

    if (!frame)
    {
        return NULL;
    }

    rwObjectInitialize(frame, FRAME_OBJECTTYPE, 0);
    rwLinkListInitialize(&frame->objectList);

    frame->modelling.flags = rwMATRIXTYPEORTHONORMAL;
    RwMatrixSetIdentityMacro(&frame->modelling);
    frame->ltm.flags = rwMATRIXTYPEORTHONORMAL;
    RwMatrixSetIdentityMacro(&frame->ltm);

    frame->child = NULL;
    frame->next = NULL;
    frame->root = frame;

    _rwPluginRegistryInitObject(&frameTKList, frame);

    return frame;
}

S32 RwFrameDestroy(RwFrame* frame)
{
    RwFrame* child;

    _rwPluginRegistryDeInitObject(&frameTKList, frame);

    if (rwObjectGetParent(frame))
    {
        RwFrameRemoveChild(frame);
    }

    if (frame->object.privateFlags & FRAME_HIERARCHYDIRTY)
    {
        frame->inDirtyListLink.prev->next = frame->inDirtyListLink.next;
        frame->inDirtyListLink.next->prev = frame->inDirtyListLink.prev;
    }

    child = frame->child;
    while (child)
    {
        rwObjectSetParent(child, NULL);
        child = child->next;
    }

    RWSRCGLOBAL(memoryFree)(FRAME_DATA(freeList), frame);

    return 1;
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

void rwFrameDestroyRecurse(RwFrame* frame)
{
    if (frame)
    {
        RwFrame* child = frame->child;

        while (child)
        {
            RwFrame* next = child->next;

            rwFrameDestroyRecurse(child);
            child = next;
        }

        rwFrameDestroyRecurseDestroyLeaf(frame);
    }
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

RwFrame* RwFrameAddChild(RwFrame* parent, RwFrame* child)
{
    if (rwObjectGetParent(child))
    {
        RwFrameRemoveChild(child);
    }

    child->next = parent->child;
    parent->child = child;
    rwObjectSetParent(child, parent);

    rwSetHierarchyRoot(child, parent->root);

    if (child->object.privateFlags & FRAME_HIERARCHYDIRTY)
    {
        child->inDirtyListLink.prev->next = child->inDirtyListLink.next;
        child->inDirtyListLink.next->prev = child->inDirtyListLink.prev;
        child->object.privateFlags &= ~FRAME_HIERARCHYDIRTY;
    }

    RwFrameUpdateObjects(child);

    return parent;
}

RwFrame* RwFrameRemoveChild(RwFrame* child)
{
    RwFrame* parent = (RwFrame*)rwObjectGetParent(child);
    RwFrame* cur = parent->child;

    if (cur == child)
    {
        parent->child = child->next;
    }
    else
    {
        while (cur->next != child)
        {
            cur = cur->next;
        }

        cur->next = child->next;
    }

    rwObjectSetParent(child, NULL);
    child->next = NULL;

    rwSetHierarchyRoot(child, child);

    RwFrameUpdateObjects(child);

    return child;
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

RwFrame* RwFrameTransform(RwFrame* frame, const RwMatrix* m, RwOpCombineType combine)
{
    RwMatrixTransform(&frame->modelling, m, combine);
    RwFrameUpdateObjects(frame);

    return frame;
}

RwFrame* RwFrameRotate(RwFrame* frame, const RwV3d* axis, F32 angle, RwOpCombineType combine)
{
    RwMatrixRotate(&frame->modelling, axis, angle, combine);
    RwFrameUpdateObjects(frame);

    return frame;
}

RwFrame* RwFrameOrthoNormalize(RwFrame* frame)
{
    RwMatrixOrthoNormalize(&frame->modelling, &frame->modelling);
    RwFrameUpdateObjects(frame);

    return frame;
}

RwFrame* RwFrameForAllObjects(RwFrame* frame, RwObjectCallBack callBack, void* data)
{
    RwLLLink* next;
    RwLLLink* end = rwLinkListGetTerminator(&frame->objectList);
    RwLLLink* cur = rwLinkListGetFirstLLLink(&frame->objectList);

    while (cur != end)
    {
        next = rwLLLinkGetNext(cur);

        if (!callBack(OBJECTFROMFRAMELINK(cur), data))
        {
            return frame;
        }

        cur = next;
    }

    return frame;
}

S32 RwFrameRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&frameTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}
