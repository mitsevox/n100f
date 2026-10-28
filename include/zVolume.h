#ifndef ZVOLUME_H
#define ZVOLUME_H

#include "xBase.h"
#include "xEnt.h"

struct zVolumeAsset : p2BaseAsset
{
    U32 flags;
    _xBound bound;
    F32 rot;
    F32 xpivot;
    F32 zpivot;
};

struct _zVolume : xBase
{
    zVolumeAsset* asset;
};

#endif
