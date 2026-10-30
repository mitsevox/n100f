#include "xQuickCull.h"
#include "iMath3.h"

#include <math.h>

// The world box is cut into 30 blocks per axis plus one open cell on each side, 32 in all. A
// cull record holds one bit per cell on each axis; two records can touch only where the masks
// share a bit.
static F32 world_xmin;
static F32 world_ymin;
static F32 world_zmin;
static F32 world_xmax;
static F32 world_ymax;
static F32 world_zmax;
static F32 world_xsz;
static F32 world_ysz;
static F32 world_zsz;
static F32 block_xsz;
static F32 block_ysz;
static F32 block_zsz;
static F32 one_block_xsz;
static F32 one_block_ysz;
static F32 one_block_zsz;
static F32 lcl_world_xsz;
static F32 lcl_world_ysz;
static F32 lcl_world_zsz;
static F32 ucl_world_xsz;
static F32 ucl_world_ysz;
static F32 ucl_world_zsz;

#define QC_MIN(a, b) ((a) < (b) ? (a) : (b))
#define QC_MAX(a, b) ((a) > (b) ? (a) : (b))

// Cell 0 and cell 31 catch everything below and above the world box.
static inline U32 xQuickCullCellIdx(S16 i)
{
    if (i < 0)
    {
        return 0;
    }
    if (i > 29)
    {
        return 31;
    }
    return i + 1;
}

static inline void xQuickCullCellForVec(xQCData* c, const xVec3* v)
{
    F32 x = QC_MAX(lcl_world_xsz, QC_MIN(v->x, ucl_world_xsz));
    F32 y = QC_MAX(lcl_world_ysz, QC_MIN(v->y, ucl_world_ysz));
    F32 z = QC_MAX(lcl_world_zsz, QC_MIN(v->z, ucl_world_zsz));
    S16 xi = (F32)floor((x - world_xmin) / block_xsz);
    S16 yi = (F32)floor((y - world_ymin) / block_ysz);
    S16 zi = (F32)floor((z - world_zmin) / block_zsz);

    c->xmask = xQuickCullCellIdx(xi);
    c->ymask = xQuickCullCellIdx(yi);
    c->zmask = xQuickCullCellIdx(zi);
}

static inline void xQuickCullCellMerge(xQCData* o, const xQCData* a, const xQCData* b)
{
    S16 xmin = QC_MIN(a->xmask, b->xmask);
    S16 ymin = QC_MIN(a->ymask, b->ymask);
    S16 zmin = QC_MIN(a->zmask, b->zmask);
    S16 xmax = QC_MAX(a->xmask, b->xmask);
    S16 ymax = QC_MAX(a->ymask, b->ymask);
    S16 zmax = QC_MAX(a->zmask, b->zmask);

    if (xmax == 31 && xmin == 0)
    {
        o->xmask = 0xFFFFFFFF;
    }
    else
    {
        o->xmask = ((1 << (xmax - xmin + 1)) - 1) << xmin;
    }
    if (ymax == 31 && ymin == 0)
    {
        o->ymask = 0xFFFFFFFF;
    }
    else
    {
        o->ymask = ((1 << (ymax - ymin + 1)) - 1) << ymin;
    }
    if (zmax == 31 && zmin == 0)
    {
        o->zmask = 0xFFFFFFFF;
    }
    else
    {
        o->zmask = ((1 << (zmax - zmin + 1)) - 1) << zmin;
    }
}

void xQuickCullForOBB(xQCData* q, const _xBox* b, const xMat4x3* m)
{
    xSphere sph;
    iSphereInitBoundOBB(&sph, b, m);
    xQuickCullForSphere(q, &sph);
}

void xQuickCullForBox(xQCData* q, const _xBox* box)
{
    xQCData a;
    xQuickCullCellForVec(&a, &box->lower);

    xQCData b;
    xQuickCullCellForVec(&b, &box->upper);

    xQuickCullCellMerge(q, &a, &b);
}

