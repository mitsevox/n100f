#include "iSnd.h"

#include "iFile.h"
#include "xEnt.h"
#include "xMath.h"
#include "xMath3.h"
#include "xSnd.h"
#include "xstransvc.h"
#include "xTRC.h"

#include <dolphin/ai.h>
#include <dolphin/ar.h>
#include <dolphin/ax.h>
#include <dolphin/dvd.h>
#include <dolphin/mix.h>
#include <dolphin/os.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
void fn_80016600();
void fn_800443AC();
U32 fn_800443F8();
}

void zGameScreenTransitionUpdate(F32 percentComplete, char* msg, U8* rgba);

// Size: 0x20
struct vinfo
{
    AXVPB* voice;
    U32 flags;
    U32 aid;
    F32 xc;
    S32 x10;
    S32 x14;
    S32 x18;
    S32 x1c;
};

// Size: 0x104
struct iSndStream
{
    vinfo vinf;
    iSndSoundInfo info;
    U32 x84;
    U32 offset;
    U32 x8c;
    U32 x90;
    U32 x94;
    DVDFileInfo fileInfo;
    U32 source_a;
    U32 dest_a;
    U32 dest_b;
    ARQRequest request;
    U32 x100;
};

// Size: 0x108
struct iSndBankFile
{
    tag_xFile file;
    U32 base;
    iSndBank* bank;
};

u32 aram_array[40];
vinfo voices[58];
iSndStream streams[6];
iSndBankFile sinfo_array[12];
iSndFileInfo snd;

U32* ua_stream_buffer;
U32* stream_buffer;
U32 silence_buffer;
U32 zero_point;
U32 zero_end;
S32 sinfo_array_max;
volatile U32 SoundFlags;
volatile S32 fc;
U32 houston_we_have_a_problem;
static S32 sound_stream;

static void dv_callback(void* userdata)
{
    U32 stream = ((AXVPB*)userdata)->userContext;
    xSndVoiceInfo* info = &gSnd.voice[stream];
    info->sndID = 0;
    info->flags = 0;
    if (stream < 6)
    {
        if (streams[stream].vinf.voice == NULL)
        {
            return;
        }

        DVDCancelAsync(&streams[stream].fileInfo.cb, NULL);
        ARQRemoveRequest(&streams[stream].request);
        AXSetVoiceState(streams[stream].vinf.voice, 0);
        MIXReleaseChannel(streams[stream].vinf.voice);

        streams[stream].vinf.voice = NULL;
        streams[stream].vinf.flags = 0;
        streams[stream].vinf.aid = 0;
    }
    else
    {
        U32 idx = stream - 6;
        if (voices[idx].voice == NULL)
        {
            return;
        }

        MIXReleaseChannel(voices[idx].voice);
        AXSetVoiceState(voices[idx].voice, 0);

        voices[idx].voice = NULL;
        voices[idx].flags = 0;
        voices[idx].aid = 0;
    }
}

static void arq_callback(u32)
{
    SoundFlags = 0;
}

static void arqcb(u32 pointerToARQRequest);

static void dvdcb(s32 result, DVDFileInfo* info)
{
    iSndStream* data = (iSndStream*)info->cb.userData;
    if (data->vinf.voice == NULL)
    {
        return;
    }

    data->offset += result;
    data->x90 += result;
    data->vinf.flags |= 0x800;
    data->vinf.flags &= ~0x100;
    if (data->vinf.flags & (0x200 | 0x400))
    {
        houston_we_have_a_problem = TRUE;
    }
    if ((data->vinf.flags & 0x4) == 0)
    {
        return;
    }

    U32 dest = 0;
    if ((data->vinf.flags & 0x1000) == 0)
    {
        data->vinf.flags |= 0x200;
        dest = data->dest_a;
    }
    else if ((data->vinf.flags & 0x2000) == 0)
    {
        data->vinf.flags |= 0x400;
        dest = data->dest_b;
    }

    if (dest != 0)
    {
        ARQPostRequest(&data->request, (u32)data, 0, 1, data->source_a, dest, 0x2EE0, arqcb);
    }
}

static void arqcb(u32 pointerToARQRequest)
{
    ARQRequest* request = (ARQRequest*)pointerToARQRequest;
    iSndStream* data = (iSndStream*)request->owner;
    U32 left = data->x8c - data->x90;

    if (request->length == 0)
    {
        return;
    }

    if (data->vinf.voice == NULL)
    {
        return;
    }

    if (data->vinf.flags & 0x100)
    {
        houston_we_have_a_problem = TRUE;
    }

    if (data->vinf.flags & 0x200)
    {
        data->vinf.flags |= 0x1000;
        data->vinf.flags &= ~0x200;
    }
    else if (data->vinf.flags & 0x400)
    {
        data->vinf.flags |= 0x2000;
        data->vinf.flags &= ~0x400;
    }

    if (left == 0)
    {
        data->vinf.flags |= 0x20;
    }
    else
    {
        data->vinf.flags |= 0x200000;
    }

    data->vinf.flags &= ~0x800;
    data->vinf.flags &= ~0x400000;
    if (data->vinf.flags & 0x4)
    {
        data->vinf.flags &= ~4;
        data->vinf.flags |= 0x8;
    }
}

