#include "zGlobals.h"

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
