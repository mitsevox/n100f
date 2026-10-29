#ifndef ZENTVILLAIN_H
#define ZENTVILLAIN_H

#include "zEnt.h"
#include "iModel.h"

struct _zEntBubble;

// Partial: only the goals reconstructed code compares against.
enum en_BEHAVIOUR_GOAL
{
    ZVIL_GOAL_PATROL = 6,
    ZVIL_GOAL_FOLLOW = 8
};

// Partial: members up to spd_chase. The members that no reconstructed function uses are
// left opaque.
struct st_zEntVillain : _zEnt
{
    U8 opaque_EC[0x114 - 0xEC];
    en_BEHAVIOUR_GOAL goal;
    U8 opaque_118[0x154 - 0x118];
    F32 spd_move;
    F32 spd_chase;
    U8 opaque_15C[0x1B4 - 0x15C];
    iModelTag tag_vert[12];
};

enum en_ZVILLAIN_BUBBLE_RESPONSE
{
    ZVIL_BUBBLE_NONE = 0,
    ZVIL_BUBBLE_ENVELOP = 1,
    ZVIL_BUBBLE_BREAK = 2,
    ZVIL_BUBBLE_BOUNCE = 3
};

en_ZVILLAIN_BUBBLE_RESPONSE zEntVillain_Bubble(st_zEntVillain* ent, _zEntBubble* bub);

#endif
