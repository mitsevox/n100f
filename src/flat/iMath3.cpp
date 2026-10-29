#include "iMath3.h"

#include "xEnt.h"
#include "xMath.h"
#include "xMath2.h"
#include "xMath3.h"
#include "xVec3Inlines.h"

void iMath3Init()
{
}

void iMath3Exit()
{
}

void iSphereIsectVec(const xSphere* s, const xVec3* v, xIsect* isx)
{
    xVec3Sub(&isx->norm, v, &s->center);

    isx->dist = xVec3Length(&isx->norm);
    isx->penned = isx->dist - s->r;
}

void iSphereIsectRay(const xSphere* s, const xRay3* r, xIsect* isx)
{
    F32 t_in, t_out;
    U32 num;

    if (!(r->flags & 0x400))
    {
        ((xRay3*)r)->min_t = 0.0f;
    }

    if (!(r->flags & 0x800))
    {
        ((xRay3*)r)->max_t = 1.0f;
    }

    xVec3Sub(&isx->norm, &r->origin, &s->center);

    num = xMathSolveQuadratic(xVec3Dot(&r->dir, &r->dir), 2.0f * xVec3Dot(&isx->norm, &r->dir),
                              xVec3Dot(&isx->norm, &isx->norm) - s->r * s->r, &t_in, &t_out);

    if (num == 0)
    {
        isx->penned = 1.0f;
        isx->contained = 1.0f;
    }
    else if (num == 1)
    {
        if (t_in < r->min_t || t_in > r->max_t)
        {
            isx->dist = t_in;
            isx->penned = 1.0f;
            isx->contained = 1.0f;
        }
        else
        {
            isx->dist = t_in;
            isx->penned = -1.0f;
            isx->contained = 1.0f;
        }
    }
    else if (t_in < r->min_t)
    {
        if (t_out < r->min_t)
        {
            isx->dist = t_out;
            isx->penned = 1.0f;
            isx->contained = 1.0f;
        }
        else
        {
            isx->dist = t_out;
            isx->penned = -1.0f;
            isx->contained = -1.0f;
        }
    }
    else
    {
        if (t_in <= r->max_t)
        {
            isx->dist = t_in;
            isx->penned = -1.0f;
            isx->contained = 1.0f;
        }
        else
        {
            isx->dist = t_in;
            isx->penned = 1.0f;
            isx->contained = 1.0f;
        }
    }
}

void iSphereIsectSphere(const xSphere* s, const xSphere* p, xIsect* isx)
{
    xVec3Sub(&isx->norm, &p->center, &s->center);

    isx->dist = xVec3Length(&isx->norm);
    isx->penned = isx->dist - p->r - s->r;
    isx->contained = isx->dist - s->r;
}

void iSphereInitBoundOBB(xSphere* s, const _xBox* b, const xMat4x3* m)
{
    xVec3 ext;

    xVec3Add(&s->center, &b->lower, &b->upper);
    xVec3SMulBy(&s->center, 0.5f);
    xVec3Sub(&ext, &b->upper, &s->center);

    xMat3x3RMulVec(&s->center, (const xMat3x3*)m, &s->center);
    xVec3AddTo(&s->center, &m->pos);

    s->r = xVec3Length(&m->right) * xVec3Length(&ext);
}

void iCylinderIsectVec(const _xCylinder* c, const xVec3* v, xIsect* isx)
{
    F32 b = c->center.y + c->h;

    if (v->y >= c->center.y - c->h && v->y <= b &&
        xVec2Dist(c->center.x, c->center.z, v->x, v->z) <= c->r)
    {
        isx->penned = -1.0f;
    }
    else
    {
        isx->penned = 1.0f;
    }
}

// o = p + d * t: the point at distance t along a ray.
static inline void RayPoint(xVec3* o, const xVec3* d, F32 t, const xVec3* p)
{
    xVec3Copy(o, d);
    xVec3SMulBy(o, t);
    xVec3AddTo(o, p);
}

