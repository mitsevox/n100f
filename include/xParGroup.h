#ifndef XPARGROUP_H
#define XPARGROUP_H

#include "types.h"

struct xPar;
struct xParCmdTex;

struct xParGroup
{
    xPar* m_root;
    xPar* m_dead;
    S32 m_num_of_particles;
    U8 m_alive;
    U8 m_killWhenDead;
    U8 m_active;
    U8 m_visible;
    U8 m_culled;
    U8 m_priority;
    U8 m_flags;
    U8 m_regidx;
    xParGroup* m_next;
    xParGroup* m_prev;
    void (*draw)(void*, xParGroup*);
    xParCmdTex* m_cmdTex;
};

void xParGroupAnimate(xParGroup* ps, F32 dt);
void xParGroupKillAllParticles(xParGroup* ps);
void xParGroupSetActive(xParGroup* ps, U32 isActive);
void xParGroupUnregister(xParGroup* ps);
void xParGroupRegister(xParGroup* ps);
void xParGroupSetPriority(xParGroup* ps, U8 val);
void xParGroupSetVisibility(xParGroup* ps, S32 vis);
void xParGroupSetStealing(xParGroup* ps, S32 stealing);
void xParGroupSetBack2Life(xParGroup* ps, S32 b2l);
void xParGroupSetAging(xParGroup* ps, S32 age);
void xParGroupInit(xParGroup* ps);

#endif
