#include "xPartition.h"
#include "xEnv.h"
#include "xEnt.h"
#include "xMemMgr.h"
#include "zVolume.h"
#include "rpworld.h"

#include <string.h>

extern "C" _xBound* fn_80005AFC(_zVolume* volume);

static inline _xBox* xEnvGetBBox(_xEnv* env)
{
    return (_xBox*)&env->geom->world->boundingBox;
}

static inline _tagPartLink* PartitionGetFreeLink()
{
    return (_tagPartLink*)xMemAlloc(gActiveHeap, sizeof(_tagPartLink));
}

static inline void PartitionSpaceReset(_tagPartSpace* space)
{
    memset(space, 0, sizeof(_tagPartSpace));
}

static inline void PartitionSpaceInsert(_tagPartSpace* space, void* data)
{
    space->total++;
    _tagPartLink* tmp = &space->head;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = PartitionGetFreeLink();
    tmp->next->data = data;
    tmp->next->next = NULL;
}

void xPartitionGetLoop(_tagPartition* p, S32 step, S32* xi, S32* yi, S32* zi, S32 xmid, S32 ymid,
                       S32 zmid, S32* xend, S32* yend, S32* zend)
{
    if (xi != NULL)
    {
        *xi = xmid - step;
        if (*xi < 0)
        {
            *xi = 0;
        }
        *xend = xmid + step;
        if (*xend >= p->total_x)
        {
            *xend = p->total_x - 1;
        }
    }
    if (yi != NULL)
    {
        *yi = ymid - step;
        if (*yi < 0)
        {
            *yi = 0;
        }
        *yend = ymid + step;
        if (*yend >= p->total_y)
        {
            *yend = p->total_y - 1;
        }
    }
    if (zi != NULL)
    {
        *zi = zmid - step;
        if (*zi < 0)
        {
            *zi = 0;
        }
        *zend = zmid + step;
        if (*zend >= p->total_z)
        {
            *zend = p->total_z - 1;
        }
    }
}

void xPartitionDump(_tagPartition*, char*)
{
}

void xPartitionGetVirtualIdx(_tagPartition* part, xVec3* insert_pos, S32* x, S32* y, S32* z)
{
    if (x != NULL)
    {
        *x = (insert_pos->x - part->min.x) / part->space_dim.x;
    }
    if (y != NULL)
    {
        *y = (insert_pos->y - part->min.y) / part->space_dim.y;
    }
    if (z != NULL)
    {
        *z = (insert_pos->z - part->min.z) / part->space_dim.z;
    }
}

_tagPartSpace* xPartitionGetSpace(_tagPartition* p, S32 true_idx)
{
    return &p->space[true_idx];
}

static inline void xPartitionSpaceMove(_tagPartSpace* src, _tagPartSpace* dest, U32 data)
{
    _tagPartLink* src_pre;
    _tagPartLink* src_lnk;
    _tagPartLink* dest_lnk = &dest->head;

    while (dest_lnk->next != NULL)
    {
        dest_lnk = dest_lnk->next;
    }

    src_lnk = &src->head;
    src_pre = &src->head;

    while (src_lnk->data != (void*)data)
    {
        src_pre = src_lnk;
        src_lnk = src_lnk->next;
    }

    src_pre->next = src_lnk->next;

    dest_lnk->next = src_lnk;
    dest_lnk->next->next = NULL;

    src->total--;
    dest->total++;
}

S32 xPartitionUpdate(_tagPartition* part, void* data, S32 old_idx, xVec3* current_pos)
{
    _tagPartSpace* dest;
    _tagPartSpace* src;
    S32 cur_idx;

    if (current_pos->x < part->min.x || current_pos->x > part->max.x ||
        current_pos->y < part->min.y || current_pos->y > part->max.y ||
        current_pos->z < part->min.z || current_pos->z > part->max.z)
    {
        cur_idx = -1;
    }
    else
    {
        cur_idx = xPartitionGetTrueIdx(part, (current_pos->x - part->min.x) / part->space_dim.x,
                                       (current_pos->y - part->min.y) / part->space_dim.y,
                                       (current_pos->z - part->min.z) / part->space_dim.z);
    }

    if (old_idx == cur_idx)
    {
        return cur_idx;
    }

    src = old_idx == -1 ? &part->global : &part->space[old_idx];
    dest = cur_idx == -1 ? &part->global : &part->space[cur_idx];
    xPartitionSpaceMove(src, dest, (U32)data);
    return cur_idx;
}

