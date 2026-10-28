#include "bink/binkacd.h"
#include "bink/binkio.h"
#include "bink/binkngc.h"
#include "bink/binkread.h"
#include "bink/binksnd.h"
#include "bink/expand.h"
#include "bink/popmal.h"
#include "bink/radcb.h"
#include "bink/radmem.h"
#include "bink/yuv.h"

void* memmove(void* dest, const void* src, U32 n);

typedef struct BinkHdr {
    U32 marker;
    U32 size;
    U32 frames;
    U32 largest_frame;
    U32 num_offsets;
    U32 width;
    U32 height;
    U32 rate;
    U32 rate_div;
    U32 flags;
    U32 num_tracks;
} BinkHdr;

#define TRACKBITS(t) ((((t) >> 30) & 1) * 8 + 8)
#define TRACKCHANS(t) ((((t) >> 29) & 1) + 1)
#define TRACKFREQ(t) ((t) & 0xFFFF)

static U32 TrackNums[8] = {0};
extern U8 LogoData[];
static char binkerr[256];
static BinkSndSysOpen sysopen = 0;
static BinkSndOpen sndopen = 0;
static U32 numopensounds = 0;
static RADCBHANDLER* cb_bink_IO = 0;
static RADCBHANDLER* cb_bink_sound = 0;
static U32 ForceRate = -1;
static U32 IOBufferSize = -1;
static U32 Simulate = -1;
static U32 TotTracks = 1;
static BinkIOOpen UserOpen = 0;
static U32 ForceRateDiv;

static void* bpopmalloc(Bink* bink, U32 amt)
{
    bink->total_mem += popmalloctotal() + amt;
    return popmalloc(amt);
}

void BinkSetError(const char* err)
{
    strcpy(binkerr, err);
}

char* BinkGetError(void)
{
    return binkerr;
}

S32 BinkSetSoundSystem(BinkSndSysOpen open, U32 param)
{
    BinkSndOpen o;

    if (open == 0)
        return 0;

    if (sysopen == 0)
        sysopen = open;

    if (sysopen != open) {
        if (numopensounds)
            return 0;
        sysopen = open;
    }

    o = sysopen(param);
    if (o)
        sndopen = o;

    return sndopen ? 1 : 0;
}

static U8* conv16to8(U8* dest, S16* src, U32 bytes)
{
    U32 i;

    for (i = bytes >> 1; i; i--)
        *dest++ = (U8)(*src++ >> 8);

    return dest;
}

static U32 dosilence(BinkSnd* snd)
{
    U32 amt, ms, over;

    if (snd->buffered < snd->prime) {
        amt = snd->prime - snd->buffered;
        ms = (amt * 1000) / ((snd->freq * snd->bits * snd->chans) >> 3);
        snd->read_ptr -= amt;
        if (snd->read_ptr < snd->buf) {
            over = snd->buf - snd->read_ptr;
            snd->read_ptr += snd->buf_size;
            memset(snd->buf, 0, amt - over);
            memset(snd->read_ptr, 0, over);
        } else {
            memset(snd->read_ptr, 0, amt);
        }
        snd->buffered += amt;
        return ms;
    }
    return 0;
}

static void checksound(Bink* bink)
{
    U32 i;
    U8* addr;
    U32 len;
    U32 amt;

    if (bink->tracks[bink->num_opened - 1].ready == 0)
        return;

    if (RADCB_try_to_suspend_callback(cb_bink_sound, &bink->snd_cb)) {
        for (i = 0; i < bink->num_opened; i++) {
            while (bink->tracks[i].ready(&bink->tracks[i]) &&
                   ((bink->tracks[i].buffered > bink->tracks[i].best_size) ||
                    (((S32)bink->frame_num > bink->tracks[i].end_frame) && bink->tracks[i].buffered))) {
                if (!bink->tracks[i].lock(&bink->tracks[i], &addr, &len))
                    break;

                if (bink->tracks[i].convert8)
                    len += len;

                if (len > bink->tracks[i].buffered)
                    len = bink->tracks[i].buffered;

                if ((S32)bink->frame_num <= bink->tracks[i].end_frame)
                    len &= bink->tracks[i].size_mask << bink->tracks[i].convert8;

                bink->tracks[i].buffered -= len;

                amt = bink->tracks[i].buf_end - bink->tracks[i].read_ptr;
                if (amt < len) {
                    if (amt) {
                        if (bink->tracks[i].convert8) {
                            addr = conv16to8(addr, (S16*)bink->tracks[i].read_ptr, amt);
                        } else {
                            memcpy(addr, bink->tracks[i].read_ptr, amt);
                            addr += amt;
                        }
                    }
                    if (bink->tracks[i].convert8)
                        conv16to8(addr, (S16*)bink->tracks[i].buf, len - amt);
                    else
                        memcpy(addr, bink->tracks[i].buf, len - amt);
                    bink->tracks[i].read_ptr = bink->tracks[i].buf + (len - amt);
                } else {
                    if (bink->tracks[i].convert8)
                        conv16to8(addr, (S16*)bink->tracks[i].read_ptr, len);
                    else
                        memcpy(addr, bink->tracks[i].read_ptr, len);
                    bink->tracks[i].read_ptr += len;
                }

                if (bink->tracks[i].convert8)
                    len >>= 1;

                bink->tracks[i].unlock(&bink->tracks[i], len);
            }
        }
        RADCB_resume_callback(cb_bink_sound, &bink->snd_cb);
    }
}

static void inittimer(Bink* bink)
{
    if (bink->sync_start_time == 0) {
        bink->sync_start_time = RADTimerRead();
        bink->sync_start_frame = bink->played_frames - 1;
    }
}

static void GotoFrame(Bink* bink, U32 frame)
{
    if (frame)
        frame--;

    bink->unk300 = 0;

    if (bink->num_opened)
        checksound(bink);

    bink->last_frame_num = bink->frame_num;

    if (frame == 0) {
        if (bink->sound_on) {
            BinkSetSoundOnOff(bink, 0);
            BinkSetSoundOnOff(bink, 1);
        }
        bink->sync_start_time = 0;
        bink->sync_adjust = 0;
    }

    if (bink->preload) {
        bink->compframe = bink->preload + ((bink->frameoffsets[frame] & ~1) - (bink->frameoffsets[0] & ~1));
    } else {
        bink->io.read_frame(&bink->io, frame, bink->frameoffsets[frame] & ~1, bink->compframe,
                            (bink->frameoffsets[frame + 1] & ~1) - (bink->frameoffsets[frame] & ~1));
    }

    if (bink->num_opened)
        checksound(bink);

    bink->frame_num = frame + 1;
}

void BinkSetFrameRate(U32 rate, U32 div)
{
    ForceRate = rate;
    ForceRateDiv = div;
}

void BinkSetIOSize(U32 size)
{
    IOBufferSize = size;
}

void BinkSetIO(BinkIOOpen open)
{
    UserOpen = open;
}

void BinkSetSimulate(U32 sim)
{
    Simulate = sim;
}

void BinkSetSoundTrack(U32 total, U32* tracks)
{
    U32 i;

    if (total > 8)
        total = 8;

    TotTracks = total;
    for (i = 0; i < total; i++)
        TrackNums[i] = tracks[i];
}

