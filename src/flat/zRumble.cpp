#include "zRumble.h"

extern "C" S32 fn_80031AC8(S32 pad_id, S32 type, F32 time, S32 replace, U32 fxflags);

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
        fn_80031AC8(pad_id, 6, rumble_it, 1, 1);
    }
}

void zRumbleStart(S32 pad_id, _tagSDRumbleType rumble_type)
{
    switch (rumble_type)
    {
    case SDR_Headbutt:
        fn_80031AC8(pad_id, 5, 0.2f, 1, 0);
        break;
    case SDR_ButtbounceJump:
        fn_80031AC8(pad_id, 6, 0.15f, 1, 0);
        break;
    case SDR_Buttbounce:
        fn_80031AC8(pad_id, 6, 0.15f, 1, 0);
        break;
    case SDR_VillainCollideScare:
    case SDR_GotScared:
        fn_80031AC8(pad_id, 8, 0.15f, 1, 0);
        break;
    case SDR_FleaScratch:
        fn_80031AC8(pad_id, 4, 1.5f, 1, 1);
        break;
    case SDR_ScreechingStop:
        fn_80031AC8(pad_id, 2, 0.5f, 1, 1);
        break;
    case SDR_Digging:
        fn_80031AC8(pad_id, 6, 0.1f, 1, 0);
        fn_80031AC8(pad_id, 0, 0.15f, 0, 0);
        fn_80031AC8(pad_id, 6, 0.1f, 0, 0);
        fn_80031AC8(pad_id, 0, 0.15f, 0, 0);
        fn_80031AC8(pad_id, 6, 0.1f, 0, 1);
        break;
    case SDR_WebDestroyed:
        fn_80031AC8(pad_id, 2, 0.1f, 1, 0);
        break;
    case SDR_XploDestroyedObj:
        fn_80031AC8(pad_id, 6, 0.1f, 1, 0);
        fn_80031AC8(pad_id, 8, 0.1f, 0, 1);
        break;
    case SDR_DustDestroyedObj:
        fn_80031AC8(pad_id, 4, 0.05f, 1, 0);
        fn_80031AC8(pad_id, 6, 0.15f, 0, 1);
        break;
    case SDR_Test:
        fn_80031AC8(pad_id, 3, 0.5f, 1, 1);
        break;
    case SDR_EventLight:
        fn_80031AC8(pad_id, 4, 0.35f, 1, 1);
        break;
    case SDR_EventMedium:
        fn_80031AC8(pad_id, 6, 0.35f, 1, 1);
        break;
    case SDR_EventHeavy:
        fn_80031AC8(pad_id, 8, 0.35f, 1, 1);
        break;
    case SDR_FireworkBang:
        break;
    }
}
