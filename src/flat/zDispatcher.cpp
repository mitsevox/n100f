#include "version.h"

#if VERSION_GIHE78_REV0
#include "xBase.h"
#include "xMemMgr.h"
#include "iSnd.h"
#include "zLaugh.h"
#include "zMusic.h"
#include "zGlobals.h"
#include "zScene.h"
#include "zHud.h"
#include "zGameState.h"
#include "zRumble.h"
#include "zFMV.h"
#include "xScene.h"
#include "xScrFx.h"
#include "xMath.h"
#include "xPad.h"
#include "zVar.h"
#include "zMenu.h"

#include <string.h>

struct st_ZDISPATCH_DATA : xBase
{
    p2BaseAsset* rawass;
    S32 placeholder;
};

struct st_ZDISPATCH_DEPOT
{
    st_ZDISPATCH_DATA* raw_pool;
    S32 raw_cnt;
};

enum en_DISPATCH_COMMAND
{
    ZDSP_CMD_INITDFLT = -1,
    ZDSP_CMD_CTRL_CFGGET = 0,
    ZDSP_CMD_CTRL_CFGSET = 1,
    ZDSP_CMD_CTRL_VIBEGET = 2,
    ZDSP_CMD_CTRL_VIBESET = 3,
    ZDSP_CMD_SNDMOD_GET = 4,
    ZDSP_CMD_SNDMOD_SET = 5,
    ZDSP_CMD_SNDVOL_GET = 6,
    ZDSP_CMD_SNDVOL_SET = 7,
    ZDSP_CMD_SNDVOL_INCR = 8,
    ZDSP_CMD_SNDVOL_DECR = 9,
    ZDSP_CMD_MUSVOL_GET = 10,
    ZDSP_CMD_MUSVOL_SET = 11,
    ZDSP_CMD_MUSVOL_INCR = 12,
    ZDSP_CMD_MUSVOL_DECR = 13,
    ZDSP_CMD_SFXVOL_GET = 14,
    ZDSP_CMD_SFXVOL_SET = 15,
    ZDSP_CMD_SFXVOL_INCR = 16,
    ZDSP_CMD_SFXVOL_DECR = 17,
    ZDSP_CMD_GSTATE_GET = 18,
    ZDSP_CMD_GSTATE_SET = 19,
    ZDSP_CMD_GMODE_GET = 20,
    ZDSP_CMD_GMODE_SET = 21,
    ZDSP_CHECKPNT_SET = 22,
    ZDSP_CMD_NOMORE = 23
};

struct st_ZDISPATCH_CONTEXT
{
    en_DISPATCH_COMMAND cmd;
    void* indata;
    void* inxtra;
    void* result;
};

U32 zVarEntryCB_VibrationOn();
U32 zVarEntryCB_SndMode();
U32 zVarEntryCB_SndMasterVol();
U32 zVarEntryCB_SndMusicVol();
U32 zVarEntryCB_SndFXVol();
U32 finishedGame();
void xSndSetMasterVol(F32 left, F32 right);
void xSndSetStreamVol(F32 left, F32 right);
void xSndSetFXVol(F32 left, F32 right);
void zSaveLoad_DispatchCB(U32 dispatchEvent, F32* toParam);
void zGameStateSwitchEvent(S32);
void zEntPlayer_StoreCheckPoint(F32*);

static S32 ZDSP_elcb_event(xBase*, xBase* xb, U32 toEvent, F32* toParam, xBase* toParamWidget);
static F32 ZDSP_volScale(S32 iv);
void zDispatcher_InitDep(st_ZDISPATCH_DATA* data, zScene*);
void zDispatcher_Init(st_ZDISPATCH_DATA* data, p2BaseAsset* asset);
void zDispatcher_Load(st_ZDISPATCH_DATA* data, xSerial* serial);
void zDispatcher_Save(st_ZDISPATCH_DATA* data, xSerial* serial);
st_ZDISPATCH_DATA* zDispatcher_getInst(st_ZDISPATCH_DATA* pool, S32 idx);
st_ZDISPATCH_DATA* zDispatcher_memPool(S32 cnt);
void zDispatcher_sceneFinish();
void zDispatcher_scenePrepare();
void zDispatcher_Shutdown();
void zDispatcher_Startup();

