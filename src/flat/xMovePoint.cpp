#include "xMovePoint.h"

void xMovePointReset(xMovePoint* m)
{
    xBaseReset(m, m->asset);

    m->on = m->asset->on;
    m->delay = m->asset->delay;
}

void xMovePointSplineDestroy(xMovePoint* m)
{
    if (m->spl != NULL)
    {
        m->spl = NULL;
    }
}
