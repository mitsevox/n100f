#include "xMovePoint.h"

#include "iMath.h"
#include "xMath3.h"
#include "xMemMgr.h"
#include "xScene.h"
#include "xSpline.h"

void xMovePointInit(xMovePoint* m, p2MovePointAsset* asset)
{
    xBaseInit(m, asset);
    m->asset = asset;
    m->pos = &m->asset->pos;
    m->on = asset->on;
    m->delay = asset->delay;
    m->flg_props = asset->flg_props;
    m->spl = NULL;

    if (asset->numPoints != 0)
    {
        m->nodes = (xMovePoint**)xMemAlloc(gActiveHeap, asset->numPoints * sizeof(xMovePoint*));
    }
    else
    {
        m->nodes = NULL;
    }
}

void xMovePointSave(xMovePoint* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

void xMovePointLoad(xMovePoint* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

void xMovePointReset(xMovePoint* m)
{
    xBaseReset(m, m->asset);

    m->on = m->asset->on;
    m->delay = m->asset->delay;
}

void xMovePointSetup(xMovePoint* m, _xScene* sc)
{
    U32* pointIds;
    U16 idx;

    m->node_wt_sum = 0;
    pointIds = (U32*)(m->asset + 1);

    for (idx = 0; idx < m->asset->numPoints; idx++)
    {
        m->nodes[idx] = (xMovePoint*)xSceneResolvID(sc, pointIds[idx]);
        m->node_wt_sum += m->nodes[idx]->asset->wt;
        m->nodes[idx]->prev = m;
    }
}

void xMovePointSplineSetup(xMovePoint* m)
{
    xMovePoint* w0;
    xMovePoint* w2;
    xMovePoint* w3;
    xVec3 points[2];
    xVec3 p1;
    xVec3 p2;

    if (m->asset->bezIndex != 1)
    {
        return;
    }

    if (m->spl != NULL)
    {
        return;
    }

    w0 = m->prev;
    w2 = m->nodes[0];

    points[0] = *w0->pos;

    if (w2->asset->bezIndex != 0)
    {
        w3 = w2->nodes[0];
        p1 = *m->pos;
        p2 = *w2->pos;
        points[1] = *w3->pos;
    }
    else
    {
        p1.x = (1.0f / 3.0f) * w0->pos->x + (2.0f / 3.0f) * m->pos->x;
        p1.y = (1.0f / 3.0f) * w0->pos->y + (2.0f / 3.0f) * m->pos->y;
        p1.z = (1.0f / 3.0f) * w0->pos->z + (2.0f / 3.0f) * m->pos->z;
        p2.x = (2.0f / 3.0f) * m->pos->x + (1.0f / 3.0f) * w2->pos->x;
        p2.y = (2.0f / 3.0f) * m->pos->y + (1.0f / 3.0f) * w2->pos->y;
        p2.z = (2.0f / 3.0f) * m->pos->z + (1.0f / 3.0f) * w2->pos->z;
        points[1] = *w2->pos;
    }

    m->spl = xSpline3_Bezier(points, NULL, 2, 0, &p1, &p2);
    xSpline3_ArcInit(m->spl, 20);
}

void xMovePointSplineDestroy(xMovePoint* m)
{
    if (m->spl != NULL)
    {
        m->spl = NULL;
    }
}

F32 xMovePointGetNext(const xMovePoint* m, const xMovePoint* prev, xMovePoint** next, xVec3* hdng)
{
    S32 rnd;
    U16 idx;
    xMovePoint* previousOption;

    if (m->asset->numPoints < 1)
    {
        *next = NULL;
        return 0.0f;
    }

    previousOption = NULL;
    rnd = irand() % m->node_wt_sum;

    for (idx = 0; idx < m->asset->numPoints; idx++)
    {
        *next = m->nodes[idx];
        rnd -= (*next)->asset->wt;

        if (!(*next)->on)
        {
            *next = NULL;
        }
        else
        {
            previousOption = *next;

            if (*next == prev)
            {
                *next = NULL;
            }
            else if (rnd < 0)
            {
                break;
            }
        }
    }

    if (*next == NULL)
    {
        if (previousOption != NULL)
        {
            *next = previousOption;
        }
        else
        {
            return 0.0f;
        }
    }

    if (hdng != NULL)
    {
        xVec3Sub(hdng, (*next)->pos, m->pos);
        return xVec3Normalize(hdng, hdng);
    }

    return 0.0f;
}

xVec3* xMovePointGetPos(const xMovePoint* m)
{
    return m->pos;
}