void xQuickCullForSphere(xQCData* q, const xSphere* s)
{
    xVec3 v1;
    xVec3 v2;

    F32 r = s->r;
    F32 nr = -r;

    v1.x = s->center.x + nr;
    v1.y = s->center.y + nr;
    v1.z = s->center.z + nr;
    v2.x = s->center.x + r;
    v2.y = s->center.y + r;
    v2.z = s->center.z + r;

    xQCData a;
    xQuickCullCellForVec(&a, &v1);

    xQCData b;
    xQuickCullCellForVec(&b, &v2);

    xQuickCullCellMerge(q, &a, &b);
}

void xQuickCullForRay(xQCData* q, const xRay3* r)
{
    xVec3 p1;
    xVec3 p2;

    if (r->flags & 0x400)
    {
        xVec3 delta;
        F32 t = r->min_t;
        delta.x = r->dir.x * t;
        delta.y = r->dir.y * t;
        delta.z = r->dir.z * t;
        p1.x = r->origin.x + delta.x;
        p1.y = r->origin.y + delta.y;
        p1.z = r->origin.z + delta.z;
    }
    else
    {
        p1.x = r->origin.x;
        p1.y = r->origin.y;
        p1.z = r->origin.z;
    }

    if (r->flags & 0x800)
    {
        F32 len;
        if (r->flags & 0x400)
        {
            len = r->max_t - r->min_t;
        }
        else
        {
            len = r->max_t;
        }
        p2.x = r->dir.x * len;
        p2.y = r->dir.y * len;
        p2.z = r->dir.z * len;
    }
    else
    {
        p2.x = r->dir.x;
        p2.y = r->dir.y;
        p2.z = r->dir.z;
    }

    p2.x = p1.x + p2.x;
    p2.y = p1.y + p2.y;
    p2.z = p1.z + p2.z;

    xQCData a;
    xQuickCullCellForVec(&a, &p1);

    xQCData b;
    xQuickCullCellForVec(&b, &p2);

    xQuickCullCellMerge(q, &a, &b);
}

void xQuickCullForBound(xQCData* q, const _xBound* b)
{
    if (b->type == 1)
    {
        xQuickCullForSphere(q, &b->sph);
    }
    else if (b->type == 2)
    {
        xQuickCullForBox(q, &b->box.box);
    }
    else if (b->type == 4)
    {
        xQuickCullForOBB(q, &b->box.box, b->mat);
    }
}

S32 xQuickCullIsects(const xQCData* a, const xQCData* b)
{
    return (a->xmask & b->xmask) | (a->ymask & b->ymask) | (a->zmask & b->zmask);
}

void xQuickCullInit(F32 xmin, F32 ymin, F32 zmin, F32 xmax, F32 ymax, F32 zmax)
{
    world_xmin = xmin;
    world_ymin = ymin;
    world_zmin = zmin;
    world_xmax = xmax;
    world_ymax = ymax;
    world_zmax = zmax;
    world_xsz = xmax - xmin;
    world_ysz = ymax - ymin;
    world_zsz = zmax - zmin;
    if (world_xsz <= 0.0f || world_ysz <= 0.0f || world_zsz <= 0.0f)
    {
        world_xsz = world_ysz = world_zsz = 1.0f;
    }
    lcl_world_xsz = xmin - world_xsz;
    lcl_world_ysz = ymin - world_ysz;
    lcl_world_zsz = zmin - world_zsz;
    block_xsz = world_xsz / 30.0f;
    block_ysz = world_ysz / 30.0f;
    block_zsz = world_zsz / 30.0f;
    one_block_xsz = 1.0f / block_xsz;
    one_block_ysz = 1.0f / block_ysz;
    one_block_zsz = 1.0f / block_zsz;
    ucl_world_xsz = xmax + world_xsz;
    ucl_world_ysz = ymax + world_ysz;
    ucl_world_zsz = zmax + world_zsz;
}

void xQuickCullInit(const _xBox* box)
{
    xQuickCullInit(box->lower.x, box->lower.y, box->lower.z, box->upper.x, box->upper.y,
                   box->upper.z);
}
