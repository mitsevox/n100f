#include "xSnd.h"
#include "xMath3.h"

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
