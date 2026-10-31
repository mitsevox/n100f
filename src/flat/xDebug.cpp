#include "xDebug.h"

#include <string.h>

// Debug display modes: the first 14 are the engine's own (dm_names, in DM_* order); xDebugModeAdd
// hands out the rest, up to 46.
void (*sDebugModeCallbackFunc[46])();
char* sDebugModeName[46];

static char dm_names[14][16] = {
    "",       "dm_collision", "dm_anim", "dm_ent",    "dm_shadow", "dm_snd",  "dm_graph",
    "dm_gui", "dm_par",       "dm_fade", "dm_render", "dm_mem",    "dm_vram", "dm_pad",
};

// Debug printing is compiled out.
void xprintf(const char* msg, ...)
{
}

// A new named mode in the first free slot after the built-in ones; -1 when the name is taken or
// every slot is used.
static inline S32 xDebugModeAdd(char* debugModeName)
{
    S32 i;

    for (i = 14; i < 46; i++)
    {
        if (sDebugModeName[i] && strcmp(sDebugModeName[i], debugModeName) == 0)
        {
            return -1;
        }
    }

    for (i = 14; i < 46; i++)
    {
        if (!sDebugModeName[i])
        {
            sDebugModeName[i] = debugModeName;
            return i;
        }
    }

    return -1;
}

S32 xDebugModeAdd(char* debugModeName, void (*func)())
{
    S32 modeid = xDebugModeAdd(debugModeName);

    if (modeid >= 0)
    {
        sDebugModeCallbackFunc[modeid] = func;
    }

    return modeid;
}

void xDebugModeRegisterCallback(S32 mode, void (*func)())
{
    sDebugModeCallbackFunc[mode] = func;
}

// Clear every mode's callback and name the built-in modes.
void xDebugInit()
{
    for (S32 i = 0; i < 46; i++)
    {
        sDebugModeCallbackFunc[i] = NULL;
        if (i < 14)
        {
            sDebugModeName[i] = dm_names[i];
        }
        else
        {
            sDebugModeName[i] = NULL;
        }
    }
}

// The retail build keeps no debug display: the per-frame update and shutdown do nothing.
void xDebugUpdate()
{
}

void xDebugExit()
{
}
