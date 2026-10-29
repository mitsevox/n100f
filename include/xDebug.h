#ifndef XDEBUG_H
#define XDEBUG_H

#include "types.h"

// Debug display modes, in the order of the debug mode names (dm_names) in xDebug.cpp.
enum
{
    DM_NONE,
    DM_COLLISION,
    DM_ANIM,
    DM_ENT,
    DM_SHADOW,
    DM_SND,
    DM_GRAPH,
    DM_GUI,
    DM_PAR,
    DM_RENDER,
    DM_MEM,
    DM_VRAM,
    DM_PAD
};

void xDebugModeRegisterCallback(S32 mode, void (*func)());
S32 xDebugModeAdd(char* debugModeName, void (*func)());
void xprintf(const char* msg, ...);

#endif