static U32 high1secrate(U32 frames, U32* offsets, U32 fps, U32* highframe, S32* allkeys)
{
    S32 i;
    U32 max = 0;
    U32 maxi = 0;
    S32 keys = 1;
    U32 d;

    for (i = 0; i < (S32)(frames - fps); i++) {
        d = offsets[i + fps] - offsets[i];
        if (d > max) {
            max = d;
            maxi = i;
        }
    }

    if (max == 0)
        max = mult64anddiv(offsets[frames] - offsets[0], fps, frames);

    for (i = 0; i < (S32)frames; i++) {
        if ((offsets[i] & 1) == 0) {
            keys = 0;
            break;
        }
    }

    *allkeys = keys;
    *highframe = maxi;
    return max;
}

static void bink_suspend_io(BinkIO* io)
{
    RADCB_suspend_callback(cb_bink_IO, &io->cb);
}

static S32 bink_try_suspend_io(BinkIO* io)
{
    return RADCB_try_to_suspend_callback(cb_bink_IO, &io->cb);
}

static void bink_resume_io(BinkIO* io)
{
    RADCB_resume_callback(cb_bink_IO, &io->cb);
}

static void bink_idle_on_io(BinkIO* io)
{
    RADCB_idle_on_callbacks();
}

static S32 bink_get_priority_sound(RADCB* cb, U32 time)
{
    Bink* bink = (Bink*)((U8*)cb - 836);
    U32 last = bink->last_almost_empty;

    if (last != time)
        return ~last;
    return 0;
}

static S32 bink_sound_callback(RADCB* cb, U32 time)
{
    Bink* bink = (Bink*)((U8*)cb - 836);

    if (bink->num_opened)
        checksound(bink);
    bink->last_almost_empty = time;
}

