#ifndef ZSAVELOAD_H
#define ZSAVELOAD_H

#include "types.h"

struct zSaveLoadGame
{
    char label[64];
    char date[32];
    S32 progress;
};

struct zSaveLoadUI
{
    U32 entry;
    U32 nameID;
    char* name;
};

extern zSaveLoadGame zSaveLoadGameTable[];
extern zSaveLoadUI zSaveLoadUITable[61];

void zSaveLoad_Tick();
U32 zSaveLoad_SaveLoop();
U32 zSaveLoad_LoadLoop();
S32 zSaveLoad_LoadGame();
S32 zSaveLoad_SaveGame();
S32 zSaveLoad_GameSelect(S32 mode);
void zSaveLoad_BuildName(char* name_txt, S32 idx);
S32 zSaveLoad_CardPick(S32 mode);
S32 zSaveLoad_CardCheckSlotOverwrite(S32 cardNumber, S32 gameNumber);
S32 zSaveLoad_CardCheckSlotEmpty(S32 cardNumber, S32 gameNumber);
S32 zSaveLoad_CardCheckGameSlot(S32 cardNumber, S32 gameNumber, S32 mode);
S32 zSaveLoad_CardCheckGames(S32 cardNumber, S32 mode);
S32 zSaveLoad_CardCheckSpace(S32 cardNumber, S32 mode, S32 checkSlots);
S32 zSaveLoad_CardCheckValid(S32 cardNumber, S32 mode, S32 checkSlots);
S32 zSaveLoad_CardCheckFormatted(S32 cardNumber, S32 mode);
S32 zSaveLoad_CardCheck(S32 cardNumber, S32 mode);
S32 zSaveLoad_getcard();
S32 zSaveLoad_getgame();

#endif
