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

void zSaveLoad_BuildName(char* name_txt, S32 idx);
S32 zSaveLoad_getcard();
S32 zSaveLoad_getgame();

#endif