Bink* BinkOpen(const char* name, U32 flags)
{
    Bink b;
    BinkHdr hdr;
    S32 allkeys;
    Bink* bink;
    BinkIOOpen open = BinkFileOpen;
    U32 sim;
    U32 i;
    S32 j;
    S32 type;
    U32 freq;
    U32 amt;

    if (!(flags & 0x4000)) {
        TotTracks = 1;
        TrackNums[0] = 0;
    }

    memset(&b, 0, sizeof(b));
    b.open_time = RADTimerRead();
    binkerr[0] = 0;

    if (flags & 0x4000000) {
        hdr = *(BinkHdr*)name;
    } else {
        if ((flags & 0x2000000) && UserOpen)
            open = UserOpen;
        UserOpen = 0;
        if (!open(&b.io, name, flags)) {
            if (binkerr[0] == 0)
                BinkSetError("Error opening file.");
            return 0;
        }
        b.io.read_header(&b.io, 0, &hdr, sizeof(hdr));
    }

    if ((hdr.marker != 0x664B4942) && (hdr.marker != 0x674B4942) && (hdr.marker != 0x684B4942) &&
        (hdr.marker != 0x694B4942)) {
        BinkSetError("Not a Bink file.");
    fail:
        if (!(flags & 0x4000000))
            b.io.close(&b.io);
        return 0;
    }

    if (hdr.frames == 0) {
        BinkSetError("The file doesn't contain any compressed frames yet.");
        goto fail;
    }

    b.width = hdr.width;
    b.height = hdr.height;
    b.decode_width = hdr.width;
    b.decode_height = hdr.height;
    b.header_flags = hdr.flags;
    b.uvwidth = (((hdr.width + 1) >> 1) + 7) & ~7;
    b.uvheight = (((hdr.height + 1) >> 1) + 7) & ~7;
    b.ywidth = b.uvwidth * 2;
    b.yheight = b.uvheight * 2;
    b.mask_pitch = b.ywidth >> 4;
    b.open_flags = (flags & 0x8FFFFFFF) | (hdr.flags & 0x20000);
    if (!(hdr.flags & 0x100000))
        b.open_flags &= ~0x100000;

    if ((flags & 0x70000000) != 0x70000000) {
        if (flags & 0x70000000)
            b.open_flags |= flags & 0x70000000;
        else
            b.open_flags |= hdr.flags & 0x70000000;

        switch (b.open_flags & 0x70000000) {
            case 0x10000000:
            case 0x20000000:
                b.height *= 2;
                break;
            case 0x30000000:
                b.width *= 2;
                break;
            case 0x40000000:
            case 0x50000000:
                b.width *= 2;
                b.height *= 2;
                break;
        }
    }

    if ((hdr.marker == 0x664B4942) || (hdr.marker == 0x674B4942))
        b.open_flags |= 0x18000;
    else if (hdr.marker == 0x684B4942)
        b.open_flags |= 0x8000;

    b.frames = hdr.frames;
    b.num_offsets = hdr.num_offsets;

    if ((flags & 0x1000) && (ForceRate != -1)) {
        b.frame_rate = ForceRate;
        b.frame_rate_div = ForceRateDiv;
        ForceRate = -1;
    } else {
        b.frame_rate = hdr.rate;
        b.frame_rate_div = hdr.rate_div;
    }

    b.size = hdr.size;
    b.num_tracks = hdr.num_tracks;
    b.largest_frame = hdr.largest_frame;
    b.file_rate = hdr.rate;
    b.file_rate_div = hdr.rate_div;
    b.fps = (hdr.rate + (hdr.rate_div >> 1)) / hdr.rate_div;
    b.mask_size = ((b.ywidth >> 4) * b.yheight) >> 4;
    if (b.fps == 0)
        b.fps = 1;
    b.hist_bytes = (b.fps - 1) * 4;

    pushmalloc((void**)&b.mask, b.mask_size + 16);
    pushmalloc((void**)&b.hist_start, b.fps * 4);
    pushmalloc((void**)&b.hist_audio, b.fps * 4);
    pushmalloc((void**)&b.hist_video, b.fps * 4);
    pushmalloc((void**)&b.hist_blit, b.fps * 4);
    pushmalloc((void**)&b.hist_fg, b.fps * 4);
    pushmalloc((void**)&b.hist_idle, b.fps * 4);
    pushmalloc((void**)&b.hist_bg, b.fps * 4);
    pushmalloc((void**)&b.tracks, (TotTracks ? TotTracks : 1) * sizeof(BinkSnd));
    pushmalloc((void**)&b.track_index, TotTracks * 4);

    ExpandBundleSizes(b.bundles, b.ywidth);
    pushmalloc(&b.bundles[0], (U32)b.bundles[0]);
    pushmalloc(&b.bundles[1], (U32)b.bundles[1]);
    pushmalloc(&b.bundles[2], (U32)b.bundles[2]);
    pushmalloc(&b.bundles[3], (U32)b.bundles[3]);
    pushmalloc(&b.bundles[4], (U32)b.bundles[4]);
    pushmalloc(&b.bundles[5], (U32)b.bundles[5]);
    pushmalloc(&b.bundles[6], (U32)b.bundles[6]);
    pushmalloc(&b.bundles[7], (U32)b.bundles[7]);
    pushmalloc(&b.bundles[8], (U32)b.bundles[8]);

    if (!(flags & 0x4000000)) {
        pushmalloc((void**)&b.frameoffsets, (b.num_offsets + 1) * 4);
        pushmalloc((void**)&b.track_max_size, b.num_tracks * 4);
        pushmalloc((void**)&b.track_types, b.num_tracks * 4);
        pushmalloc((void**)&b.track_ids, b.num_tracks * 4);
    }

    bink = bpopmalloc(&b, sizeof(Bink));
    if (bink == 0) {
    oom:
        BinkSetError("Out of memory.");
        goto fail;
    }

    *bink = b;
    bink->io.bink = bink;
    bink->hist_audio[0] = 0;
    bink->hist_video[0] = 0;
    bink->hist_blit[0] = 0;
    bink->hist_fg[0] = 0;
    bink->hist_idle[0] = 0;
    bink->hist_bg[0] = 0;

    if (flags & 0x4000000) {
        bink->track_max_size = (U32*)(name + 44);
        bink->track_types = (S32*)(name + 44 + bink->num_tracks * 4);
        bink->track_ids = (S32*)(name + 44 + bink->num_tracks * 8);
        bink->frameoffsets = (U32*)(name + 44 + bink->num_tracks * 12);
    } else {
        bink->io.read_header(&bink->io, -1, bink->track_max_size, bink->num_tracks * 4);
        bink->io.read_header(&bink->io, -1, bink->track_types, bink->num_tracks * 4);
        bink->io.read_header(&bink->io, -1, bink->track_ids, bink->num_tracks * 4);
        bink->io.read_header(&bink->io, -1, bink->frameoffsets, (bink->num_offsets + 1) * 4);
    }

    bink->high_rate = high1secrate(bink->frames, bink->frameoffsets, bink->fps, &bink->high_frame, &allkeys);

    if (bink->open_flags & 0x100000) {
        pushmalloc((void**)&bink->alpha[0], bink->ywidth * bink->yheight);
        if (!allkeys)
            pushmalloc((void**)&bink->alpha[1], bink->ywidth * bink->yheight);
    }
    if (!allkeys)
        pushmalloc((void**)&bink->planes[1], bink->ywidth * bink->yheight + bink->uvwidth * bink->uvheight * 2);

    bink->planes[0] = bpopmalloc(bink, bink->ywidth * bink->yheight + bink->uvwidth * bink->uvheight * 2);
    if (bink->planes[0] == 0) {
    freebink:
        radfree(bink);
        goto oom;
    }

    if (allkeys) {
        bink->planes[1] = bink->planes[0];
        bink->alpha[1] = bink->alpha[0];
    }

    if ((flags & 0x1000000) && (IOBufferSize != -1)) {
        bink->io_buf_size = IOBufferSize;
        IOBufferSize = -1;
    } else {
        bink->io_buf_size = bink->high_rate;
    }

    if ((flags & 0x400000) && (Simulate != -1)) {
        sim = Simulate;
        Simulate = -1;
    } else {
        sim = 0;
    }

    if (flags & 0x4000000) {
        bink->preload = (U8*)name + (bink->frameoffsets[0] & ~1);
    } else {
        bink->io_buf_size = bink->io.get_buffer_size(&bink->io, bink->io_buf_size);
        if (bink->io_buf_size >= (bink->size * 9) / 10) {
            flags |= 0x2000;
            bink->open_flags |= 0x2000;
        }

        if (flags & 0x2000) {
            amt = bink->size + 8 - (bink->frameoffsets[0] & ~1);
            bink->preload = bpopmalloc(bink, amt);
            if (bink->preload == 0) {
                radfree(b.planes[0]);
                goto freebink;
            }
            bink->io.set_info(&bink->io, 0, 0, bink->size + 8, sim);
            bink->io.read_frame(&bink->io, 0, bink->frameoffsets[0] & ~1, bink->preload, amt);
            bink->io.close(&bink->io);
            bink->io.fg_time = 0;
        } else {
            pushmalloc((void**)&bink->compframe, bink->largest_frame);
            bink->io_buf = bpopmalloc(bink, bink->io_buf_size);
            if (bink->io_buf == 0)
                bink->io_buf_size = 0;
            bink->io.set_info(&bink->io, bink->io_buf, bink->io_buf_size, bink->size + 8, sim);
        }
    }

    bink->mask[bink->mask_size] = 0;
    bink->frame_num = -1;

    if (bink->frame_rate)
        bink->resync = mult64anddiv(2000, bink->frame_rate_div, bink->frame_rate);
    else
        bink->resync = 2000;

    bink->video_on = 1;
    GotoFrame(bink, 1);

    bink->open_time = RADTimerRead() - bink->open_time;
    bink->tracks[0].speed = 0x10000;
    bink->num_opened = 0;

    if (bink->num_tracks && TotTracks) {
        for (i = 0; i < TotTracks; i++) {
            for (j = 0; j < (S32)bink->num_tracks; j++) {
                if (bink->track_ids[j] == TrackNums[i]) {
                    bink->track_index[bink->num_opened++] = j;
                    break;
                }
            }
        }
    }

    TrackNums[0] = 0;
    TotTracks = 1;

    i = 0;
    while (i < bink->num_opened) {
        type = bink->track_types[bink->track_index[i]];
        if ((type < 0) || (type & 0x10000000)) {
            if (sndopen == 0)
                BinkSetSoundSystem(BinkOpenNGCSound, 0);

            bink->tracks[i].buf = 0;
            if (sndopen) {
                freq = TRACKFREQ(bink->track_types[bink->track_index[i]]);
                if (b.frame_rate && b.frame_rate_div)
                    freq = (U32)(((F64)freq * (F64)b.frame_rate * (F64)b.file_rate_div) /
                                 ((F64)b.frame_rate_div * (F64)b.file_rate));

                if (sndopen(&bink->tracks[i], freq, TRACKBITS(bink->track_types[bink->track_index[i]]),
                            TRACKCHANS(bink->track_types[bink->track_index[i]]), bink->open_flags, bink)) {
                    if (bink->tracks[i].size_mask == 0)
                        bink->tracks[i].size_mask = -1;

                    bink->tracks[i].buf_size = (bink->track_max_size[bink->track_index[i]] + 255) & ~255;
                    bink->tracks[i].buf = bpopmalloc(bink, bink->tracks[i].buf_size);
                    if (bink->tracks[i].buf == 0) {
                        bink->tracks[i].close(&bink->tracks[i]);
                    } else {
                        numopensounds++;
                        bink->tracks[i].convert8 = (TRACKBITS(bink->track_types[bink->track_index[i]]) == 8);
                        bink->tracks[i].buf_end = bink->tracks[i].buf + bink->tracks[i].buf_size;
                        bink->tracks[i].write_ptr = bink->tracks[i].buf;
                        bink->tracks[i].read_ptr = bink->tracks[i].buf;
                        bink->tracks[i].prime =
                            mult64anddiv((freq << (TRACKCHANS(bink->track_types[bink->track_index[i]]) - 1)) * 2,
                                         750 - bink->tracks[i].latency, 1000) & ~3;
                        if (bink->tracks[i].prime > bink->tracks[i].buf_size)
                            bink->tracks[i].prime = bink->tracks[i].buf_size;
                        type = bink->track_types[bink->track_index[i]];
                        bink->tracks[i].decomp =
                            BinkAudioDecompressOpen(TRACKFREQ(type), TRACKCHANS(type),
                                                    ((type >= 0) && (type & 0x10000000)) ? 1 : 0);
                        bink->tracks[i].end_frame = bink->frames - (bink->file_rate * 3) / (bink->file_rate_div * 4);
                        bink->tracks[i].buffered = 0;
                    }
                }
            }
        }

        if (bink->tracks[i].speed == 0)
            bink->tracks[i].speed = 0x10000;

        if (bink->tracks[i].buf == 0) {
            bink->num_opened--;
            memcpy(&bink->track_index[i], &bink->track_index[i + 1], (bink->num_opened - i) * 4);
        } else {
            i++;
        }
    }

    if (bink->num_opened) {
        bink->sound_on = 1;
        if (cb_bink_sound == 0) {
            cb_bink_sound = RADCB_allocate_handler(20);
            RADCB_resume_handler(cb_bink_sound);
        }
        RADCB_register_callback(cb_bink_sound, &bink->snd_cb, bink_get_priority_sound, bink_sound_callback);
    }

    bink->io.resume = bink_resume_io;
    bink->io.suspend = bink_suspend_io;
    bink->io.try_suspend = bink_try_suspend_io;
    bink->io.idle_on = bink_idle_on_io;

    if ((bink->preload == 0) && !(flags & 0x200000)) {
        while (bink->io.idle(&bink->io))
            ;
    }

    return bink;
}

