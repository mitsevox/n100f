#include "xSnd.h"
#include "xMath3.h"
#include "xEnt.h"
#include "xModel.h"
#include "zGlobals.h"
#include "zScene.h"

xSndGlobals gSnd;

void xSndInit()
{
    iSndInit();

    xSndVoiceInfo* voice = gSnd.voice;
    for (U32 i = 0; i < 64; i++, voice++)
    {
        voice->flags = 0;
    }

    gSnd.mastervol.volL = 1.0f;
    gSnd.mastervol.volR = 1.0f;
    gSnd.streamvol.volL = 1.0f;
    gSnd.streamvol.volR = 1.0f;
    gSnd.fxvol.volL = 1.0f;
    gSnd.fxvol.volR = 1.0f;
    gSnd.stereo = iSndGetStereo();

    xVec3 origin = { 0.0f, 0.0f, 0.0f };
    gSnd.nearpos = origin;
    gSnd.farpos = origin;

    iSndSuspendCD(1);
}

// Pauses every playing sound and gives the platform layer one update with no time passing.
void xSndSuspend()
{
    xSndPauseAll(1, 1);
    xSndUpdate(0.0f);
}

void xSndResume()
{
    xSndPauseAll(0, 0);
}

void xSndSetMasterVol(F32 left, F32 right)
{
    gSnd.mastervol.volL = left;
    gSnd.mastervol.volR = right;
}

void xSndSetStreamVol(F32 left, F32 right)
{
    gSnd.streamvol.volL = left;
    gSnd.streamvol.volR = right;
}

void xSndSetFXVol(F32 left, F32 right)
{
    gSnd.fxvol.volL = left;
    gSnd.fxvol.volR = right;
}

// Flag 0x2 marks a sound effect and 0x4 a stream; each kind takes its own pause setting.
void xSndPauseAll(U32 pause_effects, U32 pause_streams)
{
    for (U32 i = 0; i < 64; i++)
    {
        if (gSnd.voice[i].flags & 0x1)
        {
            if (gSnd.voice[i].flags & 0x2)
            {
                iSndPause(gSnd.voice[i].sndID, pause_effects);
            }
            else if (gSnd.voice[i].flags & 0x4)
            {
                iSndPause(gSnd.voice[i].sndID, pause_streams);
            }
        }
    }
}

void xSndStopAll()
{
    for (U32 i = 0; i < 64; i++)
    {
        if (gSnd.voice[i].flags & 0x1)
        {
            iSndStop(gSnd.voice[i].sndID);
        }
    }
}

// The listener sits at the near point while the far point is within sqrt(20) of it and at the
// far point beyond 10; in between it slides from one to the other.
void xSndUpdate(F32 time_elapsed)
{
    xVec3 d;
    xVec3Sub(&d, &gSnd.farpos, &gSnd.nearpos);
    F32 a = xVec3Dot(&d, &d);
    if (a < 20.0f)
    {
        gSnd.pos = gSnd.nearpos;
    }
    else if (a > 100.0f)
    {
        gSnd.pos = gSnd.farpos;
    }
    else
    {
        a = (a - 20.0f) / 80.0f;
        gSnd.pos.x = gSnd.nearpos.x + d.x * a;
        gSnd.pos.y = gSnd.nearpos.y + d.y * a;
        gSnd.pos.z = gSnd.nearpos.z + d.z * a;
    }

    iSndUpdate(time_elapsed);
}

void xSndExit()
{
    iSndExit();
}

