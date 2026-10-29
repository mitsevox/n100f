#include "zScene.h"

#include "xBound.h"
#include "xEnt.h"
#include "xMemMgr.h"
#include "xString.h"
#include "xserializer.h"
#include "xstransvc.h"
#include "xutil.h"
#include "zConditional.h"
#include "zDispatcher.h"
#include "zCounter.h"
#include "zEntButton.h"
#include "zEntDestructObj.h"
#include "zEntHangable.h"
#include "zEntPickup.h"
#include "zEntPlayer.h"
#include "zFog.h"
#include "zGame.h"
#include "zGlobals.h"
#include "zGroup.h"
#include "zMenu.h"
#include "zParEmitter.h"
#include "zParSys.h"
#include "zPortal.h"
#include "zSFX.h"
#include "zScript.h"
#include "zTimer.h"

#include "zEntVillain.h"
#include "zGust.h"
#include "zLight.h"
#include "zLobMaster.h"
#include "zMovePoint.h"
#include "zVolume.h"
#include "xSurface.h"
#include "zEnt.h"
#include "iSnd.h"
#include "zSurface.h"
#include "xEntMotion.h"
#include "zEntProjectile.h"
#include "xEnv.h"
#include "zEnv.h"
#include "xClimate.h"
#include "zMusic.h"
#include "zEntShaggy.h"
#include "zEntBubble.h"
#include "xScrFx.h"
#include "xMovePoint.h"
#include "zHud.h"
#include "zHologram.h"
#include "zLightning.h"
#include "zGameState.h"
#include "zUIFont.h"
#include "zFeet.h"
#include "xFont.h"
#include "zRenderState.h"
#include "xColor.h"
#include <stdio.h>
#include <string.h>

extern "C" S32 lbl_8025E528;
extern "C" U32 lbl_8025EBE0;

void xSerialTraverse(S32 (*func)(U32 clientID, xSerial* xser));
U32 zVarEntryCB_eVareVarDefeatedMM();
S32 xMemGetBase();

struct _zCutsceneMgr;
struct _zEntSimpleObj;
struct _zPendulum;
struct _zScript;
struct _xBox;
struct iEnv;
struct RpWorld;
struct zEGenerator;

S32 xSTSwitchScene(U32 sid, void* userdata, S32 (*progmon)(void*, F32));
void xSTUnLoadScene(U32 sid);
S32 xSTQueueSceneAssets(U32 sid);
S32 xSTPreLoadScene(U32 sid, void* userdata);
S32 xMemPushBase();
S32 xMemPopBase(S32 depth);
void xSceneInit(_xScene* sc, U16 num_trigs, U16 num_stats, U16 num_dyns, U16 num_npcs);
void xSceneSetup(_xScene* sc);
void xSceneAddEnt(_xScene* sc, _xEnt* ent);
void xSceneSave(_xScene* sc, xSerial* s);
void xSceneLoad(_xScene* sc, xSerial* s);
_tagxCam* xCameraInit(_tagxCamFrame* cam, U32 total);
_tagxCam* xCameraSetAsset(_tagxCamFrame* cam, xCamAsset* src, S32 idx);
void xCameraPickInitial(_tagxCamFrame* cam, _tagxCam* c);
void xCameraBegin(_tagxCamFrame* cam, S32 clear);
void xCameraEnd(_tagxCamFrame* cam, F32 seconds, S32 update_scrn_fx);
void zCameraInit(xBase* b, xCamAsset* asset);
void xSndParentDied(U32 pid);
void xSndStopAll();
void xSndUpdate(F32 time_elapsed);
void xPadDestroyRumbleChain(S32 idx);
void xPadRumbleEnable(S32 idx, S32 enable);
void xModel_SceneExit(RpWorld* world);
void xEntMotionDebugInit(U16 num_xems);
void xPartitionReset();
void xQuickCullInit(const _xBox* box);
void xCutscene_Init(void* toc);
void xFXUpdate(F32 dt);
void xParMgrRender();
U32 xShadowReceiveShadowSetup(_xEnt* ent);
void xShadowReceiveShadow(_xEnt* ent);
S32 iEnvBeginRenderFX(iEnv* env);
void iEnvRenderFX(iEnv* env);
void iFuncProfileDump();
void zAnimListInit();
void zAnimListExit();
void zRenderStateInit();
U32 zMenuGetBadCard();
void zGustInit();
void zSurfaceInit();
_zMovePoint* zMovePoint_GetMemPool(S32 cnt);
_zMovePoint* zMovePoint_GetInst(S32 n);
void zMovePointInit(_zMovePoint* m, p2MovePointAsset* asset);
void zEntVillain_Init(st_zEntVillain* ent, p2EntAsset* asset);
void zVolumeInit();
void zVolumeSetup();
_zVolume* zVolumeGetVolume(U16 n);
void zVolumeSave(_zVolume* ent, xSerial* s);
void zVolumeLoad(_zVolume* ent, xSerial* s);
void zEnvSetup(_zEnv* env);
void zEnvSave(_zEnv* ent, xSerial* s);
void zEnvLoad(_zEnv* ent, xSerial* s);
void zEntSimpleObj_Setup(_zEntSimpleObj* ent);
void zEntSimpleObj_Save(_zEntSimpleObj* ent, xSerial* s);
void zEntSimpleObj_Load(_zEntSimpleObj* ent, xSerial* s);
void zPendulum_Reset(_zPendulum* pend, _xScene* sc);
void zPendulum_Setup(_zPendulum* pend, _xScene* sc);
void zPendulum_Save(_zPendulum* pend, xSerial* s);
void zPendulum_Load(_zPendulum* pend, xSerial* s);
void zPortalSave(_zPortal* ent, xSerial* s);
void zPortalLoad(_zPortal* ent, xSerial* s);
void zEGenerator_Setup(zEGenerator* egen, _xScene* sc);
void zEGenerator_Reset(zEGenerator* egen, _xScene* sc);
void zEGenerator_Save(zEGenerator* ent, xSerial* s);
void zEGenerator_Load(zEGenerator* ent, xSerial* s);
void zCutsceneMgrUpdate(xBase* to, _xScene* sc, F32 dt);
void zCutsceneMgrSave(_zCutsceneMgr* mgr, xSerial* s);
void zCutsceneMgrLoad(_zCutsceneMgr* mgr, xSerial* s);
void zScriptUpdate(xBase* to, _xScene* sc, F32 dt);
void zScriptSave(_zScript* ent, xSerial* s);
void zScriptLoad(_zScript* ent, xSerial* s);
void zVarSave(xSerial* s);
void zVarLoad(xSerial* s);
S32 zSceneCalcProgress_serialTraverseCB(U32 clientID, xSerial* xser);
void zEnvRender(_xEnv* env);
void zActionLineRender();
void zFeetRenderSkidMark(_tagFeetSkidMark* m);
void iShadowRender();
void xFontRender();
void xFontBoxSetBackdrop(xFontBox* box, iColor_tag color);
void xFontBoxSetInset(xFontBox* box, F32 left, F32 top, F32 right, F32 bottom);
void DrawScreenSizeRectangle();
void zEntPickup_RenderList(_zEntPickup* plist, U32 pcount);
extern _tagFeetSkidMark sPlayerSkidMark[5];
extern "C" void fn_800167F0();
extern "C" void fn_80009CCC(xVec3* pts, S32 count, U32 id, F32 start_width, F32 end_width);
extern "C" void fn_8000A138(iEnv* env);

// A per-scene override of how far an entity counts from the camera when the see-through
// entities are sorted for drawing.
struct zSceneRenderDist
{
    U32 sceneID;
    U32 entID;
    S32 mode; // 0: from a fixed point, 1: a fixed distance, 2: another entity's plus an offset
    union
    {
        F32 x;
        F32 dist;
        U32 refID;
    };
    union
    {
        F32 z;
        F32 offset;
    };
};

extern "C" zSceneRenderDist lbl_801C82A0[66];
void zLightUpdate(xBase* to, _xScene* sc, F32 dt);
void zSurfaceUpdate(xBase* to, _xScene* sc, F32 dt);
void zActionLineUpdate(F32 seconds);
void xClimateUpdate(_tagClimate* climate, F32 seconds);

extern F32 gSceneUpdateTime;
extern F32 gFontWiggleTime;
void zGameScreenTransitionBegin();
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg);
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg, U8* rgba);
void zGameScreenTransitionEnd();
F32 xSTLoadStep(U32 sid);
void xFFXPoolInit(U32 num_ffx);
void xFFXShakePoolInit(U32 num);
void xFFXRotMatchPoolInit(U32 num);
void zFeetGetIDs();
void zParCmdFindClipVolumes();
void zActionLineInit();
void xClimateInit(_tagClimate* climate);
extern "C" void fn_80016600();
extern "C" U32 lbl_8025EEC8;
void zPlatform_Reset(_zPlatform* plat, _xScene* sc);
void zPlatform_Setup(_zPlatform* plat, _xScene* sc);
void zMovePointSetup(_zMovePoint* mvpt, zScene* scn);
void zGustSetup();
void zEntVillain_InitDep(st_zEntVillain* ent, zScene* sc);
void zEntPlayerReset(_xEnt* ent);
void zEntPlayer_LoadCheckPoint();
void zEntPlayer_Load(_xEnt* ent, xSerial* serial);
struct zEntTrigger;
struct _zPlatform;
struct _zLight;
void zEntTriggerSave(zEntTrigger* ent, xSerial* s);
void zEntTriggerLoad(zEntTrigger* ent, xSerial* s);
void zLightSave(_zLight* ent, xSerial* s);
void zLightLoad(_zLight* ent, xSerial* s);
void zPlatform_Save(_zPlatform* ent, xSerial* s);
void zPlatform_Load(_zPlatform* ent, xSerial* s);
void zCameraSave(_tagxCam* ent, xSerial* s);
void zCameraLoad(_tagxCam* ent, xSerial* s);
void zMovePointSave(_zMovePoint* ent, xSerial* s);
void zMovePointLoad(_zMovePoint* ent, xSerial* s);
void zSurfaceSave(xSurface* ent, xSerial* s);
void zSurfaceLoad(xSurface* ent, xSerial* s);
void zGustSave(_zGust* ent, xSerial* s);
void zGustLoad(_zGust* ent, xSerial* s);
void zFX_SceneExit(RpWorld* world);
void zEntPlayerExit(_xEnt* ent);
void zEntEventAll(xBase* from, U32 fromEvent, U32 toEvent, F32* toParam);

extern S32 gOccludeCount;
void xCameraSetTarget(_tagxCamFrame* cam, xVec3* target);
void xCameraSetTarget(_tagxCamFrame* cam, xMat4x3* target);