void iCylinderIsectRay(const _xCylinder* c, const xRay3* r, xIsect* isx)
{
    F32 a, b;
    F32 t_in, t_out;
    U32 num;
    F32 dz = r->origin.z - c->center.z;
    F32 dx = r->origin.x - c->center.x;

    num = xMathSolveQuadratic(r->dir.x * r->dir.x + r->dir.z * r->dir.z,
                              2.0f * (dx * r->dir.x + dz * r->dir.z),
                              dx * dx + dz * dz - c->r * c->r, &t_in, &t_out);

    if (num == 0)
    {
        isx->penned = 1.0f;
        return;
    }

    a = c->center.y - c->h;
    b = c->center.y + c->h;

    if (t_in < 0.0f)
    {
        if (num == 1 || t_out < 0.0f)
        {
            isx->penned = 1.0f;
            return;
        }

        RayPoint(&isx->norm, &r->dir, t_in, &r->origin);

        if (isx->norm.y >= a && isx->norm.y <= b)
        {
            isx->dist = t_out;
            isx->contained = -1.0f;
        }
        else
        {
            isx->penned = 1.0f;
            return;
        }
    }
    else
    {
        RayPoint(&isx->norm, &r->dir, t_in, &r->origin);

        if (isx->norm.y >= a && isx->norm.y <= b)
        {
            isx->dist = t_in;
            isx->contained = 1.0f;
        }
        else
        {
            RayPoint(&isx->norm, &r->dir, t_in, &r->origin);

            if (isx->norm.y >= a && isx->norm.y <= b)
            {
                isx->dist = t_out;
                isx->contained = 1.0f;
            }
            else
            {
                isx->penned = 1.0f;
                return;
            }
        }
    }

    isx->penned = -1.0f;
}

