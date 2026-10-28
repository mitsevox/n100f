#ifndef ZENTTRIGGER_H
#define ZENTTRIGGER_H

#include "zEnt.h"

struct p2EntTriggerAsset
{
    xVec3 p[4];
    xVec3 direction;
    U32 flags;
};

struct zEntTrigger : _zEnt
{
};

#endif