static void fcb()
{
    fc++;
    S32 i;
    S32 need_update = FALSE;
    for (i = 0; i < 6; i++)
    {
        xSndVoiceInfo* gsnd_voice = &gSnd.voice[i];
        U32 orig_flags = streams[i].vinf.flags;
        if (streams[i].vinf.voice == NULL)
        {
            continue;
        }

        u16 addrHi = streams[i].vinf.voice->pb.addr.currentAddressHi;
        u16 addrLo = streams[i].vinf.voice->pb.addr.currentAddressLo;
        if ((addrHi << 16) + addrLo >= zero_end - 0x5DC0 && (addrHi << 16) + addrLo < zero_end)
        {
            DVDCancelAsync(&streams[i].fileInfo.cb, NULL);
            ARQRemoveRequest(&streams[i].request);
            AXSetVoiceState(streams[i].vinf.voice, 0);
            MIXReleaseChannel(streams[i].vinf.voice);
            AXFreeVoice(streams[i].vinf.voice);
            voices[i].voice = NULL;
            voices[i].flags = 0;
            voices[i].aid = 0;
            continue;
        }

        if ((orig_flags & 0x4) || (orig_flags & 0x40000) || (orig_flags & 0x80000) ||
            (orig_flags & 0x20000) || (orig_flags & 0x2))
        {
            continue;
        }

        u32 addr = addrHi << 16;
        addr += addrLo;
        if (addr < streams[i].dest_b * 2 && (orig_flags & 0x4000) == 0 && (orig_flags & 0x8) == 0)
        {
            streams[i].vinf.flags |= 0x4000;
            streams[i].vinf.flags &= ~0x8000;
            streams[i].vinf.flags &= ~0x2000;
            if (streams[i].x90 == streams[i].x94)
            {
                streams[i].vinf.flags |= 0x20;
            }
            else
            {
                streams[i].x94 = streams[i].x90;
            }
            orig_flags = streams[i].vinf.flags;
        }
        else if (addr >= streams[i].dest_b * 2 && (orig_flags & 0x8000) == 0)
        {
            streams[i].vinf.flags |= 0x8000;
            streams[i].vinf.flags &= ~0x4000;
            streams[i].vinf.flags &= ~0x1000;
            orig_flags = streams[i].vinf.flags;
        }

        if ((orig_flags & 0x20) && (orig_flags & 0x4000))
        {
            streams[i].vinf.flags &= ~0x20;
            U32 end = streams[i].dest_a * 2;
            end += streams[i].info.dsp.num_adpcm_nibbles % 0xBB80;
            AXSetVoiceLoopAddr(streams[i].vinf.voice, zero_point);
            AXSetVoiceEndAddr(streams[i].vinf.voice, end - 1);
            AXSetVoiceLoop(streams[i].vinf.voice, 0);
            AXSetVoiceType(streams[i].vinf.voice, 0);
            streams[i].vinf.flags |= 0x40;
            orig_flags = streams[i].vinf.flags;
        }

        if (orig_flags & 0x100000)
        {
            AXFreeVoice(streams[i].vinf.voice);
            streams[i].vinf.voice = NULL;
            streams[i].vinf.flags = 0;
            streams[i].vinf.aid = 0;
            continue;
        }

        if (addr >= zero_point && addr < zero_end && (orig_flags & 0x40))
        {
            streams[i].vinf.flags &= ~0x40;
            DVDCancelAsync(&streams[i].fileInfo.cb, NULL);
            ARQRemoveRequest(&streams[i].request);
            AXSetVoiceState(streams[i].vinf.voice, 0);
            MIXReleaseChannel(streams[i].vinf.voice);

            if (streams[i].vinf.flags & 0x10000)
            {
                u32 priority = gsnd_voice->priority;
                if (priority > 0xff)
                {
                    priority = 0xff;
                }

                AXFreeVoice(streams[i].vinf.voice);
                streams[i].vinf.voice = AXAcquireVoice(priority >> 3, dv_callback, i);
                if (streams[i].vinf.voice == NULL)
                {
                    streams[i].vinf.voice = NULL;
                    streams[i].vinf.flags = 0;
                    streams[i].vinf.aid = 0;
                    continue;
                }

                MIXInitChannel(streams[i].vinf.voice, 0xc, 0, -0x384, -0x384, 0x40, 0x7f, -0x384);
                streams[i].vinf.flags = 0x4;
                iSndPlay(gsnd_voice);
                continue;
            }
            streams[i].vinf.flags = 0x40000;
            continue;
        }

        if (orig_flags & 0x8)
        {
            if ((streams[i].vinf.flags & 0x2) == 0)
            {
                AXSetVoiceState(streams[i].vinf.voice, 1);
            }
            MIXUnMute(streams[i].vinf.voice);
            need_update = TRUE;
            streams[i].vinf.flags |= 0x10;
            streams[i].vinf.flags |= 0x4000;
            streams[i].vinf.flags &= ~0x8;
            orig_flags = streams[i].vinf.flags;
        }

        if ((orig_flags & 0x200000) && (orig_flags & 0x200) == 0 && (orig_flags & 0x400) == 0 &&
            (orig_flags & 0x100) == 0 && (orig_flags & 0x400000) == 0)
        {
            streams[i].vinf.flags &= ~0x200000;
            streams[i].vinf.flags |= 0x100;
            streams[i].vinf.flags |= 0x400000;

            U32 size = streams[i].x8c - streams[i].x90;
            if (size > 0x2EE0)
            {
                DVDReadAsyncPrio(&streams[i].fileInfo, (void*)streams[i].source_a, 0x2EE0,
                                 streams[i].offset, dvdcb, 2);
            }
            else
            {
                DVDReadAsyncPrio(&streams[i].fileInfo, (void*)streams[i].source_a, size,
                                 streams[i].offset, dvdcb, 2);
            }
        }

        if (orig_flags & 0x800)
        {
            U32 length = 0x2EE0;
            if (streams[i].x90 % 0x2EE0)
            {
                length = streams[i].x90 % 0x2EE0;
            }

            U32 flags_bit_12 = orig_flags & 0x1000;
            if (flags_bit_12 && (orig_flags & 0x4000) && (orig_flags & 0x400) == 0 &&
                (orig_flags & 0x2000) == 0)
            {
                streams[i].vinf.flags |= 0x400;
                ARQPostRequest(&streams[i].request, (u32)&streams[i], 0, 1, streams[i].source_a,
                               streams[i].dest_b, length, arqcb);
            }
            else if ((orig_flags & 0x2000) && (orig_flags & 0x8000) && (orig_flags & 0x200) == 0 &&
                     !flags_bit_12)
            {
                streams[i].vinf.flags |= 0x200;
                ARQPostRequest(&streams[i].request, (u32)&streams[i], 0, 1, streams[i].source_a,
                               streams[i].dest_a, length, arqcb);
            }
        }

        if (streams[i].vinf.x10 == streams[i].vinf.x14 &&
            streams[i].vinf.x18 == streams[i].vinf.x1c)
        {
            continue;
        }

        streams[i].vinf.x14 = streams[i].vinf.x10;
        streams[i].vinf.x1c = streams[i].vinf.x18;
        MIXAdjustInput(streams[i].vinf.voice,
                       streams[i].vinf.x1c - MIXGetInput(streams[i].vinf.voice));
        MIXAdjustPan(streams[i].vinf.voice, streams[i].vinf.x1c - MIXGetPan(streams[i].vinf.voice));
        need_update = TRUE;
    }

    for (i = 0; i < 58; i++)
    {
        if (voices[i].voice == NULL || (voices[i].flags & 0x1))
        {
            continue;
        }

        U32 addr = (voices[i].voice->pb.addr.currentAddressHi << 16) +
                   voices[i].voice->pb.addr.currentAddressLo;
        if ((voices[i].flags & 0x4) && !voices[i].voice->pb.addr.loopFlag && addr >= zero_point &&
            addr < zero_end)
        {
            voices[i].flags &= ~0x4;
            AXSetVoiceState(voices[i].voice, 0);
            MIXReleaseChannel(voices[i].voice);
            AXFreeVoice(voices[i].voice);
            voices[i].voice = NULL;
            voices[i].flags = 0;
            voices[i].aid = 0;
            continue;
        }

        if (voices[i].x10 == voices[i].x14 && voices[i].x18 == voices[i].x1c)
        {
            continue;
        }

        voices[i].x14 = voices[i].x10;
        voices[i].x1c = voices[i].x18;
        MIXAdjustInput(voices[i].voice, voices[i].x1c - MIXGetInput(voices[i].voice));
        MIXAdjustPan(voices[i].voice, voices[i].x1c - MIXGetPan(voices[i].voice));
        need_update = TRUE;
    }

    if (need_update)
    {
        MIXUpdateSettings();
    }
}