struct p2MarkerAsset
{
    xVec3 pos;
};

static U32 zSceneInitFunc_Dispatcher(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx);
static U32 zSceneInitFunc_LobMaster(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx);
static U32 zSceneInitFunc_Volume(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx);
static U32 zSceneInitFunc_Gust(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx);
static U32 zSceneInitFunc_Surface(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx);
static U32 zSceneInitFunc_Camera(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx);
static U32 zSceneInitFunc_Player(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx);
static U32 zSceneInitFunc_Villain(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx);
static U32 zSceneInitFunc_MovePoint(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx);
static U32 zSceneInitFunc_Default(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx);
static U32 zSceneInitFunc_DefaultEnt(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx);

void zEntTriggerInit(void* ent, void* asset);
void zPlatform_Init(void* plat, void* asset);
void zPendulum_Init(void* pend, void* asset);
void zEntSimpleObj_Init(void* ent, void* asset);
void zLightInit(void* b, void* tasset);
void zEnvInit(void* env, void* easset);
void zCutsceneMgrInit(void* b, void* tasset);
void zEGenerator_Init(void* egen, void* asset);

static zSceneLevel sLevelTable[13] = {
    { "Fishing Village", 'F', 0, 50 },
    { "Lighthouse", 'L', 1, 50 },
    { "Shipwreck", 'W', 2, 50 },
    { "Hedge Maze", 'E', 3, 50 },
    { "Cliffs & Caves", 'C', 4, 50 },
    { "Cemetery", 'G', 5, 50 },
    { "Secret Passages", 'P', 6, 50 },
    { "Cellars", 'B', 7, 50 },
    { "Super Secret Lab", 'S', 8, 50 },
    { "1st Floor", 'I', 9, 50 },
    { "Attic & Balconies", 'R', 10, 50 },
    { "Tower", 'O', 11, 50 },
    { "Hub", 'H', 12, 50 },
};

static zSceneArea sAreaTable[30] = {
    { "", eSceneArea_Unknown, 999, 999, 999, { 0, 0, 0, 0 } },
    { "The Mystery Machine", eSceneArea_H001, 118, 118, 1, { 'H001', 'H002', 'H003', 0 } },
    { "On Edge in the Hedge!", eSceneArea_E001, 371, 371, 2, { 'E001', 'E002', 'E003', 'E004' } },
    { "It's a Mean Greenhouse, Scooby!", eSceneArea_E005, 143, 143, 0, { 'E005', 'E006', 0, 0 } },
    { "Chills & Spills on Haunted Hill!", eSceneArea_E007, 309, 309, 0, { 'E007', 'E008', 'E009', 0 } },
    { "Scared Stiff at Skull Cliff!", eSceneArea_C001, 394, 394, 0, { 'C001', 'C002', 'C003', 'C004' } },
    { "Misbehavin'? Cause a Cave In", eSceneArea_C005, 257, 257, 1, { 'C005', 'C006', 'C007', 0 } },
    { "A Tight Spot for a Grave Plot!", eSceneArea_G001, 116, 116, 1, { 'G001', 'G002', 0, 0 } },
    { "A One Way Trip to the Crypt!", eSceneArea_G003, 224, 224, 1, { 'G003', 'G004', 'G005', 'G006' } },
    { "Gloom & Doom Down in the Tomb!", eSceneArea_G007, 264, 264, 1, { 'G007', 'G008', 'G009', 0 } },
    { "Shock on the Dock!", eSceneArea_F001, 154, 154, 0, { 'F001', 'F003', 0, 0 } },
    { "Fishy Clues for Scooby-Doo?", eSceneArea_F004, 386, 386, 2, { 'F004', 'F005', 'F006', 'F007' } },
    { "Fear on the Pier!", eSceneArea_F008, 202, 202, 0, { 'F008', 'F009', 'F010', 0 } },
    { "Coast for Some Ghosts!", eSceneArea_L011, 373, 373, 2, { 'L011', 'L013', 'L014', 0 } },
    { "Going Down Witch Way?", eSceneArea_L015, 121, 121, 0, { 'L015', 0, 0, 0 } },
    { "Lighthouse Fright House!", eSceneArea_L017, 269, 269, 0, { 'L017', 'L018', 'L019', 0 } },
    { "Wreck on the Deck!", eSceneArea_W020, 268, 268, 1, { 'W020', 'W021', 'W022', 0 } },
    { "Aghast by the Mast!", eSceneArea_W023, 375, 375, 1, { 'W023', 'W025', 'W026', 0 } },
    { "Shiver Your Timbers, Scooby!", eSceneArea_W027, 117, 117, 0, { 'W027', 'W028', 0, 0 } },
    { "Clamor in the Manor!", eSceneArea_I001, 220, 220, 1, { 'I001', 'I020', 'I021', 'I003' } },
    { "Mind Your Manors!", eSceneArea_I004, 228, 228, 1, { 'I004', 'I005', 'I006', 0 } },
    { "All Scares Upstairs!", eSceneArea_R001, 390, 390, 1, { 'R001', 'R020', 'R021', 'R003' } },
    { "Don't Look Down, Scooby-Doo!", eSceneArea_R004, 163, 163, 0, { 'R004', 'R005', 'R006', 0 } },
    { "Panic in the Attic!", eSceneArea_O001, 342, 342, 2, { 'O001', 'O002', 'O003', 'O004' } },
    { "A Dark and Stormy Knight!", eSceneArea_O005, 240, 240, 0, { 'O005', 'O006', 'O008', 0 } },
    { "Who's Yella' in the Cellar?", eSceneArea_B001, 205, 205, 1, { 'B001', 'B002', 'B003', 'B004' } },
    { "Creepy Crawlies in the Hallways", eSceneArea_P001, 165, 165, 0, { 'P001', 'P002', 'P003', 0 } },
    { "Gusts Abound Underground", eSceneArea_P004, 110, 110, 0, { 'P004', 'P005', 0, 0 } },
    { "Little Lab of Horrors", eSceneArea_S001, 160, 160, 2, { 'S001', 'S002', 'S003', 0 } },
    { "Mastermind Unmasked!", eSceneArea_S004, 64, 64, 0, { 'S004', 'S005', 0, 0 } },
};

static zSceneObjectInstanceDesc sInitTable[34] = {
    { "Trig", 1, 'TRIG', 0xEC, zSceneInitFunc_DefaultEnt, zEntTriggerInit },
    { "Move Point", 13, 'MVPT', 0x34, zSceneInitFunc_MovePoint, NULL },
    { "Pickup", 4, 'PKUP', 0x168, zSceneInitFunc_DefaultEnt, zEntPickupInit },
    { "Platform", 6, 'PLAT', 0x178, zSceneInitFunc_DefaultEnt, zPlatform_Init },
    { "Pendulum", 18, 'PEND', 0x164, zSceneInitFunc_DefaultEnt, zPendulum_Init },
    { "Hanger", 23, 'HANG', 0x144, zSceneInitFunc_DefaultEnt, zEntHangable_Init },
    { "Simple", 11, 'SIMP', 0xF4, zSceneInitFunc_DefaultEnt, zEntSimpleObj_Init },
    { "DestructObj", 27, 'DSTR', 0x100, zSceneInitFunc_DefaultEnt, zEntDestructObj_Init },
    { "Villain", 2, 'NPC ', 0x0, zSceneInitFunc_Villain, NULL },
    { "Player", 3, 'PLYR', 0xEC, zSceneInitFunc_Player, NULL },
    { "Timer", 14, 'TIMR', 0x1C, zSceneInitFunc_Default, zTimerInit },
    { "Counter", 22, 'CNTR', 0x18, zSceneInitFunc_Default, zCounterInit },
    { "SFX", 19, 'SFX ', 0x18, zSceneInitFunc_Default, zSFXInit },
    { "Group", 17, 'GRUP', 0x14, zSceneInitFunc_Default, zGroupInit },
    { "Portal", 16, 'PORT', 0x14, zSceneInitFunc_Default, zPortalInit },
    { "Camera", 7, 'CAM ', 0xA0, zSceneInitFunc_Camera, NULL },
    { "Button", 24, 'BUTN', 0x170, zSceneInitFunc_DefaultEnt, zEntButton_Init },
    { "Surface", 26, 'SURF', 0x28, zSceneInitFunc_Surface, NULL },
    { "Gust", 28, 'GUST', 0x24, zSceneInitFunc_Gust, NULL },
    { "Volume", 29, 'VOLU', 0x14, zSceneInitFunc_Volume, NULL },
    { "Conditional", 31, 'COND', 0x14, zSceneInitFunc_Default, zConditionalInit },
    { "Lob Master", 35, 'LOBM', 0x0, zSceneInitFunc_LobMaster, NULL },
    { "Env", 5, 'ENV ', 0x14, zSceneInitFunc_Default, zEnvInit },
    { "Dispatcher", 30, 'DPAT', 0x0, zSceneInitFunc_Dispatcher, NULL },
    { "UI", 32, 'UI  ', 0xF8, zSceneInitFunc_DefaultEnt, zUI_Init },
    { "UI Font", 33, 'UIFT', 0x100, zSceneInitFunc_Default, zUIFont_Init },
    { "Fog", 36, 'FOG ', 0x14, zSceneInitFunc_Default, zFogInit },
    { "Light", 37, 'LITE', 0x64, zSceneInitFunc_Default, zLightInit },
    { "ParticleSystem", 39, 'PARS', 0x28, zSceneInitFunc_Default, zParSysInit },
    { "ParticleEmitter", 38, 'PARE', 0x50, zSceneInitFunc_Default, (void (*)(void*, void*))zParEmitterInit },
    { "CutSceneMgr", 40, 'CSNM', 0x24, zSceneInitFunc_Default, zCutsceneMgrInit },
    { "EGenerator", 41, 'EGEN', 0x11C, zSceneInitFunc_DefaultEnt, zEGenerator_Init },
    { "Script", 42, 'SCRP', 0x1C, zSceneInitFunc_Default, zScriptInit },
    { NULL, 0, 0, 0x0, NULL, NULL },
};

static S32 sMemDepthSceneStart = -1;
static S32 scobj_size = -1;
static S32 nidbps = -1;

static S32 roomCnt;
static S32 bytesNeeded;
static S32 availOnDisk;
static S32 lbl_8025F0AC;
static S32 lbl_8025F0B0;
static S32 lbl_8025F0B4;
static IDBasePair* scobj_idbps;
static S32 lbl_8025F0C0;
static F32 lbl_8025F0B8;
static F32 lbl_8025F0BC;
static F32 lbl_8025F0C4;
static F32 lbl_8025F0C8;

