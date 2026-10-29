#include "zGlobals.h"
#include "xEvent.h"

void xCameraNext(_tagxCamFrame* cam, _tagxCam* trans_cam);

S32 zCameraEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*);

// Set up the base and its event handler. The links follow the 0x88-byte camera asset.
void zCameraInit(xBase* b, xCamAsset* asset)
{
    xBaseInit(b, (p2BaseAsset*)asset);
    if (b->linkCount)
        b->link = (p2LinkAsset*)((U8*)asset + 0x88);
    b->eventFunc = zCameraEventCB;
}

// Save the camera base state.
void zCameraSave(_tagxCam* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

// Restore the camera base state.
void zCameraLoad(_tagxCam* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

// Reset restores the base flags; event 38 switches the active camera to this one.
S32 zCameraEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    _tagxCam* t = (_tagxCam*)to;
    switch (toEvent)
    {
    case eEventReset:
        xBaseReset(t, (p2BaseAsset*)t->asset);
        break;
    case 38:
        xCameraNext(&globals.camera, t);
        break;
    }
    return 1;
}
