#include "xBound.h"
#include "xCollide.h"
#include "iCollide.h"
#include "iMath3.h"
#include "xMath.h"
#include "xMathInlines.h"
#include "xQuickCull.h"

// Bound types: 1 sphere, 2 axis-aligned box, 3 cylinder, 4 oriented box.

static inline void xMat4x3Toworld(xVec3* o, const xMat4x3* m, const xVec3* v)
{
    xMat3x3RMulVec(o, (const xMat3x3*)m, v);
    o->x += m->pos.x;
    o->y += m->pos.y;
    o->z += m->pos.z;
}

static inline void xMat4x3Tolocal(xVec3* o, const xMat4x3* m, const xVec3* v)
{
    o->x = v->x - m->pos.x;
    o->y = v->y - m->pos.y;
    o->z = v->z - m->pos.z;
    xMat3x3Tolocal(o, (const xMat3x3*)m, o);
}

xVec3* xBoundCenter(const _xBound* b)
{
    if (b->type == 1)
    {
        return (xVec3*)&b->sph.center;
    }
    else if (b->type == 4)
    {
        return (xVec3*)&b->box.center;
    }
    else if (b->type == 2)
    {
        return (xVec3*)&b->box.center;
    }
    else if (b->type == 3)
    {
        return (xVec3*)&b->cyl.center;
    }
    return NULL;
}

// Recomputes the centre of a box bound, in world space for an oriented one, then its cull data.
void xBoundUpdate(_xBound* b)
{
    if (b->type == 2)
    {
        xVec3Add(&b->box.center, &b->box.box.lower, &b->box.box.upper);
        xVec3SMulBy(&b->box.center, 0.5f);
    }
    else if (b->type == 4)
    {
        xVec3Add(&b->box.center, &b->box.box.lower, &b->box.box.upper);
        xVec3SMulBy(&b->box.center, 0.5f);
        xMat4x3Toworld(&b->box.center, b->mat, &b->box.center);
    }

    if (b->type != 0)
    {
        xQuickCullForBound(&b->qcd, b);
    }
}

// Only sphere-against-anything is handled.
void xBoundHitsBound(const _xBound* a, const _xBound* b, _xCollis* c)
{
    if (!xQuickCullIsects(&a->qcd, &b->qcd))
    {
        c->flags &= ~0x1;
        return;
    }

    switch (a->type)
    {
    case 1:
        switch (b->type)
        {
        case 1:
            xSphereHitsSphere(&a->sph, &b->sph, c);
            break;
        case 4:
            xSphereHitsOBB(&a->sph, &b->box.box, b->mat, c);
            break;
        case 2:
            xSphereHitsBox(&a->sph, &b->box.box, c);
            break;
        case 3:
            break;
        }
        break;
    case 2:
    case 3:
    case 4:
        break;
    }
}

// Only a sphere bound tests against the model.
void xBoundHitsModel(const _xBound* b, const xModelInstance* m, _xCollis* colls, U8 ncolls, F32 sth)
{
    switch (b->type)
    {
    case 1:
        iSphereHitsModel3(&b->sph, m, colls, ncolls, sth);
        break;
    case 2:
    case 3:
    case 4:
        break;
    }
}

// An oriented box is tested in its own space; when its matrix is scaled, the ray's range is
// scaled by the length of the right axis and the matrix is normalized for the direction.
void xRayHitsBound(const _xRay3* r, const _xBound* b, _xCollis* c)
{
    xIsect isect;

    if (b->type == 1)
    {
        iSphereIsectRay(&b->sph, r, &isect);
    }
    else if (b->type == 4)
    {
        _xRay3 xfr;
        xMat3x3 mnormal;
        const xMat3x3* m;
        F32 len2 = SQR(b->mat->right.x) + SQR(b->mat->right.y) + SQR(b->mat->right.z);

        if ((F32)iabs(len2 - 1.0f) >= 0.00001f)
        {
            F32 len_inv = 1.0f / xsqrt(len2);
            xfr.max_t = r->max_t * len_inv;
            xfr.min_t = r->min_t * len_inv;
            xMat3x3Normalize(&mnormal, (const xMat3x3*)b->mat);
            m = &mnormal;
        }
        else
        {
            xfr.max_t = r->max_t;
            xfr.min_t = r->min_t;
            m = (const xMat3x3*)b->mat;
        }

        xMat4x3Tolocal(&xfr.origin, b->mat, &r->origin);
        xMat3x3Tolocal(&xfr.dir, m, &r->dir);
        xfr.flags = r->flags;
        iBoxIsectRay(&b->box.box, &xfr, &isect);
    }
    else if (b->type == 2)
    {
        iBoxIsectRay(&b->box.box, r, &isect);
    }

    if (isect.penned <= 0.0f)
    {
        c->flags |= 0x1;
        c->dist = isect.dist;
    }
    else
    {
        c->flags &= ~0x1;
    }
}

void xRayHitsBound(const _xRay3* r, const xQCData* q, const _xBound* b, _xCollis* c)
{
    if (!xQuickCullIsects(q, &b->qcd))
    {
        c->flags &= ~0x1;
        return;
    }

    xRayHitsBound(r, b, c);
}

void xVecHitsBound(const xVec3* v, const _xBound* b, _xCollis* c)
{
    xIsect isect;

    if (b->type == 1)
    {
        iSphereIsectVec(&b->sph, v, &isect);
    }
    else if (b->type == 4)
    {
        xVec3 lv;
        xMat4x3Tolocal(&lv, b->mat, v);
        iBoxIsectVec(&b->box.box, &lv, &isect);
    }
    else if (b->type == 2)
    {
        iBoxIsectVec(&b->box.box, v, &isect);
    }

    if (isect.penned <= 0.0f)
    {
        c->flags |= 0x1;
    }
    else
    {
        c->flags &= ~0x1;
    }
}
