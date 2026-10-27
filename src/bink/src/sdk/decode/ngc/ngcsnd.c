#include <dolphin/ar.h>
#include <dolphin/ax.h>
#include <dolphin/os.h>
#include <math.h>
#include "bink/binkngc.h"
#include "bink/binksnd.h"
#include "bink/radmem.h"

typedef struct NGCSND {
    S32 volume;
    F32 pan;
    S32 status;
    S32 paused;
    volatile S32 lockidx;
    U32 writepos;
    U32 wrapat;
    U32 minsize;
    U32 bufsize;
    U32 lowwater;
    U8* buf;
    U8* buf2;
    U32 lasttime;
    U32 timeout;
    ARQRequest tasks[4];
    AXVPB* voices[2];
    U32 aram;
    U32 shift;
} NGCSND;

#define P ((NGCSND*)snd->priv)

inline void MyAXSetVoiceCurrentAddr(NGCSND* p, U32 addr)
{
    AXSetVoiceCurrentAddr(p->voices[0], addr >> p->shift);
    if (p->voices[1])
        AXSetVoiceCurrentAddr(p->voices[1], (addr + p->bufsize) >> p->shift);
}

inline void MyAXSetVoiceLoopAddr(NGCSND* p, U32 addr)
{
    AXSetVoiceLoopAddr(p->voices[0], addr >> p->shift);
    if (p->voices[1])
        AXSetVoiceLoopAddr(p->voices[1], (addr + p->bufsize) >> p->shift);
}

inline void MyAXSetVoiceEndAddr(NGCSND* p, U32 addr)
{
    AXSetVoiceEndAddr(p->voices[0], (addr >> p->shift) - 1);
    if (p->voices[1])
        AXSetVoiceEndAddr(p->voices[1], ((addr + p->bufsize) >> p->shift) - 1);
}

inline void MyAXSetVoiceState(NGCSND* p, U16 state)
{
    if (p->voices[0])
        AXSetVoiceState(p->voices[0], state);
    if (p->voices[1])
        AXSetVoiceState(p->voices[1], state);
}

inline U32 get_play_pos(NGCSND* p)
{
    return *(U32*)&p->voices[0]->pb.addr.currentAddressHi << p->shift;
}

inline U32 get_end_pos(NGCSND* p)
{
    return *(U32*)&p->voices[0]->pb.addr.endAddressHi << p->shift;
}

inline ARQRequest* get_task(NGCSND* p, U32 ch, U32 idx)
{
    return &p->tasks[idx + ch * 2];
}

inline void ConvDataToStereo8(U32* src, U16* l, U16* r, U32 len)
{
    U32 i;
    U32 n;

    n = len >> 2;
    for (i = 0; i < n; i++)
    {
        U32 v = *src++;
        *l++ = ((v >> 16) & 0xFF00) | ((v >> 8) & 0xFF);
        *r++ = ((v >> 8) & 0xFF00) | (v & 0xFF);
    }

    n = (len - n * 4) >> 1;
    for (i = 0; i < n; i++)
    {
        U16 v = *(U16*)src;
        src = (U32*)((U16*)src + 1);
        *(U8*)l = v >> 8;
        l = (U16*)((U8*)l + 1);
        *(U8*)r = v;
        r = (U16*)((U8*)r + 1);
    }
}

inline void ConvDataToStereo16(U32* src, U32* l, U32* r, U32 len)
{
    U32 i;
    U32 n;

    n = len >> 3;
    for (i = 0; i < n; i++)
    {
        U32 a = src[0];
        U32 b = src[1];
        src += 2;
        *l++ = (a >> 16) | (b & 0xFFFF0000);
        *r++ = (a & 0xFFFF) | (b << 16);
    }

    n = (len - n * 8) >> 2;
    for (i = 0; i < n; i++)
    {
        U32 v = *src++;
        *(U16*)l = v >> 16;
        l = (U32*)((U16*)l + 1);
        *(U16*)r = v;
        r = (U32*)((U16*)r + 1);
    }
}

static void startVoices(U32 req)
{
    ARQRequest* task = (ARQRequest*)req;
    NGCSND* p = (NGCSND*)(task->owner & ~1);

    if (p && p->status == 0)
    {
        MyAXSetVoiceState(p, 1);
        p->status = 2;
    }
    task->owner &= ~1;
}

