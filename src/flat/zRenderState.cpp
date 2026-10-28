#include "types.h"

#include "rwcore.h"

void zRenderStateSetDstBlendMode(S32 xmode);
void zRenderStateSetSrcBlendMode(S32 xmode);

// The game's blend modes map one to one onto RenderWare's rwBLEND values.
static S32 sBlendTable[11] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

void zRenderStateSetDstBlendMode(S32 xmode)
{
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)sBlendTable[xmode]);
}

void zRenderStateSetSrcBlendMode(S32 xmode)
{
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)sBlendTable[xmode]);
}
