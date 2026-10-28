#include <rwcore.h>

void _rwObjectHasFrameSetFrame(void* object, RwFrame* frame)
{
    RwObjectHasFrame* ohf = (RwObjectHasFrame*)object;

    if (rwObjectGetParent(ohf))
    {
        ohf->lFrame.prev->next = ohf->lFrame.next;
        ohf->lFrame.next->prev = ohf->lFrame.prev;
    }

    rwObjectSetParent(ohf, frame);

    if (frame)
    {
        ohf->lFrame.next = frame->objectList.link.next;
        ohf->lFrame.prev = &frame->objectList.link;
        frame->objectList.link.next->prev = &ohf->lFrame;
        frame->objectList.link.next = &ohf->lFrame;

        RwFrameUpdateObjects(frame);
    }
}

void _rwObjectHasFrameReleaseFrame(void* object)
{
    RwObjectHasFrame* ohf = (RwObjectHasFrame*)object;

    if (rwObjectGetParent(ohf))
    {
        ohf->lFrame.prev->next = ohf->lFrame.next;
        ohf->lFrame.next->prev = ohf->lFrame.prev;
    }
}