static st_ZDISPATCH_DEPOT g_zdsp_depot = {};
static S32 g_zdsp_init;

void zDispatcher_Startup()
{
}

void zDispatcher_Shutdown()
{
}

// Reset the shared depot when the first scene enters.
void zDispatcher_scenePrepare()
{
    st_ZDISPATCH_DEPOT* depot = &g_zdsp_depot;

    if (g_zdsp_init++ == 0)
    {
        memset(depot, 0, sizeof(st_ZDISPATCH_DEPOT));
    }
}

// Clear the depot after the last scene exits.
void zDispatcher_sceneFinish()
{
    st_ZDISPATCH_DEPOT* depot = &g_zdsp_depot;

    if (--g_zdsp_init == 0)
    {
        memset(depot, 0, sizeof(st_ZDISPATCH_DEPOT));
    }
}

st_ZDISPATCH_DATA* zDispatcher_memPool(S32 cnt)
{
    st_ZDISPATCH_DATA* pool;
    st_ZDISPATCH_DEPOT* depot = &g_zdsp_depot;

    if (cnt < 1)
    {
        return NULL;
    }

    pool = (st_ZDISPATCH_DATA*)xMemAlloc(gActiveHeap, cnt * sizeof(st_ZDISPATCH_DATA));
    memset(pool, 0, cnt * (S32)sizeof(st_ZDISPATCH_DATA));
    depot->raw_pool = pool;
    depot->raw_cnt = cnt;
    return pool;
}

// Select one dispatcher from the allocated pool.
st_ZDISPATCH_DATA* zDispatcher_getInst(st_ZDISPATCH_DATA* pool, S32 idx)
{
    return &pool[idx];
}

// Initialize the base and attach the event callback and asset links.
void zDispatcher_Init(st_ZDISPATCH_DATA* data, p2BaseAsset* asset)
{
    xBaseInit(data, asset);
    data->rawass = asset;
    data->eventFunc = ZDSP_elcb_event;
    if (data->linkCount)
    {
        data->link = (p2LinkAsset*)(data->rawass + 1);
    }
    else
    {
        data->link = NULL;
    }
    xSceneID2Name(globals.sceneCur, data->id);
}

// Look up the dispatcher name in the current scene.
void zDispatcher_InitDep(st_ZDISPATCH_DATA* data, zScene*)
{
    xSceneID2Name(globals.sceneCur, data->id);
}

// Save the base state to the stream.
void zDispatcher_Save(st_ZDISPATCH_DATA* data, xSerial* serial)
{
    xBaseSave(data, serial);
}

// Restore the base state from the save stream.
void zDispatcher_Load(st_ZDISPATCH_DATA* data, xSerial* serial)
{
    xBaseLoad(data, serial);
}

