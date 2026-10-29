#ifndef ZGAMESTATE_H
#define ZGAMESTATE_H

#include "types.h"

enum eGameMode
{
    eGameMode_Boot,
    eGameMode_Intro,
    eGameMode_Title,
    eGameMode_Start,
    eGameMode_Load,
    eGameMode_Options,
    eGameMode_Save,
    eGameMode_Pause,
    eGameMode_WorldMap,
    eGameMode_MonsterGallery,
    eGameMode_ConceptArtGallery,
    eGameMode_Game,
    eGameMode_Count
};

enum _GameOstrich
{
    eGameOstrich_Loading,
    eGameOstrich_PlayingMovie,
    eGameOstrich_InScene,
    eGameOstrich_Total
};

extern S32 gGameState;
extern eGameMode gGameMode;
extern _GameOstrich gGameOstrich;

S32 zGameModeGet();
S32 zGameStateGet();
void zGameModeSwitch(eGameMode modeNew);
void zGameStateSwitch(S32 theNewState);
void zGameStateSwitchEvent(S32 event);
void zGameSetOstrich(_GameOstrich o);
_GameOstrich zGameGetOstrich();

#endif
