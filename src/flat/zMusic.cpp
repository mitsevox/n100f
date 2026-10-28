#include "zMusic.h"
#include "zGameState.h"
#include "zGlobals.h"
#include "zScene.h"
#include "xDebug.h"
#include "xSnd.h"
#include "xString.h"
#include "iMath.h"
#include <stdio.h>

#define TRACK_COUNT 2

static U32 sMusicSoundID[45][TRACK_COUNT];
static zMusicTrackInfo sMusicTrack[TRACK_COUNT];

static zMusicSituation sMusicInfo[7] = {
    { 0, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0 },
    { 1, 0, 0, 0.0f, 1.0f, 0.0f, 0, 0, 1, 0, 0 },
    { 0, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 2, 0, 0 },
    { 0, 0, 0, 0.0f, 2.0f, 0.0f, 0, 0, 3, 0, 0 },
    { 0, 0, 0, 0.0f, 1.0f, 0.0f, 0, 0, 4, 0, 0 },
    { 0, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 5, 0, 0 },
    { 1, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 6, 0, 0 },
};

static U32 sMusicPaused;
static zMusicSituation* sMusicQueueData[TRACK_COUNT];

static S32 sMusicLastEnum[TRACK_COUNT] = { 0, 0 };
static F32 sMusicTimer[TRACK_COUNT] = { 0.0f, 0.0f };