static void NGC_SoundPlay(BinkSnd* snd, U32 idx, U32 len)
{
    ARQRequest* task = get_task(P, 0, idx);
    U32 end = P->writepos + len;

    if (P->voices[1])
    {
        ARQRequest* task2 = get_task(P, 1, idx);
        DCFlushRange((void*)task2->source, len);
        ARQPostRequest(task2, 0, 0, 1, task2->source, P->writepos + P->bufsize, len, 0);
    }

    DCFlushRange((void*)task->source, len);
    ARQPostRequest(task, task->owner, 0, 1, task->source, P->writepos, len, startVoices);

    if (end > P->aram + P->bufsize - P->minsize)
    {
        if (get_play_pos(P) > end)
            P->wrapat = end;
        else
        {
            MyAXSetVoiceEndAddr(P, end);
            end = P->aram;
        }
    }

    P->writepos = end;
    P->lasttime = RADTimerRead();
}

static S32 NGC_SoundReinit(BinkSnd* snd)
{
    U32 i;

    MyAXSetVoiceState(P, 0);

    P->lockidx = -1;
    P->status = 0;
    P->writepos = P->aram;
    MyAXSetVoiceCurrentAddr(P, P->writepos);
    MyAXSetVoiceLoopAddr(P, P->writepos);
    MyAXSetVoiceEndAddr(P, P->writepos + P->bufsize);
    P->wrapat = 0;

    for (i = 0; i < 4; i++)
    {
        if (P->tasks[i].owner & 1)
            ARQRemoveRequest(&P->tasks[i]);
        P->tasks[i].owner = (U32)P;
    }

    return 1;
}

static void SetStreamVolumePan(AXVPB* v, S32 volume, S32 left, S32 right)
{
    AXPBMIX* mix = &v->pb.mix;

    memset(mix, 0, sizeof(*mix));
    mix->vL = left;
    mix->vR = right;
    v->pb.ve.currentVolume = volume;
    v->pb.ve.currentDelta = 0;
    v->pb.mixerCtrl = 3;
    v->sync |= AX_SYNC_FLAG_COPYVOL | AX_SYNC_FLAG_COPYAXPBMIX | AX_SYNC_FLAG_COPYMXRCTRL;
}

static void NGC_SoundVolume(BinkSnd* snd)
{
    S32 left = powf(1.0f - P->pan, 0.3f) * 65535.0f;
    S32 right = powf(P->pan, 0.3f) * 65535.0f;

    if (P->voices[0])
        SetStreamVolumePan(P->voices[0], P->volume, left, (snd->chans == 1) ? right : 0);
    if (P->voices[1])
        SetStreamVolumePan(P->voices[1], P->volume, 0, right);
}

static S32 NGC_SoundInit(BinkSnd* snd)
{
    U32 i;
    U32 rate;

    P->timeout = 720;
    P->lockidx = -1;
    P->status = 0;

    rate = (snd->freq * snd->bits) >> 3;
    P->minsize = ((rate / 25) + 31) & ~31;
    P->bufsize = ((rate * 800 / 1000) + 63) & ~63;
    P->lowwater = P->bufsize - P->bufsize * 90 / 100;
    snd->best_size = P->minsize * snd->chans;
    if (snd->bits != 16)
        snd->best_size *= 2;
    snd->size_mask = -(snd->chans * 32);

    P->buf = radmalloc(P->bufsize * snd->chans);
    if (P->buf == 0)
        return 0;

    if (snd->chans == 2)
    {
        P->buf2 = radmalloc(P->bufsize);
        if (P->buf2 == 0)
            return 0;
    }

    P->aram = (U32)radaudiomalloc(P->bufsize * snd->chans);
    if (P->aram == 0)
        return 0;

    P->shift = (snd->bits == 16);

    for (i = 0; i < 4; i++)
        P->tasks[i].owner = (U32)P;

    P->writepos = P->aram;
    P->wrapat = 0;

    for (i = 0; i < snd->chans; i++)
    {
        AXPBADDR addr;
        U32 start;
        U32 end;

        P->voices[i] = AXAcquireVoice(31, 0, 0);
        if (P->voices[i] == 0)
            return 0;

        start = (P->aram + i * P->bufsize) >> P->shift;
        end = ((P->aram + P->bufsize * (i + 1)) >> P->shift) - 1;

        addr.loopFlag = 1;
        addr.format = (snd->bits == 16) ? 10 : 25;
        addr.loopAddressHi = start >> 16;
        addr.loopAddressLo = start;
        addr.endAddressHi = end >> 16;
        addr.endAddressLo = end;
        addr.currentAddressHi = start >> 16;
        addr.currentAddressLo = start;
        AXSetVoiceAddr(P->voices[i], &addr);

        if (snd->freq == 32000)
            AXSetVoiceSrcType(P->voices[i], 0);
        else
        {
            AXPBSRC src;

            AXSetVoiceSrcType(P->voices[i], 1);
            src.ratioHi = 1;
            src.ratioLo = 0;
            src.currentAddressFrac = 0;
            src.last_samples[0] = 0;
            src.last_samples[1] = 0;
            src.last_samples[2] = 0;
            src.last_samples[3] = 0;
            AXSetVoiceSrc(P->voices[i], &src);
            AXSetVoiceSrcRatio(P->voices[i], (F32)snd->freq / 32000.0f);
        }
    }

    NGC_SoundVolume(snd);
    return 1;
}