S32 xPartitionInsert(_tagPartition* part, void* insert_data, xVec3* insert_pos)
{
    if (insert_pos->x < part->min.x || insert_pos->x > part->max.x || insert_pos->y < part->min.y ||
        insert_pos->y > part->max.y || insert_pos->z < part->min.z || insert_pos->z > part->max.z)
    {
        PartitionSpaceInsert(&part->global, insert_data);
        return -1;
    }

    S32 idx = xPartitionGetTrueIdx(part, (insert_pos->x - part->min.x) / part->space_dim.x,
                                   (insert_pos->y - part->min.y) / part->space_dim.y,
                                   (insert_pos->z - part->min.z) / part->space_dim.z);
    PartitionSpaceInsert(&part->space[idx], insert_data);
    return idx;
}

void xPartitionWorld(_tagPartition* part, _xEnv* env, S32 x_spaces, S32 y_spaces, S32 z_spaces)
{
    S32 z;
    S32 y;
    S32 x;
    F32 dz;
    F32 dy;
    F32 dx;
    _xBox* bb;

    memset(part, 0, sizeof(_tagPartition));
    bb = xEnvGetBBox(env);
    part->min = bb->lower;
    part->max = bb->upper;

    dx = part->max.x - part->min.x;
    dy = part->max.y - part->min.y;
    dz = part->max.z - part->min.z;

    part->total_x = x_spaces;
    part->total_y = y_spaces;
    part->total_z = z_spaces;

    part->space_dim.x = dx / part->total_x;
    part->space_dim.y = dy / part->total_y;
    part->space_dim.z = dz / part->total_z;

    part->space = (_tagPartSpace*)xMemAlloc(
        gActiveHeap, sizeof(_tagPartSpace) * part->total_x * part->total_y * part->total_z);

    for (z = 0; z < z_spaces; z++)
    {
        for (y = 0; y < y_spaces; y++)
        {
            for (x = 0; x < x_spaces; x++)
            {
                PartitionSpaceReset(&part->space[xPartitionGetTrueIdx(part, x, y, z)]);
            }
        }
    }

    PartitionSpaceReset(&part->global);
}

void xPartitionVolume(_tagPartition* part, _zVolume* volume, S32 x_spaces, S32 y_spaces,
                      S32 z_spaces)
{
    S32 z;
    S32 y;
    S32 x;
    F32 dz;
    F32 dy;
    F32 dx;
    _xBox* bb;
    _xBound* xb;

    memset(part, 0, sizeof(_tagPartition));
    xb = fn_80005AFC(volume);
    bb = &xb->box.box;
    part->min = bb->lower;
    part->max = bb->upper;

    dx = part->max.x - part->min.x;
    dy = part->max.y - part->min.y;
    dz = part->max.z - part->min.z;

    part->total_x = x_spaces;
    part->total_y = y_spaces;
    part->total_z = z_spaces;

    part->space_dim.x = dx / part->total_x;
    part->space_dim.y = dy / part->total_y;
    part->space_dim.z = dz / part->total_z;

    part->space = (_tagPartSpace*)xMemAlloc(
        gActiveHeap, sizeof(_tagPartSpace) * part->total_x * part->total_y * part->total_z);

    for (z = 0; z < z_spaces; z++)
    {
        for (y = 0; y < y_spaces; y++)
        {
            for (x = 0; x < x_spaces; x++)
            {
                PartitionSpaceReset(&part->space[xPartitionGetTrueIdx(part, x, y, z)]);
            }
        }
    }

    PartitionSpaceReset(&part->global);
}

S32 xPartitionGetTrueIdx(_tagPartition* part, S32 x, S32 y, S32 z)
{
    S32 idx = y * (part->total_x * part->total_z);
    idx += part->total_x * z;
    idx += x;
    return idx;
}

void xPartitionReset()
{
}