// Run one dispatcher command: read or change the controller, sound mode, the three volumes,
// the game state or the game mode.
static S32 ZDSP_doCommand(st_ZDISPATCH_DATA*, st_ZDISPATCH_CONTEXT* cmdCtxt)
{
    en_DISPATCH_COMMAND cmd = cmdCtxt->cmd;
    S32* indata = (S32*)cmdCtxt->indata;
    S32* result = (S32*)cmdCtxt->result;
    S32 iv;
    static S32 warned = 0;

    if (!warned)
    {
        warned = 1;
    }

    switch (cmd)
    {
    case ZDSP_CMD_CTRL_CFGGET:
        *result = 0;
        break;
    case ZDSP_CMD_CTRL_CFGSET:
        if (result)
        {
            *result = 0;
        }
        break;
    case ZDSP_CMD_CTRL_VIBEGET:
        *result = zVarEntryCB_VibrationOn();
        break;
    case ZDSP_CMD_CTRL_VIBESET:
        globals.option_vibration = *indata;
        xPadRumbleEnable(0, *indata);
        zRumbleStart(0, SDR_Test);
        break;
    case ZDSP_CMD_SNDMOD_GET:
        *result = zVarEntryCB_SndMode();
        break;
    case ZDSP_CMD_SNDMOD_SET:
        if (*indata == 0)
        {
            iSndStereo(0);
        }
        else if (*indata == 1)
        {
            iSndStereo(1);
        }
        if (result)
        {
            *result = zVarEntryCB_SndMode();
        }
        break;
    case ZDSP_CMD_SNDVOL_GET:
        *result = zVarEntryCB_SndMasterVol();
        break;
    case ZDSP_CMD_SNDVOL_SET:
    {
        iv = *indata;
        F32 vol = ZDSP_volScale(iv);
        xSndSetMasterVol(vol, vol);
        if (result)
        {
            *result = zVarEntryCB_SndMasterVol();
        }
        break;
    }
    case ZDSP_CMD_SNDVOL_INCR:
    {
        iv = zVarEntryCB_SndMasterVol();
        if (indata)
        {
            iv += *indata;
        }
        else
        {
            iv++;
        }
        F32 vol = ZDSP_volScale(iv);
        xSndSetMasterVol(vol, vol);
        if (result)
        {
            *result = zVarEntryCB_SndMasterVol();
        }
        break;
    }
    case ZDSP_CMD_SNDVOL_DECR:
    {
        iv = zVarEntryCB_SndMasterVol();
        if (indata)
        {
            iv -= *indata;
        }
        else
        {
            iv--;
        }
        F32 vol = ZDSP_volScale(iv);
        xSndSetMasterVol(vol, vol);
        if (result)
        {
            *result = zVarEntryCB_SndMasterVol();
        }
        break;
    }
    case ZDSP_CMD_MUSVOL_GET:
        *result = zVarEntryCB_SndMusicVol();
        break;
    case ZDSP_CMD_MUSVOL_SET:
    {
        iv = *indata;
        F32 vol = ZDSP_volScale(iv);
        xSndSetStreamVol(vol, vol);
        if (result)
        {
            *result = zVarEntryCB_SndMusicVol();
        }
        break;
    }
    case ZDSP_CMD_MUSVOL_INCR:
    {
        iv = zVarEntryCB_SndMusicVol();
        if (indata)
        {
            iv += *indata;
        }
        else
        {
            iv++;
        }
        F32 vol = ZDSP_volScale(iv);
        xSndSetStreamVol(vol, vol);
        if (result)
        {
            *result = zVarEntryCB_SndMusicVol();
        }
        break;
    }
    case ZDSP_CMD_MUSVOL_DECR:
    {
        iv = zVarEntryCB_SndMusicVol();
        if (indata)
        {
            iv -= *indata;
        }
        else
        {
            iv--;
        }
        F32 vol = ZDSP_volScale(iv);
        xSndSetStreamVol(vol, vol);
        if (result)
        {
            *result = zVarEntryCB_SndMusicVol();
        }
        break;
    }
    case ZDSP_CMD_SFXVOL_GET:
        *result = zVarEntryCB_SndFXVol();
        break;
    case ZDSP_CMD_SFXVOL_SET:
    {
        iv = *indata;
        F32 vol = ZDSP_volScale(iv);
        xSndSetFXVol(vol, vol);
        if (result)
        {
            *result = zVarEntryCB_SndFXVol();
        }
        break;
    }
    case ZDSP_CMD_SFXVOL_INCR:
    {
        iv = zVarEntryCB_SndFXVol();
        if (indata)
        {
            iv += *indata;
        }
        else
        {
            iv++;
        }
        F32 vol = ZDSP_volScale(iv);
        xSndSetFXVol(vol, vol);
        if (result)
        {
            *result = zVarEntryCB_SndFXVol();
        }
        break;
    }
    case ZDSP_CMD_SFXVOL_DECR:
    {
        iv = zVarEntryCB_SndFXVol();
        if (indata)
        {
            iv -= *indata;
        }
        else
        {
            iv--;
        }
        F32 vol = ZDSP_volScale(iv);
        xSndSetFXVol(vol, vol);
        if (result)
        {
            *result = zVarEntryCB_SndFXVol();
        }
        break;
    }
    case ZDSP_CMD_GSTATE_GET:
        *result = zGameStateGet();
        break;
    case ZDSP_CMD_GSTATE_SET:
        zGameStateSwitch(*indata);
        if (result)
        {
            *result = zGameStateGet();
        }
        break;
    case ZDSP_CMD_GMODE_GET:
        *result = zGameModeGet();
        break;
    case ZDSP_CMD_GMODE_SET:
        zGameModeSwitch((eGameMode)*indata);
        if (result)
        {
            *result = zGameModeGet();
        }
        break;
    case ZDSP_CHECKPNT_SET:
        break;
    }

    return 1;
}

