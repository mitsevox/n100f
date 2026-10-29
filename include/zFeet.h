#ifndef ZFEET_H
#define ZFEET_H

#include "types.h"
#include "xVec3.h"

struct _xEnt;

struct tagzFeet
{
    S32 total;
    _xEnt* owner;
    S16 entity_type;
    S16 pitch;
};

struct _tagFoot
{
    tagzFeet* feet;
    S32 idx;
};

struct _tagSkidEdge
{
    _xVec3 centre;
    _xVec3 local_x;
    F32 width;
};

struct _tagFeetSkidMark
{
    S32 total_feet;
    _tagSkidEdge* edge[4];
    S32 first_idx;
    S32 total_edges;
    F32 sample_timer;
    F32 remove_timer;
};

void zFeetUpdateSkidMark(_tagFeetSkidMark* m, F32 seconds);
void zFeetInitSkidMark(_tagFeetSkidMark* m, S32 total_feet);

#endif
