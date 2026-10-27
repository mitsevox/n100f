#ifndef ZENTBUTTON_H
#define ZENTBUTTON_H

#include "zEnt.h"
#include "xEntMotion.h"

struct zEntButtonAsset
{
    U32 actMethod;
    S32 initButtonState;
    S32 isReset;
    S32 isSpringBB;
    U8 buttonFlags;
};

struct _zEntButton : _zEnt
{
    zEntButtonAsset* basset;
    _xEntMotion motion;
    U32 state;
    F32 speed;
    U32 oldState;
    S32 oldMotState;
    U32 counter;
};

void zEntButton_Init(void* ent, void* asset);
void zEntButton_Init(_zEntButton* ent, p2EntAsset* asset);
void zEntButton_Setup(_zEntButton* ent, _xScene* sc);
void zEntButton_Save(_zEntButton* ent, xSerial* s);
void zEntButton_Load(_zEntButton* ent, xSerial* s);
void zEntButton_Reset(_zEntButton* ent, _xScene* sc);
U32 zEntButton_Headbutt(_zEntButton* ent);
U32 zEntButton_Stand(_zEntButton* ent);
U32 zEntButton_ButtBounce(_zEntButton* ent);
U32 zEntButton_CheckState(_zEntButton* ent);

#endif
