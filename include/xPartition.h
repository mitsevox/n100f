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
struct _xEnv;
struct _zVolume;
void xPartitionReset();
void xPartitionVolume(_tagPartition* part, _zVolume* volume, S32 x_spaces, S32 y_spaces, S32 z_spaces);
void xPartitionWorld(_tagPartition* part, _xEnv* env, S32 x_spaces, S32 y_spaces, S32 z_spaces);
S32 xPartitionInsert(_tagPartition* part, void* insert_data, xVec3* insert_pos);
S32 xPartitionUpdate(_tagPartition* part, void* data, S32 old_idx, xVec3* current_pos);
_tagPartSpace* xPartitionGetSpace(_tagPartition* p, S32 true_idx);
void xPartitionGetVirtualIdx(_tagPartition* part, xVec3* insert_pos, S32* x, S32* y, S32* z);
void xPartitionDump(_tagPartition* part, char* name);
void xPartitionGetLoop(_tagPartition* p, S32 step, S32* xi, S32* yi, S32* zi, S32 xmid, S32 ymid, S32 zmid,
                       S32* xend, S32* yend, S32* zend);

#endif
