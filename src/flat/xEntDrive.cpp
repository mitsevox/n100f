#include "xEnt.h"
#include "xMath3.h"

static inline void xMat4x3Toworld(xVec3* o, const xMat4x3* m, const xVec3* v)
{
    xMat3x3RMulVec(o, (const xMat3x3*)m, v);
    xVec3AddTo(o, &m->pos);
}

static inline void xMat4x3Tolocal(xVec3* o, const xMat4x3* m, const xVec3* v)
{
    xVec3Sub(o, v, &m->pos);
    xMat3x3Tolocal(o, (const xMat3x3*)m, o);
}

// An entity is driven when it stands on a moving one, such as a platform: it keeps its position
// relative to the driver and, with flag 1, turns with it. Mounting and dismounting blend in and
// out over a time: s is the weight of the new driver and os the weight of the old one.

void xEntDriveInit(_xEntDrive* drv, _xEnt* driven)
{
    drv->flags = 0;
    drv->driven = driven;
    drv->driver = NULL;
    drv->s = 0.0f;
    drv->tm = 0.0f;
    drv->tmr = 0.0f;
    drv->odriver = NULL;
    drv->os = 0.0f;
    drv->otm = 0.0f;
    drv->otmr = 0.0f;
}

// Mounting the driver that is still blending out picks the blend up where it is.
void xEntDriveMount(_xEntDrive* drv, _xEnt* driver, F32 mt)
{
    if (driver == drv->odriver && drv->os)
    {
        drv->driver = driver;
        drv->s = drv->os;
        drv->tm = mt;
        drv->tmr = mt * (1.0f - drv->s);
        drv->odriver = NULL;
        drv->os = 0.0f;
        drv->otm = 0.0f;
        drv->otmr = 0.0f;
    }
    else
    {
        drv->driver = driver;
        drv->s = 0.0f;
        drv->tm = mt;
        drv->tmr = mt;
    }

    if (drv->flags & 1)
    {
        xVec3 euler;
        xMat3x3GetEuler((xMat3x3*)&drv->driver->frame->mat, &euler);
        drv->yaw = euler.x;
    }

    xVec3Copy(&drv->q, &drv->driven->frame->mat.pos);
    xMat4x3Tolocal(&drv->p, &drv->driver->frame->mat, &drv->q);
}

void xEntDriveDismount(_xEntDrive* drv, F32 dmt)
{
    if (drv->driver != NULL)
    {
        drv->odriver = drv->driver;
        drv->os = drv->s;
        drv->otm = dmt;
        drv->otmr = dmt * drv->os;
        drv->driver = NULL;
        drv->s = 0.0f;
        drv->tm = 0.0f;
        drv->tmr = 0.0f;
        xVec3Copy(&drv->q, &drv->driven->frame->mat.pos);
        xMat4x3Tolocal(&drv->op, &drv->odriver->frame->mat, &drv->q);
    }
}

// Moves the driven entity by the weighted change of its spot on the old and the new driver.
void xEntDriveUpdate(_xEntDrive* drv, _xScene* sc, F32 dt)
{
    if (drv->odriver == NULL && drv->driver == NULL)
    {
        return;
    }

    if (drv->otmr > 0.0f)
    {
        drv->otmr -= dt;
        if (drv->otmr <= 0.0f)
        {
            drv->os = 0.0f;
            drv->otmr = 0.0f;
        }
        else
        {
            drv->os = drv->otmr / drv->otm;
        }
    }

    if (drv->tmr > 0.0f)
    {
        drv->tmr -= dt;
        if (drv->tmr <= 0.0f)
        {
            drv->s = 1.0f;
            drv->tmr = 0.0f;
        }
        else
        {
            drv->s = 1.0f - drv->tmr / drv->tm;
        }
    }

    if (!drv->os && !drv->s)
    {
        return;
    }

    if (drv->s && (drv->flags & 1))
    {
        xVec3 euler;
        xMat3x3 rot;
        xMat3x3GetEuler((xMat3x3*)&drv->driver->frame->mat, &euler);
        xMat3x3RotY(&rot, drv->s * (euler.x - drv->yaw));
        xMat3x3Mul((xMat3x3*)&drv->driven->frame->mat, (xMat3x3*)&drv->driven->frame->mat, &rot);
        drv->yaw = euler.x;
    }

    xVec3 newq;
    if (drv->os)
    {
        xMat4x3Toworld(&newq, &drv->odriver->frame->mat, &drv->op);
        xVec3Sub(&drv->driven->frame->dpos, &newq, &drv->q);
        xVec3SMulBy(&drv->driven->frame->dpos, drv->os);
        xVec3AddTo(&drv->driven->frame->mat.pos, &drv->driven->frame->dpos);
    }

    if (drv->s)
    {
        xMat4x3Toworld(&newq, &drv->driver->frame->mat, &drv->p);
        xVec3Sub(&drv->driven->frame->dpos, &newq, &drv->q);
        xVec3SMulBy(&drv->driven->frame->dpos, drv->s);
        xVec3AddTo(&drv->driven->frame->mat.pos, &drv->driven->frame->dpos);
    }

    xVec3Copy(&drv->q, &drv->driven->frame->mat.pos);

    if (drv->os)
    {
        xMat4x3Tolocal(&drv->op, &drv->odriver->frame->mat, &drv->q);
    }

    if (drv->s)
    {
        xMat4x3Tolocal(&drv->p, &drv->driver->frame->mat, &drv->q);
    }
}
