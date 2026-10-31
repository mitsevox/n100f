#include "xsavegame.h"
#include "xDraw.h"
#include "xDebug.h"
#include "iDebug.h"
#include "iEnv.h"
#include "iSnd.h"
#include "iTime.h"
#include "xEvent.h"
#include "xPad.h"
#include "xParMgr.h"
#include "xSnd.h"
#include "xString.h"
#include "xutil.h"
#include "zEnt.h"
#include "zGameState.h"
#include "zGlobals.h"
#include "zSaveLoad.h"
#include "zScene.h"

#include <dolphin/os.h>
#include <stdio.h>
#include <string.h>

S32 xSGT_LoadLoadCB(void* vp, st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, U32 ui,
                    S32 i);
S32 xSGT_LoadPrefsCB(void* vp, st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt,
                     U32 ui, S32 i);
S32 xSGT_SaveInfoCB(void* vp, st_XSAVEGAME_DATA* xsgdata, S32* need, S32* most);
S32 xSGT_SaveProcCB(void* vp, st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt);
S32 xSGT_SaveInfoPrefsCB(void* vp, st_XSAVEGAME_DATA* xsgdata, S32* need, S32* most);
S32 xSGT_SaveProcPrefsCB(void* vp, st_XSAVEGAME_DATA* xsgdata,
                         st_XSAVEGAME_WRITECONTEXT* wctxt);
void xPadRumbleEnable(S32 idx, S32 enable);
void zEntPlayer_SNDSavePickup(_xEnt* ent);
void zEntPlayer_SNDVan(_xEnt* ent);
void xCameraBegin(_tagxCamFrame* cam, S32 clear);
void xCameraEnd(_tagxCamFrame* cam, F32 seconds, S32 update_scrn_fx);
void xSndUpdate(F32 dt);
S32 xSerial_svgame_register(st_XSAVEGAME_DATA* sgctxt, en_SAVEGAME_MODE mode);

// The memory-card layer: its message text and button labels, the card and slot the messages
// name, and its card-repair and reset handling.
extern "C" S32 fn_80041548(S32 mode, char* dst);
extern "C" S32 fn_80041ADC(S32 mode, char* dst);
extern "C" S32 fn_80041B10(S32 mode, char* dst);
extern "C" S32 fn_80041BC8(S32 mode, char* dst);
extern "C" void fn_80041C4C(S32 mode, S32 value);
extern "C" void fn_80041C78(S32 mode, S32 available, S32 needed, S32 files, S32 filesNeeded,
                            S32 needFile);
extern "C" void fn_80041CC4(S32 mode, S32 message);
extern "C" void fn_80041CE8(S32 mode, S32 card, S32 slot);
extern "C" void fn_80041D58(S32 mode);
extern "C" S32 fn_80041310(S32 card, S32 mode);
extern "C" S32 fn_800411A8(S32 card, S32 repair, S32* files, S32* repaired);
extern "C" S32 fn_80040FCC(S32 card, S32 game);
extern "C" void fn_80016600();
extern "C" void fn_80016698(S32 reset);
// The memory card's file counts, from the save-game layer.
extern "C" void fn_80038948(st_XSAVEGAME_DATA* xsgdata, S32 tidx, S32* sectorSize, S32* filesFree);
// The per-frame helpers the front-end loops share.
extern "C" void fn_8004436C();

extern "C" U32 lbl_8025E9F8;

zSaveLoadGame zSaveLoadGameTable[3];

static U32 saveSuccess;
static F32 time_last;
static F32 time_current;
static iTime t0;
static iTime t1;
static S32 promptSel;
static S32 badCard;
static S32 sSkipAllowed;

static F32 time_elapsed = 0.01f;
static S32 currentCard = -1;
static S32 currentGame = -1;
static F32 dontPoll = 1.0f;

static char currSceneStr[32] = "TEMP";
static char sceneRead[32] = "0000";
zSaveLoadUI zSaveLoadUITable[61] = {
    { 0, 0, "ld gameslot group" },
    { 1, 0, "ld memcards group" },
    { 2, 0, "ld format prompt group" },
    { 3, 0, "ld mc missing group" },
    { 4, 0, "mnu3 ld mc1" },
    { 5, 0, "mnu3 ld mc2" },
    { 6, 0, "ld gameslot 0" },
    { 7, 0, "ld gameslot 1" },
    { 8, 0, "ld gameslot 2" },
    { 9, 0, "ld gameslot 3" },
    { 10, 0, "ld gameslot 4" },
    { 11, 0, "ld gameslot 5" },
    { 12, 0, "ld gameslot 6" },
    { 13, 0, "ld gameslot 7" },
    { 14, 0, "ld format prompt" },
    { 15, 0, "ld format yes" },
    { 16, 0, "ld format no" },
    { 17, 0, "ld mc missing" },
    { 18, 0, "ld nogames" },
    { 19, 0, "ld nogame" },
    { 20, 0, "mnu3 start group" },
    { 21, 0, "sv gameslot group" },
    { 22, 0, "sv memcards group" },
    { 23, 0, "sv mc1" },
    { 24, 0, "sv mc2" },
    { 25, 0, "sv gameslot 0" },
    { 26, 0, "sv gameslot 1" },
    { 27, 0, "sv gameslot 2" },
    { 28, 0, "sv gameslot 3" },
    { 29, 0, "sv gameslot 4" },
    { 30, 0, "sv gameslot 5" },
    { 31, 0, "sv gameslot 6" },
    { 32, 0, "sv gameslot 7" },
    { 33, 0, "sv format prompt" },
    { 34, 0, "sv overwrite" },
    { 35, 0, "sv mc missing" },
    { 36, 0, "sv nospace" },
    { 37, 0, "sv nospacegame" },
    { 38, 0, "ld mc dontremove" },
    { 39, 0, "sv mc dontremove" },
    { 40, 0, "mnu4 mc dontremove" },
    { 41, 0, "ld badload" },
    { 42, 0, "sv badsave" },
    { 43, 0, "mnu3 badformat" },
    { 44, 0, "mnu4 badformat" },
    { 45, 0, "mnu4 badformat cantrecover" },
    { 46, 0, "mnu3 format confirm" },
    { 47, 0, "mnu4 format confirm" },
    { 48, 0, "sv format group" },
    { 49, 0, "ld format prompt group" },
    { 50, 0, "ld badload" },
    { 51, 0, "mnu3 badformat" },
    { 52, 0, "ld format prompt group" },
    { 53, 0, "ld format prompt group" },
    { 54, 0, "ld_nospace" },
    { 55, 0, "sv badsave" },
    { 56, 0, "mnu4 badformat" },
    { 57, 0, "sv mc missing" },
    { 58, 0, "sv format group" },
    { 59, 0, "sv nospace" },
    { 0, 0, NULL },
};

