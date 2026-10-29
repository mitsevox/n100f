#ifndef ISND_H
#define ISND_H

#include "types.h"

#include "iFile.h"

struct xSndVoiceInfo;

struct iSndVol
{
    S16 volL;
    S16 volR;
};

struct iSndInfo
{
    U32 flags;
    iSndVol vol;
    U32 pitch;
    S32 lastStreamBuffer;
};

// The 0x60-byte header of a GameCube DSP ADPCM sound.
struct sDSPADPCM
{
    U32 num_samples;
    U32 num_adpcm_nibbles;
    U32 sample_rate;
    U16 loop_flag;
    U16 format;
    U32 sa;
    U32 ea;
    U32 ca;
    U16 coef[16];
    U16 gain;
    U16 ps;
    U16 yn1;
    U16 yn2;
    U16 lps;
    U16 lyn1;
    U16 lyn2;
    U16 pad[11];
};

// One sound of a sound bank: its DSP header and asset ID.
struct iSndSoundInfo
{
    sDSPADPCM dsp;
    U32 assetID;
};

// A sound bank asset: the sounds that load into ARAM, then the streams.
struct iSndBank
{
    U32 num_sounds;
    U32 size;
    U32 num_streams;
    iSndSoundInfo sounds[1];
};

// The GC layout: the last sound looked up, with the bank file it came from.
struct iSndFileInfo
{
    iSndSoundInfo info;
    U32 ID;
    tag_xFile file;
};

void iSndInit();
void iSndExit();
void iSndInitSceneLoaded();

U32 iSndIsPlaying(U32 assetID);
U32 iSndIsPlaying(U32 assetID, U32 parid);
iSndFileInfo* iSndLookup(U32 id);

void iSndPause(U32 snd, U32 pause);
void iSndStop(U32 snd);
void iSndUpdate(F32 dt);

U32 iSndFindFreeVoice(U32 priority, U32 flags);

U32 iSndPlay(xSndVoiceInfo* vp);
U32 iSndGetStereo();
void iSndStereo(U32 stereo);

void iSndWaitForDeadSounds();
void iSndSuspendCD(U32 suspend);

void iSndSceneExit();

void iSndLoadSounds(void* data);
void iSndDIEDIEDIE();

#endif
