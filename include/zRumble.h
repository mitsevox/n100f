#ifndef ZRUMBLE_H
#define ZRUMBLE_H

#include "types.h"

enum _tagSDRumbleType
{
    SDR_Digging = 0,
    SDR_Headbutt = 1,
    SDR_ButtbounceJump = 2,
    SDR_Buttbounce = 3,
    SDR_VillainCollideScare = 4,
    SDR_FleaScratch = 5,
    SDR_ScreechingStop = 6,
    SDR_GotScared = 7,
    SDR_WebDestroyed = 8,
    SDR_XploDestroyedObj = 9,
    SDR_DustDestroyedObj = 10,
    SDR_EventLight = 11,
    SDR_EventMedium = 12,
    SDR_EventHeavy = 13,
    SDR_Test = 14,
    SDR_FireworkBang = 15,
    SDR_Total = 16
};

void zRumbleStart(S32 pad_id, _tagSDRumbleType rumble_type);
void zRumbleStartDistance(S32 pad_id, F32 real_dist, F32 max_dist);

#endif
