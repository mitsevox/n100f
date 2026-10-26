#ifndef ZVOLUME_H
#define ZVOLUME_H

#include "xBase.h"

struct zVolumeAsset;

struct _zVolume : xBase
{
    zVolumeAsset* asset;
};

#endif