S32 zSaveLoad_MessagePrompt(S32 mode, S32 kind, S32 (*pollfunc)(S32), S32 pollarg);
U8 zSaveLoad_poll(S32 i);
S32 zSaveLoad_CardCheckSpaceSingle_doCheck(st_XSAVEGAME_DATA* xsgdata, S32 cardNumber,
                                           S32 checkSlots, S32* slotsFull);
S32 zSaveLoad_CardCheckFormattedSingle(S32 cardNumber, S32* badEncode);
S32 format(S32 num, S32 mode);

static inline void zSaveLoad_UIEvent(S32 i, U32 toEvent)
{
    zEntEvent(zSceneFindObject(zSaveLoadUITable[i].nameID), toEvent);
}

// The UI entry for the load menu (mode 1) or the save menu.
static inline S32 zSaveLoad_ModeUI(S32 mode, S32 load, S32 save)
{
    S32 i = save;
    if (mode == 1)
    {
        i = load;
    }
    return i;
}

static inline void zSaveLoadUITableInit(zSaveLoadUI* saveTable)
{
    for (S32 i = 0; i < 60; i++)
    {
        saveTable[i].nameID = xStrHash(saveTable[i].name);
    }
}

static inline void zSaveLoadGameTableInit(zSaveLoadGame* saveTable)
{
    for (S32 i = 0; i < 3; i++)
    {
        saveTable[i].label[0] = '\0';
        saveTable[i].date[0] = '\0';
        saveTable[i].progress = 0;
    }
}

static inline void zSaveLoadInit()
{
    zSaveLoadUITableInit(zSaveLoadUITable);
    zSaveLoadGameTableInit(zSaveLoadGameTable);
}

// 1 while the given card is still in its slot, 0 once it is gone.
static inline S32 zSaveLoad_CardCheckSingle(S32 cardNumber)
{
    st_XSAVEGAME_DATA* ldinst = xSGInit(XSG_MODE_LOAD);
    S32 tgtmax;

    switch (xSGTgtCount(ldinst, &tgtmax))
    {
    case 2:
        xSGDone(ldinst);
        return 1;
    case 1:
    {
        S32 idx = xSGTgtPhysSlotIdx(ldinst, 0);
        xSGDone(ldinst);
        if (idx == cardNumber)
        {
            return 1;
        }
        return 0;
    }
    case 0:
        xSGDone(ldinst);
        return 0;
    }
    return -1;
}

// Polled while a prompt waits: a card pulled out of its slot answers the prompt with i.
static inline void zSaveLoad_CardPoll(S32 i)
{
    if (dontPoll < 0.0f)
    {
        dontPoll = 1.0f;
    }
    else if (zSaveLoad_CardCheckSingle(currentCard) == 0)
    {
        promptSel = i;
        currentGame = 0;
    }
}

// The target index of the given card: the card itself with both slots filled, 0 with one.
static inline S32 CardtoTgt(S32 card)
{
    st_XSAVEGAME_DATA* ldinst = xSGInit(XSG_MODE_LOAD);
    S32 tgtmax;

    switch (xSGTgtCount(ldinst, &tgtmax))
    {
    case 2:
        xSGDone(ldinst);
        return card;
    case 1:
        xSGDone(ldinst);
        return 0;
    case 0:
        xSGDone(ldinst);
        return -1;
    }
    return -1;
}

static inline void BuildIt(char* build_txt, S32 i)
{
    char date1[32] = {};
    char biggerbuf[256] = {};
    char date2[32];

    strncpy(date1, zSaveLoadGameTable[i].date, 5);
    date1[2] = '/';
    sprintf(biggerbuf, "%s/%c%c", date1, zSaveLoadGameTable[i].date[8],
            zSaveLoadGameTable[i].date[9]);
    strncpy(date2, biggerbuf, sizeof(date2));
    date2[31] = '\0';
    sprintf(biggerbuf, "%s   %d%%\n%s", date2, zSaveLoadGameTable[i].progress,
            zSaveLoadGameTable[i].label);
    strncpy(build_txt, biggerbuf, 64);
    build_txt[63] = '\0';
}

static inline S32 zSaveLoad_CardPrompt(S32 mode)
{
    zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 0, 21), eEventEnable);
    zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 0, 21), eEventUIFocusOff_Unselect);
    zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 1, 22), eEventUIFocusOff_Unselect);
    zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 17, 35), eEventUIFocusOn_Select);
    promptSel = -1;
    while (promptSel == -1)
    {
        zSaveLoad_Tick();
    }
    zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 17, 35), eEventUIFocusOff_Unselect);
    if (mode == 1)
    {
        zSaveLoad_UIEvent(0, eEventDisable);
    }
    return promptSel;
}

static inline S32 zSaveLoad_CardCheckSpaceSingle(S32 cardNumber, S32 checkSlots, S32* slotsFull)
{
    st_XSAVEGAME_DATA* ldinst = xSGInit(XSG_MODE_SAVE);
    S32 tgtmax;
    S32 rc = 0;

    switch (xSGTgtCount(ldinst, &tgtmax))
    {
    case 2:
        rc = zSaveLoad_CardCheckSpaceSingle_doCheck(ldinst, cardNumber, checkSlots, slotsFull);
        if (rc != 0)
        {
            rc = 1;
        }
        break;
    case 1:
    {
        S32 idx = xSGTgtPhysSlotIdx(ldinst, 0);
        if (idx != cardNumber)
        {
            rc = 5;
        }
        if (!(idx ^ cardNumber))
        {
            rc = zSaveLoad_CardCheckSpaceSingle_doCheck(ldinst, 0, checkSlots, slotsFull);
        }
        break;
    }
    case 0:
        rc = 5;
        break;
    }

    xSGDone(ldinst);
    return rc;
}

static inline S32 zSaveLoad_CardCheckGamesSingle_doCheck(st_XSAVEGAME_DATA* xsgdata,
                                                         S32 cardNumber)
{
    S32 badEncode = 0;
    S32 rc = 0;

    xSGTgtSelect(xsgdata, cardNumber);
    if (xSGTgtIsFormat(xsgdata, cardNumber, &badEncode) == 0)
    {
        rc = 6;
    }
    else if (xSGTgtHasGameDir(xsgdata, cardNumber) == 0)
    {
        rc = 0;
    }
    else
    {
        for (S32 i = 0; i < 3; i++)
        {
            if (xSGGameIsEmpty(xsgdata, i) == 0)
            {
                rc = 1;
                break;
            }
        }
    }
    return rc;
}