// Build a command context and run it.
static void ZDSP_injectCmd(st_ZDISPATCH_DATA* dspdata, en_DISPATCH_COMMAND cmd, void* indata,
                           void* inxtra, void* result)
{
    st_ZDISPATCH_CONTEXT cmdctxt = { ZDSP_CMD_INITDFLT, NULL, NULL, NULL };

    cmdctxt.cmd = cmd;
    cmdctxt.indata = indata;
    cmdctxt.inxtra = inxtra;
    cmdctxt.result = result;
    ZDSP_doCommand(dspdata, &cmdctxt);
}

// Send a command that takes no input.
static void ZDSP_injectCmd(st_ZDISPATCH_DATA* dspdata, en_DISPATCH_COMMAND cmd)
{
    ZDSP_injectCmd(dspdata, cmd, NULL, NULL, NULL);
}

// Send a command whose input is one integer value.
static void ZDSP_injectCmd(st_ZDISPATCH_DATA* dspdata, en_DISPATCH_COMMAND cmd, S32 iv)
{
    ZDSP_injectCmd(dspdata, cmd, &iv, NULL, NULL);
}

// The dispatcher's event handler: turns menu and script events into dispatcher commands,
// game-state switches, save and load requests, unlocks, music and movie playback.
static S32 ZDSP_elcb_event(xBase*, xBase* xb, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    st_ZDISPATCH_DATA* dspdata = (st_ZDISPATCH_DATA*)xb;

    switch (toEvent)
    {
    case 0xa:
    {
        p2BaseAsset* asset = dspdata->rawass;
        xBaseReset(dspdata, asset);
        memset(&dspdata->rawass, 0, sizeof(*dspdata) - sizeof(xBase));
        zDispatcher_Init(dspdata, asset);
        xSceneID2Name(globals.sceneCur, dspdata->id);
        break;
    }
    case 0x10c:
        zLaughNotify(13);
        break;
    case 0x10d:
        zMusicNotify(3);
        break;
    case 0x10e:
        zMusicNotify(3);
        break;
    case 0x10f:
        zMusicNotify(3);
        break;
    case 0x110:
        zMusicNotify(3);
        break;
    case 0x129:
        zMusicNotifyEvent(toParam, toParamWidget);
        break;
    case 0x60:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_CTRL_CFGSET, 0);
        break;
    case 0x61:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_CTRL_CFGSET, 1);
        break;
    case 0x62:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_CTRL_CFGSET, 2);
        break;
    case 0x63:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_CTRL_CFGSET, 3);
        break;
    case 0x64:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_CTRL_VIBESET, 1);
        break;
    case 0x65:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_CTRL_VIBESET, 0);
        break;
    case 0x66:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_SNDMOD_SET, 0);
        break;
    case 0x67:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_SNDMOD_SET, 1);
        break;
    case 0x68:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_SNDVOL_INCR);
        break;
    case 0x69:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_SNDVOL_DECR);
        break;
    case 0x6a:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_MUSVOL_INCR);
        break;
    case 0x6b:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_MUSVOL_DECR);
        break;
    case 0x6c:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_SFXVOL_INCR);
        break;
    case 0x6d:
        ZDSP_injectCmd(dspdata, ZDSP_CMD_SFXVOL_DECR);
        break;
    // Events the dispatcher lists but ignores (enable/disable and two UI events).
    case 0x1:
    case 0x2:
        break;
    case 0x59:
    case 0x5a:
        break;
    case 0x89:
    case 0x8a:
    case 0x8b:
    case 0x8c:
    case 0x8d:
    case 0x8e:
    case 0x8f:
    case 0x90:
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
    case 0x95:
    case 0x96:
    case 0x97:
    case 0x98:
    case 0x99:
    case 0x9a:
    case 0x9b:
    case 0x9c:
    case 0x9d:
    case 0x9e:
    case 0x9f:
    case 0xa0:
    case 0xa1:
    case 0xa2:
    case 0xa3:
    case 0xa5:
    case 0xa6:
        zGameStateSwitchEvent(toEvent);
        break;
    case 0xa8:
    case 0xa9:
    case 0xaa:
    case 0xab:
    case 0xac:
    case 0xad:
        zSaveLoad_DispatchCB(toEvent, toParam);
        break;
    case 0xc2:
        zEntPlayer_StoreCheckPoint(toParam);
        break;
    case 0x101:
        zVarGiveMap();
        break;
    case 0x102:
        zVarUnlockR001();
        break;
    case 0x103:
        zVarUnlockS001();
        break;
    case 0x104:
        zVarUnlockE001();
        break;
    case 0x105:
        zVarUnlockF001();
        break;
    case 0x11e:
        zHudShow();
        break;
    case 0x11f:
        zHudHide();
        break;
    case 0x120:
    {
        iColor_tag black = { 0, 0, 0, 255 };
        iColor_tag clear = { 0, 0, 0, 0 };
        xScrFxFade(&clear, &black, toParam[0], NULL, 1, 1);
        break;
    }
    case 0x126:
        switch ((U32)toParam[0])
        {
        case 0:
            zMenuFMVPlay(zFMVFileGetName(eFMVFile_WBExtra0), 1, 0.1f);
            break;
        case 1:
            zMenuFMVPlay(zFMVFileGetName(eFMVFile_WBExtra1), 1, 0.1f);
            break;
        case 2:
            zMenuFMVPlay(zFMVFileGetName(eFMVFile_Cin11), 0, 0.0f);
            break;
        case 3:
            zMenuFMVPlay(zFMVFileGetName(eFMVFile_EndCredits), 0, 0.0f);
            zMusicKill();
            break;
        case 4:
            zFMVPlay(zFMVFileGetName(eFMVFile_EndCredits), 1, 0.1f);
            break;
        case 5:
            if (finishedGame())
            {
                zMenuFMVPlay(zFMVFileGetName(eFMVFile_EndCredits), 1, 0.1f);
            }
            else
            {
                zMenuFMVPlay(zFMVFileGetName(eFMVFile_Credits), 0x10001, 0.1f);
            }
            break;
        case 6:
            zFMVPlay(zFMVFileGetName((eFMVFile)(eFMVFile_Cin1 + toParam[1] - 1.0f)), 1, 0.1f);
            break;
        }
        break;
    case 0x127:
        zVarDefeatMasterMind();
        break;
    case 0x130:
        zSceneSetMenuStickDisabled(1);
        zSceneSnapshotLocalFloats();
        break;
    case 0x131:
        zSceneSetMenuStickDisabled(0);
        break;
    }

    return 1;
}

// Scale a 0-10 volume setting to 0-1.
static F32 ZDSP_volScale(S32 iv)
{
    F32 vol = 0.1f * iv;
    return CLAMP(vol, 0.0f, 1.0f);
}

#endif
