#ifndef ZGLOBALS_H
#define ZGLOBALS_H

#include "types.h"
#include "xBase.h"
#include "xMath3.h"
#include "zEntPlayer.h"
#include "iCamera.h"
#include "iTime.h"
#include "xCamera.h"

struct zScene;
struct zAssetPickupTable;
struct _zCutsceneMgr;
struct _tagxPad;

struct zGlobalGameStats
{
    F32 gameTime;
    U32 visited;
    F32 levelTime;
    F32 levelTime_sum;
    F32 areaTime;
    F32 areaTime_sum;
    F32 roomTime;
    F32 roomTime_sum;
    F32 timeRunning;
    F32 timeRunning_sum;
    F32 timeWalking;
    F32 timeWalking_sum;
    U32 scares;
    U32 scares_sum;
    U32 deaths;
    U32 deaths_sum;
    U32 singleJumps;
    U32 singleJumps_sum;
    U32 doubleJumps;
    U32 doubleJumps_sum;
    U32 gumShot;
    U32 gumShot_sum;
    U32 gumHit;
    U32 gumHit_sum;
    U32 gumEmpty;
    U32 gumEmpty_sum;
    U32 soapShot;
    U32 soapShot_sum;
    U32 soapHit;
    U32 soapHit_sum;
    U32 soapEmpty;
    U32 soapEmpty_sum;
    U32 food;
    U32 food_sum;
    U32 snackBoxes;
    U32 snackBoxes_sum;
    U32 enemyKills;
    U32 enemyKills_sum;
    U32 enemyCount;
    U32 enemyCount_sum;
    U32 specials;
    U32 specials_tot;
    U32 tokens;
    U32 tokens_tot;
    U32 snacks;
    U32 snacks_tot;
    U32 chances;
    U32 chances_tot;
    U32 keys;
    U32 keys_tot;
    U32 clues;
    U32 clues_tot;
};

struct zGlobals
{
    _tagxCamFrame camera;
    _tagxPad* pad0;
    _tagxPad* pad1;
    zPlayerGlobals player;
    zAssetPickupTable* pickupTable;
    S32 profile;
    _zCutsceneMgr* cmgr;
    char profFunc[6][128];
    zScene* sceneCur;
    zScene* scenePreload;
    S32 sceneFirst;
    char sceneStart[32];
    iFogParams fog;
    iFogParams fogA;
    iFogParams fogB;
    iTime fog_t0;
    iTime fog_t1;
    zGlobalGameStats stats;
    zGlobalGameStats areaStats[30];
    zGlobalGameStats levelStats[13];
    zGlobalGameStats worldStats[4];
    S32 option_vibration;
    F32 update_dt;
};

extern zGlobals globals;

#endif
