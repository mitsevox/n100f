#ifndef ZMUSIC_H
#define ZMUSIC_H

#include "types.h"
#include "xBase.h"

// A reason to change the music. zMusicNotify queues the situation on its track once its delay has
// passed and it has played fewer than countMax times (0 means no limit).
struct zMusicSituation
{
    S32 track;
    U32 priority;
    U32 countMax;
    F32 delay;
    F32 punchDelay;
    F32 elapsedTime;
    U32 count;
    U32 sndid;
    S32 situation;
    S32 music_enum;
    S32 game_state;
};

struct zMusicTrackInfo
{
    U32 snd_id;
    U32 loop;
    zMusicSituation* situation;
    U32 assetID;
    F32 lastVol;
};

void zMusicUnpause(S32 kill);
void zMusicPause();
void zMusicKill();
void zMusicUpdate(F32 dt);
void zMusicNotifyEvent(F32* toParam, xBase* base);
void zMusicNotify(S32 situation);
void zMusicInit();

#endif
