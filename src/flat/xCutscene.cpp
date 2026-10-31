#include "types.h"

struct xCutscene;

// The GC build plays its cutscenes as movies (zFMV), so the cutscene player is reduced to stubs:
// there is no cutscene table to set up, and destroying a cutscene always reports success.
// (GC/1.3.2 emits a file's functions in reverse order: Destroy comes first in the target.)
void xCutscene_Init(void* toc)
{
}

S32 xCutscene_Destroy(xCutscene* csn)
{
    return 1;
}