void iBoxVecDist(const _xBox* box, const xVec3* v, xIsect* isx)
{
    if (v->x < box->lower.x)
    {
        if (v->y < box->lower.y)
        {
            if (v->z < box->lower.z)
            {
                isx->norm.x = box->lower.x - v->x;
                isx->norm.y = box->lower.y - v->y;
                isx->norm.z = box->lower.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x00000000;
                isx->flags |= 0x80000000;
            }
            else if (v->z <= box->upper.z)
            {
                isx->norm.x = box->lower.x - v->x;
                isx->norm.y = box->lower.y - v->y;
                isx->norm.z = 0.0f;
                isx->flags = isx->flags & ~0x1F000000 | 0x01000000;
                isx->flags |= 0x40000000;
            }
            else
            {
                isx->norm.x = box->lower.x - v->x;
                isx->norm.y = box->lower.y - v->y;
                isx->norm.z = box->upper.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x02000000;
                isx->flags |= 0x80000000;
            }
        }
        else if (v->y <= box->upper.y)
        {
            if (v->z < box->lower.z)
            {
                isx->norm.x = box->lower.x - v->x;
                isx->norm.y = 0.0f;
                isx->norm.z = box->lower.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x03000000;
                isx->flags |= 0x40000000;
            }
            else if (v->z <= box->upper.z)
            {
                isx->norm.x = box->lower.x - v->x;
                isx->norm.y = 0.0f;
                isx->norm.z = 0.0f;
                isx->flags = isx->flags & ~0x1F000000 | 0x04000000;
                isx->flags |= 0x20000000;
            }
            else
            {
                isx->norm.x = box->lower.x - v->x;
                isx->norm.y = 0.0f;
                isx->norm.z = box->upper.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x05000000;
                isx->flags |= 0x40000000;
            }
        }
        else
        {
            if (v->z < box->lower.z)
            {
                isx->norm.x = box->lower.x - v->x;
                isx->norm.y = box->upper.y - v->y;
                isx->norm.z = box->lower.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x06000000;
                isx->flags |= 0x80000000;
            }
            else if (v->z <= box->upper.z)
            {
                isx->norm.x = box->lower.x - v->x;
                isx->norm.y = box->upper.y - v->y;
                isx->norm.z = 0.0f;
                isx->flags = isx->flags & ~0x1F000000 | 0x07000000;
                isx->flags |= 0x40000000;
            }
            else
            {
                isx->norm.x = box->lower.x - v->x;
                isx->norm.y = box->upper.y - v->y;
                isx->norm.z = box->upper.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x08000000;
                isx->flags |= 0x80000000;
            }
        }
    }
    else if (v->x <= box->upper.x)
    {
        if (v->y < box->lower.y)
        {
            if (v->z < box->lower.z)
            {
                isx->norm.x = 0.0f;
                isx->norm.y = box->lower.y - v->y;
                isx->norm.z = box->lower.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x09000000;
                isx->flags |= 0x40000000;
            }
            else if (v->z <= box->upper.z)
            {
                isx->norm.x = 0.0f;
                isx->norm.y = box->lower.y - v->y;
                isx->norm.z = 0.0f;
                isx->flags = isx->flags & ~0x1F000000 | 0x0A000000;
                isx->flags |= 0x20000000;
            }
            else
            {
                isx->norm.x = 0.0f;
                isx->norm.y = box->lower.y - v->y;
                isx->norm.z = box->upper.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x0B000000;
                isx->flags |= 0x40000000;
            }
        }
        else if (v->y <= box->upper.y)
        {
            if (v->z < box->lower.z)
            {
                isx->norm.x = 0.0f;
                isx->norm.y = 0.0f;
                isx->norm.z = box->lower.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x0C000000;
                isx->flags |= 0x20000000;
            }
            else if (v->z <= box->upper.z)
            {
            }
            else
            {
                isx->norm.x = 0.0f;
                isx->norm.y = 0.0f;
                isx->norm.z = box->upper.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x0E000000;
                isx->flags |= 0x20000000;
            }
        }
        else
        {
            if (v->z < box->lower.z)
            {
                isx->norm.x = 0.0f;
                isx->norm.y = box->upper.y - v->y;
                isx->norm.z = box->lower.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x0F000000;
                isx->flags |= 0x40000000;
            }
            else if (v->z <= box->upper.z)
            {
                isx->norm.x = 0.0f;
                isx->norm.y = box->upper.y - v->y;
                isx->norm.z = 0.0f;
                isx->flags = isx->flags & ~0x1F000000 | 0x10000000;
                isx->flags |= 0x20000000;
            }
            else
            {
                isx->norm.x = 0.0f;
                isx->norm.y = box->upper.y - v->y;
                isx->norm.z = box->upper.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x11000000;
                isx->flags |= 0x40000000;
            }
        }
    }
    else
    {
        if (v->y < box->lower.y)
        {
            if (v->z < box->lower.z)
            {
                isx->norm.x = box->upper.x - v->x;
                isx->norm.y = box->lower.y - v->y;
                isx->norm.z = box->lower.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x12000000;
                isx->flags |= 0x80000000;
            }
            else if (v->z <= box->upper.z)
            {
                isx->norm.x = box->upper.x - v->x;
                isx->norm.y = box->lower.y - v->y;
                isx->norm.z = 0.0f;
                isx->flags = isx->flags & ~0x1F000000 | 0x13000000;
                isx->flags |= 0x40000000;
            }
            else
            {
                isx->norm.x = box->upper.x - v->x;
                isx->norm.y = box->lower.y - v->y;
                isx->norm.z = box->upper.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x14000000;
                isx->flags |= 0x80000000;
            }
        }
        else if (v->y <= box->upper.y)
        {
            if (v->z < box->lower.z)
            {
                isx->norm.x = box->upper.x - v->x;
                isx->norm.y = 0.0f;
                isx->norm.z = box->lower.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x15000000;
                isx->flags |= 0x40000000;
            }
            else if (v->z <= box->upper.z)
            {
                isx->norm.x = box->upper.x - v->x;
                isx->norm.y = 0.0f;
                isx->norm.z = 0.0f;
                isx->flags = isx->flags & ~0x1F000000 | 0x16000000;
                isx->flags |= 0x20000000;
            }
            else
            {
                isx->norm.x = box->upper.x - v->x;
                isx->norm.y = 0.0f;
                isx->norm.z = box->upper.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x17000000;
                isx->flags |= 0x40000000;
            }
        }
        else
        {
            if (v->z < box->lower.z)
            {
                isx->norm.x = box->upper.x - v->x;
                isx->norm.y = box->upper.y - v->y;
                isx->norm.z = box->lower.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x18000000;
                isx->flags |= 0x80000000;
            }
            else if (v->z <= box->upper.z)
            {
                isx->norm.x = box->upper.x - v->x;
                isx->norm.y = box->upper.y - v->y;
                isx->norm.z = 0.0f;
                isx->flags = isx->flags & ~0x1F000000 | 0x19000000;
                isx->flags |= 0x40000000;
            }
            else
            {
                isx->norm.x = box->upper.x - v->x;
                isx->norm.y = box->upper.y - v->y;
                isx->norm.z = box->upper.z - v->z;
                isx->flags = isx->flags & ~0x1F000000 | 0x1A000000;
                isx->flags |= 0x80000000;
            }
        }
    }

    isx->dist = xVec3Length(&isx->norm);
}