// Open addressing on the low bits of the id; scobj_size is a power of two and an
// id of 0 marks an empty slot.
static xBase* zSceneObjHashtableGet(U32 id)
{
    S32 k, chkd;

    chkd = id & (scobj_size - 1);

    for (k = 0; k < scobj_size; k++)
    {
        IDBasePair* idbp = &scobj_idbps[chkd];

        if (idbp->id == id)
        {
            return idbp->base;
        }

        if (idbp->id == 0)
        {
            return NULL;
        }

        chkd++;

        if (chkd == scobj_size)
        {
            chkd = 0;
        }
    }

    return NULL;
}

static void zSceneObjHashtableAdd(U32 id, xBase* base)
{
    S32 k, chkd;

    chkd = id & (scobj_size - 1);

    for (k = 0; k < scobj_size; k++)
    {
        IDBasePair* idbp = &scobj_idbps[chkd];

        if (idbp->id == 0)
        {
            idbp->id = id;
            idbp->base = base;
            nidbps++;
            break;
        }

        chkd++;

        if (chkd == scobj_size)
        {
            chkd = 0;
        }
    }
}

static void zSceneObjHashtableInit(S32 count)
{
    scobj_idbps = (IDBasePair*)xMemAlloc(gActiveHeap, count * sizeof(IDBasePair));

    memset(scobj_idbps, 0, count * sizeof(IDBasePair));

    scobj_size = count;
    nidbps = 0;
}

static void zSceneObjHashtableExit()
{
    scobj_idbps = NULL;
    scobj_size = -1;
    nidbps = -1;
}

static void zSceneSet(xBase* b, U32 index)
{
    globals.sceneCur->base[index] = b;
}

static U32 zSceneInitFunc_DefaultEnt(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    U8* block;
    S32 count;
    U32 assetSize, offset;
    xBase* b;

    block = NULL;
    assetSize = 0;
    offset = desc->sizeRuntime;
    count = s->baseCount[desc->baseType];

    if (count)
    {
        block = (U8*)xMemAlloc(gActiveHeap, count * offset);

        s->baseList[desc->baseType] = (xBase*)block;
    }

    for (S32 i = 0; i < count; i++)
    {
        void* asset = xSTFindAssetByType(desc->assetType, i, &assetSize);
        b = (xBase*)(block + i * offset);

        zSceneSet(b, base_idx);

        if (desc->objectInitFunc)
        {
            desc->objectInitFunc(b, asset);
        }

        xSceneAddEnt(s, s->ents[base_idx]);
        zSceneObjHashtableAdd(b->id, b);

        base_idx++;
    }

    return base_idx;
}

static U32 zSceneInitFunc_Default(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    U8* block;
    S32 count;
    U32 assetSize, offset;
    xBase* b;

    block = NULL;
    assetSize = 0;
    offset = desc->sizeRuntime;
    count = s->baseCount[desc->baseType];

    if (count)
    {
        block = (U8*)xMemAlloc(gActiveHeap, count * offset);

        s->baseList[desc->baseType] = (xBase*)block;
    }

    for (S32 i = 0; i < count; i++)
    {
        void* asset = xSTFindAssetByType(desc->assetType, i, &assetSize);
        b = (xBase*)(block + i * offset);

        zSceneSet(b, base_idx);

        if (desc->objectInitFunc)
        {
            desc->objectInitFunc(b, asset);
        }

        zSceneObjHashtableAdd(b->id, b);

        base_idx++;
    }

    return base_idx;
}