static void NGC_SoundPause(BinkSnd* snd)
{
    MyAXSetVoiceState(P, 0);
}

static void NGC_SoundResume(BinkSnd* snd)
{
    MyAXSetVoiceState(P, 1);
}

static void NGC_SoundShutdown(BinkSnd* snd)
{
    U32 i;

    P->paused = 1;
    MyAXSetVoiceState(P, 0);

    for (i = 0; i < 2; i++)
    {
        if (P->voices[i])
        {
            AXFreeVoice(P->voices[i]);
            P->voices[i] = 0;
        }
    }

    if (P->buf)
    {
        radfree(P->buf);
        P->buf = 0;
    }

    if (P->buf2)
    {
        radfree(P->buf2);
        P->buf2 = 0;
    }

    if (P->aram)
    {
        radaudiofree((void*)P->aram);
        P->aram = 0;
    }
}

static S32 Lock(BinkSnd* snd, U8** addr, U32* len)
{
    if (P->lockidx >= 0)
    {
        U32 pos;
        U32 amt;
        U8* buf;

        pos = get_play_pos(P);
        if (P->writepos >= pos)
            amt = P->aram + P->bufsize - P->writepos;
        else
            amt = pos - P->writepos - 1;

        if (amt > P->bufsize / 2)
            amt = P->bufsize / 2;
        else if (amt < P->minsize)
            amt = P->minsize;
        amt *= snd->chans;

        buf = P->buf + P->lockidx * P->bufsize / 2;
        get_task(P, 0, P->lockidx)->source = (U32)buf;
        get_task(P, 1, P->lockidx)->source = (U32)(buf + P->bufsize);

        *addr = (snd->chans == 2) ? P->buf2 : buf;
        *len = amt;
        return 1;
    }
    return 0;
}

static S32 Unlock(BinkSnd* snd, U32 len)
{
    U32 i;

    if (P->lockidx == -1)
        return 0;

    get_task(P, 0, P->lockidx)->owner |= 1;

    if (snd->chans == 2)
    {
        U32* l = (U32*)get_task(P, 0, P->lockidx)->source;
        U32* r = (U32*)get_task(P, 1, P->lockidx)->source;
        U32* src = (U32*)P->buf2;

        if (snd->bits == 16)
            ConvDataToStereo16(src, l, r, len);
        else
            ConvDataToStereo8(src, (U16*)l, (U16*)r, len);
        len >>= 1;
    }

    if (len & 31)
    {
        U32 alen = (len + 31) & ~31;

        for (i = 0; i < snd->chans; i++)
            memset((U8*)get_task(P, i, P->lockidx)->source + len, 0, alen - len);
        len = alen;
    }

    if (P->status == 0)
    {
        get_task(P, 0, P->lockidx)->length = len;
        if (P->lockidx == 1)
        {
            NGC_SoundPlay(snd, 0, P->tasks[0].length);
            NGC_SoundPlay(snd, 1, P->tasks[1].length);
        }
    }
    else
        NGC_SoundPlay(snd, P->lockidx, len);

    return 1;
}

