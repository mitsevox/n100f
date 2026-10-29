#ifndef XPARTITION_H
#define XPARTITION_H

#include "xMath3.h"

struct _tagPartLink
{
    void* data;
    _tagPartLink* next;
};

struct _tagPartSpace
{
    S32 total;
    _tagPartLink head;
};

struct _tagPartition
{
    xVec3 min;
    xVec3 max;
    xVec3 space_dim;
    S32 total_x;
    S32 total_y;
    S32 total_z;
    _tagPartSpace* space;
    _tagPartSpace global;
};

S32 xPartitionGetTrueIdx(_tagPartition* part, S32 x, S32 y, S32 z);

#endif
