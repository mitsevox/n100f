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

#define XPARGROUP_UNK1 0x1
#define XPARGROUP_ALLOCPARS 0x2
#define XPARGROUP_NOAGING 0x4
#define XPARGROUP_NOBACK2LIFE 0x8
#define XPARGROUP_NOSTEAL 0x10

void xParGroupAnimate(xParGroup* ps, F32 dt);
xPar* xParGroupAddPar(xParGroup* ps);
void xParGroupKillAllParticles(xParGroup* ps);
void xParGroupKillPar(xParGroup* ps, xPar* p);
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