static inline S32 zSaveLoad_CardCheckGamesSingle(S32 cardNumber)
{
    st_XSAVEGAME_DATA* ldinst = xSGInit(XSG_MODE_LOAD);
    S32 tgtmax;
    S32 rc = 0;

    switch (xSGTgtCount(ldinst, &tgtmax))
    {
    case 2:
        rc = zSaveLoad_CardCheckGamesSingle_doCheck(ldinst, cardNumber);
        if (rc != 0)
        {
            rc = 1;
        }
        break;
    case 1:
    {
        S32 idx = xSGTgtPhysSlotIdx(ldinst, 0);
        if (idx != cardNumber)
        {
            rc = 5;
        }
        if (!(idx ^ cardNumber))
        {
            rc = zSaveLoad_CardCheckGamesSingle_doCheck(ldinst, 0);
        }
        break;
    }
    case 0:
        rc = 5;
        break;
    }

    xSGDone(ldinst);
    return rc;
}

static inline S32 zSaveLoad_CardCheckSlotEmpty_hasGame_doCheck(st_XSAVEGAME_DATA* xsgdata,
                                                               S32 cardNumber, S32 gameNumber)
{
    S32 badEncode = 0;
    S32 rc;

    xSGTgtSelect(xsgdata, cardNumber);
    if (xSGTgtIsFormat(xsgdata, cardNumber, &badEncode) == 0)
    {
        rc = -1;
    }
    else
    {
        rc = xSGGameIsEmpty(xsgdata, gameNumber);
        if (rc != 0)
        {
            rc = 0;
        }
        else if (rc == 0)
        {
            rc = 1;
        }
    }
    return rc;
}

static inline S32 zSaveLoad_CardCheckSlotEmpty_hasGame(S32 cardNumber, S32 gameNumber)
{
    st_XSAVEGAME_DATA* ldinst = xSGInit(XSG_MODE_LOAD);
    S32 tgtmax;
    S32 rc = 0;

    switch (xSGTgtCount(ldinst, &tgtmax))
    {
    case 2:
        rc = zSaveLoad_CardCheckSlotEmpty_hasGame_doCheck(ldinst, cardNumber, gameNumber);
        if (rc != 0)
        {
            rc = 1;
        }
        break;
    case 1:
    {
        S32 idx = xSGTgtPhysSlotIdx(ldinst, 0);
        if (idx != cardNumber)
        {
            rc = -1;
        }
        if (!(idx ^ cardNumber))
        {
            rc = zSaveLoad_CardCheckSlotEmpty_hasGame_doCheck(ldinst, 0, gameNumber);
        }
        break;
    }
    case 0:
        rc = -1;
        break;
    }

    xSGDone(ldinst);
    return rc;
}

static inline S32 zSaveLoad_CardCheckSlotOverwrite_Free(S32 cardNumber, S32 gameNumber)
{
    S32 i = zSaveLoad_CardCheckSlotEmpty_hasGame(cardNumber, gameNumber);
    switch (i)
    {
    case -1:
        return i;
    case 0:
        return 1;
    case 1:
        return 0;
    default:
        return 0;
    }
}

// Shows a memory-card message of the given kind and waits for it to be answered, calling
// pollfunc each frame. Returns the answer.
S32 zSaveLoad_MessagePrompt(S32 mode, S32 kind, S32 (*pollfunc)(S32), S32 pollarg)
{
    S32 ui = 0;
    switch (kind)
    {
    case 0:
        if (mode == 1)
        {
            ui = 50;
        }
        else
        {
            ui = 55;
        }
        break;
    case 1:
        if (mode == 1)
        {
            ui = 51;
        }
        else
        {
            ui = 56;
        }
        break;
    case 2:
        if (mode == 1)
        {
            ui = 52;
        }
        else
        {
            ui = 57;
        }
        break;
    case 3:
        if (mode == 1)
        {
            ui = 53;
        }
        else
        {
            ui = 58;
        }
        break;
    case 4:
        if (mode == 1)
        {
            ui = 54;
        }
        else
        {
            ui = 59;
        }
        break;
    }
    if (ui == 0)
    {
        return 0;
    }

    if (mode == 1)
    {
        zSaveLoad_UIEvent(0, eEventEnable);
        zSaveLoad_UIEvent(0, eEventUIFocusOff_Unselect);
        zSaveLoad_UIEvent(1, eEventUIFocusOff_Unselect);
    }
    else
    {
        zSaveLoad_UIEvent(21, eEventUIFocusOff_Unselect);
        zSaveLoad_UIEvent(22, eEventUIFocusOff_Unselect);
    }

    if (kind == 4)
    {
        sSkipAllowed = 1;
    }
    if (kind == 3)
    {
        zSaveLoad_UIEvent(ui, eEventUIFocusOn);
    }
    else
    {
        zSaveLoad_UIEvent(ui, eEventUIFocusOn_Select);
    }

    S32 done = 0;
    promptSel = -1;
    while (promptSel == -1)
    {
        zSaveLoad_Tick();
        if (pollfunc != NULL)
        {
            done = pollfunc(pollarg);
        }
        if (done)
        {
            break;
        }
    }
    sSkipAllowed = 0;
    zSaveLoad_UIEvent(ui, eEventUIFocusOff_Unselect);
    S32 rc = promptSel;
    promptSel = -1;
    return rc;
}

// Polled while a message waits: a card pulled out of its slot answers the message. True when
// the answer changed.
U8 zSaveLoad_poll(S32 i)
{
    S32 old = promptSel;
    zSaveLoad_CardPoll(5);
    return promptSel != old;
}

// The memory-card layer's alternate button label.
extern "C" S32 fn_800C9698(char* dst)
{
    return fn_80041ADC(1, dst);
}

// The memory-card layer's action button label.
extern "C" S32 fn_800C96C0(char* dst)
{
    return fn_80041B10(1, dst);
}

// The memory-card layer's cancel button label.
extern "C" S32 fn_800C96E8(char* dst)
{
    return fn_80041BC8(1, dst);
}

// The memory-card layer's current message.
extern "C" S32 fn_800C9710(char* dst)
{
    return fn_80041548(1, dst);
}

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

void zSaveLoad_DispatchCB(U32 dispatchEvent, F32* toParam)
{
    switch (dispatchEvent)
    {
    case eEventDispatcher_SLBack:
        promptSel = 4;
        break;
    case eEventDispatcher_SLCancel:
        if (sSkipAllowed)
        {
            promptSel = 2;
        }
        break;
    case eEventDispatcher_SLRetry:
        promptSel = 3;
        break;
    case eEventDispatcher_SLSelectCard:
        currentCard = (S32)*toParam;
        break;
    case eEventDispatcher_SLSelectSlot:
        currentGame = (S32)*toParam;
        break;
    case eEventDispatcher_SLOkay:
        promptSel = 3;
        break;
    }
}

