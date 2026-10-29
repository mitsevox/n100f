#ifndef ZENV_H
#define ZENV_H

#include "xBase.h"
#include "xClimate.h"

struct _zEnv : xBase
{
    p2EnvAsset* easset;
};

struct _xEnv;
struct xSerial;

void zEnvInit(void* env, void* easset);
void zEnvInit(_zEnv* env, p2EnvAsset* easset);
void zEnvSetup(_zEnv* env);
void zEnvStartingCamera(_zEnv* env);
void zEnvRender(_xEnv* env);
void zEnvSave(_zEnv* ent, xSerial* s);
void zEnvLoad(_zEnv* ent, xSerial* s);
S32 zEnvEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);

#endif