S32 BinkCopyToBuffer(Bink* bink, void* dest, S32 destpitch, U32 destheight, U32 destx, U32 desty, U32 flags)
{
    return BinkCopyToBufferRect(bink, dest, destpitch, destheight, destx, desty, 0, 0, bink->width, bink->height,
                                flags);
}

static inline void idle_io(Bink* bink)
{
    if ((bink->preload == 0) && (bink->io.reading == 0) &&
        ((((bink->io.cur_used + 1) * 100) / (bink->io.cur_size + 1)) < 75)) {
        if (RADCB_try_to_suspend_callback(cb_bink_IO, &bink->io.cb)) {
            bink->io.idle(&bink->io);
            RADCB_resume_callback(cb_bink_IO, &bink->io.cb);
        } else if (!(bink->open_flags & 0x8000000)) {
            RADCB_idle_on_callbacks();
        }
    }
}

S32 BinkCopyToBufferRect(Bink* bink, void* dest, S32 destpitch, U32 destheight, U32 destx, U32 desty, U32 srcx,
                         U32 srcy, U32 srcw, U32 srch, U32 flags)
{
    S32 skipped = 0;
    U32 now, target;

    if ((bink == 0) || (dest == 0))
        return 0;

    srcx = mult64anddiv(srcx, bink->decode_width, bink->width);
    srcw = mult64anddiv(srcw, bink->decode_width, bink->width);
    srcy = mult64anddiv(srcy, bink->decode_height, bink->height);
    srch = mult64anddiv(srch, bink->decode_height, bink->height);

    if (srcx >= bink->decode_width)
        srcx = bink->decode_width;
    if (srcy >= bink->decode_height)
        srcy = bink->decode_height;
    if (srcx + srcw > bink->decode_width)
        srcw = bink->decode_width - srcx;
    if (srcy + srch > bink->decode_height)
        srch = bink->decode_height - srcy;

    if ((srcw == 0) || (srch == 0) || bink->unk300)
        return bink->unk300;

    bink->io.busy = 1;
    flags |= bink->open_flags & 0x70030000;
    YUV_init(flags & 15);

    if (destpitch < 0) {
        dest = (U8*)dest + (-destpitch) * (destheight - desty - 1);
        desty = 0;
    }

    if (bink->num_opened && bink->frame_rate && (bink->paused == 0)) {
        inittimer(bink);
        now = RADTimerRead();
        target = mult64andshift(mult64anddiv((bink->played_frames - bink->sync_start_frame) * 1000,
                                             bink->frame_rate_div, bink->frame_rate),
                                bink->tracks[0].speed, 16);
        now -= bink->sync_start_time + bink->sync_adjust;
        if (now >= target) {
            now -= target;
            if (now > bink->resync) {
                if (++bink->skip_count > 3) {
                    bink->skip_count = 0;
                } else if ((flags & 0x80000) || (bink->open_flags & 0x80000)) {
                    skipped = 1;
                } else {
                    bink->io.busy = 0;
                    bink->skipped++;
                    bink->skip_blit = 1;
                    return 1;
                }
            } else {
                bink->skip_blit = 0;
                bink->skip_count = 0;
            }
        } else {
            bink->skip_blit = 0;
        }
    }

    idle_io(bink);

    bink->last_blit_flags = flags;

    if (flags & 0x80000000) {
    copyall:
        switch (flags & 15) {
            case 5:
                if (bink->alpha[0]) {
                    YUV_blit_32abpp(dest, destx, desty, destpitch, bink->planes[bink->plane_idx], srcx, srcy, srcw,
                                    srch, bink->ywidth, bink->yheight, bink->alpha[bink->plane_idx], flags);
                    break;
                }
            case 3:
                YUV_blit_32bpp(dest, destx, desty, destpitch, bink->planes[bink->plane_idx], srcx, srcy, srcw, srch,
                               bink->ywidth, bink->yheight, flags);
                break;
            case 7:
                if (bink->alpha[0]) {
                    YUV_blit_16a4bpp(dest, destx, desty, destpitch, bink->planes[bink->plane_idx], srcx, srcy, srcw,
                                     srch, bink->ywidth, bink->yheight, bink->alpha[bink->plane_idx], flags);
                    break;
                }
            case 9:
            case 10:
            case 11:
            case 12:
                YUV_blit_16bpp(dest, destx, desty, destpitch, bink->planes[bink->plane_idx], srcx, srcy, srcw, srch,
                               bink->ywidth, bink->yheight, flags);
                break;
            case 13:
                YUV_blit_YUY2(dest, destx, desty, destpitch, bink->planes[bink->plane_idx], srcx, srcy, srcw, srch,
                              bink->ywidth, bink->yheight, flags);
                break;
        }
    } else {
        if (strlen((char*)bink->mask) >= bink->mask_size)
            goto copyall;

        switch (flags & 15) {
            case 5:
                if (bink->alpha[0]) {
                    YUV_blit_32abpp_mask(dest, destx, desty, destpitch, bink->mask, bink->mask_pitch,
                                         bink->planes[bink->plane_idx], srcx, srcy, srcw, srch, bink->ywidth,
                                         bink->yheight, bink->alpha[bink->plane_idx], flags);
                    break;
                }
            case 3:
                YUV_blit_32bpp_mask(dest, destx, desty, destpitch, bink->mask, bink->mask_pitch,
                                    bink->planes[bink->plane_idx], srcx, srcy, srcw, srch, bink->ywidth, bink->yheight,
                                    flags);
                break;
            case 7:
                if (bink->alpha[0]) {
                    YUV_blit_16a4bpp_mask(dest, destx, desty, destpitch, bink->mask, bink->mask_pitch,
                                          bink->planes[bink->plane_idx], srcx, srcy, srcw, srch, bink->ywidth,
                                          bink->yheight, bink->alpha[bink->plane_idx], flags);
                    break;
                }
            case 9:
            case 10:
            case 11:
            case 12:
                YUV_blit_16bpp_mask(dest, destx, desty, destpitch, bink->mask, bink->mask_pitch,
                                    bink->planes[bink->plane_idx], srcx, srcy, srcw, srch, bink->ywidth, bink->yheight,
                                    flags);
                break;
            case 13:
                YUV_blit_YUY2_mask(dest, destx, desty, destpitch, bink->mask, bink->mask_pitch,
                                   bink->planes[bink->plane_idx], srcx, srcy, srcw, srch, bink->ywidth, bink->yheight,
                                   flags);
                break;
        }
    }

    bink->io.busy = 0;
    return skipped;
}