// Runs the save menu until the game is saved or the player backs out.
U32 zSaveLoad_SaveLoop()
{
    zSaveLoadInit();

    S32 state = 0;
    saveSuccess = 0;
    zEntPlayer_SNDSavePickup(NULL);
    while (state != 6)
    {
        switch (state)
        {
        case 0:
            state = 1;
            break;
        case 1:
        {
            S32 rc = zSaveLoad_CardPick(0);
            if (rc == 1)
            {
                state = 2;
            }
            else if (rc == 4)
            {
                state = 11;
            }
            else
            {
                state = 0;
            }
            break;
        }
        case 2:
            switch (zSaveLoad_GameSelect(0))
            {
            case 0:
                break;
            case 2:
            case 4:
            case 5:
            case 6:
                state = 1;
                break;
            case 1:
                state = 10;
                break;
            case -1:
                fn_80041CE8(1, currentCard, currentCard);
                fn_80041CC4(1, 3);
                zSaveLoad_MessagePrompt(0, 1, NULL, 0);
                state = 1;
                break;
            }
            break;
        case 3:
            break;
        case 11:
            zGameModeSwitch((eGameMode)11);
            zGameStateSwitch(1);
            zEntPlayer_SNDVan(NULL);
            state = 6;
            break;
        case 10:
            fn_80041CE8(1, currentCard, currentCard);
            fn_80041CC4(1, 1);
            if (zSaveLoad_SaveGame() == 1)
            {
                zGameModeSwitch((eGameMode)11);
                zGameStateSwitch(1);
                state = 6;
                saveSuccess = 1;
                zEntPlayer_SNDVan(NULL);
            }
            else
            {
                state = 1;
            }
            break;
        }
    }
    return saveSuccess;
}

// Runs the load menu until a game is loaded or the player backs out, and returns the loaded
// scene's id.
U32 zSaveLoad_LoadLoop()
{
    zSaveLoadInit();

    S32 state = 0;
    while (state != 6)
    {
        switch (state)
        {
        case 0:
            zSaveLoad_UIEvent(20, eEventUIFocusOff_Unselect);
            state = 1;
            break;
        case 1:
            switch (zSaveLoad_CardPick(1))
            {
            case 2:
                break;
            case 1:
                state = 2;
                break;
            case 4:
                state = 8;
                break;
            case -1:
                fn_80041CE8(1, currentCard, currentCard);
                fn_80041CC4(1, 7);
                state = 0;
                break;
            case 0:
            default:
                state = 0;
                break;
            }
            break;
        case 2:
            switch (zSaveLoad_GameSelect(1))
            {
            case 0:
                break;
            case 2:
            case 4:
            case 5:
            case 6:
                state = 1;
                break;
            case 1:
                state = 9;
                break;
            case -1:
                fn_80041CE8(1, currentCard, currentCard);
                fn_80041CC4(1, 7);
                zSaveLoad_MessagePrompt(1, 1, NULL, 0);
                state = 1;
                break;
            }
            break;
        case 3:
            break;
        case 8:
            zGameModeSwitch(eGameMode_Title);
            zGameStateSwitch(0);
            state = 6;
            break;
        case 9:
            fn_80041CE8(1, currentCard, currentCard);
            fn_80041CC4(1, 5);
            if (zSaveLoad_LoadGame() == 1)
            {
                zGameModeSwitch((eGameMode)11);
                zGameStateSwitch(0);
                state = 6;
            }
            else
            {
                fn_80041CE8(1, currentCard, currentCard);
                fn_80041CC4(1, 7);
                zSaveLoad_MessagePrompt(1, 1, NULL, 0);
                state = 1;
            }
            break;
        }
    }

    return (sceneRead[0] << 24) | (sceneRead[1] << 16) | (sceneRead[2] << 8) | sceneRead[3];
}

// Loads the game from the chosen slot. Returns 1 when it was loaded.
S32 zSaveLoad_LoadGame()
{
    S32 success = 0;
    S32 teststat = 1;
    en_XSGASYNC_STATUS asstat = XSG_ASTAT_NOOP;
    S32 use_tgt = CardtoTgt(currentCard);
    en_XSG_WHYFAIL whyFail = XSG_WHYERR_NONE;
    S32 rc;

    zSaveLoad_UIEvent(40, eEventUIFocusOn);
    zSaveLoad_Tick();

    st_XSAVEGAME_DATA* xsgdata = xSGInit(XSG_MODE_LOAD);
    xSGTgtSelect(xsgdata, use_tgt);
    xSGGameSet(xsgdata, currentGame);
    if (xsgdata == NULL)
    {
        teststat = 0;
    }

    xSGAddLoadClient(xsgdata, 'ROOM', NULL, xSGT_LoadLoadCB);
    xSGAddLoadClient(xsgdata, 'PREF', NULL, xSGT_LoadPrefsCB);
    xSerial_svgame_register(xsgdata, XSG_MODE_LOAD);
    if (!xSGSetup(xsgdata))
    {
        teststat = 0;
    }

    if (teststat)
    {
        rc = xSGProcess(xsgdata);
        if (rc)
        {
            asstat = xSGAsyncStatus(xsgdata, 1, &whyFail, NULL);
        }
        if (!rc)
        {
            teststat = 0;
        }
        else
        {
            switch (asstat)
            {
            case XSG_ASTAT_SUCCESS:
                success = 1;
                break;
            case XSG_ASTAT_FAILED:
                success = 0;
                break;
            }
        }
    }

    if (teststat && !xSGWrapup(xsgdata))
    {
        teststat = 0;
    }
    zSaveLoad_UIEvent(40, eEventUIFocusOff);
    if (!xSGDone(xsgdata))
    {
        teststat = 0;
    }

    if (success && teststat)
    {
        return 1;
    }
    return -1;
}

