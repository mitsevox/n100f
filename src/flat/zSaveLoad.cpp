#include "xsavegame.h"

#include <string.h>

S32 xSGT_LoadLoadCB(void* vp, st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, U32 ui,
                    S32 i);
S32 xSGT_SaveInfoCB(void* vp, st_XSAVEGAME_DATA* xsgdata, S32* need, S32* most);

static char currSceneStr[32] = "TEMP";
static char sceneRead[32] = "0000";

S32 xSGT_LoadLoadCB(void* vp, st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, U32 ui,
                    S32 i)
{
    char bigbuf[32] = {};
    S32 compdiff = 0;

    // The saved scene id is accepted only if it has the current id's length.
    xSGReadData(xsgdata, rctxt, bigbuf, 1, strlen(currSceneStr));
    if (strlen(currSceneStr) != strlen(bigbuf))
    {
        compdiff = 1;
    }
    if (compdiff == 0)
    {
        strcpy(sceneRead, bigbuf);
    }

    return compdiff == 0;
}

S32 xSGT_SaveInfoCB(void* vp, st_XSAVEGAME_DATA* xsgdata, S32* need, S32* most)
{
    *need = xSGWriteStrLen(currSceneStr);
    *most = *need << 1;
    return 1;
}
