#include "zSFX.h"
#include "xEnt.h"
#include "xEvent.h"
#include "xSnd.h"
#include "iMath.h"
#include "zScene.h"

void zSFXInit(void* t, void* asset)
{
    xBaseInit((zSFX*)t, (zSFXAsset*)asset);
    ((zSFX*)t)->eventFunc = zSFXEventCB;
    ((zSFX*)t)->asset = (zSFXAsset*)asset;
    if (((zSFX*)t)->linkCount)
    {
        ((zSFX*)t)->link = (p2LinkAsset*)(((zSFX*)t)->asset + 1);
    }
    else
    {
        ((zSFX*)t)->link = NULL;
    }
    ((zSFX*)t)->sndID = 0;
}

void zSFXSave(zSFX* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

void zSFXLoad(zSFX* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

// flagsSFX bit 1 plays the sound in 3D, at the attached entity when bit 3 is set and otherwise at
// pos; bit 2 adds sound flag 0x100. eEventPlayMaybe plays when a roll of 0-99 is at least param 0.
S32 zSFXEventCB(xBase*, xBase* to, U32 toEvent, F32* toParam, xBase*)
{
    zSFX* t = (zSFX*)to;
    U32 sndFlags;

    switch (toEvent)
    {
    case eEventReset:
        xBaseReset(t, t->asset);
        break;
    case eEventPlay:
        sndFlags = 0;
        if (t->asset->flagsSFX & 4)
        {
            sndFlags |= 0x100;
        }
        if (t->asset->flagsSFX & 2)
        {
            if (t->asset->flagsSFX & 8)
            {
                _xEnt* ent = (_xEnt*)zSceneFindObject(t->asset->attachID);
                t->sndID = xSndPlay3D(t->asset->soundAssetID, 0.5f * (t->asset->volume / 100.0f),
                                      t->asset->freq * t->asset->freqm, t->asset->priority,
                                      sndFlags, ent, t->asset->range);
            }
            else
            {
                t->sndID = xSndPlay3D(t->asset->soundAssetID, 0.5f * (t->asset->volume / 100.0f),
                                      t->asset->freq * t->asset->freqm, t->asset->priority,
                                      sndFlags, &t->asset->pos, t->asset->range);
            }
        }
        else
        {
            t->sndID = xSndPlay(t->asset->soundAssetID, 0.5f * (t->asset->volume / 100.0f),
                                0.5f * (t->asset->volume / 100.0f), t->asset->freq * t->asset->freqm,
                                t->asset->priority, sndFlags, 0);
        }
        break;
    case eEventPlayMaybe:
        sndFlags = 0;
        if (t->asset->flagsSFX & 4)
        {
            sndFlags |= 0x100;
        }
        if (irand() % 100 >= toParam[0])
        {
            if (t->asset->flagsSFX & 2)
            {
                if (t->asset->flagsSFX & 8)
                {
                    _xEnt* ent = (_xEnt*)zSceneFindObject(t->asset->attachID);
                    t->sndID =
                        xSndPlay3D(t->asset->soundAssetID, 0.5f * (t->asset->volume / 100.0f),
                                   t->asset->freq * t->asset->freqm, t->asset->priority, sndFlags,
                                   ent, t->asset->range);
                }
                else
                {
                    t->sndID =
                        xSndPlay3D(t->asset->soundAssetID, 0.5f * (t->asset->volume / 100.0f),
                                   t->asset->freq * t->asset->freqm, t->asset->priority, sndFlags,
                                   &t->asset->pos, t->asset->range);
                }
            }
            else
            {
                t->sndID = xSndPlay(t->asset->soundAssetID, 0.5f * (t->asset->volume / 100.0f),
                                    0.5f * (t->asset->volume / 100.0f),
                                    t->asset->freq * t->asset->freqm, t->asset->priority, sndFlags,
                                    0);
            }
        }
        break;
    case eEventStop:
        if (t->sndID)
        {
            xSndStop(t->sndID);
            t->sndID = 0;
        }
        break;
    case eEventPause:
        if (t->sndID)
        {
            xSndPause(t->sndID, 1);
        }
        break;
    }

    return 1;
}
