#include <rwcore.h>

// Private frame flags: the hierarchy root is on the dirty list, this frame's local
// transformation matrix needs recomputing, and its objects need resynchronising.
#define FRAME_ROOTDIRTY 0x01
#define FRAME_LTMDIRTY 0x04
#define FRAME_OBJECTSDIRTY 0x08
#define FRAME_SUBTREEDIRTY (FRAME_LTMDIRTY | FRAME_OBJECTSDIRTY)
#define FRAME_PRIVATEMASK 0x0F

// The frame that owns a link in the dirty frame list.
#define FRAMEFROMDIRTYLINK(link) \
    ((RwFrame*)((U8*)(link) - (S32)&((RwFrame*)0)->inDirtyListLink))

static RwObject* SyncObject(RwObject* object, void* data)
{
    RwObjectHasFrame* ohf = (RwObjectHasFrame*)object;

    ohf->sync(ohf);

    return object;
}

static void FrameSyncHierarchyRecurse(RwFrame* frame, S32 flags)
{
    while (frame)
    {
        S32 accumFlags = flags | frame->object.privateFlags;

        if (accumFlags & FRAME_LTMDIRTY)
        {
            RwFrame* parent = (RwFrame*)rwObjectGetParent(frame);

            RwMatrixMultiply(&frame->ltm, &frame->modelling, &parent->ltm);
        }

        if (!rwLinkListEmpty(&frame->objectList))
        {
            RwFrameForAllObjects(frame, SyncObject, NULL);
        }

        frame->object.privateFlags &= ~FRAME_SUBTREEDIRTY;

        FrameSyncHierarchyRecurse(frame->child, accumFlags);

        frame = frame->next;
    }
}

static void FrameSyncHierarchyRecurseNoLTM(RwFrame* frame)
{
    while (frame)
    {
        if (!rwLinkListEmpty(&frame->objectList))
        {
            RwFrameForAllObjects(frame, SyncObject, NULL);
        }

        frame->object.privateFlags &= ~FRAME_OBJECTSDIRTY;

        FrameSyncHierarchyRecurseNoLTM(frame->child);

        frame = frame->next;
    }
}

static void FrameSyncHierarchy(RwFrame* frame)
{
    U8 flags = frame->object.privateFlags;

    if (flags & FRAME_ROOTDIRTY)
    {
        U8 ltmFlags = flags & FRAME_LTMDIRTY;

        if (ltmFlags)
        {
            frame->ltm = frame->modelling;
        }

        if (!rwLinkListEmpty(&frame->objectList))
        {
            RwFrameForAllObjects(frame, SyncObject, NULL);
        }

        FrameSyncHierarchyRecurse(frame->child, ltmFlags);
    }
    else
    {
        if (!rwLinkListEmpty(&frame->objectList))
        {
            RwFrameForAllObjects(frame, SyncObject, NULL);
        }

        FrameSyncHierarchyRecurseNoLTM(frame->child);
    }

    frame->object.privateFlags = flags & ~FRAME_PRIVATEMASK;
}

S32 _rwFrameSyncDirty(void)
{
    RwLLLink* end = rwLinkListGetTerminator(&RWSRCGLOBAL(dirtyFrameList));
    RwLLLink* cur = rwLinkListGetFirstLLLink(&RWSRCGLOBAL(dirtyFrameList));

    while (cur != end)
    {
        FrameSyncHierarchy(FRAMEFROMDIRTYLINK(cur));

        cur = rwLLLinkGetNext(cur);
    }

    rwLinkListInitialize(&RWSRCGLOBAL(dirtyFrameList));

    return TRUE;
}

static void FrameSyncHierarchyLTMRecurse(RwFrame* frame, S32 flags)
{
    while (frame)
    {
        S32 accumFlags = flags | frame->object.privateFlags;

        if (accumFlags & FRAME_LTMDIRTY)
        {
            RwFrame* parent = (RwFrame*)rwObjectGetParent(frame);

            RwMatrixMultiply(&frame->ltm, &frame->modelling, &parent->ltm);
            frame->object.privateFlags &= ~FRAME_LTMDIRTY;
        }

        FrameSyncHierarchyLTMRecurse(frame->child, accumFlags);

        frame = frame->next;
    }
}

void _rwFrameSyncHierarchyLTM(RwFrame* frame)
{
    U8 flags = frame->object.privateFlags;

    if (flags & FRAME_LTMDIRTY)
    {
        frame->ltm = frame->modelling;
    }

    FrameSyncHierarchyLTMRecurse(frame->child, flags);

    frame->object.privateFlags = flags & ~(FRAME_ROOTDIRTY | FRAME_LTMDIRTY);
}