void iSndInit()
{
    S32 i;
    AIInit(NULL);
    ARInit(aram_array, 40);
    ARQInit();
    AXInit();
    MIXInit();

    for (i = 0; i < 58; i++)
    {
        voices[i].voice = NULL;
        voices[i].flags = 0;
    }

    for (i = 0; i < 6; i++)
    {
        streams[i].vinf.voice = NULL;
        streams[i].vinf.flags = 0;
        streams[i].source_a = 0;
        streams[i].dest_a = 0;
        streams[i].dest_b = 0;
    }

    ua_stream_buffer = (U32*)malloc(0x11A40);
    stream_buffer = (U32*)(((U32)ua_stream_buffer + 0x1f) & ~0x1f);
    U32* local_buffer = stream_buffer;
    for (i = 0; i < 0x3A98; i++)
    {
        local_buffer[i] = 0;
    }

    silence_buffer = ARAlloc(0xEA60);
    zero_point = silence_buffer;
    zero_point *= 2;
    zero_point += 2;
    zero_end = zero_point + 0x1D4C0;

    SoundFlags |= 0x1;

    BOOL enabled = OSDisableInterrupts();
    DCInvalidateRange((void*)silence_buffer, 0xEA60);
    ARQRequest request;
    ARQPostRequest(&request, 0, 0, 1, (u32)local_buffer, silence_buffer, 0xEA60, arq_callback);
    OSRestoreInterrupts(enabled);

    while (SoundFlags != 0)
        ;

    for (i = 0; i < 6; i++)
    {
        u32 buf = ARAlloc(0x5DC0);
        streams[i].dest_a = buf;
        streams[i].dest_b = buf + 0x2EE0;
        streams[i].source_a = (u32)stream_buffer + i * 0x2EE0;
    }
    AXRegisterCallback(fcb);
}


