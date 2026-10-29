#ifndef ZVAR_H
#define ZVAR_H

#include "types.h"

struct xSerial;

struct zVarEntry
{
    U32 entry;
    U32 varNameID;
    char* varName;
    U32 (*varCB)();
};

struct zVarMagicEntry
{
    U32 varNameID;
    char* varName;
    char* (*varCB)(char*);
};

extern zVarEntry zVarEntryTable[108];

void zVarMagicProcessText(char* src, char* dst);
void zVarInit(zVarEntry* table);
void zVarNewGame();
void zVarSave(xSerial* s);
void zVarLoad(xSerial* s);
void zVarSetSnackGateValue(U32 value);
void zVarSetArea(U32 sceneID);
void zVarDefeatMasterMind();
void zVarUnlockF001();
void zVarUnlockE001();
void zVarUnlockS001();
void zVarUnlockR001();
void zVarGiveMap();
void zVarUnlockAllMovies();
void zVarUnlockEndGame();

#endif