static void NGC_StarvedClear(BinkSnd* snd)
{
    S32 i;

    for (i = 0; i <= 99999; i++)
    {
        U32 idx = i & 1;

        if (!(get_task(P, 0, idx)->owner & 1))
        {
            U32 start = P->writepos;
            U32 end = start + P->minsize;
            U8* buf;

            i = idx;
            if (snd->chans == 2)
                buf = P->buf2;
            else
                buf = P->buf + i * P->bufsize / 2;

            memset(buf, 0, P->minsize);
            get_task(P, 0, idx)->source = (U32)buf;
            get_task(P, 1, idx)->source = (U32)buf;
            NGC_SoundPlay(snd, idx, P->minsize);

            if (get_play_pos(P) > end)
                MyAXSetVoiceCurrentAddr(P, start);
            MyAXSetVoiceLoopAddr(P, start);
            MyAXSetVoiceEndAddr(P, end);
            break;
        }
    }
}

static S32 Ready(BinkSnd* snd)
{
    S32 idx = -1;
    U32 now;
    U32 pos;

    if (P->paused || !snd->on || !P->voices[0])
        return 0;

    now = RADTimerRead();
    pos = get_play_pos(P);

    if (P->status == 2)
    {
        U32 end = get_end_pos(P);
        U32 avail = 0;

        if (pos < P->wrapat)
        {
            end = P->wrapat;
            MyAXSetVoiceEndAddr(P, end);
            P->wrapat = avail;
            P->writepos = P->aram;
        }

        if (now - P->lasttime < P->timeout)
        {
            if (pos < P->writepos)
                avail = P->writepos - pos;
            else
                avail = (end - pos) + (P->writepos - P->aram);
            if (avail >= P->lowwater)
                goto ok;
        }

        NGC_StarvedClear(snd);
        snd->starved = 1;
        P->status = 1;
    }

ok:
    if (P->wrapat == 0)
    {
        if (P->writepos >= pos || pos - P->writepos > P->minsize)
        {
            for (idx = 0; idx <= 1; idx++)
            {
                if (!(get_task(P, 0, idx)->owner & 1))
                    goto found;
            }
        }
        idx = -1;
    }

found:
    P->lasttime = now;
    P->lockidx = idx;
    return idx != -1;
}

static void Volume(BinkSnd* snd, S32 volume)
{
    if (volume < 0)
        volume = 0;
    if (volume > 32767)
        volume = 32767;
    P->volume = volume;
    NGC_SoundVolume(snd);
}

static void Pan(BinkSnd* snd, S32 pan)
{
    if (pan < 0)
        pan = 0;
    if (pan > 65536)
        pan = 65536;
    P->pan = (F32)pan * (1.0f / 65536.0f);
    NGC_SoundVolume(snd);
}

static S32 SetOnOff(BinkSnd* snd, S32 status)
{
    if (status == 1 && snd->on == 0)
    {
        if (NGC_SoundReinit(snd))
            snd->on = 1;
    }
    else if (status == 0 && snd->on == 1)
    {
        MyAXSetVoiceState(P, 0);
        snd->on = 0;
    }
    return snd->on;
}

static S32 Pause(BinkSnd* snd, S32 status)
{
    if (status)
    {
        NGC_SoundPause(snd);
        P->paused = 1;
    }
    else
    {
        NGC_SoundResume(snd);
        P->paused = 0;
    }
    return P->paused;
}

static void Close(BinkSnd* snd)
{
    NGC_SoundShutdown(snd);
}

static S32 Open(BinkSnd* snd, U32 freq, S32 bits, S32 chans, U32 flags, struct Bink* bink)
{
    memset(snd, 0, sizeof(*snd));
    snd->freq = freq;
    snd->bits = bits;
    snd->chans = chans;
    snd->starved = 0;
    P->paused = 0;
    P->volume = 32767;
    P->pan = 0.5f;
    snd->on = 1;
    snd->ready = Ready;
    snd->lock = Lock;
    snd->unlock = Unlock;
    snd->volume = Volume;
    snd->pan = Pan;
    snd->pause = Pause;
    snd->set_onoff = SetOnOff;
    snd->close = Close;

    if (!NGC_SoundInit(snd))
        return 0;
    snd->latency = 0;
    return 1;
}

BinkSndOpen BinkOpenNGCSound(U32 param)
{
    return Open;
}
