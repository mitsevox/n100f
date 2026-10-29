#include "types.h"

extern "C" S32 fn_800BF644();
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg, U8* rgba);

// Update the loading screen unless the menu suppresses transitions.
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg)
{
    if (!fn_800BF644())
    {
        zGameScreenTransitionUpdate(percentComplete, msg, NULL);
    }
}