// Entry [1] of each sound is its loop flag. Entries 21 to 23 are set twice: the first writes store
// 0 over the upper-case names' hashes (probably [0] written for [1]), and the mixed-case names
// replace them below.
void zMusicInit()
{
    sMusicPaused = 0;

    for (S32 i = 0; i < TRACK_COUNT; i++)
    {
        sMusicTrack[i].snd_id = 0;
        sMusicTrack[i].loop = 0;
        sMusicTrack[i].situation = NULL;
    }

    sMusicSoundID[0][0] = xStrHash("BLACK KNIGHT-L");
    sMusicSoundID[0][1] = 1;
    sMusicSoundID[1][0] = xStrHash("REDBEARD-L");
    sMusicSoundID[1][1] = 1;
    sMusicSoundID[2][0] = xStrHash("GREEN GHOST-L");
    sMusicSoundID[2][1] = 1;
    sMusicSoundID[3][0] = xStrHash("MASTERMIND TEMP-L");
    sMusicSoundID[3][1] = 1;
    sMusicSoundID[4][0] = xStrHash("Scooby 03 loop-L");
    sMusicSoundID[4][1] = 1;
    sMusicSoundID[5][0] = xStrHash("Scooby 04 loop-L");
    sMusicSoundID[5][1] = 1;
    sMusicSoundID[6][0] = xStrHash("Scooby 05 loop-L");
    sMusicSoundID[6][1] = 1;
    sMusicSoundID[7][0] = xStrHash("Scooby 06 loop-L");
    sMusicSoundID[7][1] = 1;
    sMusicSoundID[8][0] = xStrHash("Scooby 07 loop-L");
    sMusicSoundID[8][1] = 1;
    sMusicSoundID[9][0] = xStrHash("Scooby 07 loop-L");
    sMusicSoundID[9][1] = 1;
    sMusicSoundID[10][0] = xStrHash("Scooby 09 loop-L");
    sMusicSoundID[10][1] = 1;
    sMusicSoundID[11][0] = xStrHash("Scooby 10 loop-L");
    sMusicSoundID[11][1] = 1;
    sMusicSoundID[12][0] = xStrHash("Scooby 11 loop-L");
    sMusicSoundID[12][1] = 1;
    sMusicSoundID[13][0] = xStrHash("Scooby 12 loop-L");
    sMusicSoundID[13][1] = 1;
    sMusicSoundID[14][0] = xStrHash("ACTION LOOP 01-L");
    sMusicSoundID[14][1] = 1;
    sMusicSoundID[15][0] = xStrHash("ACTION LOOP 02-L");
    sMusicSoundID[15][1] = 1;
    sMusicSoundID[16][0] = xStrHash("ACTION LOOP 03-L");
    sMusicSoundID[16][1] = 1;
    sMusicSoundID[17][0] = xStrHash("ACTION LOOP 04-L");
    sMusicSoundID[17][1] = 1;
    sMusicSoundID[18][0] = xStrHash("ACTION LOOP 05-L");
    sMusicSoundID[18][1] = 1;
    sMusicSoundID[19][0] = xStrHash("ACTION LOOP 06-L");
    sMusicSoundID[19][1] = 1;
    sMusicSoundID[20][0] = xStrHash("ACTION LOOP 07-L");
    sMusicSoundID[20][1] = 1;
    sMusicSoundID[21][0] = xStrHash("CINEMATIC NEMESIS 1-L");
    sMusicSoundID[21][0] = 0;
    sMusicSoundID[22][0] = xStrHash("CINEMATIC NEMESIS 2-L");
    sMusicSoundID[22][0] = 0;
    sMusicSoundID[23][0] = xStrHash("CINEMATIC NEMESIS 3-L");
    sMusicSoundID[23][0] = 0;
    sMusicSoundID[24][0] = xStrHash("Cinematic Mood 1-L");
    sMusicSoundID[24][1] = 1;
    sMusicSoundID[25][0] = xStrHash("Cinematic Mood 2-L");
    sMusicSoundID[25][1] = 1;
    sMusicSoundID[26][0] = xStrHash("Cinematic Mood 3-L");
    sMusicSoundID[26][1] = 1;
    sMusicSoundID[27][0] = xStrHash("Cinematic Generic 1-L");
    sMusicSoundID[27][1] = 1;
    sMusicSoundID[28][0] = xStrHash("Cinematic Silly-L");
    sMusicSoundID[28][1] = 1;
    sMusicSoundID[29][0] = xStrHash("Cinematic Van-L");
    sMusicSoundID[29][1] = 1;
    sMusicSoundID[30][0] = xStrHash("Comedic Duo loop-L");
    sMusicSoundID[30][1] = 1;
    sMusicSoundID[31][0] = xStrHash("Mystery loop-L");
    sMusicSoundID[31][1] = 1;
    sMusicSoundID[32][0] = xStrHash("Jeopardy loop-L");
    sMusicSoundID[32][1] = 1;
    sMusicSoundID[21][0] = xStrHash("Cinematic Nemesis 1-L");
    sMusicSoundID[21][1] = 0;
    sMusicSoundID[22][0] = xStrHash("Cinematic Nemesis 2-L");
    sMusicSoundID[22][1] = 0;
    sMusicSoundID[23][0] = xStrHash("Cinematic Nemesis 3-L");
    sMusicSoundID[23][1] = 0;
    sMusicSoundID[33][0] = xStrHash("theramin 01");
    sMusicSoundID[33][1] = 0;
    sMusicSoundID[34][0] = xStrHash("theramin 02");
    sMusicSoundID[34][1] = 0;
    sMusicSoundID[35][0] = xStrHash("theramin 03");
    sMusicSoundID[35][1] = 0;
    sMusicSoundID[36][0] = xStrHash("theramin 04");
    sMusicSoundID[36][1] = 0;
    sMusicSoundID[37][0] = xStrHash("theramin 05");
    sMusicSoundID[37][1] = 0;
    sMusicSoundID[38][0] = xStrHash("theramin 06");
    sMusicSoundID[38][1] = 0;
    sMusicSoundID[39][0] = xStrHash("theramin 07");
    sMusicSoundID[39][1] = 0;
    sMusicSoundID[40][0] = xStrHash("theramin 08");
    sMusicSoundID[40][1] = 0;
    sMusicSoundID[41][0] = xStrHash("theramin 09");
    sMusicSoundID[41][1] = 0;
    sMusicSoundID[42][0] = xStrHash("theramin 10");
    sMusicSoundID[42][1] = 0;
    sMusicSoundID[43][0] = xStrHash("theramin 11");
    sMusicSoundID[43][1] = 0;
    sMusicSoundID[44][0] = xStrHash("theramin 12");
    sMusicSoundID[44][1] = 0;

    for (S32 i = 0; i < 7; i++)
    {
        sMusicInfo[i].elapsedTime = sMusicInfo[i].delay;
        sMusicInfo[i].count = 0;
    }
}