S32 BinkDoFrame(Bink* bink)
{
    S32 suspended = 0;
    U32* p;
    U32 size;
    U32* src;
    U32 left;
    S16* out;
    U32 outlen;
    U32 room;
    U32 t;
    U32 now;
    S32 i;
    S32 j;

    if ((bink == 0) || (bink->last_decomp == bink->frame_num))
        return 0;

    bink->unk300 = 0;
    bink->io.busy = 1;
    bink->frame_start = RADTimerRead();

    memmove(bink->hist_start + 1, bink->hist_start, bink->hist_bytes);
    memmove(bink->hist_video + 1, bink->hist_video, bink->hist_bytes);
    memmove(bink->hist_audio + 1, bink->hist_audio, bink->hist_bytes);
    memmove(bink->hist_blit + 1, bink->hist_blit, bink->hist_bytes);
    memmove(bink->hist_fg + 1, bink->hist_fg, bink->hist_bytes);
    memmove(bink->hist_idle + 1, bink->hist_idle, bink->hist_bytes);
    memmove(bink->hist_bg + 1, bink->hist_bg, bink->hist_bytes);
    bink->hist_start[0] = bink->frame_start;
    bink->hist_video[0] = bink->video_time;
    bink->hist_audio[0] = bink->audio_time;
    bink->hist_blit[0] = bink->blit_time;
    bink->hist_fg[0] = bink->io.fg_time;
    bink->hist_idle[0] = bink->io.idle_time;
    bink->hist_bg[0] = bink->io.bg_time;

    if (bink->start_time == 0) {
        bink->hist_start[1] = bink->frame_start - mult64anddiv(1000, bink->file_rate_div, bink->file_rate);
        bink->io.bg_time = 0;
        bink->io.idle_time = 0;
        bink->start_time = bink->frame_start;
    } else if (bink->num_opened) {
        checksound(bink);
    }

    bink->frame_t0 = bink->frame_start;

    p = (U32*)bink->compframe;

    if (bink->io.error)
        bink->read_error = 1;

    if (bink->read_error)
        return 1;

    for (i = 0; i < (S32)bink->num_tracks; i++) {
        size = *p++;
        for (j = 0; j < (S32)bink->num_opened; j++) {
            if (bink->track_index[j] == i)
                goto found;
        }
        j = -1;
    found:
        if ((j != -1) && size) {
            src = p;
            left = *src++;
            if (!suspended) {
                suspended = 1;
                RADCB_suspend_callback(cb_bink_sound, &bink->snd_cb);
            }
            while (left) {
                room = bink->tracks[j].buf_size - bink->tracks[j].buffered;
                BinkAudioDecompress(bink->tracks[j].decomp, &out, &outlen, src, (void**)&src);
                if (outlen > left)
                    outlen = left;
                left -= outlen;

                if (outlen > room) {
                    room = outlen - room;
                    bink->tracks[j].read_ptr += room;
                    bink->tracks[j].buffered -= room;
                    if (bink->tracks[j].read_ptr > bink->tracks[j].buf_end)
                        bink->tracks[j].read_ptr -= bink->tracks[j].buf_size;
                }

                bink->tracks[j].buffered += outlen;

                room = bink->tracks[j].buf_end - bink->tracks[j].write_ptr;
                if (room < outlen) {
                    if (room) {
                        memcpy(bink->tracks[j].write_ptr, out, room);
                        outlen -= room;
                        out = (S16*)((U8*)out + room);
                    }
                    memcpy(bink->tracks[j].buf, out, outlen);
                    bink->tracks[j].write_ptr = bink->tracks[j].buf + outlen;
                } else {
                    memcpy(bink->tracks[j].write_ptr, out, outlen);
                    bink->tracks[j].write_ptr += outlen;
                }
            }
        }
        p = (U32*)((U8*)p + size);
    }

    if (suspended)
        RADCB_resume_callback(cb_bink_sound, &bink->snd_cb);

    t = RADTimerRead();

    if (bink->video_on) {
        if (bink->skip_blit == 0)
            memset(bink->mask, 0, (bink->ywidth >> 4) * (bink->yheight >> 4));

        idle_io(bink);

        ExpandBink(bink->planes[bink->plane_idx ^ 1], bink->planes[bink->plane_idx], bink->alpha[bink->plane_idx ^ 1],
                   bink->alpha[bink->plane_idx], bink->mask, bink->decode_width, bink->decode_height, bink->ywidth,
                   bink->yheight, p, bink->frameoffsets[bink->frame_num - 1] & 1, bink->bundles, bink->open_flags,
                   bink->header_flags);
        bink->plane_idx ^= 1;
    }

    if (bink->num_opened)
        checksound(bink);

    bink->num_rects = -1;
    bink->played_frames++;
    inittimer(bink);

    now = RADTimerRead();
    bink->audio_time += t - bink->frame_start;
    bink->video_time += now - t;
    bink->last_decomp = bink->frame_num;
    bink->last_frame_num = bink->frame_num;
    bink->blit_start = now;
    bink->io.busy = 0;
    return 0;
}

static void timeframe(Bink* bink, U32 now)
{
    if (bink->blit_start) {
        bink->blit_time += now - bink->blit_start;
        bink->blit_start = 0;
    }
}

void BinkNextFrame(Bink* bink)
{
    S32 resync = 0;
    U32 i;

    if (bink) {
        bink->io.busy = 1;
        bink->io.busy = 0;

        for (i = 0; i < bink->num_opened; i++) {
            if (bink->tracks[i].starved) {
                bink->tracks[i].starved = 0;
                if ((bink->frame_num > 1) && ((S32)bink->frame_num <= bink->tracks[i].end_frame))
                    resync = 1;
            }
        }

        if (resync) {
            bink->sound_skips++;
            RADCB_suspend_2_callbacks(cb_bink_sound, &bink->snd_cb, cb_bink_IO, &bink->io.cb);
            BinkSetSoundOnOff(bink, 0);
            while (((((bink->io.cur_used + 1) * 100) / (bink->io.cur_size + 1)) < 90) && bink->io.idle(&bink->io))
                ;
            bink->sync_start_time = 0;
            bink->sync_adjust = 0;
            BinkSetSoundOnOff(bink, 1);
            RADCB_resume_callback(cb_bink_sound, &bink->snd_cb);
            RADCB_resume_callback(cb_bink_IO, &bink->io.cb);
        }

        bink->io.busy = 1;
        if (bink->num_opened)
            checksound(bink);
        timeframe(bink, RADTimerRead());
        GotoFrame(bink, (bink->frame_num < bink->frames) ? bink->frame_num + 1 : 1);
        bink->io.busy = 0;
    }
}

U32 BinkGetKeyFrame(Bink* bink, U32 frame, S32 flags)
{
    S32 i;
    U32 j;

    if (bink) {
        if (!(flags & 128) && (bink->frameoffsets[frame - 1] & 1))
            return frame;

        switch (flags & 127) {
            case 0:
                for (i = frame - 2; i > 0; i--) {
                    if (bink->frameoffsets[i] & 1)
                        break;
                }
                return i + 1;

            case 1:
                for (j = frame; j < bink->frames;) {
                    if (bink->frameoffsets[j++] & 1)
                        return j;
                }
                break;

            case 2:
                i = frame - 2;
                j = frame;
                for (;;) {
                    if (i >= 0) {
                        if (bink->frameoffsets[i] & 1)
                            return i + 1;
                        if (j < bink->frames) {
                            if (bink->frameoffsets[j++] & 1)
                                return j;
                        }
                    } else {
                        if (j >= bink->frames)
                            return 0;
                        if (bink->frameoffsets[j++] & 1)
                            return j;
                    }
                    i--;
                }
        }
    }

    return 0;
}

