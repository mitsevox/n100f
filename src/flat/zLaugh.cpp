#include "zLaugh.h"
#include "iSnd.h"
#include "xSnd.h"
#include "xString.h"

#include <stdio.h>

#define LAUGH_SITUATION_COUNT 15
#define LAUGH_TRACK_COUNT 17

static zLaughSituation sLaughInfo[LAUGH_SITUATION_COUNT] = {
    { 0, 120.0f, 1.0f, 0.0f, 0 },
    { 0, 120.0f, 1.0f, 0.0f, 0 },
    { 0, 120.0f, 1.0f, 0.0f, 0 },
    { 0, 120.0f, 1.0f, 0.0f, 0 },
    { 0, 120.0f, 1.0f, 0.0f, 0 },
    { 0, 120.0f, 1.0f, 0.0f, 0 },
    { 0xFFFFFFFF, 120.0f, 1.0f, 0.0f, 0 },
    { 0, 120.0f, 1.0f, 0.0f, 0 },
    { 0, 120.0f, 1.0f, 0.0f, 0 },
    { 0, 120.0f, 1.0f, 0.0f, 0 },
    { 0, 120.0f, 1.0f, 0.0f, 0 },
    { 0, 120.0f, 1.0f, 0.0f, 0 },
    { 0, 120.0f, 1.0f, 0.0f, 0 },
    { 0, 2.0f, 0.0f, 0.0f, 0 },
    { 0, 120.0f, 1.0f, 0.0f, 0 },
};

static U32 sLaughSoundID[LAUGH_TRACK_COUNT];
static S32 sLastLaughEnum;
static F32 sLaughTimer;
static zLaughSituation* sLaughQueueData;
static S32 sLastRefLaugh;

void zLaughInit()
{
    char str[32];
    S32 i;

    for (i = 0; i < LAUGH_TRACK_COUNT; i++)
    {
        sprintf(str, "laugh_track%02d", i + 1);
        sLaughSoundID[i] = xStrHash(str);
    }

    for (i = 0; i < LAUGH_SITUATION_COUNT; i++)
    {
        sLaughInfo[i].elapsedTime = sLaughInfo[i].delay;
        sLaughInfo[i].count = 0;
    }
}

// Round-robin over the laugh tracks, skipping any whose sound is not loaded.
static S32 zLaughNextLaughEnum()
{
    S32 i;

    if (sLastRefLaugh >= LAUGH_TRACK_COUNT)
    {
        sLastRefLaugh = 0;
    }

    for (i = 0; i < LAUGH_TRACK_COUNT; i++)
    {
        if (iSndLookup(sLaughSoundID[sLastRefLaugh]))
        {
            return sLastRefLaugh++;
        }

        sLastRefLaugh++;
        if (sLastRefLaugh >= LAUGH_TRACK_COUNT)
        {
            sLastRefLaugh = 0;
        }
    }

    return -1;
}

void zLaughNotify(S32 situation)
{
    zLaughSituation* s = &sLaughInfo[situation];

    if (sLaughQueueData)
    {
        return;
    }

    if (s->countMax == 0xFFFFFFFF)
    {
        return;
    }

    if (s->countMax && s->count >= s->countMax)
    {
        return;
    }

    if (s->delay > s->elapsedTime)
    {
        return;
    }

    sLaughQueueData = s;
    sLaughTimer = s->punchDelay;
}

void zLaughUpdate(F32 dt)
{
    S32 i;

    for (i = 0; i < LAUGH_SITUATION_COUNT; i++)
    {
        sLaughInfo[i].elapsedTime += dt;
    }

    if (!sLaughTimer && !sLaughQueueData)
    {
        return;
    }

    if (sLaughTimer > 0.0f)
    {
        sLaughTimer -= dt;
        if (sLaughTimer < 0.0f)
        {
            sLaughTimer = 0.0f;
        }
    }

    if (sLaughTimer == 0.0f && sLaughQueueData)
    {
        S32 laugh = zLaughNextLaughEnum();

        if (laugh >= 0 && xSndPlay(sLaughSoundID[laugh], 0.5f, 0.5f, 0.0f, 128, 0x202, 0))
        {
            if (sLaughQueueData)
            {
                sLaughQueueData->elapsedTime = 0.0f;
                sLaughQueueData->count++;
                sLaughQueueData = NULL;
            }
            sLastLaughEnum = laugh;
        }
        else
        {
            if (sLaughQueueData)
            {
                sLaughQueueData->elapsedTime = 0.0f;
            }
            sLaughQueueData = NULL;
        }
    }
}