void iBoxIsectVec(const _xBox* b, const xVec3* v, xIsect* isx)
{
    if (v->x >= b->lower.x && v->x <= b->upper.x && v->y >= b->lower.y && v->y <= b->upper.y &&
        v->z >= b->lower.z && v->z <= b->upper.z)
    {
        isx->penned = -1.0f;
    }
    else
    {
        isx->penned = 1.0f;
    }
}

static U32 ClipPlane(F32 denom, F32 numer, F32* t_in, F32* t_out)
{
    if (denom > 0.0f)
    {
        if (numer > denom * (*t_out))
        {
            return 0;
        }

        if (numer > denom * (*t_in))
        {
            *t_in = numer / denom;
        }

        return 1;
    }
    else if (denom < 0.0f)
    {
        if (numer > denom * (*t_in))
        {
            return 0;
        }

        if (numer > denom * (*t_out))
        {
            *t_out = numer / denom;
        }

        return 1;
    }

    return (numer <= 0.0f);
}

// Clips a ray (origin r3 relative to the box centre, direction r5) against the six slabs of a box with
// half-extents r4, narrowing t_in and t_out.
static U32 ClipBox(const xVec3* r3, const xVec3* r4, const xVec3* r5, F32* t_in, F32* t_out)
{
    return (ClipPlane(r5->x, -r4->x - r3->x, t_in, t_out) &&
            ClipPlane(-r5->x, r4->x - r3->x, t_in, t_out) &&
            ClipPlane(r5->y, -r4->y - r3->y, t_in, t_out) &&
            ClipPlane(-r5->y, r4->y - r3->y, t_in, t_out) &&
            ClipPlane(r5->z, -r4->z - r3->z, t_in, t_out) &&
            ClipPlane(-r5->z, r4->z - r3->z, t_in, t_out));
}

void iBoxIsectRay(const _xBox* b, const xRay3* r, xIsect* isx)
{
    xVec3 var_14, var_20;
    F32 t_in, t_out;

    if (!(r->flags & 0x400))
    {
        ((xRay3*)r)->min_t = 0.0f;
    }

    if (!(r->flags & 0x800))
    {
        ((xRay3*)r)->max_t = 1.0f;
    }

    t_in = -1e19f;
    t_out = 1e19f;

    var_14.x = b->upper.x - b->lower.x;
    var_14.y = b->upper.y - b->lower.y;
    var_14.z = b->upper.z - b->lower.z;

    var_20.x = b->lower.x + b->upper.x;
    var_20.y = b->lower.y + b->upper.y;

    var_14.x *= 0.5f;
    var_14.y *= 0.5f;
    var_14.z *= 0.5f;

    var_20.z = b->lower.z + b->upper.z;

    var_20.x *= -0.5f;
    var_20.y *= -0.5f;
    var_20.z *= -0.5f;

    var_20.x += r->origin.x;
    var_20.y += r->origin.y;
    var_20.z += r->origin.z;

    if (ClipBox(&var_14, &var_20, &r->dir, &t_in, &t_out))
    {
        if (t_in < r->min_t)
        {
            if (t_out < r->min_t)
            {
                isx->dist = t_out;
                isx->penned = 1.0f;
                isx->contained = 1.0f;
            }
            else
            {
                isx->dist = t_out;
                isx->penned = -1.0f;
                isx->contained = -1.0f;
            }
        }
        else
        {
            if (t_in <= r->max_t)
            {
                isx->dist = t_in;
                isx->penned = -1.0f;
                isx->contained = 1.0f;
            }
            else
            {
                isx->dist = t_in;
                isx->penned = 1.0f;
                isx->contained = 1.0f;
            }
        }
    }
    else
    {
        isx->penned = 1.0f;
        isx->contained = 1.0f;
    }
}