void BinkGoto(Bink* bink, U32 frame, S32 flags)
{
    U32 start;
    U32 key;
    S32 videooff;
    S32 soundwason;

    if (bink == 0)
        return;

    if (frame == 0)
        frame = 1;

    bink->io.busy = 1;

    if (frame > bink->frames)
        frame = bink->frames;

    if ((flags & 2) || (bink->num_opened == 0)) {
        start = frame;
    } else {
        start = (bink->frame_rate + (bink->frame_rate_div - 1)) / bink->frame_rate_div;
        if (start >= frame)
            start = 1;
        else
            start = frame - start;
    }

    if (bink->frame_num != frame) {
        if ((flags & 3) != 3) {
            if (!(flags & 1))
                key = BinkGetKeyFrame(bink, frame, 0);
            else
                key = frame;

            if (key > start) {
                videooff = bink->video_on;
                BinkSetVideoOnOff(bink, 0);
            } else {
                start = key;
                videooff = 0;
            }

            if ((frame < bink->frame_num) || (start > bink->frame_num)) {
                GotoFrame(bink, start);
                if (start == frame)
                    goto done;
            }

            RADCB_suspend_callback(cb_bink_sound, &bink->snd_cb);

            soundwason = bink->sound_on;
            if (soundwason)
                BinkSetSoundOnOff(bink, 0);

            if (bink->frame_num != bink->last_decomp) {
                BinkDoFrame(bink);
                bink->skip_blit = 1;
            }
            BinkNextFrame(bink);

            while (bink->frame_num != frame) {
                if (videooff && (bink->frame_num == key)) {
                    BinkSetVideoOnOff(bink, 1);
                    videooff = 0;
                }
                BinkDoFrame(bink);
                bink->skip_blit = 1;
                BinkNextFrame(bink);
            }

            bink->sync_start_time = 0;

            if (videooff)
                BinkSetVideoOnOff(bink, 1);

            RADCB_resume_callback(cb_bink_sound, &bink->snd_cb);

            if (soundwason)
                BinkSetSoundOnOff(bink, 1);
        } else {
            GotoFrame(bink, frame);
        }
    }

done:
    bink->io.busy = 0;
}

void BinkClose(Bink* bink)
{
    RADCBHANDLER* ioh;
    RADCBHANDLER* sndh;
    U32 i;

    if (bink == 0)
        return;

    BinkPause(bink, 1);

    ioh = ((bink->open_flags & 0x8000000) || bink->preload) ? 0 : cb_bink_IO;
    sndh = bink->num_opened ? cb_bink_sound : 0;

    switch (RADCB_unregister_2_callbacks(ioh, &bink->io.cb, sndh, &bink->snd_cb, 1)) {
        case 1:
            cb_bink_IO = 0;
            break;
        case 2:
            cb_bink_sound = 0;
            break;
        case 3:
            cb_bink_IO = 0;
            cb_bink_sound = 0;
            break;
    }

    for (i = 0; i < bink->num_opened; i++) {
        bink->tracks[i].close(&bink->tracks[i]);
        BinkAudioDecompressClose(bink->tracks[i].decomp);
        if (bink->tracks[i].buf)
            radfree(bink->tracks[i].buf);
    }

    if (bink->preload) {
        if (!(bink->open_flags & 0x4000000))
            radfree(bink->preload);
    } else {
        bink->io.close(&bink->io);
        radfree(bink->io_buf);
    }

    if (bink->planes[0])
        radfree(bink->planes[0]);

    memset(bink, 0, sizeof(Bink));
    radfree(bink);
}

static void endframe(Bink* bink, U32 now)
{
    if (bink->frame_t0) {
        now -= bink->frame_t0;
        bink->frame_t0 = 0;
        if (now > bink->slowest) {
            bink->slowest2 = bink->slowest;
            bink->slowest2_frame = bink->slowest_frame;
            bink->slowest = now;
            bink->slowest_frame = bink->frame_num;
        } else if (now > bink->slowest2) {
            bink->slowest2 = now;
            bink->slowest2_frame = bink->frame_num;
        }
    }
}

S32 BinkWait(Bink* bink)
{
    U32 start;
    U32 now;
    S32 target;
    S32 elapsed;

    if (bink == 0)
        return 0;

    if ((bink->played_frames == 0) && (bink->paused == 0))
        return 0;

    if (bink->read_error)
        return 0;

    start = bink->sync_start_time;
    if (start == 0) {
        inittimer(bink);
        start = bink->sync_start_time;
    }

    if (bink->num_opened)
        checksound(bink);

    now = RADTimerRead();
    timeframe(bink, now);
    endframe(bink, now);

    if (bink->paused)
        goto busy;

    if (bink->num_opened && (bink->sound_on == 0))
        goto busy;

    if (bink->frame_rate == 0)
        return 0;

    target = mult64andshift(mult64anddiv((bink->played_frames - bink->sync_start_frame) * 1000, bink->frame_rate_div,
                                         bink->frame_rate),
                            bink->tracks[0].speed, 16);
    elapsed = now - (start + bink->sync_adjust);
    if (elapsed < target)
        goto busy;

    elapsed -= target;
    if (elapsed > (S32)bink->resync) {
        if (bink->num_opened == 0) {
            bink->sync_start_time = now;
            bink->sync_start_frame = bink->played_frames - 1;
        } else {
            bink->sync_adjust = elapsed;
            bink->sync_step = mult64anddiv(elapsed, bink->frame_rate_div, bink->frame_rate);
        }
    }

    if (bink->sync_adjust < bink->sync_step)
        bink->sync_adjust = 0;
    else
        bink->sync_adjust -= bink->sync_step;

    return 0;

busy:
    if ((bink->preload == 0) && RADCB_try_to_suspend_callback(cb_bink_IO, &bink->io.cb)) {
        bink->io.idle(&bink->io);
        RADCB_resume_callback(cb_bink_IO, &bink->io.cb);
    }
    return 1;
}

S32 BinkPause(Bink* bink, S32 pause)
{
    U32 now;
    U32 i;

    if (bink == 0)
        return 0;

    now = RADTimerRead();
    timeframe(bink, now);
    endframe(bink, now);

    if ((pause == 0) && bink->paused) {
        bink->sync_start_time = 0;
        bink->sync_adjust = 0;
    }

    bink->paused = pause;

    for (i = 0; i < bink->num_opened; i++)
        bink->tracks[i].pause(&bink->tracks[i], pause);

    if (bink->played_frames && bink->num_opened)
        checksound(bink);

    return bink->paused;
}

