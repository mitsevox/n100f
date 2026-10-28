#ifndef XPARGROUP_H
#define XPARGROUP_H

#include "types.h"

// Opaque: no reconstructed code reads its members.
struct xParGroup
{
    U8 opaque_0[0x24];
};

void xParGroupKillAllParticles(xParGroup* ps);
void xParGroupSetActive(xParGroup* ps, U32 isActive);
void xParGroupUnregister(xParGroup* ps);
void xParGroupSetVisibility(xParGroup* ps, S32 vis);
void xParGroupSetStealing(xParGroup* ps, S32 stealing);
void xParGroupSetBack2Life(xParGroup* ps, S32 b2l);
void xParGroupSetAging(xParGroup* ps, S32 age);

#endif
