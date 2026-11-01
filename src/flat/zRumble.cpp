#include "zRumble.h"
#include "xPad.h"


void zRumbleStart(S32 pad_id, _tagSDRumbleType rumble_type)
{
    switch (rumble_type)
    {
    case SDR_Headbutt:
        xPadAddRumble(pad_id, eRumble_MediumHi, 0.2f, 1, 0);
        break;
    case SDR_ButtbounceJump:
        xPadAddRumble(pad_id, eRumble_Medium, 0.15f, 1, 0);
        break;
    case SDR_Buttbounce:
        xPadAddRumble(pad_id, eRumble_Medium, 0.15f, 1, 0);
        break;
    case SDR_VillainCollideScare:
    case SDR_GotScared:
        xPadAddRumble(pad_id, eRumble_Heavy, 0.15f, 1, 0);
        break;
    case SDR_FleaScratch:
        xPadAddRumble(pad_id, eRumble_Light, 1.5f, 1, 1);
        break;
    case SDR_ScreechingStop:
        xPadAddRumble(pad_id, eRumble_VeryLight, 0.5f, 1, 1);
        break;
    case SDR_Digging:
        xPadAddRumble(pad_id, eRumble_Medium, 0.1f, 1, 0);
        xPadAddRumble(pad_id, eRumble_Off, 0.15f, 0, 0);
        xPadAddRumble(pad_id, eRumble_Medium, 0.1f, 0, 0);
        xPadAddRumble(pad_id, eRumble_Off, 0.15f, 0, 0);
        xPadAddRumble(pad_id, eRumble_Medium, 0.1f, 0, 1);
        break;
    case SDR_WebDestroyed:
        xPadAddRumble(pad_id, eRumble_VeryLight, 0.1f, 1, 0);
        break;
    case SDR_XploDestroyedObj:
        xPadAddRumble(pad_id, eRumble_Medium, 0.1f, 1, 0);
        xPadAddRumble(pad_id, eRumble_Heavy, 0.1f, 0, 1);
        break;
    case SDR_DustDestroyedObj:
        xPadAddRumble(pad_id, eRumble_Light, 0.05f, 1, 0);
        xPadAddRumble(pad_id, eRumble_Medium, 0.15f, 0, 1);
        break;
    case SDR_Test:
        xPadAddRumble(pad_id, eRumble_LightHi, 0.5f, 1, 1);
        break;
    case SDR_EventLight:
        xPadAddRumble(pad_id, eRumble_Light, 0.35f, 1, 1);
        break;
    case SDR_EventMedium:
        xPadAddRumble(pad_id, eRumble_Medium, 0.35f, 1, 1);
        break;
    case SDR_EventHeavy:
        xPadAddRumble(pad_id, eRumble_Heavy, 0.35f, 1, 1);
        break;
    case SDR_FireworkBang:
        break;
    }
}

void zRumbleStartDistance(S32 pad_id, F32 real_dist, F32 max_dist)
{
    F32 rumble_it;

    if (real_dist < 0.0f)
    {
        real_dist = 0.0f;
    }
    else if (real_dist > max_dist)
    {
        real_dist = max_dist;
    }

    rumble_it = 0.35f * (1.0f - real_dist / max_dist);
    if (rumble_it > 0.0f)
    {
        xPadAddRumble(pad_id, eRumble_Medium, rumble_it, 1, 1);
    }
}
