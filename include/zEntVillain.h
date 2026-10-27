#ifndef ZENTVILLAIN_H
#define ZENTVILLAIN_H

#include "zEnt.h"

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
};

#endif
