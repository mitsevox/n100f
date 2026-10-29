#ifndef ZENTSIMPLEOBJ_H
#define ZENTSIMPLEOBJ_H

#include "zEnt.h"

struct p2SimpleObjAsset;

struct _zEntSimpleObj : _zEnt
{
    p2SimpleObjAsset* sasset;
    U32 sflags;
};

#endif
