#ifndef RTQUAT_H
#define RTQUAT_H

#include "rwcore.h"

typedef struct RtQuat RtQuat;
struct RtQuat
{
    RwV3d imag;
    F32 real;
};

#endif
