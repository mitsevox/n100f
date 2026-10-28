#ifndef ZFEET_H
#define ZFEET_H

#include "types.h"

struct _xEnt;
struct _tagSkidEdge;
struct xAnimTable;

struct tagzFeet
{
    S32 total;
    _xEnt* owner;
    S16 entity_type;
    S16 pitch;
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
void zFeetAddSkidMark(_xEnt* ent, _tagFeetSkidMark* m, F32 seconds);
void zFeetExit(_xEnt* ent, xAnimTable* table);

#endif