// Plays the sound for the track's queued situation: situation 3 plays the boss theme in the four boss
// scenes, situation 0 plays the scene's own theme (a random Scooby loop where none is listed),
// situations 5 and 6 play the sound the event asked for, situation 2 the first action loop and
// situation 4 keeps the random pick.
static S32 zMusicDo(S32 track)
{
    F32 vol = 0.33f;
    S32 snd_enum = irand() % 45;

    switch (sMusicQueueData[track]->situation)
    {
    case 3:
        vol = 1.0f;
        if (globals.sceneCur->sceneID == zSceneIDFromName("O008"))
        {
            snd_enum = 0;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("W028"))
        {
            snd_enum = 1;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("G009"))
        {
            snd_enum = 2;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("S005"))
        {
            snd_enum = 3;
        }
        break;
    case 5:
        snd_enum = sMusicQueueData[track]->music_enum;
        break;
    case 6:
        snd_enum = sMusicQueueData[track]->music_enum;
        break;
    case 0:
        if (globals.sceneCur->sceneID == zSceneIDFromName("H003"))
        {
            snd_enum = 31;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("G009"))
        {
            snd_enum = 11;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("O008"))
        {
            snd_enum = 11;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("W028"))
        {
            snd_enum = 11;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("S005"))
        {
            snd_enum = 11;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("MNU4") ||
                 globals.sceneCur->sceneID == zSceneIDFromName("MNU3"))
        {
            snd_enum = 13;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("H001"))
        {
            snd_enum = 11;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("H002"))
        {
            snd_enum = 19;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("I001"))
        {
            snd_enum = 31;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("I020"))
        {
            snd_enum = 6;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("I021"))
        {
            snd_enum = 5;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("I003"))
        {
            snd_enum = 12;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("I004"))
        {
            snd_enum = 31;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("I005"))
        {
            snd_enum = 12;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("I006"))
        {
            snd_enum = 10;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("R001"))
        {
            snd_enum = 11;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("R020"))
        {
            snd_enum = 7;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("R021"))
        {
            snd_enum = 30;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("R003"))
        {
            snd_enum = 12;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("R004"))
        {
            snd_enum = 20;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("R005"))
        {
            snd_enum = 20;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("R005"))
        {
            snd_enum = 4;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("o001"))
        {
            snd_enum = 6;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("o002"))
        {
            snd_enum = 5;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("o003"))
        {
            snd_enum = 32;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("o004"))
        {
            snd_enum = 17;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("o005"))
        {
            snd_enum = 11;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("o006"))
        {
            snd_enum = 12;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("o008"))
        {
            snd_enum = 24;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("f001"))
        {
            snd_enum = 10;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("f003"))
        {
            snd_enum = 30;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("f009"))
        {
            snd_enum = 4;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("f010"))
        {
            snd_enum = 8;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("e001"))
        {
            snd_enum = 31;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("e002"))
        {
            snd_enum = 5;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("e003"))
        {
            snd_enum = 11;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("e004"))
        {
            snd_enum = 28;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("e005"))
        {
            snd_enum = 32;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("e006"))
        {
            snd_enum = 10;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("e007"))
        {
            snd_enum = 30;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("e008"))
        {
            snd_enum = 6;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("e009"))
        {
            snd_enum = 8;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("c001"))
        {
            snd_enum = 7;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("c002"))
        {
            snd_enum = 12;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("c003"))
        {
            snd_enum = 19;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("c004"))
        {
            snd_enum = 32;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("c005"))
        {
            snd_enum = 31;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("c006"))
        {
            snd_enum = 5;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("c007"))
        {
            snd_enum = 6;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("g001"))
        {
            snd_enum = 11;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("g002"))
        {
            snd_enum = 18;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("g003"))
        {
            snd_enum = 32;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("g004"))
        {
            snd_enum = 30;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("g005"))
        {
            snd_enum = 10;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("g006"))
        {
            snd_enum = 19;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("g007"))
        {
            snd_enum = 7;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("g008"))
        {
            snd_enum = 31;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("f004"))
        {
            snd_enum = 12;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("f005"))
        {
            snd_enum = 7;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("f005"))
        {
            snd_enum = 7;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("f006"))
        {
            snd_enum = 13;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("f007"))
        {
            snd_enum = 32;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("f008"))
        {
            snd_enum = 31;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("l011"))
        {
            snd_enum = 5;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("l013"))
        {
            snd_enum = 8;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("l014"))
        {
            snd_enum = 4;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("l015"))
        {
            snd_enum = 10;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("l015"))
        {
            snd_enum = 10;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("l017"))
        {
            snd_enum = 20;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("l018"))
        {
            snd_enum = 30;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("l019"))
        {
            snd_enum = 6;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("w020"))
        {
            snd_enum = 4;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("w021"))
        {
            snd_enum = 8;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("w022"))
        {
            snd_enum = 7;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("w023"))
        {
            snd_enum = 32;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("w025"))
        {
            snd_enum = 5;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("w026"))
        {
            snd_enum = 19;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("w027"))
        {
            snd_enum = 12;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("p001"))
        {
            snd_enum = 12;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("p002"))
        {
            snd_enum = 6;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("p003"))
        {
            snd_enum = 11;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("p004"))
        {
            snd_enum = 30;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("p005"))
        {
            snd_enum = 14;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("b001"))
        {
            snd_enum = 16;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("b002"))
        {
            snd_enum = 32;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("b003"))
        {
            snd_enum = 4;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("b003"))
        {
            snd_enum = 10;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("b004"))
        {
            snd_enum = 12;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("s001"))
        {
            snd_enum = 15;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("s002"))
        {
            snd_enum = 17;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("s003"))
        {
            snd_enum = 19;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("s004"))
        {
            snd_enum = 20;
        }
        else if (globals.sceneCur->sceneID == zSceneIDFromName("s005"))
        {
            snd_enum = 31;
        }
        else
        {
            snd_enum = irand() % 9 + 4;
        }
        break;
    case 4:
        break;
    case 2:
        snd_enum = 14;
        break;
    default:
        snd_enum = irand() % 45;
        break;
    }

    if (sMusicTrack[track].snd_id != 0)
    {
        xSndStop(sMusicTrack[track].snd_id);
        sMusicTrack[track].snd_id = 0;
    }

    sMusicTrack[track].loop = sMusicSoundID[snd_enum][1];
    sMusicTrack[track].snd_id = xSndPlay(sMusicSoundID[snd_enum][0], vol, vol, 0.0f, 0xFFFF,
                                         (sMusicTrack[track].loop ? 0x100 : 0) | 0x604, 0);
    xprintf("zMusicDo %d\n", sMusicQueueData[track]->sndid);

    if (sMusicTrack[track].snd_id != 0)
    {
        sMusicTrack[track].assetID = sMusicSoundID[snd_enum][0];
        sMusicTrack[track].lastVol = vol;

        if (sMusicQueueData[track] != NULL)
        {
            sMusicQueueData[track]->sndid = sMusicTrack[track].snd_id;
            sMusicQueueData[track]->elapsedTime = 0.0f;
            sMusicQueueData[track]->count++;
            sMusicQueueData[track] = NULL;
        }

        sMusicLastEnum[track] = snd_enum;
        return 1;
    }

    return 0;
}