void BinkGetSummary(Bink* bink, BinkSummary* sum)
{
    U32 now;
    U32 bytes;

    if ((bink == 0) || (sum == 0))
        return;

    now = RADTimerRead();
    timeframe(bink, now);
    endframe(bink, now);

    memset(sum, 0, sizeof(BinkSummary));

    sum->frame_rate = bink->frame_rate;
    sum->frame_rate_div = bink->frame_rate_div;
    sum->skipped_blits = bink->skipped;
    sum->sound_skips = bink->sound_skips;
    sum->file_rate = bink->file_rate;
    sum->file_rate_div = bink->file_rate_div;
    sum->frames = bink->frames;
    sum->played_frames = bink->played_frames;
    sum->total_time = RADTimerRead() - bink->start_time;
    sum->open_time = bink->open_time;
    sum->audio_time = bink->audio_time;
    sum->video_time = bink->video_time;
    sum->blit_time = bink->blit_time;
    sum->mem += bink->total_mem;
    sum->io_mem = bink->io_buf_size;
    sum->read_speed = mult64anddiv(bink->io.bytes_read, 1000, bink->io.total_time + 1);
    sum->read_time = bink->io.fg_time;
    sum->idle_time = bink->io.idle_time;
    sum->bg_time = bink->io.bg_time;
    bytes = bink->size - (bink->frameoffsets[0] & ~1);
    sum->data_rate = mult64anddiv(bytes, bink->file_rate, bink->file_rate_div * bink->frames);
    sum->frame_size = bytes / bink->frames;
    sum->high_rate = bink->high_rate;
    sum->high_frame = bink->high_frame + 1;
    sum->width = bink->width;
    sum->height = bink->height;
    sum->slowest = bink->slowest;
    sum->slowest2 = bink->slowest2;
    sum->slowest_frame = bink->slowest_frame;
    sum->slowest2_frame = bink->slowest2_frame;
    sum->io_mem = bink->io.buf_size;
    sum->io_peak = bink->io.buf_peak;
}

void BinkGetRealtime(Bink* bink, BinkRealtime* rt, U32 frames)
{
    U32 now;

    now = RADTimerRead();
    timeframe(bink, now);
    endframe(bink, now);

    if ((frames == 0) || (frames >= bink->fps))
        frames = bink->fps - 1;

    if (frames > bink->frame_num) {
        frames = bink->frame_num - 1;
        if (frames == 0)
            frames = 1;
    }

    rt->frame_num = bink->last_frame_num;
    rt->frame_rate = bink->frame_rate;
    rt->frame_rate_div = bink->frame_rate;
    rt->io_size = bink->io.cur_size;
    rt->io_used = bink->io.cur_used;
    rt->data_rate = mult64anddiv(bink->frameoffsets[bink->frame_num] - bink->frameoffsets[bink->frame_num - frames],
                                 bink->file_rate, frames * bink->file_rate_div);
    rt->frames = frames;
    rt->time = bink->hist_start[0] - bink->hist_start[frames];
    if (rt->time == 0)
        rt->time = 1;
    rt->video_time = bink->hist_video[0] - bink->hist_video[frames];
    rt->audio_time = bink->hist_audio[0] - bink->hist_audio[frames];
    rt->blit_time = bink->hist_blit[0] - bink->hist_blit[frames];
    rt->read_time = bink->hist_fg[0] - bink->hist_fg[frames];
    rt->idle_time = bink->hist_idle[0] - bink->hist_idle[frames];
    rt->bg_time = bink->hist_bg[0] - bink->hist_bg[frames];
}

static S32 smallestrect(BinkRect* out, U8* mask, S32 pitch, BinkRect* in)
{
    S32 x, y, w, h;
    U8* m;

    mask += in->x / 16;
    mask += (in->y / 16) * pitch;
    h = in->h / 16;
    w = in->w / 16;

    for (y = 0, m = mask; y < h; y++, m += pitch) {
        for (x = 0; x < w; x++) {
            if (m[x])
                goto top;
        }
    }
    return 0;

top:
    mask += pitch * y;
    h -= y;
    out->y = in->y + y * 16;
    out->h = in->h - y * 16;

    if (out->h > 1) {
        for (y = 0, m = mask + (h - 1) * pitch; y < h; y++, m -= pitch) {
            for (x = 0; x < w; x++) {
                if (m[x])
                    goto bottom;
            }
        }
    bottom:
        h -= y;
        out->h -= y * 16;
    }

    for (x = 0; x < w; x++) {
        for (y = 0, m = mask + x; y < h; y++, m += pitch) {
            if (*m)
                goto left;
        }
    }

left:
    w -= x;
    mask += x;
    out->x = in->x + x * 16;
    out->w = in->w - x * 16;

    if (out->w > 1) {
        w--;
        for (x = w; x >= 0; x--) {
            for (y = 0, m = mask + x; y < h; y++, m += pitch) {
                if (*m)
                    goto right;
            }
        }
    right:
        out->w -= (w - x) * 16;
    }

    return 1;
}

static S32 trysplit(BinkRect* outa, BinkRect* outb, BinkRect* in, U8* mask, S32 pitch)
{
    BinkRect va, vb;
    BinkRect t;
    S32 hs, vs;

    if (in->w >= 32) {
        t = *in;
        hs = ((t.w / 2) + 15) & ~15;
        t.w = hs;
        smallestrect(outa, mask, pitch, &t);
        t = *in;
        t.w -= hs;
        t.x += hs;
        smallestrect(outb, mask, pitch, &t);
        hs = in->w * in->h - outa->w * outa->h - outb->w * outb->h;
    } else {
        hs = 0;
    }

    if (in->h >= 32) {
        t = *in;
        vs = ((t.h / 2) + 15) & ~15;
        t.h = vs;
        smallestrect(&va, mask, pitch, &t);
        t = *in;
        t.h -= vs;
        t.y += vs;
        smallestrect(&vb, mask, pitch, &t);
        vs = in->w * in->h - va.w * va.h - vb.w * vb.h;
        if (vs > hs) {
            *outa = va;
            *outb = vb;
            return vs;
        }
    }

    return hs;
}

S32 BinkGetRects(Bink* bink, U32 flags)
{
    BinkRect r[8];
    BinkRect a[8];
    BinkRect b[8];
    S32 save[8];
    S32 i, k, best, max, sel;
    U32 key, min;

    if ((flags & 0x4000000) ||
        (!(flags & 0x80000) && !(bink->open_flags & 0x80000) && bink->skip_blit))
        return 0;

    if (bink->num_rects == -1) {
        r[0].y = 0;
        r[0].x = 0;
        r[0].w = bink->ywidth;
        r[0].h = bink->yheight;
        bink->num_rects = smallestrect(&bink->rects[0], bink->mask, bink->mask_pitch, &r[0]);

        if (!(flags & 0x8000000) && bink->num_rects) {
            r[0] = bink->rects[0];
            save[0] = trysplit(&a[0], &b[0], &r[0], bink->mask, bink->mask_pitch);

            for (;;) {
                best = -1;
                max = 0;
                for (i = 0; i < bink->num_rects; i++) {
                    if (save[i] > max) {
                        max = save[i];
                        best = i;
                    }
                }

                if (best == -1)
                    break;

                r[best] = a[best];
                r[bink->num_rects] = b[best];
                if (++bink->num_rects == 8)
                    break;

                save[best] = trysplit(&a[best], &b[best], &r[best], bink->mask, bink->mask_pitch);
                best = bink->num_rects - 1;
                save[best] = trysplit(&a[best], &b[best], &r[best], bink->mask, bink->mask_pitch);
            }
        }

        for (i = 0; i < bink->num_rects; i++) {
            min = 0x7FFF0000;
            sel = 0;
            for (k = 0; k < bink->num_rects; k++) {
                key = (r[k].y << 16) + r[k].x;
                if (key < min) {
                    min = key;
                    sel = k;
                }
            }

            switch (bink->last_blit_flags & 0x70000000) {
                case 0x10000000:
                case 0x20000000:
                    r[sel].y *= 2;
                    r[sel].h *= 2;
                    break;
                case 0x30000000:
                    r[sel].x *= 2;
                    r[sel].w *= 2;
                    break;
                case 0x40000000:
                case 0x50000000:
                    r[sel].x *= 2;
                    r[sel].w *= 2;
                    r[sel].y *= 2;
                    r[sel].h *= 2;
                    break;
            }

            if ((U32)(r[sel].x + r[sel].w) > bink->width)
                r[sel].w -= (r[sel].x + r[sel].w) - bink->width;
            if ((U32)(r[sel].y + r[sel].h) > bink->height)
                r[sel].h -= (r[sel].y + r[sel].h) - bink->height;

            bink->rects[i] = r[sel];
            r[sel].x = 0;
            r[sel].y = 0x7FFFFFFF;
        }
    }

    return bink->num_rects;
}

