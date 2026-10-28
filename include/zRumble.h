#ifndef ZRUMBLE_H
#define ZRUMBLE_H

#include "types.h"

// Partial: only the rumble types reconstructed code passes.
enum _tagSDRumbleType
{
    SDR_Headbutt = 1,
    SDR_WebDestroyed = 8,
    SDR_XploDestroyedObj = 9,
    SDR_DustDestroyedObj = 10,
    SDR_Total = 16
};

void zRumbleStart(S32 pad_id, _tagSDRumbleType rumble_type);
void zRumbleStartDistance(S32 pad_id, F32 real_dist, F32 max_dist);

#endif