// Saves the game to the chosen slot. Returns 1 when it was saved.
S32 zSaveLoad_SaveGame()
{
    S32 success = 0;
    S32 teststat = 1;
    en_XSGASYNC_STATUS asstat = XSG_ASTAT_NOOP;
    S32 use_tgt = CardtoTgt(currentCard);
    en_XSG_WHYFAIL whyFail = XSG_WHYERR_NONE;
    S32 rc;
    char label[64];

    zSaveLoad_UIEvent(40, eEventUIFocusOn);
    zSaveLoad_Tick();

    st_XSAVEGAME_DATA* xsgdata = xSGInit(XSG_MODE_SAVE);
    xSGTgtSelect(xsgdata, use_tgt);
    xSGGameSet(xsgdata, currentGame);
    if (xsgdata == NULL)
    {
        teststat = 0;
    }

    zSceneSave(globals.sceneCur, NULL);
    xSGAddSaveClient(xsgdata, 'ROOM', NULL, xSGT_SaveInfoCB, xSGT_SaveProcCB);
    xSGAddSaveClient(xsgdata, 'PREF', NULL, xSGT_SaveInfoPrefsCB, xSGT_SaveProcPrefsCB);
    xSerial_svgame_register(xsgdata, XSG_MODE_SAVE);

    S32 progress = zSceneCalcProgress();
    strncpy(label, zSceneGetAreaname(globals.sceneCur->sceneID), sizeof(label));
    label[63] = '\0';
    if (!xSGSetup(xsgdata, currentGame, label, progress, 0))
    {
        teststat = 0;
    }

    if (teststat)
    {
        rc = xSGProcess(xsgdata);
        if (rc)
        {
            asstat = xSGAsyncStatus(xsgdata, 1, &whyFail, NULL);
        }
        xSGGameIsEmpty(xsgdata, currentGame);
        xSGTgtHasGameDir(xsgdata, use_tgt);
        if (!rc)
        {
            teststat = 0;
        }
        else
        {
            switch (asstat)
            {
            case XSG_ASTAT_SUCCESS:
                success = 1;
                break;
            case XSG_ASTAT_FAILED:
                success = 0;
                break;
            }
        }
    }

    if (teststat && !xSGWrapup(xsgdata))
    {
        teststat = 0;
    }
    zSaveLoad_UIEvent(40, eEventUIFocusOff);
    if (!xSGDone(xsgdata))
    {
        teststat = 0;
    }

    if (success && teststat)
    {
        return 1;
    }

    fn_80041CE8(1, currentCard, currentCard);
    S32 err = whyFail == XSG_WHYERR_NOROOM ? fn_80040FCC(currentCard, currentGame) : -1;
    if (err > 0)
    {
        fn_80041CC4(1, 3);
        zSaveLoad_MessagePrompt(0, 1, NULL, 0);
    }
    else if (err < 0)
    {
        fn_80041CC4(1, 3);
        zSaveLoad_MessagePrompt(0, 1, NULL, 0);
    }
    else
    {
        fn_80041CC4(1, 4);
        if (zSaveLoad_MessagePrompt(0, 4, (S32(*)(S32))zSaveLoad_poll, 0) == 2)
        {
            fn_80041CC4(1, 23);
            if (zSaveLoad_MessagePrompt(0, 3, (S32(*)(S32))zSaveLoad_poll, 0) == 3)
            {
                fn_80016698(1);
            }
        }
    }
    return -1;
}

// Lists the card's save slots and waits for one to be chosen. Returns 1 when one is chosen and
// ready, 4 when the player backed out.
S32 zSaveLoad_GameSelect(S32 mode)
{
    S32 done = 0;

    badCard = 1;
    while (badCard)
    {
        S32 tgt = currentCard;
        currentGame = -1;
        promptSel = -1;
        tgt = CardtoTgt(tgt);

        if (mode == 1)
        {
            zSaveLoad_UIEvent(0, eEventEnable);
            zSaveLoad_UIEvent(0, eEventUIFocusOn);
        }

        for (S32 i = 0; i < 3; i++)
        {
            st_XSAVEGAME_DATA* ldinst = xSGInit(XSG_MODE_LOAD);
            xSGTgtSelect(ldinst, tgt);
            if (xSGGameIsEmpty(ldinst, i))
            {
                if (mode == 1)
                {
                    zSaveLoad_UIEvent(i + 6, eEventDisable);
                }
                strcpy(zSaveLoadGameTable[i].label, "Empty");
                strcpy(zSaveLoadGameTable[i].date, "Empty");
                zSaveLoadGameTable[i].progress = 0;
            }
            else
            {
                strcpy(zSaveLoadGameTable[i].label, xSGGameLabel(ldinst, i));
                strcpy(zSaveLoadGameTable[i].date, xSGGameModDate(ldinst, i));
                zSaveLoadGameTable[i].progress = xSGGameThumIndex(ldinst, i);
            }
            xSGDone(ldinst);
        }

        if (mode == 1)
        {
            zSaveLoad_UIEvent(0, eEventUISelect);
        }
        else
        {
            zSaveLoad_UIEvent(21, eEventUIFocusOn);
            zSaveLoad_UIEvent(25, eEventUISelect);
        }

        badCard = 0;
        while (done == 0 && promptSel == -1)
        {
            zSaveLoad_Tick();
            zSaveLoad_CardPoll(3);
            if (promptSel == 4)
            {
                done = promptSel;
            }
            if (currentGame != -1)
            {
                done = zSaveLoad_CardCheck(currentCard, mode);
                if (done != 1)
                {
                    currentGame = -1;
                    continue;
                }
                done = zSaveLoad_CardCheckFormatted(currentCard, mode);
                switch (done)
                {
                case 5:
                    done = 0;
                    promptSel = -1;
                    break;
                case 2:
                    currentGame = -1;
                    break;
                case 1:
                    done = zSaveLoad_CardCheckValid(currentCard, mode, 0);
                    switch (done)
                    {
                    case 2:
                        currentGame = -1;
                        break;
                    case 1:
                        if (badCard == 0)
                        {
                            done = zSaveLoad_CardCheckGameSlot(currentCard, currentGame, mode);
                            switch (done)
                            {
                            case 1:
                                break;
                            case 5:
                            case -1:
                                done = 0;
                                promptSel = -1;
                                break;
                            case 2:
                                currentGame = -1;
                                break;
                            }
                        }
                        else
                        {
                            done = 0;
                            currentGame = -1;
                        }
                        break;
                    default:
                        done = 0;
                        promptSel = -1;
                        break;
                    }
                    break;
                }
            }
        }
    }

    if (mode == 1)
    {
        zSaveLoad_UIEvent(0, eEventEnable);
    }
    zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 0, 21), eEventUIFocusOff_Unselect);

    if (done == 1)
    {
        return done;
    }
    if (promptSel != -1)
    {
        return promptSel;
    }
    return done;
}

void zSaveLoad_BuildName(char* name_txt, S32 idx)
{
    char desired[64];
    char current_name[64];

    BuildIt(desired, idx);

    S32 counter = 0;
    for (S32 i = 0; i < idx; i++)
    {
        BuildIt(current_name, i);
        if (strcmp(current_name, desired) == 0)
        {
            counter++;
        }
    }

    if (counter > 0)
    {
        sprintf(name_txt, "%s (%d)", desired, counter);
        name_txt[0x3f] = '\0';
    }
    else
    {
        strcpy(name_txt, desired);
    }
}

