#include "iFX.h"

#include "iTime.h"
#include "xEnt.h"
#include "xMath.h"
#include "xMemMgr.h"
#include "xModel.h"
#include "xString.h"

#include <math.h>
#include <string.h>

_xEnt* warbEnt;
static xVec3* vbuf;
static xVec3* nbuf;
static U32 warbMode;
static F32 warbTime;
static F32 warbc[4] = { 0.5f, 2.0f, 0.9f, 1.0f };

static void iFXwarb(xVec3* orig_verts, xVec3* orig_normals, RpGeometry* geom);

RxPipeline* iFXanimUVCreatePipe()
{
    return NULL;
}

void iFXWarbInit(void* vp_ent, iFXWarbModes warb_mode)
{
    _xEnt* ent = (_xEnt*)vp_ent;
    if (ent != NULL)
    {
        RpGeometry* geom = ent->model->Data->geometry;
        S32 n = geom->numVertices;
        xVec3* verts = (xVec3*)geom->morphTarget->verts;
        vbuf = (xVec3*)xMemAlloc(gActiveHeap, n * 12);
        memcpy(vbuf, verts, n * sizeof(xVec3));

        if (ent->id == xStrHash("Tide"))
        {
            ent->material_alpha = 230.0f;
            ent->asset->seeThruSpeed = 0.0f;
        }

        F32 coeffs[4] = { 0.5f, 2.0f, 0.9f, 1.0f };
        iFXWarbSetParams(warb_mode, coeffs);
        warbEnt = ent;
        warbTime = 0.0f;
    }
}

void iFXWarbSetParams(U32 mode, F32* val)
{
    warbMode = mode;
    if (mode == IFX_WARB_NONE)
    {
        warbEnt = NULL;
        vbuf = NULL;
        nbuf = NULL;
    }

    if (val != NULL)
    {
        memcpy(warbc, val, sizeof(warbc));
    }
}

void iFXWarbUpdate(F32 dt)
{
    warbTime += dt;
    if (warbEnt != NULL)
    {
        iTimeGet();
        iFXwarb(vbuf, nbuf, warbEnt->model->Data->geometry);
        iTimeGet();
    }
}

static inline F32 WarbAngle(F32 a)
{
    return fmod(a, 2 * PI);
}

static inline F32 WarbSin(F32 a)
{
    return sin(a);
}

static void iFXwarb(xVec3* orig_verts, xVec3* orig_normals, RpGeometry* geom)
{
    if (geom != NULL && RpGeometryLock(geom, 6))
    {
        F32 miny;
        F32 maxy;
        S32 i;
        F32 t = warbTime;
        xVec3* ov = orig_verts;
        xVec3* nv = (xVec3*)geom->morphTarget->verts;

        switch (warbMode)
        {
        case IFX_WARB_WATER_WAVE:
            for (i = 0; i < geom->numVertices; i++, ov++, nv++)
            {
                F32 x = WarbAngle(warbc[1] * (ov->x + t));
                F32 y = WarbAngle(warbc[3] * (ov->z + t));
                y = WarbSin(y);
                x = WarbSin(x);
                nv->y = warbc[0] * x + ov->y + warbc[2] * y;
            }
            break;

        case IFX_WARB_WATER_RIPPLE:
            for (i = 0; i < geom->numVertices; i++, ov++, nv++)
            {
                F32 x = sqrtf(ov->x * ov->x + ov->y * ov->y + ov->z * ov->z);
                F32 y = WarbAngle(warbc[1] * (x + t));
                F32 a = cos(y);
                nv->y = warbc[0] * a + ov->y;
            }
            break;

        case IFX_WARB_DAN1:
            for (i = 0; i < geom->numVertices; i++, ov++, nv++)
            {
                F32 x = WarbAngle(warbc[1] * (ov->x + t));
                F32 a = WarbSin(x);
                nv->y = warbc[0] * a + ov->y;
            }
            break;

        case IFX_WARB_DAN2:
            for (i = 0; i < geom->numVertices; i++, ov++, nv++)
            {
                F32 x = WarbAngle(warbc[1] * (ov->x + t));
                F32 a = WarbSin(x);
                nv->y = warbc[0] * a + ov->y;
            }
            break;

        case IFX_WARB_TREE:
        {
            miny = ov->y;
            maxy = ov->y;
            for (i = 0; i < geom->numVertices; i++, ov++)
            {
                miny = MIN(miny, ov->y);
                maxy = MAX(maxy, ov->y);
            }

            miny = 0.5f * (maxy - miny) + miny;
            F32 ooy = 1.0f / (maxy - miny);
            t = -t;

            ov = orig_verts;
            for (i = 0; i < geom->numVertices; i++, ov++, nv++)
            {
                F32 mul;
                if (ov->y >= miny)
                {
                    F32 percent = ooy * (ov->y - miny);
                    F32 x = warbc[1] * (0.4 * percent + t);
                    x = WarbAngle(x);
                    x = sin(x);
                    mul = 0.5f * (percent * (warbc[0] * x));
                }
                else
                {
                    mul = 0.0f;
                }
                nv->x = ov->x + mul;
                nv->z = ov->z + mul;
            }
            break;
        }

        case IFX_WARB_WEB:
        {
            maxy = miny = orig_verts->y;
            for (i = 0; i < geom->numVertices; i++, ov++)
            {
                miny = MIN(miny, ov->y);
                maxy = MAX(maxy, ov->y);
            }

            F32 mul = 0.1f * (maxy - miny);
            miny += mul;
            maxy -= mul;
            F32 nt = -t;

            ov = orig_verts;
            for (i = 0; i < geom->numVertices; i++, ov++, nv++)
            {
                if (ov->y >= miny && ov->y <= maxy)
                {
                    F32 percent = MIN(ov->y - miny, maxy - ov->y);
                    F32 x = warbc[1] * (nt + percent);
                    F32 b = WarbAngle(x);
                    F32 a = WarbSin(b);
                    mul = percent * (warbc[0] * a);
                }
                else
                {
                    mul = 0.0f;
                }
                nv->x = ov->x + mul;
                nv->z = ov->z + mul;
            }
            break;
        }
        }

        RpGeometryUnlock(geom);
    }
}

// Unused.
static void WarbUnusedConsts(F32* f)
{
    *f = 0.0f;
}