void iSndExit()
{
    AXQuit();
}

void iSndInitSceneLoaded()
{
}

U32 iSndIsPlaying(U32 assetID)
{
    if (assetID == 0)
    {
        return FALSE;
    }

    for (S32 i = 0; i < 6; i++)
    {
        if (gSnd.voice[i].assetID == assetID)
        {
            if (streams[i].vinf.flags & 0xC000 && (streams[i].vinf.flags & 0x82) == 0)
            {
                return TRUE;
            }
            return FALSE;
        }
    }

    for (S32 i = 0; i < 0x3a; i++)
    {
        if (gSnd.voice[i + 6].assetID == assetID)
        {
            if (voices[i].flags & 0x4 && (voices[i].flags & 0x8) == 0)
            {
                return TRUE;
            }
            return FALSE;
        }
    }
    return FALSE;
}

U32 iSndIsPlaying(U32 assetID, U32 parid)
{
    for (U32 i = 0; i < 0x40; i++)
    {
        if ((assetID == 0 || gSnd.voice[i].assetID == assetID) && gSnd.voice[i].parentID == parid)
        {
            if (iSndIsPlaying(gSnd.voice[i].assetID))
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

iSndFileInfo* iSndLookup(U32 id)
{
    static S32 strm_id = 1;
    static S32 snd_id = 0x1000;

    sound_stream = 0;
    if (id == 0)
    {
        return NULL;
    }

    for (S32 i = sinfo_array_max - 1; i >= 0; i--)
    {
        iSndBank* bank = sinfo_array[i].bank;
        iSndSoundInfo* sounds = bank->sounds;
        U32 j;

        for (j = 0; j < bank->num_sounds; j++)
        {
            if (id == sounds[j].assetID)
            {
                memcpy(&snd, &sounds[j], sizeof(iSndSoundInfo));
                memcpy(&snd.file, &sinfo_array[i].file, sizeof(tag_xFile));
                snd.ID = snd_id++;
                if (snd_id >= 0x7FFA)
                {
                    snd_id = 0x1000;
                }
                return &snd;
            }
        }

        for (; j < bank->num_streams + bank->num_sounds; j++)
        {
            if (id == sounds[j].assetID)
            {
                memcpy(&snd, &sounds[j], sizeof(iSndSoundInfo));
                memcpy(&snd.file, &sinfo_array[i].file, sizeof(tag_xFile));
                if (i > 1)
                {
                    snd.ID = snd_id++;
                    if (snd_id >= 0x7FFA)
                    {
                        snd_id = 0x1000;
                    }
                    sound_stream = 1;
                }
                else
                {
                    snd.ID = strm_id++;
                    if (strm_id >= 0xFFE)
                    {
                        strm_id = 1;
                    }
                }
                return &snd;
            }
        }
    }

    return NULL;
}

void iSndPause(U32 snd, U32 pause)
{
    S32 i;
    for (i = 0; i < 64; i++)
    {
        if (gSnd.voice[i].sndID == snd)
        {
            break;
        }
    }

    if (i < 6)
    {
        if (streams[i].vinf.voice != NULL)
        {
            if (pause)
            {
                AXSetVoiceState(streams[i].vinf.voice, 0);
                streams[i].vinf.flags |= 0x2;
            }
            else if (!fn_800443F8() && gTrcDisk[1] == TRC_DiskIdentified)
            {
                AXSetVoiceState(streams[i].vinf.voice, 1);
                streams[i].vinf.flags &= ~0x2;
            }
        }
    }
    else if (i < 64)
    {
        U32 idx = i - 6;
        if (voices[idx].voice != NULL)
        {
            if (pause)
            {
                AXSetVoiceState(voices[idx].voice, 0);
                voices[idx].flags |= 0x8;
            }
            else
            {
                AXSetVoiceState(voices[idx].voice, 1);
                voices[idx].flags &= ~0x8;
            }
        }
    }
}

void iSndStop(U32 snd)
{
    BOOL enabled = OSDisableInterrupts();
    S32 i;
    for (i = 0; i < 64; i++)
    {
        if (gSnd.voice[i].sndID == snd)
        {
            gSnd.voice[i].sndID = 0;
            gSnd.voice[i].flags = 0;
            break;
        }
    }

    if (i < 6)
    {
        U32 idx = i;
        if (streams[idx].vinf.voice != NULL)
        {
            DVDCancelAsync(&streams[idx].fileInfo.cb, NULL);
            ARQRemoveRequest(&streams[idx].request);
            AXSetVoiceState(streams[idx].vinf.voice, 0);
            MIXReleaseChannel(streams[idx].vinf.voice);
            streams[idx].vinf.flags = 0x40000;
        }
    }
    else if (i < 64)
    {
        U32 idx = i - 6;
        if (voices[idx].voice != NULL)
        {
            MIXReleaseChannel(voices[idx].voice);
            AXSetVoiceState(voices[idx].voice, 0);
            AXFreeVoice(voices[idx].voice);
            voices[idx].voice = NULL;
            voices[idx].flags = 0;
            voices[idx].aid = 0;
        }
    }

    OSRestoreInterrupts(enabled);
}

static void iSndCalcVol(xSndVoiceInfo* vp, vinfo* info)
{
    F32 vol = 1.0f;
    if (vp->flags & 0x400)
    {
        vol *= MAX(gSnd.streamvol.volL, gSnd.streamvol.volL);
    }
    else
    {
        vol *= MAX(gSnd.fxvol.volL, gSnd.fxvol.volR);
    }
    vol *= MAX(gSnd.mastervol.volL, gSnd.mastervol.volR);

    S32 fader = 200.0f * vol - 200.0f;
    if (fader <= -0xBE)
    {
        fader = -0x384;
    }

    info->x10 = fader;
    info->x14 = fader;
    info->x18 = 0x40;
    info->x1c = 0x40;
    MIXAdjustFader(info->voice, fader - MIXGetFader(info->voice));
    MIXAdjustPan(info->voice, 0x40 - MIXGetPan(info->voice));
}

static void iSndCalcVol3d(xSndVoiceInfo* vp, vinfo* info)
{
    xVec3 dir;
    dir.x = vp->pos.x - gSnd.pos.x;
    dir.y = vp->pos.y - gSnd.pos.y;
    dir.z = vp->pos.z - gSnd.pos.z;
    F32 dist2 = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
    xVec3Normalize(&dir, &dir);
    F32 pan = xVec3Dot(&dir, &gSnd.right);

    F32 radius2 = MIN(gSnd.radius2, vp->radius2);
    F32 vol;
    if (dist2 <= radius2)
    {
        vol = (radius2 - dist2) / radius2;
    }
    else
    {
        vol = 0.0f;
    }
    vol = sqrtf(vol);

    if (vp->flags & 0x400)
    {
        vol *= MAX(gSnd.streamvol.volL, gSnd.streamvol.volR);
    }
    else
    {
        vol *= MAX(gSnd.fxvol.volL, gSnd.fxvol.volR);
    }
    vol *= MAX(gSnd.mastervol.volL, gSnd.mastervol.volR);

    S32 ipan = (S32)(64.0f * pan) + 0x40;
    S32 fader = 200.0f * vol - 200.0f;
    if (fader <= -0xBE)
    {
        fader = -0x384;
    }
    if (ipan < 0)
    {
        ipan = 0;
    }
    else if (ipan > 0x7F)
    {
        ipan = 0x7F;
    }

    info->x10 = fader;
    info->x14 = fader;
    info->x18 = ipan;
    info->x1c = ipan;
    MIXAdjustFader(info->voice, fader - MIXGetFader(info->voice));
    MIXAdjustPan(info->voice, ipan - MIXGetPan(info->voice));
}

static void iSndVolUpdate(xSndVoiceInfo* vp, vinfo* info)
{
    MIXUnMute(info->voice);

    if ((vp->flags & 0x10) && (vp->flags & 0x8))
    {
        if (vp->parentPos != NULL)
        {
            vp->pos = *vp->parentPos;
        }
        else
        {
            _xEnt* ent = (_xEnt*)(vp->parentID & ~3);
            if (ent->baseFlags & 0x1)
            {
                vp->pos = *xEntGetPos(ent);
            }
            else
            {
                vp->flags &= 0xFFEF;
            }
        }
    }

    if (vp->flags & 0x8)
    {
        iSndCalcVol3d(vp, info);
    }
    else
    {
        iSndCalcVol(vp, info);
    }
}

static void iSndUpdateStreams(F32)
{
    U8 ided = gTrcDisk[1] == TRC_DiskIdentified;
    U32 suspend = fn_800443F8() | !ided;

    for (S32 i = 0; i < 6; i++)
    {
        if (streams[i].vinf.voice == NULL)
        {
            continue;
        }

        U32 flags = streams[i].vinf.flags;
        if (flags & 0x40000)
        {
            streams[i].vinf.flags = 0x80000;
        }
        else if (flags & 0x80000)
        {
            streams[i].vinf.flags = 0x100000;
        }
        else if (suspend)
        {
            if ((flags & 0x20000) == 0)
            {
                streams[i].vinf.flags = flags | 0x20000;
                AXSetVoiceState(streams[i].vinf.voice, 0);
            }
        }
        else if (flags & 0x20000)
        {
            streams[i].vinf.flags = flags & ~0x20000;
            if ((streams[i].vinf.flags & 0x2) == 0)
            {
                AXSetVoiceState(streams[i].vinf.voice, 1);
            }
        }
        else
        {
            iSndVolUpdate(&gSnd.voice[i], &streams[i].vinf);
        }
    }
}

void iSndUpdate(F32 dt)
{
    S32 i;
    BOOL enabled = OSDisableInterrupts();
    iSndUpdateStreams(dt);

    for (i = 0; i < 58; i++)
    {
        if (voices[i].voice != NULL)
        {
            xSndVoiceInfo* vp = &gSnd.voice[i + 6];
            iSndVolUpdate(vp, &voices[i]);
        }
    }

    MIXUpdateSettings();
    OSRestoreInterrupts(enabled);
}

U32 iSndFindFreeVoice(U32 priority, U32 flags)
{
    if (priority > 0xff)
    {
        priority = 0xff;
    }

    if (flags & 0x4)
    {
        for (S32 i = 0; i < 6; i++)
        {
            if (streams[i].vinf.voice == NULL)
            {
                BOOL enabled = OSDisableInterrupts();
                AXVPB* voice = AXAcquireVoice(priority >> 3, dv_callback, i);
                if (voice != NULL)
                {
                    AXSetVoiceState(voice, 0);
                    MIXInitChannel(voice, 0xc, 0, -0x384, -0x384, 0x40, 0x7f, -0x384);
                    streams[i].vinf.voice = voice;
                    streams[i].vinf.flags = 0;
                    streams[i].vinf.flags |= 0x4;
                    OSRestoreInterrupts(enabled);
                    return i;
                }
                OSRestoreInterrupts(enabled);
                return -2;
            }
        }
    }
    else
    {
        for (S32 i = 0; i < 58; i++)
        {
            if (voices[i].voice == NULL)
            {
                voices[i].flags = 0;
                voices[i].flags |= 0x1;
                return i + 6;
            }
        }

        S32 i;
        U32 lowest = 0xFFFFFFFF;
        S32 lowest_idx = 0;
        for (i = 6; i < 64; i++)
        {
            if (gSnd.voice[i].priority < lowest)
            {
                lowest = gSnd.voice[i].priority;
                lowest_idx = i;
            }
        }

        if (lowest < (priority >> 3))
        {
            BOOL enabled = OSDisableInterrupts();
            U32 idx = lowest_idx - 6;
            MIXReleaseChannel(voices[idx].voice);
            AXSetVoiceState(voices[idx].voice, 0);
            AXFreeVoice(voices[idx].voice);
            voices[idx].voice = NULL;
            voices[idx].flags = 0;
            voices[idx].flags |= 0x1;
            voices[idx].aid = 0;
            OSRestoreInterrupts(enabled);
            return lowest_idx;
        }
    }

    return -2;
}

static U32 iSndPlayStream(xSndVoiceInfo* vp)
{
    S32 i = vp - gSnd.voice;
    iSndStream* s = &streams[i];
    if (s->vinf.voice != NULL && (s->vinf.flags & 0x1))
    {
        s->offset = s->info.dsp.sa;
        s->x90 = 0;
        DVDFileInfo* fileInfo = &s->fileInfo;

        BOOL enabled = OSDisableInterrupts();
        s->vinf.flags |= 0x400000;
        DVDReadAsyncPrio(fileInfo, (void*)s->source_a, s->x8c > 0x2EE0 ? 0x2EE0 : s->x8c,
                         s->offset, dvdcb, 2);

        AXPBADDR addr;
        memcpy(&addr, &s->info.dsp.loop_flag, sizeof(AXPBADDR));
        addr.format = 0;
        addr.loopFlag = 1;
        U32 start = s->dest_a * 2 + 2;
        U32 end = s->dest_a * 2 + 0xBB7F;
        addr.loopAddressHi = start >> 16;
        addr.loopAddressLo = start;
        addr.currentAddressHi = start >> 16;
        addr.currentAddressLo = start;
        addr.endAddressHi = end >> 16;
        addr.endAddressLo = end;
        AXSetVoiceAddr(s->vinf.voice, &addr);
        AXSetVoiceAdpcm(s->vinf.voice, (AXPBADPCM*)s->info.dsp.coef);
        AXSetVoiceSrcType(s->vinf.voice, 1);
        AXSetVoiceType(s->vinf.voice, 1);

        F32 ratio = s->info.dsp.sample_rate / 32000.0f;
        s->vinf.voice->pb.src.ratioHi = ratio;
        s->vinf.voice->pb.src.ratioLo = 65536.0f * ratio;
        s->vinf.voice->sync |= AX_SYNC_FLAG_COPYALL;

        streams[i].vinf.x14 = 0x7FFFFFFF;
        streams[i].vinf.x1c = 0x7FFFFFFF;
        iSndVolUpdate(vp, &s->vinf);

        OSRestoreInterrupts(enabled);
        return vp->sndID;
    }

    return 0;
}

static U32 iSndPlaySound(xSndVoiceInfo* vp)
{
    if (vp->flags & 0x200)
    {
        for (S32 j = 0; j < 64; j++)
        {
            if (&gSnd.voice[j] != vp && gSnd.voice[j].assetID == vp->assetID &&
                gSnd.voice[j].parentID == vp->parentID && (gSnd.voice[j].flags & 0x1))
            {
                return 0;
            }
        }
    }

    if (snd.info.assetID != vp->assetID)
    {
        iSndLookup(vp->assetID);
        if (snd.info.assetID != vp->assetID)
        {
            return 0;
        }
    }

    if (sound_stream)
    {
        vp->flags |= 0x400;
        sound_stream = 0;
    }

    U32 priority = vp->priority;
    S32 i = vp - gSnd.voice;
    if (priority >= 0xff)
    {
        priority = 0xff;
    }

    U32 idx = i - 6;
    if (voices[idx].voice == NULL)
    {
        voices[idx].aid = vp->assetID;
        if (voices[idx].aid == 0x84838AEC)
        {
            vp->flags |= 0x400;
        }

        voices[idx].xc = 1000.0f * snd.info.dsp.num_samples;
        voices[idx].xc /= snd.info.dsp.sample_rate;

        AXPBADDR addr;
        memcpy(&addr, &snd.info.dsp.loop_flag, sizeof(AXPBADDR));
        if (snd.info.dsp.loop_flag == 0)
        {
            addr.loopAddressHi = zero_point >> 16;
            addr.loopAddressLo = zero_point;
        }

        voices[idx].voice = AXAcquireVoice(priority >> 3, dv_callback, i);
        if (voices[idx].voice == NULL)
        {
            return 0;
        }

        BOOL enabled = OSDisableInterrupts();
        AXSetVoiceAddr(voices[idx].voice, &addr);
        AXSetVoiceType(voices[idx].voice, 0);
        AXSetVoiceAdpcm(voices[idx].voice, (AXPBADPCM*)snd.info.dsp.coef);
        AXSetVoiceSrcType(voices[idx].voice, 1);
        MIXInitChannel(voices[idx].voice, 4, 0, -0x388, -0x388, 0x40, 0x7f, -0x388);
        iSndVolUpdate(vp, &voices[idx]);

        F32 ratio = snd.info.dsp.sample_rate / 32000.0f;
        voices[idx].voice->pb.src.ratioHi = ratio;
        voices[idx].voice->pb.src.ratioLo = 65536.0 * ratio;
        voices[idx].voice->sync |= AX_SYNC_FLAG_COPYALL;
        MIXUnMute(voices[idx].voice);
        MIXUpdateSettings();
        AXSetVoiceState(voices[idx].voice, 1);
        OSRestoreInterrupts(enabled);

        voices[idx].flags &= ~0x1;
        voices[idx].flags |= 0x4;
        return vp->sndID;
    }

    return 0;
}

static U32 iSndPrepStream(xSndVoiceInfo* vp)
{
    S32 i = vp - gSnd.voice;
    iSndStream* s = &streams[i];
    if (snd.info.assetID != vp->assetID)
    {
        iSndLookup(vp->assetID);
        if (snd.info.assetID != vp->assetID)
        {
            return 64;
        }
    }

    memcpy(&s->info, &snd, sizeof(iSndSoundInfo));
    memcpy(&s->fileInfo, &snd.file.ps.fileInfo, sizeof(DVDFileInfo));
    s->fileInfo.cb.userData = s;
    s->x8c = (((s->info.dsp.num_adpcm_nibbles + 1) >> 1) + 0x1F) & ~0x1F;
    s->vinf.flags |= 0x1;
    s->vinf.aid = snd.info.assetID;
    if (s->info.dsp.loop_flag != 0 || (vp->flags & 0x100))
    {
        s->vinf.flags |= 0x10000;
    }
    return i;
}

U32 iSndPlay(xSndVoiceInfo* vp)
{
    S32 i = vp - gSnd.voice;
    if (i < 0 || i >= 64)
    {
        return 0;
    }

    if (i < 6)
    {
        U32 ret = iSndPrepStream(vp);
        if (ret < 0x3A)
        {
            return iSndPlayStream(vp);
        }
        return ret;
    }

    return iSndPlaySound(vp);
}

U32 iSndGetStereo()
{
    return OSGetSoundMode() == OS_SOUND_MODE_STEREO;
}

void iSndStereo(U32 stereo)
{
    if (stereo == 0)
    {
        OSSetSoundMode(OS_SOUND_MODE_MONO);
        gSnd.stereo = 0;
    }
    else
    {
        OSSetSoundMode(OS_SOUND_MODE_STEREO);
        gSnd.stereo = 1;
    }
}

void iSndWaitForDeadSounds()
{
    fc = 0;
    while (fc < 0x8C)
    {
        S32 start = fc;
        while (fc < start + 0xE)
            ;
        iSndUpdate(0.07f);
    }
}

void iSndSuspendCD(U32)
{
}

void iSndSceneExit()
{
    BOOL enabled = OSDisableInterrupts();
    S32 done = FALSE;
    ARQFlushQueue();
    DVDCancelAll();

    S32 i;
    for (i = 0; i < 64; i++)
    {
        if (gSnd.voice[i].sndID != 0)
        {
            iSndStop(gSnd.voice[i].sndID);
        }
    }
    for (i = 0; i < 64; i++)
    {
        if (gSnd.voice[i].sndID != 0)
        {
            iSndStop(gSnd.voice[i].sndID);
        }
    }
    OSRestoreInterrupts(enabled);

    fc = 0;
    while (!done && fc < 400)
    {
        done = TRUE;
        S32 start = fc;
        while (fc < start + 0xE)
            ;
        iSndUpdate(0.07f);

        for (i = 0; i < 6; i++)
        {
            if (streams[i].vinf.voice != NULL)
            {
                done = FALSE;
            }
        }
        for (i = 0; i < 58; i++)
        {
            if (voices[i].voice != NULL)
            {
                done = FALSE;
            }
        }
    }

    u32 length;
    ARFree(&length);
    sinfo_array_max--;
    free(sinfo_array[sinfo_array_max].bank);
}

static void sndloadcb(tag_xFile*)
{
    SoundFlags = 0;
}

void iSndLoadSounds(void* data)
{
    iSndBank* bank = (iSndBank*)data;
    U32 total = 0;
    U32 lowest = 0xFFFFFFFF;
    U32 highest = 0;

    if (bank->num_sounds == 0 && bank->num_streams == 0)
    {
        return;
    }

    if (sinfo_array_max >= 12)
    {
        exit(-1);
    }

    iSndSoundInfo* sounds = bank->sounds;
    sinfo_array[sinfo_array_max].bank = bank;
    char* name = xST_xAssetID_HIPFullPath(bank->sounds[0].assetID);
    tag_xFile* file = &sinfo_array[sinfo_array_max].file;
    iFileOpen(name, 0, file);
    iFileSeek(file, 0, IFILE_SEEK_SET);

    U32 i;
    for (i = 0; i < bank->num_sounds; i++)
    {
        st_xST_XASSETINFO ainfo;
        xSTGetAssetInfo(sounds[i].assetID, &ainfo);
        U32 offset = ainfo.plus_offset + ((ainfo.sector - file->ps.fileInfo.startAddr) << 5);
        U32 end = ainfo.size + offset;
        if (lowest > offset)
        {
            lowest = offset;
        }
        if (highest < end)
        {
            highest = end;
        }
        total += ainfo.size;
        sounds[i].dsp.sa += offset * 2;
        sounds[i].dsp.ea += offset * 2;
        sounds[i].dsp.ca += offset * 2;
    }

    for (i = 0; i < bank->num_streams; i++)
    {
        st_xST_XASSETINFO ainfo;
        xSTGetAssetInfo(sounds[i + bank->num_sounds].assetID, &ainfo);
        if (sinfo_array_max < 2)
        {
            sounds[i + bank->num_sounds].dsp.sa =
                ainfo.plus_offset + ((ainfo.sector - file->ps.fileInfo.startAddr) << 5);
        }
        else
        {
            U32 offset = ainfo.plus_offset + ((ainfo.sector - file->ps.fileInfo.startAddr) << 5);
            U32 end = ainfo.size + offset;
            if (lowest > offset)
            {
                lowest = offset;
            }
            if (highest < end)
            {
                highest = end;
            }
            total += ainfo.size;
            sounds[i + bank->num_sounds].dsp.sa += offset * 2;
            sounds[i + bank->num_sounds].dsp.ea += offset * 2;
            sounds[i + bank->num_sounds].dsp.ca += offset * 2;
        }
    }

    if (bank->num_sounds != 0 || (sinfo_array_max >= 2 && bank->num_streams != 0))
    {
        bank->size = total;
        U32 aram = ARAlloc(total);
        U32 delta = (aram - lowest) * 2;

        for (i = 0; i < bank->num_sounds; i++)
        {
            sounds[i].dsp.sa += delta;
            sounds[i].dsp.ea += delta;
            sounds[i].dsp.ca += delta;
        }

        if (sinfo_array_max >= 2)
        {
            for (i = 0; i < bank->num_streams; i++)
            {
                sounds[i + bank->num_sounds].dsp.sa += delta;
                sounds[i + bank->num_sounds].dsp.ea += delta;
                sounds[i + bank->num_sounds].dsp.ca += delta;
            }
        }

        sinfo_array[sinfo_array_max].base = lowest;
        for (U32 done = 0; done < total;)
        {
            U32 size = 0x11940;
            if (done + 0x11940 >= total)
            {
                size = total - done;
            }

            SoundFlags |= 0x1;
            U32 dest = aram + done;
            iFileSeek(file, lowest + done, IFILE_SEEK_SET);
            iFileReadAsync(file, stream_buffer, size, sndloadcb);
            while (SoundFlags != 0)
            {
                fn_80016600();
                fn_800443AC();
                if ((fc & 0xF) == 0)
                {
                    zGameScreenTransitionUpdate(0.5f, NULL, NULL);
                }
            }

            BOOL enabled = OSDisableInterrupts();
            DCStoreRange(stream_buffer, size);
            ARQRequest request;
            ARQPostRequest(&request, 0, 0, 1, (u32)stream_buffer, dest, size, arq_callback);
            SoundFlags |= 0x1;
            OSRestoreInterrupts(enabled);
            while (SoundFlags != 0)
                ;
            done += size;
        }

        while (SoundFlags != 0)
            ;
    }

    sinfo_array_max++;
}

void iSndDIEDIEDIE()
{
    S32 i;
    BOOL enabled = OSDisableInterrupts();

    for (i = 0; i < 6; i++)
    {
        if (streams[i].vinf.voice != NULL)
        {
            MIXReleaseChannel(streams[i].vinf.voice);
            AXSetVoiceState(streams[i].vinf.voice, 0);
            AXFreeVoice(streams[i].vinf.voice);
            streams[i].vinf.voice = NULL;
        }
    }

    for (i = 0; i < 58; i++)
    {
        if (voices[i].voice != NULL)
        {
            MIXReleaseChannel(voices[i].voice);
            AXSetVoiceState(voices[i].voice, 0);
            AXFreeVoice(voices[i].voice);
            voices[i].voice = NULL;
        }
    }

    AXQuit();
    OSRestoreInterrupts(enabled);
}