// Asks for a memory card. Returns 1 when one is chosen and ready, 4 when the player backed out.
S32 zSaveLoad_CardPick(S32 mode)
{
    S32 done = 0;

    currentCard = -1;
    promptSel = -1;

    zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 1, 22), eEventUIFocusOn);
    zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 4, 23), eEventUISelect);

    while (done == 0 && promptSel == -1)
    {
        if (currentCard != -1)
        {
            done = zSaveLoad_CardCheck(currentCard, mode);
            switch (done)
            {
            case 2:
                currentCard = -1;
                promptSel = -1;
                break;
            case -1:
                done = 0;
                promptSel = -1;
                break;
            default:
                done = zSaveLoad_CardCheckFormatted(currentCard, mode);
                switch (done)
                {
                case 5:
                case 6:
                    promptSel = -1;
                    done = 0;
                    break;
                case 2:
                case 4:
                    currentCard = -1;
                    promptSel = -1;
                    break;
                case -1:
                    promptSel = -1;
                    done = 0;
                    break;
                default:
                    done = zSaveLoad_CardCheckValid(currentCard, mode, 1);
                    switch (done)
                    {
                    case 5:
                    case 6:
                        promptSel = -1;
                        done = 0;
                        break;
                    case 2:
                    case 4:
                        promptSel = -1;
                        if (done != 1)
                        {
                            currentGame = -1;
                        }
                        zSaveLoad_Tick();
                        break;
                    case -1:
                        promptSel = -1;
                        done = 0;
                        break;
                    default:
                        zSaveLoad_Tick();
                        break;
                    }
                    break;
                }
                break;
            }
        }
        else
        {
            zSaveLoad_Tick();
        }
    }

    zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 1, 22), eEventUIFocusOff_Unselect);
    if (done == 1)
    {
        return done;
    }
    if (promptSel != -1)
    {
        return promptSel;
    }
    return done;
}

// Asks before an occupied slot is saved over. Returns 1 to save, 2 when the player backed out,
// 5 when the card was pulled.
S32 zSaveLoad_CardCheckSlotOverwrite(S32 cardNumber, S32 gameNumber)
{
    while (zSaveLoad_CardCheckSlotOverwrite_Free(cardNumber, gameNumber) != 1)
    {
        if (zSaveLoad_CardCheckSlotOverwrite_Free(cardNumber, gameNumber) == -1)
        {
            return -1;
        }

        zSaveLoad_UIEvent(21, eEventUIFocusOff_Unselect);
        zSaveLoad_UIEvent(34, eEventUIFocusOn_Select);
        promptSel = -1;
        while (promptSel == -1)
        {
            zSaveLoad_Tick();
            zSaveLoad_CardPoll(5);
        }
        zSaveLoad_UIEvent(34, eEventUIFocusOff_Unselect);
        switch (promptSel)
        {
        case 5:
            return promptSel;
        case 2:
        case 4:
            return 2;
        }
        break;
    }
    return 1;
}

// Checks that the chosen slot holds a game. Returns 1 when it does, 2 when the player backed out.
S32 zSaveLoad_CardCheckSlotEmpty(S32 cardNumber, S32 gameNumber)
{
    while (zSaveLoad_CardCheckSlotEmpty_hasGame(cardNumber, gameNumber) != 1)
    {
        if (zSaveLoad_CardCheckSlotEmpty_hasGame(cardNumber, gameNumber) == -1)
        {
            return -1;
        }

        zSaveLoad_UIEvent(0, eEventUIFocusOff_Unselect);
        zSaveLoad_UIEvent(19, eEventUIFocusOn_Select);
        promptSel = -1;
        while (promptSel == -1)
        {
            zSaveLoad_Tick();
        }
        zSaveLoad_UIEvent(19, eEventUIFocusOff_Unselect);
        if (promptSel == 2 || promptSel == 4)
        {
            return 2;
        }
    }
    return 1;
}

S32 zSaveLoad_CardCheckGameSlot(S32 cardNumber, S32 gameNumber, S32 mode)
{
    if (mode == 1)
    {
        return zSaveLoad_CardCheckSlotEmpty(cardNumber, gameNumber);
    }
    return zSaveLoad_CardCheckSlotOverwrite(cardNumber, gameNumber);
}

// Checks that the card holds a saved game. Returns 1 when it does, 2 when the player backed out.
S32 zSaveLoad_CardCheckGames(S32 cardNumber, S32 mode)
{
    while (zSaveLoad_CardCheckGamesSingle(cardNumber) != 1)
    {
        badCard = 1;
        S32 result = zSaveLoad_CardCheckGamesSingle(cardNumber);
        if (result == -1 || result == 6 || result == 5)
        {
            return result;
        }

        zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 0, 21), eEventUIFocusOff_Unselect);
        zSaveLoad_UIEvent(1, eEventUIFocusOff_Unselect);
        zSaveLoad_UIEvent(18, eEventUIFocusOn_Select);
        promptSel = -1;
        while (promptSel == -1)
        {
            zSaveLoad_Tick();
            zSaveLoad_CardPoll(3);
        }
        zSaveLoad_UIEvent(18, eEventUIFocusOff_Unselect);
        if (promptSel == 2 || promptSel == 4)
        {
            return 2;
        }
    }
    return 1;
}

// Checks for room on the card, letting the player continue without saving. Returns 1 when
// there is room, 2 when the player backed out.
S32 zSaveLoad_CardCheckSpace(S32 cardNumber, S32 mode, S32 checkSlots)
{
    S32 slotsFull = 0;
    S32 result = zSaveLoad_CardCheckSpaceSingle(cardNumber, checkSlots, &slotsFull);
    while (result != 1)
    {
        badCard = 1;
        if (result == -1 || result == 6 || result == 5)
        {
            return result;
        }

        sSkipAllowed = 1;
        zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 0, 21), eEventUIFocusOff_Unselect);
        zSaveLoad_UIEvent(22, eEventUIFocusOff_Unselect);
        zSaveLoad_UIEvent(36, eEventUIFocusOn_Select);
        promptSel = -1;
        while (promptSel == -1)
        {
            zSaveLoad_Tick();
            zSaveLoad_CardPoll(3);
        }
        zSaveLoad_UIEvent(36, eEventUIFocusOff_Unselect);

        if (slotsFull && promptSel == 4)
        {
            return 1;
        }
        sSkipAllowed = 0;
        if (promptSel == 2)
        {
            fn_80041CE8(1, cardNumber, cardNumber);
            fn_80041CC4(1, 23);
            if (zSaveLoad_MessagePrompt(mode, 3, (S32(*)(S32))zSaveLoad_poll, 0) == 3)
            {
                fn_80016698(1);
            }
            return 2;
        }
        if (promptSel == 2 || promptSel == 4)
        {
            return 2;
        }

        result = zSaveLoad_CardCheckSpaceSingle(cardNumber, checkSlots, &slotsFull);
    }
    return 1;
}

S32 zSaveLoad_CardCheckValid(S32 cardNumber, S32 mode, S32 checkSlots)
{
    if (mode == 1)
    {
        return zSaveLoad_CardCheckGames(cardNumber, mode);
    }
    return zSaveLoad_CardCheckSpace(cardNumber, mode, checkSlots);
}