void iBoxIsectSphere(const _xBox* box, const xSphere* p, xIsect* isx)
{
    U32 xcode, ycode, zcode;
    F32 lo, hi;

    lo = p->center.x - p->r;
    hi = p->center.x + p->r;

    if (lo < box->lower.x)
    {
        if (hi < box->lower.x)
        {
            xcode = 2;
        }
        else if (hi > box->upper.x)
        {
            xcode = 0;
        }
        else
        {
            xcode = 1;
        }
    }
    else
    {
        if (lo > box->upper.x)
        {
            xcode = 5;
        }
        else if (hi > box->upper.x)
        {
            xcode = 4;
        }
        else
        {
            xcode = 3;
        }
    }

    if (xcode % 3 == 2)
    {
        isx->penned = 1.0f;
        return;
    }

    lo = p->center.y - p->r;
    hi = p->center.y + p->r;

    if (lo < box->lower.y)
    {
        if (hi < box->lower.y)
        {
            ycode = 2;
        }
        else if (hi > box->upper.y)
        {
            ycode = 0;
        }
        else
        {
            ycode = 1;
        }
    }
    else
    {
        if (lo > box->upper.y)
        {
            ycode = 5;
        }
        else if (hi > box->upper.y)
        {
            ycode = 4;
        }
    }

    if (ycode % 3 == 2)
    {
        isx->penned = 1.0f;
        return;
    }

    lo = p->center.z - p->r;
    hi = p->center.z + p->r;

    if (lo < box->lower.z)
    {
        if (hi < box->lower.z)
        {
            zcode = 2;
        }
        else if (hi > box->upper.z)
        {
            zcode = 0;
        }
        else
        {
            zcode = 1;
        }
    }
    else
    {
        if (lo > box->upper.z)
        {
            zcode = 5;
        }
        else if (hi > box->upper.z)
        {
            zcode = 4;
        }
    }

    if (zcode % 3 == 2)
    {
        isx->penned = 1.0f;
        return;
    }

    iBoxIsectVec(box, &p->center, isx);

    if (isx->penned < 0.0f)
    {
        xVec3 mid;

        xVec3Add(&mid, &box->lower, &box->upper);
        xVec3SMulBy(&mid, 0.5f);
        xVec3Sub(&isx->norm, &p->center, &mid);

        isx->dist = xVec3Length(&isx->norm);
        isx->contained = -1.0f;
    }
    else
    {
        isx->flags = xcode | (ycode << 4) | (zcode << 8);

        iBoxVecDist(box, &p->center, isx);

        isx->penned = isx->dist - p->r;
        isx->contained = 1.0f;
    }
}

void iBoxInitBoundVec(_xBox* b, const xVec3* v)
{
    xVec3Copy(&b->lower, v);
    xVec3Copy(&b->upper, v);
}

void iBoxBoundVec(_xBox* o, const _xBox* b, const xVec3* v)
{
    xVec3Init(&o->lower, MIN(v->x, b->lower.x), MIN(v->y, b->lower.y), MIN(v->z, b->lower.z));
    xVec3Init(&o->upper, MAX(v->x, b->upper.x), MAX(v->y, b->upper.y), MAX(v->z, b->upper.z));
}
