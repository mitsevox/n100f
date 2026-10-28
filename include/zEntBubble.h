#ifndef ZENTBUBBLE_H
#define ZENTBUBBLE_H

#include "zEnt.h"

struct xPar;
struct zScene;

struct _zEntBubble : _zEnt
{
    U32 state;
    _zEnt* villain;
    F32 bubbleScale;
    F32 timer;
    F32 stuckTimer;
    xVec3 envelopStart;
    F32 fx_timer;
    F32 fx_scale;
    xPar* particle;
};

void zEntBubble_SceneInit(zScene* scene);
void zEntBubble_EnableCollision();
void zEntBubble_DisableCollision();
void zEntBubble_Launch(F32 wait);
void zEntBubble_Update(_zEntBubble* ent, _xScene* sc, F32 dt);
void zEntBubble_Move(_zEntBubble* ent, _xScene* sc, F32 dt, _xEntFrame* frame);
void zEntBubble_Pop(_zEntBubble* ent);
void zEntBubble_Cleanup();
void zEntBubble_Render();
S32 zEntBubbleEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);

#endif