// Checks the card, offering to repair or format it. Returns 1 when it can be used, 2 when the
// player backed out, -1 when it was pulled.
S32 zSaveLoad_CardCheckFormatted(S32 cardNumber, S32 mode)
{
    S32 badEncode = 0;

    while (zSaveLoad_CardCheckFormattedSingle(cardNumber, &badEncode) != 1)
    {
        badCard = 1;
        if (zSaveLoad_CardCheckFormattedSingle(cardNumber, &badEncode) == -1)
        {
            return -1;
        }

        S32 message;
        if (badEncode && mode == 0)
        {
            message = 17;
        }
        else if (mode == 0)
        {
            message = 16;
        }
        else if (badEncode)
        {
            message = 15;
        }
        else
        {
            message = 14;
        }
        fn_80041CE8(1, cardNumber, cardNumber);
        fn_80041CC4(1, message);

        zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 0, 21), eEventUIFocusOff_Unselect);
        zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 1, 22), eEventUIFocusOff_Unselect);
        zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 49, 48), eEventUIFocusOn);
        promptSel = -1;
        while (promptSel == -1)
        {
            zSaveLoad_Tick();
            zSaveLoad_CardPoll(5);
        }
        zSaveLoad_UIEvent(zSaveLoad_ModeUI(mode, 49, 48), eEventUIFocusOff_Unselect);
        if (promptSel == 5 || promptSel == 2 || promptSel == 4)
        {
            return 2;
        }

        fn_80041CE8(1, cardNumber, cardNumber);
        fn_80041CC4(1, 18);
        S32 rc = zSaveLoad_MessagePrompt(mode, 3, (S32(*)(S32))zSaveLoad_poll, 0);
        if (rc == 5 || rc == 2 || rc == 4)
        {
            return 2;
        }

        fn_80041CE8(1, cardNumber, cardNumber);
        fn_80041CC4(1, 19);
        zSaveLoad_UIEvent(40, eEventUIFocusOn);
        zSaveLoad_Tick();
        rc = format(cardNumber, mode);
        zSaveLoad_UIEvent(40, eEventUIFocusOff);
        if (rc != 1)
        {
            return rc;
        }

        fn_80041CE8(1, cardNumber, cardNumber);
        fn_80041CC4(1, 20);
        zSaveLoad_MessagePrompt(mode, 0, NULL, 0);
        return 1;
    }

    fn_80041D58(1);
    fn_80016600();

    S32 files = -1;
    S32 repaired = 0;
    S32 rc = fn_800411A8(cardNumber, 0, &files, NULL);
    if (rc <= 0)
    {
        if (rc < 0)
        {
            return 2;
        }
        fn_80041CE8(1, cardNumber, cardNumber);
        fn_80041CC4(1, 9);
        fn_80041C4C(1, files);
        rc = zSaveLoad_MessagePrompt(mode, 3, (S32(*)(S32))zSaveLoad_poll, 0);
        if (rc == 5 || rc == 2 || rc == 4)
        {
            return 2;
        }
        fn_80041CE8(1, cardNumber, cardNumber);
        fn_80041CC4(1, 10);
        rc = zSaveLoad_MessagePrompt(mode, 3, (S32(*)(S32))zSaveLoad_poll, 0);
        if (rc == 5 || rc == 2 || rc == 4)
        {
            return 2;
        }
        fn_80041CE8(1, cardNumber, cardNumber);
        fn_80041CC4(1, 13);
        zSaveLoad_UIEvent(40, eEventUIFocusOn);
        zSaveLoad_Tick();
        S32 result = fn_800411A8(cardNumber, 1, NULL, &repaired);
        zSaveLoad_UIEvent(40, eEventUIFocusOff);
        if (result <= 0)
        {
            if (result == 0 && repaired)
            {
                fn_80041CE8(1, cardNumber, cardNumber);
                fn_80041CC4(1, 11);
                zSaveLoad_MessagePrompt(mode, 0, NULL, 0);
                return 2;
            }
            fn_80041CE8(1, cardNumber, cardNumber);
            fn_80041CC4(1, 12);
            zSaveLoad_MessagePrompt(mode, 0, NULL, 0);
            return 2;
        }
        fn_80041CE8(1, cardNumber, cardNumber);
        fn_80041CC4(1, 11);
        zSaveLoad_MessagePrompt(mode, 0, NULL, 0);
    }
    fn_80041D58(1);
    fn_80016600();
    return 1;
}

// Waits for the card to be put back, then for the game to accept it. Returns 1 when it is
// there, 2 when the player backed out.
S32 zSaveLoad_CardCheck(S32 cardNumber, S32 mode)
{
    S32 prompted;

    do
    {
        if (fn_80041310(cardNumber, 1))
        {
            prompted = 0;
        }
        else
        {
            fn_80041CE8(1, cardNumber, cardNumber);
            fn_80041CC4(1, 27);
            S32 rc = zSaveLoad_CardPrompt(mode);
            if (rc == 2 || rc == 4)
            {
                return 2;
            }
            prompted = 1;
        }

        if (prompted == 0)
        {
            if (zSaveLoad_CardCheckSingle(cardNumber) == 0)
            {
                badCard = 1;
                fn_80041CE8(1, cardNumber, cardNumber);
                fn_80041CC4(1, 24);
                S32 rc = zSaveLoad_CardPrompt(mode);
                if (rc == 2 || rc == 4)
                {
                    return 2;
                }
                prompted = 1;
            }

            if (prompted == 0)
            {
                if (fn_80041310(cardNumber, 0))
                {
                    prompted = 0;
                }
                else
                {
                    S32 rc = zSaveLoad_CardPrompt(mode);
                    if (rc == 2 || rc == 4)
                    {
                        return 2;
                    }
                    prompted = 1;
                }
            }
        }
    } while (prompted);

    return 1;
}

