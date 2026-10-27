#ifndef BINK_BINKREAD_H
#define BINK_BINKREAD_H

#include "types.h"
#include "bink/binkio.h"
#include "bink/binksnd.h"
#include "bink/radcb.h"

typedef struct BinkRect {
    S32 x;
    S32 y;
    S32 w;
    S32 h;
} BinkRect;

struct Bink {
    U32 width;
    U32 height;
    U32 frames;
    U32 frame_num;
    U32 last_frame_num;
    U32 frame_rate;
    U32 frame_rate_div;
    U32 read_error;
    U32 open_flags;
    U32 header_flags;
    U32 size;
    U8 unk2C[8];
    BinkRect rects[8];
    S32 num_rects;
    U32 plane_idx;
    U8* planes[2];
    U8* alpha[2];
    U32 ywidth;
    U32 yheight;
    U32 uvwidth;
    U32 uvheight;
    U8* mask;
    U32 mask_pitch;
    U32 mask_size;
    U32 largest_frame;
    U32 num_offsets;
    U32 num_tracks;
    U32 high_rate;
    U32 high_frame;
    S32 paused;
    U32 unk100;
    U8* compframe;
    U8* preload;
    U32* frameoffsets;
    BinkIO io;
    U8* io_buf;
    U32 io_buf_size;
    U32 decode_width;
    U32 decode_height;
    S32* track_index;
    U32* track_max_size;
    S32* track_types;
    S32* track_ids;
    U32 unk26C;
    U32 played_frames;
    U32 start_time;
    U32 frame_start;
    U32 blit_start;
    U32 sync_start_time;
    U32 sync_start_frame;
    U32 resync;
    U32 frame_t0;
    U32 slowest;
    U32 slowest_frame;
    U32 slowest2;
    U32 slowest2_frame;
    S32 sound_on;
    S32 video_on;
    U32 total_mem;
    U32 video_time;
    U32 audio_time;
    U32 blit_time;
    U32 open_time;
    U32 file_rate;
    U32 file_rate_div;
    U32 fps;
    U32 hist_bytes;
    U32* hist_start;
    U32* hist_audio;
    U32* hist_video;
    U32* hist_blit;
    U32* hist_fg;
    U32* hist_idle;
    U32* hist_bg;
    U32 last_blit_flags;
    U32 last_decomp;
    U32 num_opened;
    U32 sound_skips;
    BinkSnd* tracks;
    S32 skip_blit;
    S32 unk300;
    U32 skipped;
    void* bundles[9];
    U32 skip_count;
    U32 sync_adjust;
    U32 sync_step;
    U8 unk338[8];
    U32 last_almost_empty;
    RADCB snd_cb;
    U8 unk360[36];
};

typedef struct BinkSummary {
    U32 width;
    U32 height;
    U32 total_time;
    U32 file_rate;
    U32 file_rate_div;
    U32 frame_rate;
    U32 frame_rate_div;
    U32 open_time;
    U32 frames;
    U32 played_frames;
    U32 skipped_frames;
    U32 skipped_blits;
    U32 sound_skips;
    U32 blit_time;
    U32 read_time;
    U32 video_time;
    U32 audio_time;
    U32 idle_time;
    U32 bg_time;
    U32 read_speed;
    U32 slowest;
    U32 slowest2;
    U32 slowest_frame;
    U32 slowest2_frame;
    U32 data_rate;
    U32 frame_size;
    U32 mem;
    U32 io_mem;
    U32 io_peak;
    U32 high_rate;
    U32 high_frame;
} BinkSummary;

typedef struct BinkRealtime {
    U32 frame_num;
    U32 frame_rate;
    U32 frame_rate_div;
    U32 frames;
    U32 time;
    U32 video_time;
    U32 audio_time;
    U32 read_time;
    U32 idle_time;
    U32 bg_time;
    U32 blit_time;
    U32 io_size;
    U32 io_used;
    U32 data_rate;
} BinkRealtime;

typedef struct BinkTrack {
    U32 freq;
    U32 bits;
    U32 chans;
    U32 max_size;
    Bink* bink;
    void* decomp;
    U32 track_num;
} BinkTrack;

void BinkSetError(const char* err);
char* BinkGetError(void);
S32 BinkSetSoundSystem(BinkSndSysOpen open, U32 param);
void BinkSetFrameRate(U32 rate, U32 div);
void BinkSetIOSize(U32 size);
void BinkSetIO(BinkIOOpen open);
void BinkSetSimulate(U32 sim);
void BinkSetSoundTrack(U32 total, U32* tracks);
Bink* BinkOpen(const char* name, U32 flags);
S32 BinkCopyToBuffer(Bink* bink, void* dest, S32 destpitch, U32 destheight, U32 destx, U32 desty, U32 flags);
S32 BinkCopyToBufferRect(Bink* bink, void* dest, S32 destpitch, U32 destheight, U32 destx, U32 desty, U32 srcx,
                         U32 srcy, U32 srcw, U32 srch, U32 flags);
S32 BinkDoFrame(Bink* bink);
void BinkNextFrame(Bink* bink);
U32 BinkGetKeyFrame(Bink* bink, U32 frame, S32 flags);
void BinkGoto(Bink* bink, U32 frame, S32 flags);
void BinkClose(Bink* bink);
S32 BinkWait(Bink* bink);
S32 BinkPause(Bink* bink, S32 pause);
void BinkGetSummary(Bink* bink, BinkSummary* sum);
void BinkGetRealtime(Bink* bink, BinkRealtime* rt, U32 frames);
S32 BinkGetRects(Bink* bink, U32 flags);
void BinkService(Bink* bink);
void BinkSetVolume(Bink* bink, S32 id, S32 volume);
void BinkSetMixBins(Bink* bink, S32 id, U32* bins, U32 total);
void BinkSetMixBinVolumes(Bink* bink, S32 id, U32* bins, S32* vols, U32 total);
void BinkSetPan(Bink* bink, S32 id, S32 pan);
void* BinkLogoAddress(void);
S32 BinkGetTrackType(Bink* bink, U32 idx);
U32 BinkGetTrackMaxSize(Bink* bink, U32 idx);
S32 BinkGetTrackID(Bink* bink, U32 idx);
BinkTrack* BinkOpenTrack(Bink* bink, U32 idx);
void BinkCloseTrack(BinkTrack* t);
U32 BinkGetTrackData(BinkTrack* t, void* dest);
S32 BinkSetVideoOnOff(Bink* bink, S32 onoff);
S32 BinkSetSoundOnOff(Bink* bink, S32 onoff);

#endif
