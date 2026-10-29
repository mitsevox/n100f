#include "xsavegame.h"
#include "iSnd.h"
#include "xSnd.h"
#include "xutil.h"
#include "zGlobals.h"
#include "zSaveLoad.h"
#include "zScene.h"

#include <string.h>

S32 xSGT_LoadLoadCB(void* vp, st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, U32 ui,
                    S32 i);
S32 xSGT_SaveInfoCB(void* vp, st_XSAVEGAME_DATA* xsgdata, S32* need, S32* most);
void xPadRumbleEnable(S32 idx, S32 enable);

static char currSceneStr[32] = "TEMP";
static char sceneRead[32] = "0000";
static S32 currentCard = -1;
static S32 currentGame = -1;

U32 zSaveLoad_slotIsEmpty(U32 i)
{
    // Empty slots are identified by their displayed label.
    return strcmp(zSaveLoadGameTable[i].label, "Empty") == 0;
}

S32 xSGT_LoadPrefsCB(void* vp, st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt,
                     U32 ui, S32 i)
{
    xSGReadData(xsgdata, rctxt, &gSnd.stereo, 1);
    // The console's current sound mode takes precedence over the saved value.
    gSnd.stereo = iSndGetStereo();
    xSGReadData(xsgdata, rctxt, &gSnd.streamvol.volL, 1);
    xSGReadData(xsgdata, rctxt, &gSnd.streamvol.volR, 1);
    xSGReadData(xsgdata, rctxt, &gSnd.fxvol.volL, 1);
    xSGReadData(xsgdata, rctxt, &gSnd.fxvol.volR, 1);
    xSGReadData(xsgdata, rctxt, &globals.option_vibration, 1);
    if (globals.option_vibration)
        xPadRumbleEnable(0, 1);
    else
        xPadRumbleEnable(0, 0);
    return 1;
}

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

S32 xSGT_SaveProcPrefsCB(void* vp, st_XSAVEGAME_DATA* xsgdata,
                         st_XSAVEGAME_WRITECONTEXT* wctxt)
{
    // Refresh the sound mode before writing the saved preferences.
    gSnd.stereo = iSndGetStereo();
    S32 sum = xSGWriteData(xsgdata, wctxt, &gSnd.stereo, 1);
    sum += xSGWriteData(xsgdata, wctxt, &gSnd.streamvol.volL, 1);
    sum += xSGWriteData(xsgdata, wctxt, &gSnd.streamvol.volR, 1);
    sum += xSGWriteData(xsgdata, wctxt, &gSnd.fxvol.volL, 1);
    sum += xSGWriteData(xsgdata, wctxt, &gSnd.fxvol.volR, 1);
    sum += xSGWriteData(xsgdata, wctxt, &globals.option_vibration, 1);
    return sum + 1;
}

S32 xSGT_SaveInfoPrefsCB(void* vp, st_XSAVEGAME_DATA* xsgdata, S32* need, S32* most)
{
    // Six four-byte preference values are written by the save callback.
    *need = 24;
    *most = *need << 1;
    return 1;
}

S32 xSGT_SaveProcCB(void* vp, st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt)
{
    // Save the current scene as its four-character ID string.
    strcpy(currSceneStr, xUtil_idtag2string(globals.sceneCur->sceneID, 0));
    return xSGWriteData(xsgdata, wctxt, currSceneStr, 1, strlen(currSceneStr)) + 1;
}

S32 xSGT_SaveInfoCB(void* vp, st_XSAVEGAME_DATA* xsgdata, S32* need, S32* most)
{
    *need = xSGWriteStrLen(currSceneStr);
    *most = *need << 1;
    return 1;
}

S32 zSaveLoad_getcard()
{
    // The selected card is shared with the save/load UI.
    return currentCard;
}

S32 zSaveLoad_getgame()
{
    // The selected save slot is shared with the save/load UI.
    return currentGame;
}