// Plays a sound on a free voice and returns its handle: -1 when the sound is not loaded, -2 when
// no voice is free and 0 when it fails to start. Sound IDs from 0x1000 up are streams. In s005 one
// sound is swapped for another when that one is loaded.
U32 xSndPlay(U32 id, F32 volL, F32 volR, F32 pitch, U32 priority, U32 flags, U32 parentID)
{
    U32 sceneID = globals.sceneCur->sceneID;
    if (id == 0x91D67287 && sceneID == zSceneIDFromName("s005") &&
        iSndLookup(0xFE619D7B) != NULL)
    {
        id = 0xFE619D7B;
    }

    iSndFileInfo* info = iSndLookup(id);
    if (info == NULL)
    {
        return -1;
    }

    U32 vflags;
    flags &= 0x700;
    vflags = flags | 4;
    if ((S32)info->ID >= 0x1000)
    {
        vflags = flags | 2;
    }

    U32 idx = iSndFindFreeVoice(priority, vflags);
    if (idx == 0xFFFFFFFE)
    {
        return -2;
    }

    xSndVoiceInfo* vp = &gSnd.voice[idx];
    vp->priority = priority;
    vp->pitch = pitch;
    vp->vol = (volL > volR) ? volL : volR;
    vp->sample_rate = info->info.dsp.sample_rate;
    vp->assetID = id;
    vp->internalID = info->ID;
    vp->parentID = parentID;
    vp->parentPos = NULL;
    vp->sndID = ++gSnd.SndCount;
    vp->flags = vflags;
    vp->flags |= 1;
    vp->deadct = 0;
    if (!iSndPlay(vp))
    {
        vp->flags &= ~1;
        return 0;
    }
    return vp->sndID;
}

// Plays a sound that follows the entity. A radius of 0 or less means it carries 1000 units.
U32 xSndPlay3D(U32 id, F32 vol, F32 pitch, U32 priority, U32 flags, _xEnt* parent, F32 radius)
{
    if (parent == NULL)
    {
        return -2;
    }

    iSndFileInfo* info = iSndLookup(id);
    if (info == NULL)
    {
        return -1;
    }

    U32 vflags;
    flags &= 0x700;
    vflags = flags | 4;
    if ((S32)info->ID >= 0x1000)
    {
        vflags = flags | 2;
    }

    U32 idx = iSndFindFreeVoice(priority, vflags);
    if (idx == 0xFFFFFFFE)
    {
        return -2;
    }

    xSndVoiceInfo* vp = &gSnd.voice[idx];
    vp->priority = priority;
    vp->pitch = pitch;
    vp->vol = vol;
    vp->sample_rate = info->info.dsp.sample_rate;
    vp->assetID = id;
    vp->internalID = info->ID;
    vp->sndID = ++gSnd.SndCount;
    vp->flags = vflags;
    vp->flags |= 1;
    vp->deadct = 0;
    vp->parentID = (U32)parent;
    vp->parentPos = NULL;
    vp->pos = *(xVec3*)&((_xEnt*)((U32)parent & ~3))->model->Mat->pos;
    if (radius <= 0.0f)
    {
        vp->radius2 = 1000000.0f;
    }
    else
    {
        vp->radius2 = radius * radius;
    }
    vp->flags |= 0x18;
    iSndPlay(vp);
    return vp->sndID;
}

// The GameCube build does not play sounds placed at a bare position.
U32 xSndPlay3D(U32 id, F32 vol, F32 pitch, U32 priority, U32 flags, xVec3* pos, F32 radius)
{
    return -1;
}

void xSndSetFrame(xVec3* right, xVec3* up, xVec3* at, xVec3* pos, F32 radius)
{
    gSnd.right = *right;
    gSnd.up = *up;
    gSnd.at = *at;
    gSnd.nearpos = *pos;
    gSnd.radius2 = radius <= 0.0f ? 1000000.0f : radius * radius;
}

void xSndSetFrameFar(xVec3* pos)
{
    gSnd.farpos = *pos;
}

void xSndPause(U32 snd, U32 pause)
{
    iSndPause(snd, pause);
}

void xSndStop(U32 snd)
{
    iSndStop(snd);
}

// Flag 0x10 ties a sound to its parent's position.
void xSndParentDied(U32 pid)
{
    xSndVoiceInfo* vp = gSnd.voice;

    for (U32 i = 0; i < 64; i++, vp++)
    {
        if (vp->parentID == pid)
        {
            vp->flags &= (U16)~0x10;
        }
    }
}

void xSndStopChildren(U32 pid)
{
    U32 i;
    xSndVoiceInfo* vp = gSnd.voice;

    for (i = 0; i < 64; i++, vp++)
    {
        if ((vp->flags & 0x1) && vp->parentID == pid)
        {
            iSndStop(vp->sndID);
            vp->flags &= (U16)~0x10;
        }
    }
}