// Whether the card has room for a new save; reports the space needed in the memory-card message
// when it has not.
S32 zSaveLoad_CardCheckSpaceSingle_doCheck(st_XSAVEGAME_DATA* xsgdata, S32 cardNumber,
                                           S32 checkSlots, S32* slotsFull)
{
    S32 badEncode = 0;
    S32 availOnDisk = 0;
    S32 filesNeeded = 0;
    S32 bytesNeeded = 0;
    S32 needFile = 0;
    S32 files = 0;
    S32 rc;

    if (slotsFull != NULL)
    {
        *slotsFull = 0;
    }

    xSGTgtSelect(xsgdata, cardNumber);
    xSGTgtIsFormat(xsgdata, cardNumber, &badEncode);
    S32 hasDir = xSGTgtHasGameDir(xsgdata, cardNumber);
    fn_80038948(xsgdata, cardNumber, &files, &filesNeeded);
    if (hasDir == 1)
    {
        rc = xSGTgtHaveRoom(xsgdata, cardNumber, 0x6000, -1, &bytesNeeded, &availOnDisk,
                            &needFile);
    }
    else
    {
        rc = xSGTgtHaveRoom(xsgdata, cardNumber, 0x6000, -1, &bytesNeeded, &availOnDisk,
                            &needFile);
    }
    if (rc == 0)
    {
        fn_80041CE8(1, cardNumber, xSGTgtPhysSlotIdx(xsgdata, cardNumber));
        fn_80041CC4(1, 32);
        fn_80041C78(1, availOnDisk, bytesNeeded, files, filesNeeded, needFile);
    }

    if (rc == 0)
    {
        rc = 0;
    }
    else if (checkSlots)
    {
        rc = 1;
        for (S32 i = 0; i < 3; i++)
        {
            if (xSGGameIsEmpty(xsgdata, i))
            {
                if (xSGTgtHaveRoom(xsgdata, cardNumber, 0x6000, i, &bytesNeeded, &availOnDisk,
                                   &needFile))
                {
                    rc = 1;
                }
                else
                {
                    fn_80041CE8(1, cardNumber, xSGTgtPhysSlotIdx(xsgdata, cardNumber));
                    fn_80041CC4(1, 33);
                    fn_80041C78(1, availOnDisk, bytesNeeded, files, filesNeeded, needFile);
                    rc = 0;
                    if (slotsFull != NULL)
                    {
                        *slotsFull = 1;
                    }
                }
                break;
            }
        }
    }
    return rc;
}

// 1 when the card is formatted, 0 when it is not, -1 when it is not in its slot.
S32 zSaveLoad_CardCheckFormattedSingle(S32 cardNumber, S32* badEncode)
{
    S32 rc;
    st_XSAVEGAME_DATA* ldinst = xSGInit(XSG_MODE_LOAD);
    S32 tgtmax;

    rc = 0;
    if (badEncode != NULL)
    {
        *badEncode = 0;
    }

    switch (xSGTgtCount(ldinst, &tgtmax))
    {
    case 2:
        rc = xSGTgtIsFormat(ldinst, cardNumber, badEncode);
        if (rc != 0)
        {
            rc = 1;
        }
        if (rc == 0)
        {
            rc = 0;
        }
        break;
    case 1:
    {
        S32 idx = xSGTgtPhysSlotIdx(ldinst, 0);
        if (idx != cardNumber)
        {
            rc = -1;
        }
        else if (!(idx ^ cardNumber))
        {
            if (xSGTgtIsFormat(ldinst, 0, badEncode) == 0)
            {
                rc = 0;
            }
            else
            {
                rc = 1;
            }
        }
        break;
    }
    case 0:
        rc = -1;
        break;
    }

    xSGDone(ldinst);
    return rc;
}

// Formats the card, then shows the result. Returns 1 when the card was formatted, 0 when it was
// not, 5 when it was pulled.
S32 format(S32 num, S32 mode)
{
    zSaveLoad_UIEvent(40, eEventUIFocusOn);
    zSaveLoad_Tick();

    st_XSAVEGAME_DATA* data = xSGInit(XSG_MODE_LOAD);
    S32 tgtmax;
    S32 canRecover = 0;
    S32 rc = 0;
    switch (xSGTgtCount(data, &tgtmax))
    {
    case 2:
        xSGTgtSelect(data, num);
        rc = xSGTgtFormatTgt(data, num, &canRecover);
        zSaveLoad_UIEvent(40, eEventUIFocusOff);
        xSGDone(data);
        if (rc == 0)
        {
            if (canRecover)
            {
                fn_80041CC4(1, 21);
            }
            else
            {
                fn_80041CC4(1, 22);
            }
            S32 i = mode == 1 ? 43 : 44;
            zSaveLoad_UIEvent(i, eEventUIFocusOn_Select);
            promptSel = -1;
            while (promptSel == -1)
            {
                zSaveLoad_Tick();
            }
            zSaveLoad_UIEvent(i, eEventUIFocusOff_Unselect);
        }
        if (rc != 0)
        {
            rc = 1;
        }
        break;
    case 1:
    {
        S32 idx = xSGTgtPhysSlotIdx(data, 0);
        if (idx != num)
        {
            zSaveLoad_UIEvent(40, eEventUIFocusOff);
            xSGDone(data);
            rc = 5;
        }
        else if (idx ^ num)
        {
            zSaveLoad_UIEvent(40, eEventUIFocusOff);
            xSGDone(data);
        }
        else
        {
            xSGTgtSelect(data, 0);
            rc = xSGTgtFormatTgt(data, 0, &canRecover);
            zSaveLoad_UIEvent(40, eEventUIFocusOff);
            xSGDone(data);
            if (rc == 0)
            {
                if (canRecover)
                {
                    fn_80041CC4(1, 21);
                }
                else
                {
                    fn_80041CC4(1, 22);
                }
                S32 i = mode == 1 ? 43 : 44;
                zSaveLoad_UIEvent(i, eEventUIFocusOn_Select);
                promptSel = -1;
                while (promptSel == -1)
                {
                    zSaveLoad_Tick();
                }
                zSaveLoad_UIEvent(i, eEventUIFocusOff_Unselect);
            }
        }
        break;
    }
    case 0:
        rc = 5;
        zSaveLoad_UIEvent(40, eEventUIFocusOff);
        xSGDone(data);
        break;
    }
    return rc;
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

void zSaveLoad_Tick()
{
    time_current = 1.0f / (F32)(OS_BUS_CLOCK >> 2) * iTimeGet();
    time_elapsed = time_current - time_last;
    if (time_elapsed < 0.0f)
    {
        time_elapsed = 1.0f / 60.0f;
    }

    time_last = time_current;
    dontPoll -= time_elapsed;
    t0 = t1;
    t1 = iTimeGet();
    xPadUpdate(0, time_elapsed);
    fn_80016600();
    if (globals.pad1)
    {
        xPadUpdate(1, time_elapsed);
    }
    xDrawBegin();
    xParMgrUpdate(time_elapsed);
    zSceneUpdate(time_elapsed);
    xSndSetFrame(&globals.camera.hi_cam.mat.right, &globals.camera.hi_cam.mat.up,
                &globals.camera.hi_cam.mat.at, &globals.camera.hi_cam.mat.pos, 25.0f);
    xSndSetFrameFar((xVec3*)&globals.player.ent.model->Mat->pos);
    xSndUpdate(time_elapsed);
    xCameraUpdate(&globals.camera, time_elapsed);
    xCameraBegin(&globals.camera, 1);
    zSceneRender();
    xDebugUpdate();
    fn_8004436C();
    xDrawEnd();
    if (gScreenDump)
    {
        iDebugScreenDump();
    }
    xCameraEnd(&globals.camera, time_elapsed, 1);
    iEnvEndRenderFX(NULL);
    xCameraShowRaster(&globals.camera);
    lbl_8025E9F8++;
}