static U32 zSceneInitFunc_MovePoint(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;
    U32 assetSize;
    _zMovePoint* movpBlock;

    assetSize = 0;
    count = s->baseCount[desc->baseType];
    movpBlock = zMovePoint_GetMemPool(count);

    if (movpBlock)
    {
        s->baseList[desc->baseType] = movpBlock;

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = zMovePoint_GetInst(idx);
            p2BaseAsset* basset = (p2BaseAsset*)xSTFindAssetByType('MVPT', idx, &assetSize);

            zSceneSet(b, base_idx);
            zMovePointInit(zMovePoint_GetInst(idx), (p2MovePointAsset*)basset);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

static U32 zSceneInitFunc_Villain(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;

    count = s->baseCount[desc->baseType];

    if (count)
    {
        st_zEntVillain* villainBlock = zEntVillain_MemPool(count);
        s->baseList[desc->baseType] = (xBase*)villainBlock;

        for (U32 idx = 0; idx < count; idx++)
        {
            _zEnt* b = (_zEnt*)zEntVillain_NPCInst(idx);
            p2EntAsset* asset = (p2EntAsset*)xSTFindAssetByType('NPC ', idx, NULL);

            zSceneSet(b, base_idx);
            zEntVillain_Init((st_zEntVillain*)b, asset);
            xSceneAddEnt(s, s->ents[base_idx]);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

// Every player slot points at the one global player; only the last is added to the scene.
static U32 zSceneInitFunc_Player(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;
    _zEnt* entBlock;

    count = s->baseCount[desc->baseType];

    if (count)
    {
        entBlock = (_zEnt*)xMemAlloc(gActiveHeap, count * sizeof(_zEnt));
        s->baseList[desc->baseType] = entBlock;

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = &globals.player.ent;
            p2EntAsset* asset;

            zSceneSet(b, base_idx);

            if (idx == count - 1)
            {
                xSceneAddEnt(s, s->ents[base_idx]);
            }

            asset = (p2EntAsset*)xSTFindAssetByType('PLYR', idx, NULL);

            globals.player.ent.id = asset->id;

            zSceneObjHashtableAdd(asset->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

// The camera markers live in a block owned by the main camera; each marker also
// registers its asset with that camera.
static U32 zSceneInitFunc_Camera(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;
    _tagxCam* camBlock;

    count = s->baseCount[desc->baseType];

    if (count)
    {
        camBlock = xCameraInit(&globals.camera, count);
        s->baseList[desc->baseType] = camBlock;

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = &camBlock[idx];
            xCamAsset* assetCam = (xCamAsset*)xSTFindAssetByType('CAM ', idx, NULL);

            zSceneSet(b, base_idx);
            xCameraSetAsset(&globals.camera, assetCam, idx);
            zCameraInit(b, assetCam);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

static U32 zSceneInitFunc_Surface(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;

    count = s->baseCount[desc->baseType];

    zSurfaceInit();

    if (count)
    {
        s->baseList[desc->baseType] = xSurfaceGetByIdx(0);

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = xSurfaceGetByIdx(idx);

            zSceneSet(b, base_idx);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

static U32 zSceneInitFunc_Gust(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;

    count = s->baseCount[desc->baseType];

    zGustInit();

    if (count)
    {
        s->baseList[desc->baseType] = zGustGetGust(0);

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = zGustGetGust(idx);

            zSceneSet(b, base_idx);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

static U32 zSceneInitFunc_Volume(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;

    count = s->baseCount[desc->baseType];

    zVolumeInit();

    if (count)
    {
        s->baseList[desc->baseType] = zVolumeGetVolume(0);

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = zVolumeGetVolume(idx);

            zLightSetVolume((_zVolume*)b);

            zSceneSet(b, base_idx);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

static U32 zSceneInitFunc_LobMaster(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;

    count = s->baseCount[desc->baseType];

    if (count)
    {
        st_LOBMASTER_DATA* lm_pool = zLobMaster_memPool(count);
        s->baseList[desc->baseType] = (xBase*)lm_pool;

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = (xBase*)zLobMaster_getInst(lm_pool, idx);
            p2EntAsset* asset = (p2EntAsset*)xSTFindAssetByType('LOBM', idx, NULL);

            zSceneSet(b, base_idx);
            zLobMaster_Init((st_LOBMASTER_DATA*)b, asset);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

static U32 zSceneInitFunc_Dispatcher(zScene* s, zSceneObjectInstanceDesc* desc, U32 base_idx)
{
    S32 count;

    count = s->baseCount[desc->baseType];

    if (count)
    {
        st_ZDISPATCH_DATA* dpat_pool = zDispatcher_memPool(count);
        s->baseList[desc->baseType] = dpat_pool;

        for (S32 idx = 0; idx < count; idx++)
        {
            xBase* b = zDispatcher_getInst(dpat_pool, idx);
            p2EntAsset* asset = (p2EntAsset*)xSTFindAssetByType('DPAT', idx, NULL);

            zSceneSet(b, base_idx);
            zDispatcher_Init((st_ZDISPATCH_DATA*)b, asset);
            zSceneObjHashtableAdd(b->id, b);

            base_idx++;
        }
    }

    return base_idx;
}

// Clears the per-room counts and the collected totals, and loads each level's clue total.
void zSceneGameStats_Init()
{
    globals.stats.scares = 0;
    globals.stats.specials = 0;
    globals.stats.specials_tot = 0;
    globals.stats.tokens = 0;
    globals.stats.tokens_tot = 0;
    globals.stats.snacks = 0;
    globals.stats.snacks_tot = 0;
    globals.stats.chances = 0;
    globals.stats.chances_tot = 0;
    globals.stats.keys = 0;
    globals.stats.keys_tot = 0;
    globals.stats.clues = 0;
    globals.stats.clues_tot = 0;
    globals.stats.roomTime = 0.0f;

    for (S32 i = 0; i < 13; i++)
    {
        globals.levelStats[i].clues_tot = sLevelTable[i].totalClues;
    }
}

// Loads a scene: reads its assets behind the transition screen, then creates every
// object through sInitTable and starts the per-scene systems.
void zSceneInit(U32 theSceneID)
{
    F32 pdone;
    zScene* s;
    S32 i;
    U8 rgba_bkgrd[4] = { 15, 15, 15, 0 };
    U32 typeCount;
    U32 base_idx;
    S32 projectile_count;
    zProjectileTypeAsset* tmp_ptass;
    S32 idx;

    pdone = 0.0f;

    gTransitionSceneID = theSceneID;
    gOccludeCount = 0;

    char b[5] = {};

    sprintf(b, xUtil_idtag2string(theSceneID, 0));
    xStrupr(b);

    theSceneID = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];

    xUtil_idtag2string(theSceneID, 0);

    sMemDepthSceneStart = xMemPushBase();

    zSceneGameStats_Init();
    zHud_sceneReset();
    fn_80016600();

    zGameScreenTransitionBegin();
    zGameScreenTransitionUpdate(0.0f, "...preparing to preload\n");

    s = globals.sceneCur = (zScene*)xMemAlloc(gActiveHeap, sizeof(zScene));

    xSceneInit(s, 205, 210, 230, 50);

    s->env = (_xEnv*)xMemAlloc(gActiveHeap, sizeof(_xEnv));
    s->sceneID = theSceneID;

    zGameScreenTransitionUpdate(0.0f, "... scene preload ...\n");

    xUtil_idtag2string(theSceneID, 0);
    xSTPreLoadScene(theSceneID, NULL);

    xUtil_idtag2string(theSceneID, 0);
    xSTQueueSceneAssets(theSceneID);

    xUtil_idtag2string(theSceneID, 0);
    zGameScreenTransitionUpdate(0.0f, "... scene asset queue ...\n");

    xUtil_idtag2string(theSceneID, 0);

    do
    {
        rgba_bkgrd[0] = 0;
        rgba_bkgrd[1] = 0;
        rgba_bkgrd[2] = 0;
        rgba_bkgrd[3] = 0;

        zGameScreenTransitionUpdate(pdone, "... scene loading ...\n", rgba_bkgrd);

        pdone = xSTLoadStep(theSceneID);
    } while (pdone < 1.0f);

    xUtil_idtag2string(theSceneID, 0);
    zGameScreenTransitionUpdate(pdone, "...initializing scene - sound\n");

    iSndInitSceneLoaded();
    xPadRumbleEnable(0, globals.option_vibration);
    xSTSwitchScene(theSceneID, NULL, NULL);

    globals.pickupTable = (zAssetPickupTable*)xSTFindAssetByType('PICK', 0, NULL);
    globals.sceneCur->resolvID = zSceneFindObject;
    globals.sceneCur->id2Name = zSceneGetName;
    globals.sceneCur->base2Name = zSceneGetName;

    g_xSceneCur = globals.sceneCur;

    zDispatcher_scenePrepare();
    zEntVillain_scenePrepare();
    zEntProjectile_scenePrepare();
    zLobMaster_scenePrepare();
    zAnimListInit();

    zGameScreenTransitionUpdate(100.0f, "...initializing scene - base types\n");

    for (i = 0; i < 43; i++)
    {
        s->baseCount[i] = 0;
        s->baseList[i] = NULL;
    }

    s->num_base = 0;

    for (i = 0; sInitTable[i].name; i++)
    {
        typeCount = xSTAssetCountByType(sInitTable[i].assetType);
        s->baseCount[sInitTable[i].baseType] = typeCount;
        s->num_base += typeCount;
    }

    if (s->num_base)
    {
        s->base = (xBase**)xMemAlloc(gActiveHeap, s->num_base * sizeof(xBase*));
    }
    else
    {
        s->base = NULL;
    }

    zSceneObjHashtableInit(2048);

    xFFXPoolInit(12);
    xFFXShakePoolInit(3);
    xFFXRotMatchPoolInit(1);
    xEntSceneInit();
    xEntMotionDebugInit(s->baseCount[6] + s->baseCount[18] + s->baseCount[24]);
    zLightSetVolume(NULL);
    xPartitionReset();

    base_idx = 0;

    for (i = 0; sInitTable[i].name; i++)
    {
        if (sInitTable[i].func)
        {
            base_idx = sInitTable[i].func(s, &sInitTable[i], base_idx);
        }
    }

    zGameScreenTransitionEnd();

    projectile_count = xSTAssetCountByType('PRJT');

    if (projectile_count)
    {
        zEntProjectile_prepSystem(projectile_count, s);

        for (idx = 0; idx < projectile_count; idx++)
        {
            tmp_ptass = (zProjectileTypeAsset*)xSTFindAssetByType('PRJT', idx, NULL);

            if (tmp_ptass)
            {
                zEntProjectile_registerType(tmp_ptass);
            }
        }
    }

    zLightResolveLinks();
    zRenderStateInit();
    zFeetGetIDs();
    zHologramInit();
    zLightningInit();
    zParCmdFindClipVolumes();
    zEntDestructObj_FindFX();
    zActionLineInit();
    xScrFxLetterbox(0);
    xClimateInit(&gClimate);
    iSndSuspendCD(0);
}

// Silences every sound attached to an object; a NULL object ends the walk.
static xBase* zSceneExitSoundIteratorCB(xBase* b, zScene*, void*)
{
    xSndParentDied((U32)b);
    return b;
}

// Stops at the first callback that returns NULL.
static void zSceneForAllBase(xBase* (*func)(xBase*, zScene*, void*), void* data)
{
    zScene* s = globals.sceneCur;

    if (s)
    {
        for (U16 i = 0; i < s->num_base; i++)
        {
            if (!func(s->base[i], s, data))
            {
                break;
            }
        }
    }
}

// Tears the scene down: sounds first, then the room/scene end events, the save of the
// scene state, the object systems, and finally the scene's assets and memory.
void zSceneExit()
{
    zScene* s = globals.sceneCur;
    RpWorld* world;
    S32 i;

    zSceneForAllBase(zSceneExitSoundIteratorCB, NULL);
    xSndStopAll();
    xSndUpdate(0.0f);
    iSndWaitForDeadSounds();
    iSndSceneExit();
    iSndSuspendCD(1);
    iFuncProfileDump();

    world = s->env->geom->world;

    xModel_SceneExit(world);
    zFX_SceneExit(world);
    zEntEventAll(NULL, 0, 0x5A, NULL);
    zEntEventAll(NULL, 0, 0x58, NULL);

    gOccludeCount = 0;

    if (globals.sceneCur->sceneID != 'MNU3')
    {
        zSceneSave(s, NULL);
    }

    zEntPlayerExit(&globals.player.ent);
    zSurfaceExit();
    zLightDestroyAll();
    xEntSceneExit();
    xEntMotionDebugExit();
    zSceneObjHashtableExit();

    if (s->baseCount[39])
    {
        zParSys* ps = (zParSys*)s->baseList[39];

        for (i = 0; i < (S32)s->baseCount[39]; i++)
        {
            if (ps[i].baseFlags & 0x4)
            {
                zParSysExit(&ps[i]);
            }
        }
    }

    zAnimListExit();
    zLobMaster_sceneFinish();
    zEntProjectile_sceneFinish();
    zEntVillain_sceneFinish();
    zDispatcher_sceneFinish();

    xSTUnLoadScene(globals.sceneCur->sceneID);
    xMemPopBase(sMemDepthSceneStart);

    sMemDepthSceneStart = -1;

    xUtil_idtag2string(globals.sceneCur->sceneID, 0);

    globals.sceneCur = NULL;
}

// Portal scene ids may be stored byte-reversed; a scene id always ends in a digit.
static void HackSwapIt(char* buf, S32 size)
{
    char* end = size + buf;
    end--;

    for (S32 i = 0; i < size / 2; i++)
    {
        char tmp = *buf;
        *buf = *end;
        *end = tmp;

        buf++;
        end--;
    }
}

// A portal into the current scene just moves the player to its marker and points the
// camera there; any other portal stays pending for the scene change.
void zSceneSwitch(_zPortal* p)
{
    zPortalAsset* passet;
    char* id;
    U32 PlayerMarkerStartID;
    U32 PlayerMarkerStartCamID;
    _tagxCam* cam;
    U32 size;
    p2MarkerAsset* m;

    globals.sceneCur->pendingPortal = p;

    passet = globals.sceneCur->pendingPortal->passet;

    id = (char*)&passet->sceneID;

    if (id[3] < '0' || id[3] > '9')
    {
        HackSwapIt(id, 4);
    }

    U32 sid = (id[0] << 24) | (id[1] << 16) | (id[2] << 8) | id[3];

    if (sid == globals.sceneCur->sceneID)
    {
        PlayerMarkerStartCamID = passet->assetCameraID;
        PlayerMarkerStartID = passet->assetMarkerID;

        cam = (_tagxCam*)zSceneObjHashtableGet(PlayerMarkerStartCamID);

        if (cam)
        {
            xCameraPickInitial(&globals.camera, cam);
            globals.player.cp.initCamID = PlayerMarkerStartCamID;
        }

        m = (p2MarkerAsset*)xSTFindAsset(PlayerMarkerStartID, &size);

        if (m)
        {
            xVec3Copy(&globals.player.ent.frame->mat.pos, &m->pos);
            xVec3Copy((xVec3*)&globals.player.ent.model->Mat->pos, &m->pos);

            xCameraSetTarget(&globals.camera, &globals.player.ent.frame->mat.pos);
            xCameraSetTarget(&globals.camera, (xMat4x3*)globals.player.ent.model->Mat);
        }

        globals.sceneCur->pendingPortal = NULL;
    }
}

// Saves the player's inventory and progress under 'PLYR', then the scene and every
// object flagged for saving under the scene's id. Base types follow sInitTable.
void zSceneSave(zScene* ent, xSerial* s)
{
    S32 i;
    xSerial xser;
    xBase* b;

    s = &xser;

    s->setClient('PLYR');
    s->Write_b1(1);
    s->Write(globals.player.Inv_Chances);
    s->Write(globals.player.Inv_ScoobySnacks);
    s->Write(globals.player.Inv_Clue);
    s->Write(globals.player.Inv_Key);
    s->Write(globals.player.Inv_Mtokens[0]);

    for (i = 0; i < 2; i++)
    {
        s->Write(globals.player.Inv_Ammo[i]);
    }

    for (i = 0; i < 2; i++)
    {
        s->Write(globals.player.Inv_AmmoMax[i]);
    }

    s->Write(globals.player.Inv_Specials);
    s->Write(globals.stats.snacks_tot);
    s->Write(globals.stats.tokens_tot);

    for (i = 0; i < 30; i++)
    {
        s->Write(globals.areaStats[i].snacks_tot);
        s->Write(globals.areaStats[i].tokens_tot);
    }

    for (i = 0; i < 26; i++)
    {
        if (zEntPickup_WarpIsOpen(i))
        {
            s->Write_b1(1);
        }
        else
        {
            s->Write_b1(0);
        }
    }

    zCutsceneMgrSave(NULL, s);
    zVarSave(s);

    s->setClient(ent->sceneID);
    s->Write_b1(1);

    xSceneSave(ent, s);

    s->Write(++globals.stats.visited);
    s->Write(globals.stats.scares_sum);
    s->Write(globals.stats.deaths_sum);

    for (i = 0; i < (S32)globals.sceneCur->num_base; i++)
    {
        b = globals.sceneCur->base[i];

        if (b->baseFlags & 0x2)
        {
            switch (b->baseType)
            {
            case 1:
                zEntTriggerSave((zEntTrigger*)b, s);
                break;
            case 2:
                zEntVillain_Save((st_zEntVillain*)b, s);
                break;
            case 4:
                zEntPickup_Save((_zEntPickup*)b, s);
                break;
            case 5:
                zEnvSave((_zEnv*)b, s);
                break;
            case 36:
                zFogSave((_zFog*)b, s);
                break;
            case 37:
                zLightSave((_zLight*)b, s);
                break;
            case 6:
                zPlatform_Save((_zPlatform*)b, s);
                break;
            case 7:
                zCameraSave((_tagxCam*)b, s);
                break;
            case 11:
                zEntSimpleObj_Save((_zEntSimpleObj*)b, s);
                break;
            case 13:
                zMovePointSave((_zMovePoint*)b, s);
                break;
            case 14:
                zTimerSave((_zTimer*)b, s);
                break;
            case 16:
                zPortalSave((_zPortal*)b, s);
                break;
            case 17:
                zGroupSave((_zGroup*)b, s);
                break;
            case 18:
                zPendulum_Save((_zPendulum*)b, s);
                break;
            case 19:
                zSFXSave((zSFX*)b, s);
                break;
            case 22:
                zCounterSave((_zCounter*)b, s);
                break;
            case 23:
                zEntHangable_Save((_zEntHangable*)b, s);
                break;
            case 24:
                zEntButton_Save((_zEntButton*)b, s);
                break;
            case 26:
                zSurfaceSave((xSurface*)b, s);
                break;
            case 27:
                zEntDestructObj_Save((_zEntDestructObj*)b, s);
                break;
            case 28:
                zGustSave((_zGust*)b, s);
                break;
            case 42:
                zScriptSave((_zScript*)b, s);
                break;
            case 29:
                zVolumeSave((_zVolume*)b, s);
                break;
            case 30:
                zDispatcher_Save((st_ZDISPATCH_DATA*)b, s);
                break;
            case 31:
                zConditionalSave((_zConditional*)b, s);
                break;
            case 32:
                zUI_Save((_zUI*)b, s);
                break;
            case 33:
                zUIFont_Save((_zUIFont*)b, s);
                break;
            case 41:
                zEGenerator_Save((zEGenerator*)b, s);
                break;
            }
        }
    }
}

// Restores what zSceneSave wrote. A scene without saved state starts its running
// totals over.
void zSceneLoad(zScene* ent, xSerial* s)
{
    xSerial xser;
    S32 sceneExist;
    S32 i;
    S32 wpset;
    xBase* b;

    s = &xser;

    s->setClient('PLYR');

    sceneExist = 0;
    s->Read_b1(&sceneExist);

    if (sceneExist)
    {
        s->Read(&globals.player.Inv_Chances);
        s->Read(&globals.player.Inv_ScoobySnacks);
        s->Read(&globals.player.Inv_Clue);
        s->Read(&globals.player.Inv_Key);
        s->Read(&globals.player.Inv_Mtokens[0]);

        for (i = 0; i < 2; i++)
        {
            s->Read(&globals.player.Inv_Ammo[i]);
        }

        for (i = 0; i < 2; i++)
        {
            s->Read(&globals.player.Inv_AmmoMax[i]);
        }

        s->Read(&globals.player.Inv_Specials);
        s->Read(&globals.stats.snacks_tot);
        s->Read(&globals.stats.tokens_tot);

        for (i = 0; i < 30; i++)
        {
            s->Read(&globals.areaStats[i].snacks_tot);
            s->Read(&globals.areaStats[i].tokens_tot);
        }

        for (i = 0; i < 26; i++)
        {
            wpset = 0;
            s->Read_b1(&wpset);

            if (wpset)
            {
                enableWarpPoint(i);
            }
        }

        zCutsceneMgrLoad(NULL, s);
        zVarLoad(s);
    }

    s->setClient(ent->sceneID);

    sceneExist = 0;
    s->Read_b1(&sceneExist);

    if (sceneExist)
    {
        xSceneLoad(ent, s);

        s->Read(&globals.stats.visited);
        s->Read(&globals.stats.scares_sum);
        s->Read(&globals.stats.deaths_sum);

        for (U16 i = 0; i < globals.sceneCur->num_base; i++)
        {
            b = globals.sceneCur->base[i];

            if (b->baseFlags & 0x2)
            {
                switch (b->baseType)
                {
                case 1:
                    zEntTriggerLoad((zEntTrigger*)b, s);
                    break;
                case 2:
                    zEntVillain_Load((st_zEntVillain*)b, s);
                    break;
                case 3:
                    zEntPlayer_Load((_xEnt*)b, s);
                    break;
                case 4:
                    zEntPickup_Load((_zEntPickup*)b, s);
                    break;
                case 5:
                    zEnvLoad((_zEnv*)b, s);
                    break;
                case 36:
                    zFogLoad((_zFog*)b, s);
                    break;
                case 37:
                    zLightLoad((_zLight*)b, s);
                    break;
                case 6:
                    zPlatform_Load((_zPlatform*)b, s);
                    break;
                case 7:
                    zCameraLoad((_tagxCam*)b, s);
                    break;
                case 11:
                    zEntSimpleObj_Load((_zEntSimpleObj*)b, s);
                    break;
                case 13:
                    zMovePointLoad((_zMovePoint*)b, s);
                    break;
                case 14:
                    zTimerLoad((_zTimer*)b, s);
                    break;
                case 16:
                    zPortalLoad((_zPortal*)b, s);
                    break;
                case 17:
                    zGroupLoad((_zGroup*)b, s);
                    break;
                case 18:
                    zPendulum_Load((_zPendulum*)b, s);
                    break;
                case 19:
                    zSFXLoad((zSFX*)b, s);
                    break;
                case 22:
                    zCounterLoad((_zCounter*)b, s);
                    break;
                case 23:
                    zEntHangable_Load((_zEntHangable*)b, s);
                    break;
                case 24:
                    zEntButton_Load((_zEntButton*)b, s);
                    break;
                case 26:
                    zSurfaceLoad((xSurface*)b, s);
                    break;
                case 27:
                    zEntDestructObj_Load((_zEntDestructObj*)b, s);
                    break;
                case 28:
                    zGustLoad((_zGust*)b, s);
                    break;
                case 42:
                    zScriptLoad((_zScript*)b, s);
                    break;
                case 29:
                    zVolumeLoad((_zVolume*)b, s);
                    break;
                case 30:
                    zDispatcher_Load((st_ZDISPATCH_DATA*)b, s);
                    break;
                case 31:
                    zConditionalLoad((_zConditional*)b, s);
                    break;
                case 32:
                    zUI_Load((_zUI*)b, s);
                    break;
                case 33:
                    zUIFont_Load((_zUIFont*)b, s);
                    break;
                case 41:
                    zEGenerator_Load((zEGenerator*)b, s);
                    break;
                }
            }
        }
    }
    else
    {
        globals.stats.visited = 1;
        globals.stats.roomTime_sum = 0.0f;
        globals.stats.timeRunning_sum = 0.0f;
        globals.stats.timeWalking_sum = 0.0f;
        globals.stats.scares_sum = 0;
        globals.stats.deaths_sum = 0;
        globals.stats.singleJumps_sum = 0;
        globals.stats.doubleJumps_sum = 0;
        globals.stats.gumShot_sum = 0;
        globals.stats.gumEmpty_sum = 0;
        globals.stats.soapShot_sum = 0;
        globals.stats.soapHit_sum = 0;
        globals.stats.soapEmpty_sum = 0;
        globals.stats.food_sum = 0;
        globals.stats.snackBoxes_sum = 0;
        globals.stats.enemyKills_sum = 0;
    }
}

// Marks the "<scene> VISITED" UI object of the current scene as visited.
static void zSceneEnableVisited(zScene* s)
{
    char uiName[64];
    U32 uiNameID;

    strcpy(uiName, xUtil_idtag2string(s->sceneID, 0));

    uiName[1] = ' ';

    strcat(uiName, " VISITED");

    uiNameID = xStrHash(uiName);

    for (S32 i = 0; i < (S32)s->num_base; i++)
    {
        if (s->base[i] && s->base[i]->baseType == 32 && uiNameID == s->base[i]->id)
        {
            s->base[i]->baseFlags |= 0x1;
        }
    }
}

// Restarts the scene from its last saved state: every object gets its reset, then the
// saved state is loaded back and the scene and room begin again.
void zSceneReset()
{
    zScene* s;
    S32 i;
    _zEnv* env;
    _tagxCam* cam;

    gOccludeCount = 0;

    s = globals.sceneCur;

    zMusicKill();
    xPadDestroyRumbleChain(0);

    globals.cmgr = NULL;

    zEntEventAll(NULL, 0, 0x5A, NULL);
    zEntEventAll(NULL, 0, 0x58, NULL);

    zSceneSave(s, NULL);

    xSndStopAll();
    xSndUpdate(0.0f);
    iSndWaitForDeadSounds();

    zEntProjectile_sceneReset();

    for (i = 0; i < (S32)s->num_base; i++)
    {
        if (s->base[i])
        {
            switch (s->base[i]->baseType)
            {
            default:
                zEntEvent(NULL, 0, s->base[i], 10, NULL, NULL, 1);
                break;
            case 22:
                zCounterReset(s->base[i]);
                break;
            case 3:
                zEntPlayerReset((_xEnt*)s->base[i]);
                break;
            case 6:
                zPlatform_Reset((_zPlatform*)s->base[i], globals.sceneCur);
                break;
            case 18:
                zPendulum_Reset((_zPendulum*)s->base[i], globals.sceneCur);
                break;
            case 23:
                zEntHangable_Reset((_zEntHangable*)s->base[i]);
                break;
            case 35:
                zEntEvent(s->base[i], 10);
                break;
            case 41:
                zEGenerator_Reset((zEGenerator*)s->base[i], globals.sceneCur);
                break;
            case 5:
                env = (_zEnv*)s->base[i];

                if (env)
                {
                    cam = (_tagxCam*)zSceneFindObject(env->easset->startCameraAssetID);

                    if (cam)
                    {
                        xCameraPickInitial(&globals.camera, cam);
                    }

                    xClimateInitAsset(&gClimate, env->easset);
                }
                break;
            }
        }
    }

    zMusicNotify(0);

    {
        zScene* sc = globals.sceneCur;

        for (i = 0; i < sc->num_act_ents; i++)
        {
            sc->act_ents[i]->updateTimer = 0.0f;
        }
    }

    zEntShaggy_SceneReset();
    zEntPlayer_LoadCheckPoint();
    zSceneLoad(globals.sceneCur, NULL);
    zSceneEnableVisited(s);
    xSerialTraverse(zSceneSetup_serialTraverseCB);

    zEntEventAll(NULL, 0, 0x57, NULL);
    zEntEventAll(NULL, 0, 0x59, NULL);
}

// Runs once all objects exist: links objects to each other, sets up the scene's
// physics constants and fades the screen in from black.
void zSceneSetup()
{
    zScene* s = globals.sceneCur;

    globals.cmgr = NULL;
    lbl_8025EEC8 = 0;

    xSceneSetup(s);

    s->gravity = -50.0f;
    s->drag = 0.08f;
    s->flags = 0x5;

    zEntBubble_SceneInit(s);

    if (s->baseCount[28])
    {
        zGustSetup();
    }

    if (s->baseCount[29])
    {
        zVolumeSetup();
    }

    gCurEnv = NULL;

    for (S32 i = 0; i < (S32)s->num_base; i++)
    {
        if (s->base[i])
        {
            switch (s->base[i]->baseType)
            {
            case 5:
                gCurEnv = (_zEnv*)s->base[i];
                zEnvSetup(gCurEnv);
                xClimateInitAsset(&gClimate, gCurEnv->easset);
                break;
            case 2:
                zEntVillain_InitDep((st_zEntVillain*)s->base[i], s);
                break;
            case 6:
                zPlatform_Reset((_zPlatform*)s->base[i], s);
                zPlatform_Setup((_zPlatform*)s->base[i], s);
                break;
            case 13:
                zMovePointSetup((_zMovePoint*)s->base[i], s);
                break;
            case 18:
                zPendulum_Reset((_zPendulum*)s->base[i], s);
                zPendulum_Setup((_zPendulum*)s->base[i], s);
                break;
            case 24:
                zEntButton_Reset((_zEntButton*)s->base[i], s);
                zEntButton_Setup((_zEntButton*)s->base[i], s);
                break;
            case 35:
                zLobMaster_InitDep((st_LOBMASTER_DATA*)s->base[i], s);
                break;
            case 11:
                zEntSimpleObj_Setup((_zEntSimpleObj*)s->base[i]);
                break;
            case 30:
                zDispatcher_InitDep((st_ZDISPATCH_DATA*)s->base[i], s);
                break;
            case 41:
                zEGenerator_Setup((zEGenerator*)s->base[i], s);
                zEGenerator_Reset((zEGenerator*)s->base[i], s);
                break;
            case 4:
                zEntPickup_Setup((_zEntPickup*)s->base[i]);
                zEntPickup_Reset((_zEntPickup*)s->base[i]);
                break;
            case 39:
                zParSysSetup((zParSys*)s->base[i]);
                break;
            case 26:
                zSurfaceSetup((xSurface*)s->base[i]);
                break;
            case 38:
                zParEmitterSetup((_zParEmitter*)s->base[i]);
                break;
            case 42:
                break;
            }
        }
    }

    zEntPickup_Setup();
    zEntHangable_SetupFX();

    {
        U32 dontcaresize;
        xCutscene_Init(xSTFindAssetByType('CTOC', 0, &dontcaresize));
    }

    for (S32 i = 0; i < (S32)s->num_base; i++)
    {
        if (s->base[i] && s->base[i]->baseType == 13)
        {
            xMovePointSplineSetup((_xMovePoint*)s->base[i]);
        }
    }

    zSceneEnableVisited(s);
    xSerialTraverse(zSceneSetup_serialTraverseCB);
    xQuickCullInit(xEntGetAllEntsBox());

    lbl_8025F0C0 = 0;

    iColor_tag black = { 0, 0, 0, 255 };
    iColor_tag clear = { 0, 0, 0, 0 };

    xScrFxFade(&black, &clear, 1.0f, NULL, 0, 1);
}

// A set bit in the save sets base flag 0x1 on the object named after the client's id
// tag, its second character replaced by a space, followed by " VISITED".
S32 zSceneSetup_serialTraverseCB(U32 clientID, xSerial* xser)
{
    char uiName[16];
    S32 val = 0;
    xBase* b;

    xser->Read_b1(&val);

    if (val == 0)
    {
        return 1;
    }

    sprintf(uiName, "%s", xUtil_idtag2string(clientID, 0));

    uiName[1] = ' ';

    strcat(uiName, " VISITED");

    b = zSceneFindObject(xStrHash(uiName));

    if (b)
    {
        b->baseFlags |= 0x1;
    }

    return 1;
}

// Steps every active object for one frame. While the game is paused only the UI and
// the "pause_timer" timers keep running.
void zSceneUpdate(F32 elapsedSec)
{
    S32 i;
    S32 isPaused;
    zScene* s;
    xBase** b;

    isPaused = zGameIsPaused();
    gSceneUpdateTime = elapsedSec;
    gFontWiggleTime += elapsedSec;

    zEntPickup_SceneUpdate(elapsedSec);
    xEntSetTimePassed(elapsedSec);

    if (globals.cmgr)
    {
        zCutsceneMgrUpdate((xBase*)globals.cmgr, globals.sceneCur, elapsedSec);
    }

    s = globals.sceneCur;
    b = s->base;

    if (s->baseCount[32])
    {
        _zUI* ui = (_zUI*)s->baseList[32];

        for (i = 0; i < (S32)s->baseCount[32]; i++)
        {
            if (ui[i].baseFlags & 0x4)
            {
                zUI_PreUpdate(&ui[i], s, elapsedSec);
            }
        }
    }

    if (s->baseCount[33])
    {
        _zUIFont* ui = (_zUIFont*)s->baseList[33];

        for (i = 0; i < (S32)s->baseCount[33]; i++)
        {
            if (ui[i].baseFlags & 0x4)
            {
                zUIFont_PreUpdate(&ui[i], s, elapsedSec);
            }
        }
    }

    for (i = 0; i < (S32)s->num_base; i++)
    {
        if (b[i]->baseFlags & 0x4)
        {
            switch (b[i]->baseType)
            {
            case 32:
            case 33:
                if (((_xEnt*)b[i])->update)
                {
                    ((_xEnt*)b[i])->update((_xEnt*)b[i], s, elapsedSec);
                }
                break;
            case 1:
            case 2:
            case 4:
            case 6:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 18:
            case 23:
            case 24:
            case 27:
            case 41:
                if (((_xEnt*)b[i])->update && !isPaused)
                {
                    ((_xEnt*)b[i])->update((_xEnt*)b[i], s, elapsedSec);
                }
                break;
            case 14:
                if (isPaused)
                {
                    static U32 sUITimers[10];
                    static U32 init = 0;
                    S32 timer;

                    if (!init)
                    {
                        char str[32];

                        for (timer = 0; timer < 10; timer++)
                        {
                            sprintf(str, "pause_timer%d", timer);
                            sUITimers[timer] = xStrHash(str);
                        }

                        init = 1;
                    }

                    for (timer = 0; timer < 10; timer++)
                    {
                        if (b[i]->id == sUITimers[timer])
                        {
                            zTimerUpdate(b[i], s, elapsedSec);
                            break;
                        }
                    }
                }
                else
                {
                    zTimerUpdate(b[i], s, elapsedSec);
                }
                break;
            case 42:
                if (!isPaused)
                {
                    zScriptUpdate(b[i], s, elapsedSec);
                }
                break;
            case 36:
                if (!isPaused)
                {
                    zFogUpdate(b[i], s, elapsedSec);
                }
                break;
            case 38:
                if (!isPaused)
                {
                    zParEmitterUpdate(b[i], s, elapsedSec);
                }
                break;
            case 39:
                if (!isPaused)
                {
                    zParSysUpdate(b[i], s, elapsedSec);
                }
                break;
            case 37:
                if (!isPaused)
                {
                    zLightUpdate(b[i], s, elapsedSec);
                }
                break;
            case 26:
                if (!isPaused)
                {
                    zSurfaceUpdate(b[i], s, elapsedSec);
                }
                break;
            case 43:
                break;
            }
        }
    }

    if (!isPaused)
    {
        zHologramUpdate(elapsedSec);
        zActionLineUpdate(elapsedSec);
        zLightningUpdate(elapsedSec);
        zGustUpdateFX(elapsedSec);
        xClimateUpdate(&gClimate, elapsedSec);
        zLobMaster_timestep(s, elapsedSec);
        zEntProjectile_poolUpdate(ZPRJ_CAT_UNKNOWN, s, elapsedSec);

        _zEnt** bubbles = s->bubbles;

        for (i = 0; i < (S32)s->num_bubbles; i++)
        {
            if (bubbles[i]->baseFlags & 0x4)
            {
                bubbles[i]->update(bubbles[i], s, elapsedSec);
            }
        }

        zEntBubble_Cleanup();
        xFXUpdate(elapsedSec);

        globals.stats.roomTime += elapsedSec;
        globals.stats.roomTime_sum += elapsedSec;

        if (s->pendingPortal)
        {
            if (s->pendingPortal->passet->sceneID == '600S' || s->sceneID == 'S006')
            {
                zGameStateSwitch(5);
            }
            else
            {
                zGameStateSwitch(4);
            }
        }
    }
}

// Lets the ground under the player catch the player's shadow: platforms, buttons,
// destructibles and simple objects among the static and dynamic entities.
static void zShadowReceivers()
{
    _xScene* sc = globals.sceneCur;
    _xEnt* ep;
    S32 i;

    for (i = 0; i < sc->num_stats; i++)
    {
        ep = sc->stats[i];

        if (ep->baseType == 6 || ep->baseType == 24 || ep->baseType == 27 || ep->baseType == 11)
        {
            if (xShadowReceiveShadowSetup(ep))
            {
                xShadowReceiveShadow(ep);
            }
        }
    }

    for (i = 0; i < sc->num_dyns; i++)
    {
        ep = sc->dyns[i];

        if (ep->baseType == 6 || ep->baseType == 24 || ep->baseType == 27 || ep->baseType == 11)
        {
            if (xShadowReceiveShadowSetup(ep))
            {
                xShadowReceiveShadow(ep);
            }
        }
    }
}

// Draws one frame of the scene: the environment, the opaque entities, then the
// see-through ones sorted by their distance from the camera, the effects, the HUD and
// the UI. In the intro the memory card warning is drawn over everything.
void zSceneRender()
{
    zScene* s = globals.sceneCur;

    if (s->sceneID == zSceneIDFromName("MNU3"))
    {
        fn_800167F0();
    }

    zRenderState(SDRS_OpaqueModels);
    zLightAddLocalEnv();
    zEnvRender(s->env);
    zLightRemoveLocalEnv();

    if (s->env->geom && s->env->geom->fx)
    {
        xCameraEnd(&globals.camera, 0.0f, 0);
        iEnvBeginRenderFX(NULL);
        xCameraBegin(&globals.camera, 0);
        zLightAddLocalEnv();
        iEnvRenderFX(NULL);
        zLightRemoveLocalEnv();
    }

    _xEnt* entities[64] = {};
    F32 distance[64] = {};
    S32 sorted_entities = 0;

    zRenderState(SDRS_OpaqueModels);

    {
        xVec3* camera_pos = &globals.camera.hi_cam.mat.pos;
        _xEnt** entptr = &s->act_ents[s->num_act_ents - 1];
        _xEnt** entlast = &s->act_ents[s->baseCount[1] + s->baseCount[4]];
        U32 shadowHackCase = 0;
        _xEnt* tide_ent = NULL;
        U32 tide_hash = xStrHash("TIDE");

        while (entptr >= entlast)
        {
            _xEnt* ent = *entptr;

            if (ent->collType != 1)
            {
                if (ent->id == tide_hash)
                {
                    tide_ent = ent;
                }
                else if ((ent->collType == 0x10 && globals.player.Transparent > 0) ||
                         (ent->flags & 0x4) || ent->material_alpha < 255.0f ||
                         (ent->moreFlags & 0x1))
                {
                    if (globals.cmgr && ent->collType != 0x10)
                    {
                        ent->render(ent);
                    }
                    else
                    {
                        xVec3 centre = *xBoundCenter(&ent->bound);

                        entities[sorted_entities] = ent;
                        F32 dz = camera_pos->z - centre.z;
                        F32 dx = camera_pos->x - centre.x;
                        distance[sorted_entities] = dx * dx + dz * dz;
                        sorted_entities++;
                    }
                }
                else if (ent->collType == 0x10)
                {
                    shadowHackCase = 1;
                }
                else if (ent->render)
                {
                    ent->render(ent);
                }
            }

            entptr--;
        }

        for (U32 i = 0; i < sorted_entities; i++)
        {
            _xEnt* ent = entities[i];

            for (U32 j = 0; j < 66; j++)
            {
                zSceneRenderDist* rd = &lbl_801C82A0[j];

                if (globals.sceneCur->sceneID == rd->sceneID && ent->id == rd->entID)
                {
                    switch (rd->mode)
                    {
                    case 0:
                    {
                        F32 dz = camera_pos->z - rd->z;
                        F32 dx = camera_pos->x - rd->x;
                        distance[i] = dx * dx + dz * dz;
                        break;
                    }
                    case 2:
                        for (U32 k = 0; k < sorted_entities; k++)
                        {
                            if (rd->refID == entities[k]->id)
                            {
                                distance[i] = distance[k] + rd->offset;
                                break;
                            }
                        }
                        break;
                    case 1:
                    default:
                        distance[i] = rd->dist;
                        break;
                    }

                    break;
                }
            }
        }

        zEntPickup_RenderList((_zEntPickup*)s->baseList[4], s->baseCount[4]);

        if (shadowHackCase == 1 && globals.player.Transparent <= 0)
        {
            globals.player.ent.render(&globals.player.ent);
            zShadowReceivers();
        }

        zRenderState(SDRS_OpaqueModels);
        zEntProjectile_poolRender(ZPRJ_CAT_UNKNOWN);

        zRenderState(SDRS_OpaqueModels);
        zEntVillain_BonusRender();

        zRenderState(SDRS_Lightning);
        zLightningRender();
        zActionLineRender();

        zRenderState(SDRS_SkidMark);

        for (S32 i = 0; i < 5; i++)
        {
            zFeetRenderSkidMark(&sPlayerSkidMark[i]);
        }

        zRenderState(SDRS_Bubble);
        zEntBubble_Render();

        if (tide_ent)
        {
            zRenderState(SDRS_Tide);
            tide_ent->render(tide_ent);
        }
    }

    if (sorted_entities > 64)
    {
        sorted_entities = 64;
    }

    for (S32 i = 0; i < sorted_entities; i++)
    {
        for (S32 index2 = 0; index2 < sorted_entities; index2++)
        {
            if (distance[i] > distance[index2])
            {
                _xEnt* swap_e = entities[i];
                F32 swap_d = distance[i];

                distance[i] = distance[index2];
                entities[i] = entities[index2];
                distance[index2] = swap_d;
                entities[index2] = swap_e;
            }
        }
    }

    if (globals.sceneCur->sceneID == zSceneIDFromName("b003"))
    {
        xVec3 pts[14] = {
            { -10.935f, 32.565f, -31.772f }, { -17.635f, 34.286f, -31.771f },
            { -24.938f, 33.703f, -31.795f }, { -31.687f, 33.524f, -32.116f },
            { -38.701f, 33.005f, -32.116f }, { -46.224f, 33.51f, -32.116f },
            { -61.621f, 36.247f, -46.187f }, { -71.097f, 36.108f, -46.674f },
            { -76.251f, 36.426f, -54.49f },  { -76.251f, 36.426f, -61.185f },
            { -69.825f, 36.541f, -68.408f }, { -61.625f, 36.541f, -68.773f },
            { -51.77f, 29.418f, -102.253f }, { -34.7f, 33.968f, -103.255f },
        };
        void* asset = xSTFindAsset(0x2AA06A3A, NULL);

        if (asset)
        {
            fn_80009CCC(pts, 14, *(U32*)asset, 1.5f, 1.5f);
        }
    }
    else if (globals.sceneCur->sceneID == zSceneIDFromName("e004"))
    {
        xVec3 pts[15] = {
            { 336.775f, 10.53f, 68.837f },   { 345.983f, 10.53f, 76.024f },
            { 369.36f, 10.596f, 75.283f },   { 407.768f, 10.55f, 68.261f },
            { 440.203f, 10.592f, 86.134f },  { 458.491f, 10.497f, 85.739f },
            { 475.495f, 10.071f, 93.686f },  { 481.907f, 10.009f, 94.842f },
            { 482.091f, 9.982f, 104.687f },  { 475.658f, 10.075f, 105.788f },
            { 467.437f, 10.51f, 113.27f },   { 467.063f, 10.638f, 144.221f },
            { 442.756f, 10.297f, 144.6f },   { 398.912f, 10.449f, 144.511f },
            { 378.616f, 10.219f, 144.498f },
        };
        void* asset = xSTFindAsset(0x6A38ADC5, NULL);

        if (asset)
        {
            fn_80009CCC(pts, 15, *(U32*)asset, 3.6f, 3.6f);
        }
    }
    else if (globals.sceneCur->sceneID == zSceneIDFromName("p003"))
    {
        xVec3 pts1[5] = {
            { 57.448f, 13.165f, -49.734f }, { 62.079f, 12.698f, -47.585f },
            { 61.519f, 14.91f, -57.24f },   { 14.714f, 14.666f, -57.482f },
            { 17.973f, 17.225f, -66.808f },
        };
        xVec3 pts2[4] = {
            { 59.459f, 17.985f, -45.893f },
            { 58.922f, 21.302f, -57.58f },
            { 17.133f, 21.198f, -57.649f },
            { 16.243f, 24.422f, -69.885f },
        };
        void* asset = xSTFindAsset(0xB0988A6C, NULL);

        if (asset)
        {
            fn_80009CCC(pts1, 5, *(U32*)asset, 1.2f, 1.2f);
            fn_80009CCC(pts2, 4, *(U32*)asset, 2.3f, 2.3f);
        }
    }
    else if (globals.sceneCur->sceneID == zSceneIDFromName("p005"))
    {
        xVec3 pts[3] = {
            { -39.211f, 10.394f, -3.671f },
            { -44.42f, 12.877f, -3.422f },
            { -49.24f, 12.468f, -3.422f },
        };
        void* asset = xSTFindAsset(0x10B4E293, NULL);

        if (asset)
        {
            fn_80009CCC(pts, 3, *(U32*)asset, 2.5f, 2.5f);
        }
    }

    if (s->env->geom && s->env->geom->fx)
    {
        xCameraEnd(&globals.camera, 0.0f, 0);
        iEnvBeginRenderFX(NULL);
        xCameraBegin(&globals.camera, 0);
        zLightAddLocalEnv();
        fn_8000A138(NULL);
        zLightRemoveLocalEnv();
    }

    zRenderState(SDRS_Particles);

    {
        zParSys* psys = (zParSys*)s->baseList[39];

        for (S32 i = s->baseCount[39] - 1; i >= 0; i--, psys++)
        {
            zParSysRender(psys);
        }
    }

    zRenderState(SDRS_AlphaModels);
    iShadowRender();

    for (S32 i = 0; i < sorted_entities; i++)
    {
        _xEnt* ent = entities[i];

        ent->render(ent);

        if (ent->baseType == 3)
        {
            zShadowReceivers();
        }
    }

    switch (zGameModeGet())
    {
    case eGameMode_Load:
    case eGameMode_WorldMap:
    case eGameMode_ConceptArtGallery:
        break;
    case eGameMode_Options:
    case eGameMode_Save:
    case eGameMode_Pause:
    case eGameMode_MonsterGallery:
    case eGameMode_Game:
        zRenderState(SDRS_HUD);
        zHud_Render();
        break;
    }

    zRenderState(SDRS_Font);
    zUIRenderAll();
    xParMgrRender();

    if (zGameModeGet() == 1 && (zGameStateGet() == 0 || zGameStateGet() == 1))
    {
        char strn[256];
        char str[256];
        S32 available;
        xFontBox* sTrcFullCardFontBox = xFontBoxCreate(0.0f, 0.0f, 640.0f, 480.0f, 1, xColorFromRGBA(255, 230, 0, 255));
        xFontBox* sTrcFullCardFontBox2 = xFontBoxCreate(0.0f, 340.0f, 640.0f, 100.0f, 0, xColorFromRGBA(255, 230, 0, 255));

        available = availOnDisk;
        str[0] = '\0';
        strn[0] = '\0';

        if (zGameStateGet() == 0)
        {
            if (availOnDisk <= 0)
            {
                available = 0;
            }

            const char* availBlocks = available / lbl_8025F0AC == 1 ? "$C is" : "s$C are";
            const char* availFiles = lbl_8025F0B4 == 1 ? "" : "s";
            const char* neededBlocks = bytesNeeded / lbl_8025F0AC == 1 ? "$C is" : "s$C are";
            const char* neededFiles = lbl_8025F0B0 == 1 ? "" : "s";

            sprintf(str,
                    "Insufficient space on the Nintendo GameCube_ Memory Card in Slot %s.\n"
                    "At least $A000210068%d file%s and %d block%s needed to save game data.\n"
                    "($A220020026%d file%s and %d block%s available.)\n\nDo you want to start?",
                    zMenuGetBadCard() == 1 ? "A" : "B", lbl_8025F0B0, neededFiles,
                    bytesNeeded / lbl_8025F0AC, neededBlocks, lbl_8025F0B4, availFiles,
                    available / lbl_8025F0AC, availBlocks);

            xFontBoxSetText(sTrcFullCardFontBox, str);
            xFontBoxSetDrawSize(sTrcFullCardFontBox, 18.0f, 24.0f);
            xFontBoxSetDrawSpacing(sTrcFullCardFontBox, 17.0f, 24.0f);
            xFontBoxSetBackdrop(sTrcFullCardFontBox, xColorFromRGBA(0, 0, 0, 255));
            xFontBoxSetInset(sTrcFullCardFontBox, 100.0f, 100.0f, 100.0f, 0.0f);
            xFontBoxSetJustification(sTrcFullCardFontBox, 0x40);

            sprintf(strn, "%s-Yes\t\t%s-No    ", "#", "}");

            xFontBoxSetText(sTrcFullCardFontBox2, strn);
            xFontBoxSetDrawSize(sTrcFullCardFontBox2, 21.0f, 28.0f);
            xFontBoxSetDrawSpacing(sTrcFullCardFontBox2, 20.0f, 28.0f);
            xFontBoxSetBackdrop(sTrcFullCardFontBox2, xColorFromRGBA(0, 0, 0, 255));
            xFontBoxSetInset(sTrcFullCardFontBox2, 40.0f, 10.0f, 0.0f, 0.0f);
            xFontBoxSetJustification(sTrcFullCardFontBox2, 0x40);
        }
        else
        {
            xFontBoxSetText(sTrcFullCardFontBox, NULL);
            xFontBoxSetBackdrop(sTrcFullCardFontBox, xColorFromRGBA(0, 0, 0, 255));
        }

        xFontBoxRender(sTrcFullCardFontBox);
        xFontBoxDestroy(sTrcFullCardFontBox);
        xFontBoxRender(sTrcFullCardFontBox2);
        xFontBoxDestroy(sTrcFullCardFontBox2);
    }

    zRenderState(SDRS_Font);
    xFontRender();

    if (zGameModeGet() == 1 && zGameStateGet() == 3)
    {
        DrawScreenSizeRectangle();
    }
}

xBase* zSceneFindObject(U32 gameID)
{
    return zSceneObjHashtableGet(gameID);
}

xBase* zSceneGetObject(S32 type, S32 idx)
{
    zScene* s = globals.sceneCur;

    if (s)
    {
        for (S32 i = 0; i < (S32)s->num_base; i++)
        {
            if (s->base[i] && type == s->base[i]->baseType)
            {
                if (idx == 0)
                {
                    return s->base[i];
                }

                idx--;
            }
        }
    }

    return NULL;
}

char* zSceneGetName(U32 gameID)
{
    xBase* b = zSceneFindObject(gameID);

    if (b)
    {
        return zSceneGetName(b);
    }

    return "";
}

char* zSceneGetName(xBase* b)
{
    if (b)
    {
        char* n = xSTAssetName(b->id);

        if (n)
        {
            return n;
        }
    }

    return "";
}

void zSceneForAllBase(xBase* (*func)(xBase*, zScene*, void*), S32 baseType, void* data)
{
    zScene* s = globals.sceneCur;

    if (s)
    {
        for (U16 i = 0; i < s->num_base; i++)
        {
            if (s->base[i]->baseType == baseType)
            {
                if (!func(s->base[i], s, data))
                {
                    break;
                }
            }
        }
    }
}

void zSceneMemLvlChkCB()
{
    xMemGetBase();
}

U32 zSceneGetArea(U32 sceneID)
{
    for (U32 i = 0; i < eSceneArea_count; i++)
    {
        for (U32 j = 0; j < 4; j++)
        {
            if (sceneID == sAreaTable[i].sceneIDs[j])
            {
                return i;
            }
        }
    }

    return 0;
}

U32 zSceneGetRoominArea(U32 sceneID)
{
    for (U32 i = 0; i < eSceneArea_count; i++)
    {
        for (U32 j = 0; j < 4; j++)
        {
            if (sceneID == sAreaTable[i].sceneIDs[j])
            {
                return j;
            }
        }
    }

    return -1;
}

// The tower's first room and the hub's gallery room show their own titles.
char* zSceneGetAreaname(U32 sceneID)
{
    if (sceneID == 'O001')
    {
        return "Cower in the Tower!";
    }

    if (sceneID == 'H003')
    {
        return "Monster Gallery";
    }

    return sAreaTable[zSceneGetArea(sceneID)].desc;
}

U32 zSceneGetAreaConstTotalSnacks(U32 sceneID)
{
    return sAreaTable[zSceneGetArea(sceneID)].constTotalSnacks;
}

U32 zSceneGetAreaTotalSnacks(U32 sceneID)
{
    return sAreaTable[zSceneGetArea(sceneID)].totalSnacks;
}

U32 zSceneGetAreaTotalSnacks(_tagSceneAreas scene)
{
    return sAreaTable[scene].totalSnacks;
}

U32 zSceneGetAreaTotalTokens(U32 sceneID)
{
    return sAreaTable[zSceneGetArea(sceneID)].totalMTokens;
}

// A room may hold more snacks than the table expected; the total grows to match.
U32 zSceneGetAreaSnacksCollected(U32 sceneID)
{
    U32 a = zSceneGetArea(sceneID);
    U32 n = globals.areaStats[a].snacks_tot;

    if (n > sAreaTable[a].totalSnacks)
    {
        sAreaTable[a].totalSnacks = n;
    }

    return n;
}

U32 zSceneGetAreaTokensCollected(U32 sceneID)
{
    U32 a = zSceneGetArea(sceneID);
    U32 n = globals.areaStats[a].tokens_tot;
    U32 t = sAreaTable[a].totalMTokens;

    if (n > t)
    {
        return t;
    }
    return n;
}

void zSceneGameStatsTally_snack(U32 amount)
{
    U32 a = zSceneGetArea(globals.sceneCur->sceneID);

    globals.stats.snacks_tot += amount;
    globals.areaStats[a].snacks_tot += amount;
}

void zSceneGameStatsTally_token()
{
    U32 a = zSceneGetArea(globals.sceneCur->sceneID);

    globals.stats.tokens_tot++;
    globals.areaStats[a].tokens_tot++;
}

void zSceneGameStatsTally_scare()
{
    globals.stats.scares++;
    globals.stats.scares_sum++;
}

void zSceneGameStatsTally_death()
{
    globals.stats.deaths++;
    globals.stats.deaths_sum++;
}

// Levels are keyed by the first letter of their scene ids; -1 when no level matches.
static S32 zSceneGetLevelIndex(char prefix)
{
    for (S32 i = 0; i < 13; i++)
    {
        if (prefix == sLevelTable[i].name)
        {
            return sLevelTable[i].index;
        }
    }
    return -1;
}

void zSceneGameStatsTally_clue()
{
    char lvl[4] = {};

    globals.stats.clues++;

    strncpy(lvl, xStrupr(xUtil_idtag2string(globals.sceneCur->sceneID, 0)), 4);

    globals.levelStats[zSceneGetLevelIndex(lvl[0])].clues++;
}

S32 zRayHitsPlayer(_xRay3* r)
{
    _xCollis c;

    xRayHitsBound(r, &globals.player.ent.bound, &c);

    return c.flags & 0x1;
}

// Percentage of the game complete: power-ups 43%, rooms 20%, snacks 20% and monster tokens
// 15%, plus 2% for beating the Mastermind. Any progress at all counts as at least 1%.
U32 zSceneCalcProgress()
{
    F32 pupPercent;
    U32 pupCnt = 0;
    F32 roomPercent;
    U32 snackCnt;
    F32 snackPercent;
    U32 mtokCnt;
    F32 mtokPercent;
    U32 killedHolly = 0;
    S32 i;
    U32 totals;

    for (i = 0; i < 15; i++)
    {
        if (globals.player.Inv_Specials & (1 << i))
        {
            pupCnt++;
        }
    }

    pupPercent = pupCnt / 13.0f;

    roomCnt = 0;
    xSerialTraverse(zSceneCalcProgress_serialTraverseCB);

    snackCnt = globals.stats.snacks_tot;
    roomPercent = roomCnt / 86.0f;
    snackPercent = snackCnt / 6748.0f;

    mtokCnt = 0;
    for (i = 0; i < 21; i++)
    {
        if (globals.player.Inv_Mtokens[i >> 5] & (1 << (i & 31)))
        {
            mtokCnt++;
        }
    }

    mtokPercent = mtokCnt / 21.0f;

    if (zVarEntryCB_eVareVarDefeatedMM())
    {
        killedHolly = 2;
    }

    totals = killedHolly + (43.0f * pupPercent + 20.0f * roomPercent + 20.0f * snackPercent +
                            15.0f * mtokPercent);

    if (totals == 0 && (pupCnt || snackCnt || mtokCnt))
    {
        totals++;
    }

    if (totals > 100)
    {
        totals = 100;
    }

    return totals;
}

// Counts the rooms the save marks as visited, except the player's own record.
S32 zSceneCalcProgress_serialTraverseCB(U32 clientID, xSerial* xser)
{
    S32 val = 0;

    if (clientID != 'PLYR')
    {
        xser->Read_b1(&val);

        if (val)
        {
            roomCnt++;
        }
    }

    return 1;
}

void zScene_UpdateFlyToInterface(F32 dt)
{
    zScene* s = globals.sceneCur;

    zEntPickup_UpdateFlyToInterface((_zEntPickup*)s->baseList[4], s->baseCount[4], dt);
}

void zSceneStats_Clear()
{
    for (U32 i = 0; i < eSceneArea_count; i++)
    {
        globals.areaStats[i].tokens_tot = 0;
        globals.areaStats[i].snacks_tot = 0;
    }
}

extern "C" void fn_800CD268(S32 on)
{
    lbl_8025F0C0 = on;
}

extern "C" void fn_800CD254()
{
    lbl_8025F0C4 = lbl_8025F0B8;
    lbl_8025F0C8 = lbl_8025F0BC;
}

extern "C" S32 fn_800CD24C()
{
    return lbl_8025F0C0;
}

// Input and text are held back while the intro room plays with the player's controls off,
// in the menus, while paused and in the pause mode.
extern "C" S32 fn_800CD170()
{
    if (globals.sceneCur && globals.sceneCur->sceneID == zSceneIDFromName("h003") &&
        !globals.player.ControlOn)
    {
        return 1;
    }

    if (zMenuRunning())
    {
        return 1;
    }

    if (zGameIsPaused())
    {
        return 1;
    }

    if (lbl_8025EBE0)
    {
        return 1;
    }

    if (lbl_8025E528 == 7)
    {
        return 1;
    }

    return 0;
}