void zMusicNotify(S32 situation)
{
    zMusicSituation* s;

    if (sMusicPaused)
    {
        return;
    }

    s = &sMusicInfo[situation];

    if (s->countMax != 0 && s->count >= s->countMax)
    {
        return;
    }

    if (s->delay > s->elapsedTime)
    {
        return;
    }

    sMusicTimer[s->track] = s->punchDelay;
    sMusicQueueData[s->track] = s;
    sMusicQueueData[s->track]->game_state = (gGameMode == eGameMode_Game);
}

// toParam[0] is the sound to play; toParam[1] picks situation 5 when zero and situation 6 otherwise.
void zMusicNotifyEvent(F32* toParam, xBase*)
{
    zMusicSituation* s;

    if (sMusicPaused)
    {
        return;
    }

    s = &sMusicInfo[0.0f == toParam[1] ? 5 : 6];

    if (sMusicQueueData[s->track] != NULL)
    {
        return;
    }

    if (s->countMax != 0 && s->count >= s->countMax)
    {
        return;
    }

    if (s->delay > s->elapsedTime)
    {
        return;
    }

    sMusicTimer[s->track] = s->punchDelay;
    sMusicQueueData[s->track] = s;
    sMusicQueueData[s->track]->game_state = (gGameMode == eGameMode_Game);
    sMusicQueueData[s->track]->music_enum = toParam[0];
}

