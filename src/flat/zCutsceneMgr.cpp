#include "zCutsceneMgr.h"

#include "iCamera.h"
#include "xString.h"
#include "xserializer.h"
#include "zEnt.h"
#include "zEntPlayer.h"
#include "zGlobals.h"
#include "zMusic.h"

struct zCutSceneNames
{
    char name[64];
    S32 played;
    S32 skipOK;
    char fmvName[64];
};

static S32 gCutsceneSkipOK;

static zCutSceneNames zCutSceneNamesTable[14] = {
    { "cin_1", 0, 1, "cin1" },
    { "cin_2", 0, 1, "cin2" },
    { "cin_3", 0, 1, "cin3" },
    { "cin_4", 0, 1, "cin4" },
    { "cin_5", 0, 1, "cin5" },
    { "cin_6", 0, 1, "cin6" },
    { "cin_7", 0, 1, "cin7" },
    { "cin_8", 0, 1, "cin8" },
    { "cin_9", 0, 1, "cin9" },
    { "cin_10", 0, 1, "cin10" },
    { "cin_11", 0, 0, "cin11" },
    { "cin_mm_post_death", 0, 0, "cin_mmpd" },
    { "cin_mm_scare", 0, 1, "cin_mmsc" },
    { "cin_sh_barrel", 0, 1, "cin_shbr" },
};

S32 xCutscene_Destroy(xCutscene* csn);
U32 zFMVPlay(char* filename, U32 buttons, F32 time);

// Set up the base, the event handler and the links after the asset.
void zCutsceneMgrInit(void* b, void* tasset)
{
    _zCutsceneMgr* t = (_zCutsceneMgr*)b;

    xBaseInit(t, (p2BaseAsset*)tasset);
    t->eventFunc = zCutsceneMgrEventCB;
    t->tasset = (zCutsceneMgrAsset*)tasset;
    t->csn = NULL;

    if (t->linkCount != 0)
    {
        t->link = (p2LinkAsset*)((U8*)t->tasset + sizeof(zCutsceneMgrAsset));
    }
    else
    {
        t->link = NULL;
    }
}

// Save which cutscenes have played, one bit each.
void zCutsceneMgrSave(_zCutsceneMgr*, xSerial* s)
{
    for (U32 i = 0; i < 14; i++)
    {
        if (zCutSceneNamesTable[i].played)
        {
            s->Write_b1(1);
        }
        else
        {
            s->Write_b1(0);
        }
    }
}

// Restore which cutscenes have been played.
void zCutsceneMgrLoad(_zCutsceneMgr*, xSerial* s)
{
    for (U32 i = 0; i < 14; i++)
    {
        s->Read_b1(&zCutSceneNamesTable[i].played);
    }
}

// Mark every cutscene as not played.
void zCutSceneNamesTable_clearAll()
{
    for (S32 i = 0; i < 14; i++)
    {
        zCutSceneNamesTable[i].played = 0;
    }
}

// Cutscenes play as movies. Starting one pauses the music and plays the movie listed for the
// cutscene asset's name, marking it played; the next update reports the cutscene finished.
S32 zCutsceneMgrEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    _zCutsceneMgr* t = (_zCutsceneMgr*)to;

    switch (toEvent)
    {
    case 0xBF:
        break;
    case 0x18:
    {
        gCutsceneSkipOK = 1;
        U32 index = 0;
        for (U32 i = 0; i < 14; i++)
        {
            if (t->tasset->cutsceneAssetID == xStrHash(zCutSceneNamesTable[i].name))
            {
                index = i;
                zCutSceneNamesTable[i].played = 1;
                gCutsceneSkipOK = zCutSceneNamesTable[i].skipOK;
                break;
            }
        }
        zFMVPlay(zCutSceneNamesTable[index].fmvName,
                 zCutSceneNamesTable[index].skipOK ? 0x10000 : 0, 3.0f);
        t->state |= 2;
        break;
    }
    case 0x13:
        t->stop = 1;
        break;
    case 0xBE:
        zMusicPause();
        if (globals.cmgr == NULL)
        {
            t->stop = 0;
            globals.cmgr = t;
        }
        t->state = 1;
        break;
    case 10:
        xBaseReset(t, t->tasset);
        if (t->csn != NULL)
        {
            xCutscene_Destroy(t->csn);
        }
        t->csn = NULL;
        break;
    }

    return 1;
}

// Nothing to finish once a cutscene has loaded.
void zCutsceneMgrFinishLoad(xBase* to)
{
    _zCutsceneMgr* t = (_zCutsceneMgr*)to;
}

// Start a requested cutscene, finish a played one, and once it has stopped hand control back
// to the player and the music.
void zCutsceneMgrUpdate(xBase* to, _xScene*, F32)
{
    _zCutsceneMgr* t = (_zCutsceneMgr*)to;

    if (t->state & 1)
    {
        t->state &= ~1;
        zEntEvent(t, t, 0x18);
    }

    if (t->state & 2)
    {
        t->state &= 2;
        zEntEvent(t, t, 0x13);
    }

    if (t->stop)
    {
        S32 pauseValue;
        if (t->tasset->cutsceneAssetID == xStrHash("cin_2") ||
            t->tasset->cutsceneAssetID == xStrHash("cin_4") ||
            t->tasset->cutsceneAssetID == xStrHash("cin_6") ||
            t->tasset->cutsceneAssetID == xStrHash("cin_10"))
        {
            pauseValue = 1;
        }
        else
        {
            pauseValue = 0;
        }

        globals.cmgr = NULL;
        zMusicUnpause(pauseValue);
        zEntEvent(t, t, 0xBF);
        zEntEvent(&globals.player.ent, 9);
        zEntEvent(&globals.player.ent, 3);
        iCameraSetNearFarClip(0.0f, 0.0f);
        t->stop = 0;
    }
}

// Report whether cutscene number has been played.
U32 zCutSceneNamesTable_getStatus(U32 number)
{
    return zCutSceneNamesTable[number - 1].played ? 1 : 0;
}
