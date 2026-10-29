#ifndef ZMENU_H
#define ZMENU_H

#include "types.h"

S32 zMenuIsFirstBoot();
void zMenuFMVPlay(char* filename, U32 buttons, F32 time);
U32 zMenuGetBadCard();
U32 zMenuLoop();
void zMenuSetup();
void zMenuExit();
void zMenuInit(U32 theSceneID);
S32 zMenuRunning();

#endif
