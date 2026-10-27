#ifndef ZRUMBLE_H
#define ZRUMBLE_H

#include "types.h"

// Partial: only the rumble types reconstructed code passes.
enum _tagSDRumbleType
{
    SDR_Headbutt = 1
};

void zRumbleStart(S32 pad_id, _tagSDRumbleType rumble_type);

#endif
