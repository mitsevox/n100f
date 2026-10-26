#include "xPar.h"

xPar* gParDead;

xPar* xParAlloc()
{
    xPar* dead = gParDead;
    if (gParDead == NULL)
    {
        return NULL;
    }
    if (gParDead->m_next != NULL)
    {
        gParDead->m_next->m_prev = NULL;
    }
    gParDead = dead->m_next;
    dead->m_next = NULL;
    dead->m_prev = NULL;
    return dead;
}

void xParFree(xPar* par)
{
    if (par->m_next != NULL)
    {
        par->m_next->m_prev = par->m_prev;
    }
    if (par->m_prev != NULL)
    {
        par->m_prev->m_next = par->m_next;
    }
    if (gParDead != NULL)
    {
        gParDead->m_prev = par;
    }
    par->m_next = gParDead;
    par->m_prev = NULL;
    gParDead = par;
}
