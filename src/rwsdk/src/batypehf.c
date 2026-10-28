#include <rwcore.h>

void _rwObjectHasFrameReleaseFrame(void* object)
{
    RwObjectHasFrame* ohf = (RwObjectHasFrame*)object;

    if (rwObjectGetParent(ohf))
    {
        ohf->lFrame.prev->next = ohf->lFrame.next;
        ohf->lFrame.next->prev = ohf->lFrame.prev;
    }
}
