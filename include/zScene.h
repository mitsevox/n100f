#ifndef ZSCENE_H
#define ZSCENE_H

#include "types.h"
#include "xBase.h"
#include "xScene.h"
#include "xMemMgr.h"

struct _zEnt;
struct _zPortal;
struct _zEnv;
struct _tagClimate;
struct _xRay3;

struct zScene : _xScene
{
    U32 sceneID;
    _zPortal* pendingPortal;
    union
    {
        U32 num_ents;
        U32 num_base;
    };
    union
    {
        xBase** base;
        _zEnt** ents;
    };
    U32 baseCount[43];
    xBase* baseList[43];
    U32 num_bubbles;
    _zEnt** bubbles;
};

struct IDBasePair
{
    U32 id;
    xBase* base;
};

enum _tagSceneAreas
{
    eSceneArea_Unknown,
    eSceneArea_H001,
    eSceneArea_E001,
    eSceneArea_E005,
    eSceneArea_E007,
    eSceneArea_C001,
    eSceneArea_C005,
    eSceneArea_G001,
    eSceneArea_G003,
    eSceneArea_G007,
    eSceneArea_F001,
    eSceneArea_F004,
    eSceneArea_F008,
    eSceneArea_L011,
    eSceneArea_L015,
    eSceneArea_L017,
    eSceneArea_W020,
    eSceneArea_W023,
    eSceneArea_W027,
    eSceneArea_I001,
    eSceneArea_I004,
    eSceneArea_R001,
    eSceneArea_R004,
    eSceneArea_O001,
    eSceneArea_O005,
    eSceneArea_B001,
    eSceneArea_P001,
    eSceneArea_P004,
    eSceneArea_S001,
    eSceneArea_S004,
    eSceneArea_count
};

// One playable area: up to four rooms and the snack and token totals the progress
// screens compare against.
struct zSceneArea
{
    char* desc;
    _tagSceneAreas index;
    U32 totalSnacks;
    U32 constTotalSnacks;
    U32 totalMTokens;
    U32 sceneIDs[4];
};

// One level, keyed by the first letter of its scene ids.
struct zSceneLevel
{
    char* desc;
    char name;
    S32 index;
    U32 totalClues;
};

struct zSceneObjectInstanceDesc
{
    char* name;
    S32 baseType;
    U32 assetType;
    U32 sizeRuntime;
    U32 (*func)(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx);
    void (*objectInitFunc)(void* ent, void* asset);
};

extern _tagClimate gClimate;
extern _zEnv* gCurEnv;

void zSceneStats_Clear();
void zScene_UpdateFlyToInterface(F32 dt);
U32 zSceneCalcProgress();
S32 zRayHitsPlayer(_xRay3* r);
void zSceneGameStatsTally_clue();
void zSceneGameStatsTally_death();
void zSceneGameStatsTally_scare();
void zSceneGameStatsTally_token();
void zSceneGameStatsTally_snack(U32 amount);
U32 zSceneGetAreaTokensCollected(U32 sceneID);
U32 zSceneGetAreaSnacksCollected(U32 sceneID);
U32 zSceneGetAreaTotalTokens(U32 sceneID);
U32 zSceneGetAreaTotalSnacks(_tagSceneAreas scene);
U32 zSceneGetAreaTotalSnacks(U32 sceneID);
U32 zSceneGetAreaConstTotalSnacks(U32 sceneID);
char* zSceneGetAreaname(U32 sceneID);
U32 zSceneGetRoominArea(U32 sceneID);
U32 zSceneGetArea(U32 sceneID);
void zSceneMemLvlChkCB();
void zSceneRender();
void zSceneUpdate(F32 dt);
void zSceneSetup();
void zSceneReset();
void zSceneLoad(zScene* ent, xSerial* s);
void zSceneSave(zScene* ent, xSerial* s);
void zSceneSwitch(_zPortal* p);
void zSceneExit();
void zSceneInit(U32 theSceneID);
void zSceneForAllBase(xBase* (*func)(xBase*, zScene*, void*), S32 baseType, void* data);
xBase* zSceneFindObject(U32 gameID);
xBase* zSceneGetObject(S32 type, S32 idx);
char* zSceneGetName(U32 gameID);
char* zSceneGetName(xBase* b);
S32 zSceneSetup_serialTraverseCB(U32 clientID, xSerial* xser);

inline U32 zSceneIDFromName(const char* name)
{
    return ((name[0] & ~0x20) << 24) | (name[1] << 16) | (name[2] << 8) | name[3];
}

extern U32 gTransitionSceneID;

#endif
