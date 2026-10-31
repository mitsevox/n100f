#include "xCollide.h"
#include "iCollide.h"
#include "iMath3.h"
#include "xEnt.h"
#include "xMath3.h"
#include "xMathInlines.h"

// _xCollis flags asked for and returned: 1 hit; 0x10 the sphere is fully inside; 0x200 fill
// norm; 0x400 fill tohit; 0x800 fill depen (the push-out); 0x1000 fill hdng; 0x200 and 0x1000
// both need the heading, and the sphere-sphere test also derives it from tohit (0x1600).

// Which side of the entity a hit lies on: floor or ceiling when the hit is mostly vertical,
// otherwise front, left, rear or right in the entity's own frame (mat's right and at axes).
_xCollsIdx xCollideGetCollsIdx(const xVec3* tohit, const xMat3x3* mat)
{
    if (tohit->y * tohit->y > tohit->x * tohit->x + tohit->z * tohit->z)
    {
        return (_xCollsIdx)!(tohit->y < 0.0f);
    }

    F32 local_x = mat->right.x * tohit->x + mat->right.z * tohit->z;
    F32 local_z = mat->at.x * tohit->x + mat->at.z * tohit->z;
    if (local_x > 0.0f)
    {
        if (local_z > 0.0f)
        {
            if (local_x > local_z)
            {
                return k_XCOLLS_IDX_LEFT;
            }
            else
            {
                return k_XCOLLS_IDX_FRONT;
            }
        }
        else
        {
            if (local_x > -local_z)
            {
                return k_XCOLLS_IDX_LEFT;
            }
            else
            {
                return k_XCOLLS_IDX_REAR;
            }
        }
    }
    else
    {
        if (local_z > 0.0f)
        {
            if (local_x < -local_z)
            {
                return k_XCOLLS_IDX_RIGHT;
            }
            else
            {
                return k_XCOLLS_IDX_FRONT;
            }
        }
        else
        {
            if (local_x < local_z)
            {
                return k_XCOLLS_IDX_RIGHT;
            }
            else
            {
                return k_XCOLLS_IDX_REAR;
            }
        }
    }
}

// Collision setup for a scene; the platform layer does the work.
void xCollideInit(_xScene* sc)
{
    iCollideInit(sc);
}

// Sphere a against sphere b. On a hit, dist is a's radius plus the penetration, tohit points
// from a's centre to the contact and depen pushes a back out.
U32 xSphereHitsSphere(const xSphere* a, const xSphere* b, _xCollis* coll)
{
    U32 uVar1;

    xIsect isx;

    iSphereIsectSphere(b, a, &isx);

    if (!(isx.penned <= 0.0f))
    {
        uVar1 = 0;
        coll->flags &= 0xfffffffe;
    }
    else
    {
        if (isx.contained <= 0.0f)
        {
            coll->flags |= 0x10;
        }
        coll->dist = a->r + isx.penned;
        if ((coll->flags & 0x1600) != 0)
        {
            if (isx.dist == 0.0f)
            {
                xVec3Copy(&coll->tohit, &g_O3);
            }
            else
            {
                xVec3SMul(&coll->tohit, &isx.norm, -coll->dist / isx.dist);
            }
        }
        if ((coll->flags & 0x800) != 0)
        {
            if (isx.dist == 0.0f)
            {
                xVec3Copy(&coll->depen, &g_O3);
            }
            else
            {
                xVec3SMul(&coll->depen, &isx.norm, -isx.penned / isx.dist);
            }
        }
        if ((coll->flags & 0x1200) != 0)
        {
            xVec3Normalize(&coll->hdng, &coll->tohit);
        }
        if ((coll->flags & 0x200) != 0)
        {
            xVec3Inv(&coll->norm, &coll->hdng);
        }
        uVar1 = 1;
        coll->flags |= 1;
    }
    return uVar1;
}

// Sphere against an axis-aligned box.
U32 xSphereHitsBox(const xSphere* a, const _xBox* b, _xCollis* coll)
{
    U32 uVar1;

    xIsect isx;

    iBoxIsectSphere(b, a, &isx);

    if (!(isx.penned <= 0.0f))
    {
        uVar1 = 0;
        coll->flags &= 0xfffffffe;
    }
    else
    {
        if (isx.contained <= 0.0f)
        {
            coll->flags |= 0x10;
        }
        coll->dist = isx.dist;
        if (coll->flags & 0x0400)
        {
            xVec3Copy(&coll->tohit, &isx.norm);
        }
        if ((coll->flags & 0x800) != 0)
        {
            if (isx.dist == 0.0f)
            {
                xVec3Copy(&coll->depen, &g_O3);
            }
            else
            {
                xVec3SMul(&coll->depen, &isx.norm, isx.penned / isx.dist);
            }
        }
        if ((coll->flags & 0x1200) != 0)
        {
            xVec3Normalize(&coll->hdng, &isx.norm);
        }
        if ((coll->flags & 0x200) != 0)
        {
            xVec3Inv(&coll->norm, &coll->hdng);
        }
        uVar1 = 1;
        coll->flags |= 1;
    }
    return uVar1;
}

// Sphere against a box with a transform: the sphere is brought into the box's space (its
// radius divided by the matrix's scale, taken from the length of the right axis), tested
// against the plain box, and the results are turned back into world space.
U32 xSphereHitsOBB(const xSphere* a, const _xBox* b, const xMat4x3* m, _xCollis* coll)
{
    xMat3x3 mnormal;
    xSphere mxa;

    F32 scale = xsqrt(xVec3Length2(&m->right));
    xMat3x3Normalize(&mnormal, (const xMat3x3*)m);
    xVec3Sub(&mxa.center, &a->center, &m->pos);
    xMat3x3Tolocal(&mxa.center, (const xMat3x3*)m, &mxa.center);
    mxa.r = a->r / scale;

    xSphereHitsBox(&mxa, b, coll);

    if (!(coll->flags & 1))
    {
        return 0;
    }

    if (coll->flags & 0x200)
    {
        xMat3x3RMulVec(&coll->norm, &mnormal, &coll->norm);
    }

    if (coll->flags & 0x800)
    {
        xMat3x3RMulVec(&coll->depen, (const xMat3x3*)m, &coll->depen);
    }

    if (coll->flags & 0x400)
    {
        xMat3x3RMulVec(&coll->tohit, (const xMat3x3*)m, &coll->tohit);
    }

    if (coll->flags & 0x1000)
    {
        xMat3x3RMulVec(&coll->hdng, &mnormal, &coll->hdng);
    }

    coll->dist *= scale;
    return 1;
}