// A queued situation starts its track once its punch delay has run out, and only while the game is
// in play or out of play as it was when the situation was queued.
void zMusicUpdate(F32 dt)
{
    S32 i;

    if (sMusicPaused == 0)
    {
        for (i = 0; i < 7; i++)
        {
            sMusicInfo[i].elapsedTime += dt;
        }

        for (i = 0; i < TRACK_COUNT; i++)
        {
            if ((sMusicTimer[i] || sMusicQueueData[i] != NULL) &&
                (gGameMode == eGameMode_Game) == sMusicQueueData[i]->game_state)
            {
                if (sMusicTimer[i] > 0.0f)
                {
                    sMusicTimer[i] -= dt;
                    if (sMusicTimer[i] < 0.0f)
                    {
                        sMusicTimer[i] = 0.0f;
                    }
                }

                if (0.0f == sMusicTimer[i] && sMusicQueueData[i] != NULL)
                {
                    zMusicDo(i);
                }
            }
        }
    }
}

void zMusicKill()
{
    for (S32 i = 0; i < TRACK_COUNT; i++)
    {
        if (sMusicTrack[i].snd_id != 0)
        {
            xSndStop(sMusicTrack[i].snd_id);
            sMusicTrack[i].snd_id = 0;
            sMusicTrack[i].loop = 0;
            sMusicTrack[i].situation = NULL;
        }
    }
}

void zMusicPause()
{
    if (sMusicPaused == 0)
    {
        for (S32 i = 0; i < TRACK_COUNT; i++)
        {
            if (sMusicTrack[i].snd_id != 0)
            {
                printf("pausing track %d (sndid= %d)\n", i, sMusicTrack[i].snd_id);
                xSndStop(sMusicTrack[i].snd_id);
            }
        }
        sMusicPaused = 1;
    }
}

// Restarts the tracks that zMusicPause stopped, or forgets them when kill is set.
void zMusicUnpause(S32 kill)
{
    if (sMusicPaused == 0)
    {
        return;
    }

    for (S32 i = 0; i < TRACK_COUNT; i++)
    {
        if (sMusicTrack[i].snd_id == 0)
        {
            continue;
        }

        if (kill)
        {
            sMusicTrack[i].snd_id = 0;
        }
        else
        {
            sMusicTrack[i].snd_id =
                xSndPlay(sMusicTrack[i].assetID, sMusicTrack[i].lastVol, sMusicTrack[i].lastVol, 0.0f,
                         0xFFFF, (sMusicTrack[i].loop ? 0x100 : 0) | 0x604, 0);
            printf("unpausing track %d (sndid= %d)\n", i, sMusicTrack[i].snd_id);
        }
    }

    sMusicPaused = 0;
}