void BinkService(Bink* bink)
{
    if (bink->num_opened)
        checksound(bink);
}

static S32 idtoindex(Bink* bink, S32 id)
{
    S32 i;

    for (i = 0; i < (S32)bink->num_opened; i++) {
        if (bink->track_ids[bink->track_index[i]] == id)
            return i;
    }
    return -1;
}

void BinkSetVolume(Bink* bink, S32 id, S32 volume)
{
    S32 i;

    if (bink && bink->num_opened) {
        i = idtoindex(bink, id);
        if ((i != -1) && bink->tracks[i].volume)
            bink->tracks[i].volume(&bink->tracks[i], volume);
    }
}

void BinkSetMixBins(Bink* bink, S32 id, U32* bins, U32 total)
{
    S32 i;

    if (bink && bink->num_opened) {
        i = idtoindex(bink, id);
        if ((i != -1) && bink->tracks[i].mix_bins)
            bink->tracks[i].mix_bins(&bink->tracks[i], bins, total);
    }
}

void BinkSetMixBinVolumes(Bink* bink, S32 id, U32* bins, S32* vols, U32 total)
{
    S32 i;

    if (bink && bink->num_opened) {
        i = idtoindex(bink, id);
        if ((i != -1) && bink->tracks[i].mix_bin_vols)
            bink->tracks[i].mix_bin_vols(&bink->tracks[i], bins, vols, total);
    }
}

void BinkSetPan(Bink* bink, S32 id, S32 pan)
{
    S32 i;

    if (bink && bink->num_opened) {
        i = idtoindex(bink, id);
        if ((i != -1) && bink->tracks[i].pan)
            bink->tracks[i].pan(&bink->tracks[i], pan);
    }
}

void* BinkLogoAddress(void)
{
    return LogoData;
}

S32 BinkGetTrackType(Bink* bink, U32 idx)
{
    if (bink)
        return bink->track_types[idx];
    return 0;
}

U32 BinkGetTrackMaxSize(Bink* bink, U32 idx)
{
    if (bink)
        return bink->track_max_size[idx];
    return 0;
}

S32 BinkGetTrackID(Bink* bink, U32 idx)
{
    if (bink)
        return bink->track_ids[idx];
    return 0;
}

BinkTrack* BinkOpenTrack(Bink* bink, U32 idx)
{
    void* ac;
    BinkTrack* t;

    if (bink && (idx < bink->num_tracks) &&
        ((bink->track_types[idx] < 0) || (bink->track_types[idx] & 0x10000000))) {
        ac = BinkAudioDecompressOpen(TRACKFREQ(bink->track_types[idx]), TRACKCHANS(bink->track_types[idx]),
                                     ((bink->track_types[idx] >= 0) && (bink->track_types[idx] & 0x10000000)) ? 1 : 0);
        if (ac == 0)
            return 0;

        t = bpopmalloc(bink, sizeof(BinkTrack));
        if (t == 0) {
            BinkAudioDecompressClose(ac);
            return 0;
        }

        memset(t, 0, sizeof(BinkTrack));
        t->bink = bink;
        t->decomp = ac;
        t->freq = TRACKFREQ(bink->track_types[idx]);
        t->bits = TRACKBITS(bink->track_types[idx]);
        t->chans = TRACKCHANS(bink->track_types[idx]);
        t->max_size = (bink->track_max_size[idx] + 3) & ~3;
        if (t->bits == 8)
            t->max_size >>= 1;
        t->track_num = idx;
        return t;
    }
    return 0;
}

void BinkCloseTrack(BinkTrack* t)
{
    if (t) {
        if (t->decomp) {
            BinkAudioDecompressClose(t->decomp);
            t->decomp = 0;
        }
        radfree(t);
    }
}

U32 BinkGetTrackData(BinkTrack* t, void* dest)
{
    U32* p;
    S32 i;
    U32 size;
    U32 left;
    U32 tot;
    S16* out;
    U32 outlen;
    U32 k;

    if (t) {
        p = (U32*)t->bink->compframe;
        for (i = 0; i < (S32)t->bink->num_tracks; i++) {
            size = *p++;
            if ((i == t->track_num) && size) {
                left = *p++;
                tot = 0;
                while (left) {
                    BinkAudioDecompress(t->decomp, &out, &outlen, p, (void**)&p);
                    if (outlen > left)
                        outlen = left;
                    left -= outlen;
                    if (t->bits == 16) {
                        memcpy(dest, out, outlen);
                    } else {
                        U8* d;
                        S16* s;
                        S32 v;

                        outlen >>= 1;
                        d = dest;
                        s = out;
                        for (k = 0; k < outlen; k++) {
                            v = (*s++ >> 8) + 128;
                            *d++ = (U8)v;
                        }
                    }
                    dest = (U8*)dest + outlen;
                    tot += outlen;
                }
                return tot;
            }
            p = (U32*)((U8*)p + size);
        }
    }
    return 0;
}

S32 BinkSetVideoOnOff(Bink* bink, S32 onoff)
{
    if (bink)
        bink->video_on = onoff;
    return onoff;
}

S32 BinkSetSoundOnOff(Bink* bink, S32 onoff)
{
    S32 ret = 0;
    S32 suspended;
    U32 i;
    S32 r;

    if (bink) {
        suspended = 0;
        for (i = 0; i < bink->num_opened; i++) {
            if (bink->tracks[i].set_onoff) {
                if (!suspended) {
                    RADCB_suspend_callback(cb_bink_sound, &bink->snd_cb);
                    suspended = 1;
                }
                r = bink->tracks[i].set_onoff(&bink->tracks[i], onoff ? 1 : 0);
                if (r == 0) {
                    if (bink->sound_on) {
                        bink->tracks[i].write_ptr = bink->tracks[i].buf;
                        bink->tracks[i].read_ptr = bink->tracks[i].buf;
                        bink->tracks[i].buffered = 0;
                        dosilence(&bink->tracks[i]);
                        bink->sound_on = 0;
                    }
                } else {
                    if (bink->sound_on == 0) {
                        bink->sound_on = 1;
                        bink->sync_start_time = 0;
                        bink->sync_adjust = 0;
                    }
                    ret = 1;
                }
            }
        }
        if (suspended)
            RADCB_resume_callback(cb_bink_sound, &bink->snd_cb);
    }
    return ret;
}
