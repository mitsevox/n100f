#include "zGameState.h"
#include "zEnt.h"

extern "C" S32 lbl_8025E510;
void xSndPauseAll(U32 pause_effects, U32 pause_streams);

S32 gGameState = 6;
eGameMode gGameMode;
_GameOstrich gGameOstrich;

// Per mode, the event sent to every dispatcher when the mode or its state changes.
static U32 sGameMode_DispatchTable[12] = { 0 };
static U32 sGameState_DispatchTable[8] = { 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0, 0 };
static U32 sPauseState_DispatchTable[2] = { 0x80, 0x81 };
static U32 sOptionsState_DispatchTable[1] = { 0x7A };
static U32 sSaveState_DispatchTable[3] = { 0x7C, 0x7D, 0x7E };
static U32 sLoadState_DispatchTable[3] = { 0x76, 0x77, 0x78 };
static U32 sTitleState_DispatchTable[2] = { 0x73, 0x74 };
static U32 sIntroState_DispatchTable[4] = { 0x6E, 0x6F, 0x70, 0x71 };

// The events that request a state; their index in the table is the new state.
static U32 sGameState_DoDispatchTable[8] = { 0x9E, 0x9F, 0xA0, 0xA1, 0x128, 0xA2, 0xA3, 0xA6 };
static U32 sPauseState_DoDispatchTable[2] = { 0x9B, 0x9C };
static U32 sSaveState_DoDispatchTable[3] = { 0x97, 0x98, 0x99 };
static U32 sOptionsState_DoDispatchTable[1] = { 0x95 };
static U32 sLoadState_DoDispatchTable[3] = { 0x91, 0x92, 0x93 };
static U32 sTitleState_DoDispatchTable[2] = { 0x8E, 0x8F };
static U32 sIntroState_DoDispatchTable[4] = { 0x89, 0x8A, 0x8B, 0x8C };

static inline S32 zGameStateFindEvent(U32* eventList, S32 eventCount, S32 targetMode, S32 targetEvent,
                                      S32* new_mode, S32* new_state)
{
    for (S32 i = 0; i < eventCount; i++)
    {
        if (targetEvent == eventList[i])
        {
            *new_mode = targetMode;
            *new_state = i;
            return 1;
        }
    }
    return 0;
}

void zGameModeSwitch(eGameMode modeNew)
{
    eGameMode modeOld = gGameMode;

    if ((modeOld == eGameMode_Game && modeNew == eGameMode_Pause) ||
        (modeOld == eGameMode_Game && modeNew == eGameMode_Save))
    {
        xSndPauseAll(1, 1);
    }
    else if ((modeOld == eGameMode_Save || modeOld == eGameMode_Pause || modeOld == eGameMode_Load) &&
             modeNew == eGameMode_Game)
    {
        xSndPauseAll(0, 0);
    }

    gGameMode = modeNew;
    zEntEventAllOfType(sGameMode_DispatchTable[modeNew], 0x1E);
}

void zGameStateSwitch(S32 theNewState)
{
    S32 stateOld = gGameState;
    U32 theEvent = 0;

    gGameState = theNewState;

    if (theNewState == 1 && stateOld == 0)
    {
        lbl_8025E510 = 1;
    }

    switch (gGameMode)
    {
    case eGameMode_Intro:
        theEvent = sIntroState_DispatchTable[theNewState];
        break;
    case eGameMode_Title:
        theEvent = sTitleState_DispatchTable[theNewState];
        break;
    case eGameMode_Load:
        theEvent = sLoadState_DispatchTable[theNewState];
        break;
    case eGameMode_Options:
        theEvent = sOptionsState_DispatchTable[theNewState];
        break;
    case eGameMode_Save:
        theEvent = sSaveState_DispatchTable[theNewState];
        break;
    case eGameMode_Pause:
        theEvent = sPauseState_DispatchTable[theNewState];
        break;
    case eGameMode_Game:
        theEvent = sGameState_DispatchTable[theNewState];
        break;
    }

    if (theEvent)
    {
        zEntEventAllOfType(theEvent, 0x1E);
    }
}

void zGameStateSwitchEvent(S32 event)
{
    S32 old_mode = gGameMode;
    S32 old_state = gGameState;
    S32 new_mode = -1;
    S32 new_state = -1;

    if (zGameStateFindEvent(sGameState_DoDispatchTable, 8, eGameMode_Game, event, &new_mode, &new_state) ||
        zGameStateFindEvent(sPauseState_DoDispatchTable, 2, eGameMode_Pause, event, &new_mode, &new_state) ||
        zGameStateFindEvent(sSaveState_DoDispatchTable, 3, eGameMode_Save, event, &new_mode, &new_state) ||
        zGameStateFindEvent(sOptionsState_DoDispatchTable, 1, eGameMode_Options, event, &new_mode,
                            &new_state) ||
        zGameStateFindEvent(sLoadState_DoDispatchTable, 3, eGameMode_Load, event, &new_mode, &new_state) ||
        zGameStateFindEvent(sTitleState_DoDispatchTable, 2, eGameMode_Title, event, &new_mode, &new_state) ||
        zGameStateFindEvent(sIntroState_DoDispatchTable, 4, eGameMode_Intro, event, &new_mode, &new_state))
    {
    }

    if (new_mode != old_mode)
    {
        zGameModeSwitch((eGameMode)new_mode);
    }

    if (new_mode != old_mode || new_state != old_state)
    {
        zGameStateSwitch(new_state);
    }
}

void zGameSetOstrich(_GameOstrich o)
{
    gGameOstrich = o;
}

_GameOstrich zGameGetOstrich()
{
    return gGameOstrich;
}

S32 zGameModeGet()
{
    return gGameMode;
}

S32 zGameStateGet()
{
    return gGameState;
}
