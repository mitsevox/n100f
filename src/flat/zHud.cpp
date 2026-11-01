#include "zHud.h"

#include "iColor.h"
#include "libc/math.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "rwcore.h"
#include "xAnim.h"
#include "xEnt.h"
#include "xEvent.h"
#include "xFont.h"
#include "xMath.h"
#include "xMath2.h"
#include "xMath3.h"
#include "xModel.h"
#include "xPad.h"
#include "xScene.h"
#include "xScrFx.h"
#include "xString.h"
#include "xstransvc.h"
#include "zEnt.h"
#include "zEntPlayer.h"
#include "zGameState.h"
#include "zGlobals.h"
#include "zRenderState.h"
#include "zScene.h"

static S32 sShakeSnackCounter;
static F32 widthAspect;
static F32 heightAspect;
static S32 sHudGameStatsTimer;
static F32 tmpRoomTime;
static F32 sHudDisplayTimer_plrStatus;
static F32 sHudDisplayTimer_snacks;
static F32 sHudDisplayTimer_gum;
static F32 sHudDisplayTimer_soap;
static F32 sHudDisplayTimer_healthBar;
static U32 oldHealthCnt;
static U32 oldHealthCnt2;
static S32 oldGumCnt;
static S32 oldGumCnt2;
static S32 oldSoapCnt;
static S32 oldSoapCnt2;
static F32 sHudDisplayTimer_allSnacksArea = -100.0f;
static F32 sHudDisplayTimer_saveDone = -100.0f;
static RwIm2DVertex Vert[4];
static U16 Idx[6] = { 0, 1, 2, 0, 2, 3 };

static zHudModel zHudModelTable[eHudModelCount] = {
    { eHudModelBubblegumPiece, "rhbg0001", 0 },
    { eHudModelBubblegumPack, "rbp00001", 0 },
    { eHudModelBubblegumMachine, "rbm00001", 0 },
    { eHudModelPinkBunnySlippers, "rbu00001", 0 },
    { eHudModelBarSoap, "rhbs0001", 0 },
    { eHudModelBoxSoap, "rhbo0001", 0 },
    { eHudModelBubblePowerUp, "rbb00001", 0 },
    { eHudModelLampShade, "rls00001", 0 },
    { eHudModelFlowerPot, "rfp00001", 0 },
    { eHudModelPlungerSuctionCup, "rps00001", 0 },
    { eHudModelBlackKnightHelmut, "rbk00001", 0 },
    { eHudModelSpring, "rsp00001", 0 },
    { eHudModelDivingHelmut, "rdh00001", 0 },
    { eHudModelSkull, "rsk00001", 0 },
    { eHudModelLightingBolt, "rlb00001", 0 },
    { eHudModelFootballHelmut, "rfh00001", 0 },
    { eHudModelUmbrella_closed, "rum00001", 0 },
    { eHudModelUmbrella_open, "rum00002", 0 },
    { eHudModelShovel, "rsh00001", 0 },
    { eHudModelIndividualScoobySnack, "rhss0001", 0 },
    { eHudModelScoobyCollar, "rhsc0003", 0 },
    { eHudModelMagnifyingGlass, "rma00001", 0 },
    { eHudModelHotSauce, "rhs00001", 0 },
    { eHudModelSphere, "sphere.dff", 0 },
    { eHudModelHead0, "rhsd0005", 0 },
    { eHudModelHead1, "rhsd0005", 0 },
    { eHudModelHead2, "rhsd0004", 0 },
    { eHudModelHead3, "rhsd0003", 0 },
    { eHudModelHead4, "rhsd0002", 0 },
    { eHudModelHead5, "rhsd0001", 0 },
    { eHudModelSandwich1, "rhsub_1", 0 },
    { eHudModelSandwich2, "rhsub_2", 0 },
    { eHudModelSandwich3, "rhsub_3", 0 },
    { eHudModelSandwich4, "rhsub_4", 0 },
    { eHudModelSandwich5, "rhsub_all", 0 },
    { eHudModelEnemyIcon, "rhss0001", 0 },
    { eHudModelClue, "rhtp0002", 0 },
    { eHudModel_err, "rhss0001", 0 },
    { eHudModelBubblegumPiece2, "rhbg0002", 0 },
    { eHudModelBubblegumPiece3, "rhbg0003", 0 },
    { eHudModelBubblegumPiece4, "rhbg0004", 0 },
    { eHudModelBubblegumPiece5, "rhbg0005", 0 },
};

static zHudItem sHudSnackArray[1];
static zHudItem sHudChancesArray[1];
static zHudItem sHudGumArray[5];
static zHudItem sHudSoapArray[6];
static zHudItem sHudSoapboxArray[1];
static zHudItem sHudGumpackArray[1];
static zHudItem sHudPlayerStatusArray[6];
static zHudItem sHudHealthbarArray[5];
static zHudItem sHudGameStatsArray[7];

void zHudElementsCB_PlayerStatusInit();
void zHudElementsCB_PlayerStatusSetup(_xScene* sc);
void zHudElementsCB_PlayerStatusUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_PlayerStatusRender();
void zHudElementsCB_ChancesInit();
void zHudElementsCB_ChancesSetup(_xScene* sc);
void zHudElementsCB_ChancesUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_ChancesRender();
void zHudElementsCB_SnacksInit();
void zHudElementsCB_SnacksSetup(_xScene* sc);
void zHudElementsCB_SnacksUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_SnacksRender();
void zHudElementsCB_GumInit();
void zHudElementsCB_GumSetup(_xScene* sc);
void zHudElementsCB_GumUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_GumRender();
void zHudElementsCB_SoapInit();
void zHudElementsCB_SoapSetup(_xScene* sc);
void zHudElementsCB_SoapUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_SoapRender();
void zHudElementsCB_MessageInit();
void zHudElementsCB_MessageSetup(_xScene* sc);
void zHudElementsCB_MessageUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_MessageRender();
void zHudElementsCB_GameStatsInit();
void zHudElementsCB_GameStatsSetup(_xScene* sc);
void zHudElementsCB_GameStatsUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_GameStatsRender();
void zHudElementsCB_SoapboxInit();
void zHudElementsCB_SoapboxSetup(_xScene* sc);
void zHudElementsCB_SoapboxUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_SoapboxRender();
void zHudElementsCB_GumpackInit();
void zHudElementsCB_GumpackSetup(_xScene* sc);
void zHudElementsCB_GumpackUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_GumpackRender();
void zHudElementsCB_HealthbarInit();
void zHudElementsCB_HealthbarSetup(_xScene* sc);
void zHudElementsCB_HealthbarUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_HealthbarRender();

// The HUD's parts, each with its callbacks and the player count it shows.
static _zHudElement sHudElements[eHudCount] = {
    { eHudPlayerStatus, zHudElementsCB_PlayerStatusInit, zHudElementsCB_PlayerStatusSetup,
      zHudElementsCB_PlayerStatusUpdate, zHudElementsCB_PlayerStatusRender,
      globals.player.Health, "" },
    { eHudChances, zHudElementsCB_ChancesInit, zHudElementsCB_ChancesSetup,
      zHudElementsCB_ChancesUpdate, zHudElementsCB_ChancesRender, globals.player.Inv_Chances,
      "" },
    { eHudSnacks, zHudElementsCB_SnacksInit, zHudElementsCB_SnacksSetup,
      zHudElementsCB_SnacksUpdate, zHudElementsCB_SnacksRender,
      globals.player.Inv_ScoobySnacks, "" },
    { eHudGum, zHudElementsCB_GumInit, zHudElementsCB_GumSetup, zHudElementsCB_GumUpdate,
      zHudElementsCB_GumRender, globals.player.Inv_Ammo[0], "" },
    { eHudSoap, zHudElementsCB_SoapInit, zHudElementsCB_SoapSetup, zHudElementsCB_SoapUpdate,
      zHudElementsCB_SoapRender, globals.player.Inv_Ammo[1], "" },
    { eHudMessage, zHudElementsCB_MessageInit, zHudElementsCB_MessageSetup,
      zHudElementsCB_MessageUpdate, zHudElementsCB_MessageRender, 0, "" },
    { eHudGameStats, zHudElementsCB_GameStatsInit, zHudElementsCB_GameStatsSetup,
      zHudElementsCB_GameStatsUpdate, zHudElementsCB_GameStatsRender, 0, "" },
    { eHudSoapbox, zHudElementsCB_SoapboxInit, zHudElementsCB_SoapboxSetup,
      zHudElementsCB_SoapboxUpdate, zHudElementsCB_SoapboxRender, 0, "" },
    { eHudGumpack, zHudElementsCB_GumpackInit, zHudElementsCB_GumpackSetup,
      zHudElementsCB_GumpackUpdate, zHudElementsCB_GumpackRender, 0, "" },
    { eHudHealthbar, zHudElementsCB_HealthbarInit, zHudElementsCB_HealthbarSetup,
      zHudElementsCB_HealthbarUpdate, zHudElementsCB_HealthbarRender, 0, "" },
};

static xFontBox* fontBox;
static iColor_tag clr = { 255, 255, 0, 200 };
static iColor_tag clrDropShadow = { 82, 0, 139, 200 };
static F32 dynOffset;
static S32 sStatInit;

// Defined in reverse address order: the deferred profile emits the last-defined function first.
void zHudInterp2d(F32 dt, F32* srcx, F32* srcy, F32 dstx, F32 dsty, F32 speed, S32 type);
void zHudInterp1d(F32 dt, F32* src, F32 dst, F32 speed, S32 type);
void zHudHide();
void zHudShow();
U32 zHudModelGetID(U32 modelEnum);
static xModelInstance* zHudLoadModel(U32 modelEnum);
void zHud_sceneReset();
void zHud_UIRender(char* asset, F32 xPos, F32 yPos, F32 w, F32 h, U8 alpha);
void zHud_DrawText(F32 x, F32 y, F32 w, F32 h, char* s, S32 just, F32 size, U8 alpha,
                   eHudDropShadowMode shadow);
void zHudElementsCB_GameStatsRender();
void zHudElementsCB_GameStatsUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_GameStatsSetup(_xScene* sc);
void zHudElementsCB_GameStatsInit();
void zHudElementsCB_HealthbarRender();
void zHudElementsCB_HealthbarUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_HealthbarSetup(_xScene* sc);
void zHudElementsCB_HealthbarInit();
void zHudElementsCB_GumpackRender();
void zHudElementsCB_GumpackUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_GumpackSetup(_xScene* sc);
void zHudElementsCB_GumpackInit();
void zHudElementsCB_SoapboxRender();
void zHudElementsCB_SoapboxUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_SoapboxSetup(_xScene* sc);
void zHudElementsCB_SoapboxInit();
void zHudElementsCB_MessageRender();
void zHudElementsCB_MessageUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_MessageSetup(_xScene* sc);
void zHudElementsCB_MessageInit();
void zHudElementsCB_SoapRender();
void zHudElementsCB_SoapUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_SoapSetup(_xScene* sc);
void zHudElementsCB_SoapInit();
void zHudElementsCB_GumRender();
void zHudElementsCB_GumUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_GumSetup(_xScene* sc);
void zHudElementsCB_GumInit();
void zHudElementsCB_SnacksRender();
void zHudShowSaveSuccess();
void zHudShowAllSnacksArea();
void zHud_showSnackSpecial();
void zHud_showSnack();
void zHud_ShakeSnackCounter();
void zHudElementsCB_SnacksUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_SnacksSetup(_xScene* sc);
void zHudElementsCB_SnacksInit();
void zHudElementsCB_ChancesRender();
void zHudElementsCB_ChancesUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_ChancesSetup(_xScene* sc);
void zHudElementsCB_ChancesInit();
void zHudElementsCB_PlayerStatusRender();
void zHud_showHealthandStatus();
void zHudElementsCB_PlayerStatusUpdate(_xScene* sc, F32 dt);
void zHudElementsCB_PlayerStatusSetup(_xScene* sc);
void zHudElementsCB_PlayerStatusInit();
void zHud_Render();
void zHud_Update(_xScene* sc, F32 dt);
void zHud_Setup(_xScene* sc);
void zHud_Init();

void zHud_Init()
{
    S32 i;

    widthAspect = 1.3333334f;
    heightAspect = 0.75f;

    clr.r = 255;
    clr.g = 255;
    clr.b = 0;
    clr.a = 200;
    clrDropShadow.r = 82;
    clrDropShadow.g = 0;
    clrDropShadow.b = 139;
    clrDropShadow.a = 200;

    for (i = 0; i < eHudModelCount; i++)
    {
        zHudModelTable[i].modelNameID = xStrHash(zHudModelTable[i].modelName);
    }

    for (i = 0; i < eHudCount; i++)
    {
        sHudElements[i].hudInit();
    }

    sShakeSnackCounter = 0;
}

void zHud_Setup(_xScene* sc)
{
    S32 i;

    for (i = 0; i < eHudCount; i++)
    {
        sHudElements[i].hudSetup(sc);
    }
}

void zHud_Update(_xScene* sc, F32 dt)
{
    S32 i;

    for (i = 0; i < eHudCount; i++)
    {
        sHudElements[i].hudUpdate(sc, dt);
    }

    if (globals.pad0->on & 0x40000)
    {
        zHudShow();
    }

    if (zGameModeGet() == eGameMode_Title || xScrFxIsLetterbox() || globals.cmgr)
    {
        zHudHide();
    }

    if (zGameModeGet() == eGameMode_Pause)
    {
        zHudShow();
    }

    if (sHudDisplayTimer_allSnacksArea < 0.0f && sHudDisplayTimer_allSnacksArea >= -10.0f)
    {
        zEntEvent("all snacks uif", eEventInvisible);
        sHudDisplayTimer_allSnacksArea = -100.0f;
    }
    else if (sHudDisplayTimer_allSnacksArea == 2.5f)
    {
        zEntEvent("all snacks uif", eEventVisible);
    }
    sHudDisplayTimer_allSnacksArea -= dt;

    if (sHudDisplayTimer_saveDone < 0.0f && sHudDisplayTimer_saveDone >= -10.0f)
    {
        zEntEvent("save ok uif", eEventInvisible);
        sHudDisplayTimer_saveDone = -100.0f;
    }
    else if (sHudDisplayTimer_saveDone == 1.5f)
    {
        zEntEvent("save ok uif", eEventVisible);
    }
    sHudDisplayTimer_saveDone -= dt;
}

void zHud_Render()
{
    S32 i;

    RwCameraClear(RwCameraGetCurrentCamera(), NULL, rwCAMERACLEARZ);

    for (i = 0; i < eHudCount; i++)
    {
        sHudElements[i].hudRender();
    }
}

// Loads the six head models, one per health level, and their idle animations.
void zHudElementsCB_PlayerStatusInit()
{
    xAnimFile* afile;
    U32 size;
    void* buff;
    S32 i;

    sHudPlayerStatusArray[0].model = zHudLoadModel(eHudModelHead0);
    sHudPlayerStatusArray[0].renderStyle = 1;
    sHudPlayerStatusArray[1].model = zHudLoadModel(eHudModelHead1);
    sHudPlayerStatusArray[1].renderStyle = 2;
    sHudPlayerStatusArray[2].model = zHudLoadModel(eHudModelHead2);
    sHudPlayerStatusArray[2].renderStyle = 3;
    sHudPlayerStatusArray[3].model = zHudLoadModel(eHudModelHead3);
    sHudPlayerStatusArray[3].renderStyle = 4;
    sHudPlayerStatusArray[4].model = zHudLoadModel(eHudModelHead4);
    sHudPlayerStatusArray[4].renderStyle = 5;
    sHudPlayerStatusArray[5].model = zHudLoadModel(eHudModelHead5);
    sHudPlayerStatusArray[5].renderStyle = 6;

    for (i = 0; i < 6; i++)
    {
        zHudItem* item = &sHudPlayerStatusArray[i];

        switch (i)
        {
        case 0:
            buff = xSTFindAsset(xStrHash("rhsd0001.anm"), &size);
            break;
        case 1:
            buff = xSTFindAsset(xStrHash("rhsd0001.anm"), &size);
            break;
        case 2:
            buff = xSTFindAsset(xStrHash("rhsd0001.anm"), &size);
            break;
        case 3:
            buff = xSTFindAsset(xStrHash("rhsd0001.anm"), &size);
            break;
        case 4:
            buff = xSTFindAsset(xStrHash("rhsd0001.anm"), &size);
            break;
        case 5:
            buff = xSTFindAsset(xStrHash("rhsd0001.anm"), &size);
            break;
        }

        if (buff)
        {
            afile = xAnimFileNew(buff, "", 0, NULL);
            item->atbl = xAnimTableNew("", NULL);
            xAnimTableNewState(item->atbl, "Idle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
            xAnimTableAddFile(item->atbl, afile, "Idle");
            xAnimPoolAlloc(&globals.sceneCur->mempool, item, item->atbl, item->model);
        }
    }
}

void zHudElementsCB_PlayerStatusSetup(_xScene* sc)
{
    S32 i;
    xAnimPlay* aplay;
    xAnimSingle* asing;

    sHudDisplayTimer_plrStatus = 2.0f;
    oldHealthCnt = globals.player.Health;

    for (i = 0; i < 6; i++)
    {
        zHudItem* item = &sHudPlayerStatusArray[i];

        item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 44.0f;
        item->zHudStatusPosTable[eHudStatusPosOnScreen].y = 65.0f;
        item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].x = -44.0f;
        item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].y = -35.0f;
        item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x = 100.0f;
        item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y = 300.0f;
        item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 80.0f;
        item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 80.0f;
        item->zHudStatusSizeTable[eHudStatusSizeZero].x = 0.0f;
        item->zHudStatusSizeTable[eHudStatusSizeZero].y = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngStop].x = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngStop].y = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngStop].z = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngIdle].x = 1.5915494f;
        item->zHudStatusAngTable[eHudStatusAngIdle].y = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngIdle].z = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngTest1].x = 1.5915494f;
        item->zHudStatusAngTable[eHudStatusAngTest1].y = 15.915494f;
        item->zHudStatusAngTable[eHudStatusAngTest1].z = 0.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].rm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].gm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].bm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].a = 255.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].rm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].gm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].bm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].a = 0.0f;

        item->statusPos = eHudStatusPosOnScreen;
        item->statusAng = eHudStatusAngStop;
        item->statusSize = eHudStatusSizeDefault;
        item->statusColor = eHudStatusColorSolid;

        item->hframe.pos.x = item->zHudStatusPosTable[eHudStatusPosOnScreen].x;
        item->hframe.pos.y = item->zHudStatusPosTable[eHudStatusPosOnScreen].y;
        item->hframe.dang.x = item->zHudStatusAngTable[eHudStatusAngStop].x;
        item->hframe.dang.y = item->zHudStatusAngTable[eHudStatusAngStop].y;
        item->hframe.dang.z = item->zHudStatusAngTable[eHudStatusAngStop].z;
        item->hframe.size.x = item->zHudStatusSizeTable[eHudStatusSizeDefault].x;
        item->hframe.size.y = item->zHudStatusSizeTable[eHudStatusSizeDefault].y;
        item->hframe.color.a = item->zHudStatusColorTable[eHudStatusColorTransparent].a;

        aplay = item->model->Anim;
        if (aplay)
        {
            asing = aplay->Single;
            asing->Time = 0.0f;
            asing->CurrentSpeed = 1.0f;
            xAnimPlaySetState(aplay->Single, item->model->Anim->Table->StateList, 0.0f);
        }
    }
}

void zHudElementsCB_PlayerStatusUpdate(_xScene* sc, F32 dt)
{
    S32 i;
    U32 num = globals.player.Health;

    switch (num)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        for (S32 i = 0; i < 6; i++)
        {
            if (i == num)
            {
                sHudPlayerStatusArray[num].statusSize = eHudStatusSizeDefault;
            }
            else
            {
                sHudPlayerStatusArray[i].statusSize = eHudStatusSizeZero;
            }
        }
        break;
    }

    if (globals.player.Health != oldHealthCnt)
    {
        oldHealthCnt = globals.player.Health;
        sHudDisplayTimer_plrStatus = 2.0f;
    }

    if (sHudDisplayTimer_plrStatus == 0.0f)
    {
        for (S32 i = 0; i < 6; i++)
        {
            sHudPlayerStatusArray[i].statusColor = eHudStatusColorTransparent;
            sHudPlayerStatusArray[i].statusPos = eHudStatusPosOffScreenDefault;
        }
    }
    else
    {
        for (S32 i = 0; i < 6; i++)
        {
            sHudPlayerStatusArray[i].statusColor = eHudStatusColorSolid;
            sHudPlayerStatusArray[i].statusPos = eHudStatusPosOnScreen;
        }
        sHudDisplayTimer_plrStatus -= dt;
        if (sHudDisplayTimer_plrStatus < 0.0f)
        {
            sHudDisplayTimer_plrStatus = 0.0f;
        }
    }

    for (i = 0; i < 6; i++)
    {
        zHudItem* item = &sHudPlayerStatusArray[i];

        switch (item->statusPos)
        {
        case eHudStatusPosOnScreen:
            zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                         item->zHudStatusPosTable[eHudStatusPosOnScreen].x,
                         item->zHudStatusPosTable[eHudStatusPosOnScreen].y, 1000.0f,
                         eHudInterpTypeDecel);
            break;
        case eHudStatusPosOffScreenDefault:
            zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                         item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].x,
                         item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].y, 750.0f,
                         eHudInterpTypeDecel);
            break;
        case eHudStatusPosMiddleOfScreen:
            zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                         item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x,
                         item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y, 250.0f,
                         eHudInterpTypeDecel);
            break;
        }

        switch (item->statusAng)
        {
        case eHudStatusAngStop:
            zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngStop].x,
                         1.0f, eHudInterpTypeLinear);
            break;
        case eHudStatusAngIdle:
            zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngIdle].x,
                         1.0f, eHudInterpTypeLinear);
            break;
        case eHudStatusAngTest1:
            zHudInterp1d(dt, &item->hframe.dang.x,
                         item->zHudStatusAngTable[eHudStatusAngTest1].x, 1.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.dang.y,
                         item->zHudStatusAngTable[eHudStatusAngTest1].y, 1.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.dang.z,
                         item->zHudStatusAngTable[eHudStatusAngTest1].z, 1.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusAngTest2:
            break;
        }

        switch (item->statusSize)
        {
        case eHudStatusSizeDefault:
            item->hframe.size.x = item->zHudStatusSizeTable[eHudStatusSizeDefault].x;
            item->hframe.size.y = item->zHudStatusSizeTable[eHudStatusSizeDefault].y;
            break;
        case eHudStatusSizeZero:
            item->hframe.size.x = item->zHudStatusSizeTable[eHudStatusSizeZero].x;
            item->hframe.size.y = item->zHudStatusSizeTable[eHudStatusSizeZero].y;
            break;
        case eHudStatusSizeSmall:
            break;
        }

        switch (item->statusColor)
        {
        case eHudStatusColorSolid:
            zHudInterp1d(dt, &item->hframe.color.a,
                         item->zHudStatusColorTable[eHudStatusColorSolid].a, 600.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusColorTransparent:
            zHudInterp1d(dt, &item->hframe.color.a,
                         item->zHudStatusColorTable[eHudStatusColorTransparent].a, 300.0f,
                         eHudInterpTypeLinear);
            break;
        }

        item->hframe.ang.x += item->hframe.dang.x * dt;
        item->hframe.ang.y += item->hframe.dang.y * dt;
        item->hframe.ang.z += item->hframe.dang.z * dt;
        xModelUpdate(item->model, dt);
        xModelEval(item->model);
    }
}

// Shows the head and the health bar for a moment.
void zHud_showHealthandStatus()
{
    sHudDisplayTimer_plrStatus = 2.0f;
    sHudDisplayTimer_healthBar = 2.0f;
}

// Scooby's head at the top left over its backdrop: one head model for each health level, of
// which only the current one is shown full size.
void zHudElementsCB_PlayerStatusRender()
{
    S32 i;

    zHud_UIRender("scooby_head_bg", sHudPlayerStatusArray[0].hframe.pos.x + -30.0f,
                  sHudPlayerStatusArray[0].hframe.pos.y + -50.0f, 65.0f * widthAspect, 85.0f,
                  sHudPlayerStatusArray[0].hframe.color.a);
    zRenderState(SDRS_HUD);

    for (i = 0; i < 6; i++)
    {
        zHudItem* item = &sHudPlayerStatusArray[i];

        if (item->hframe.color.a != 0.0f)
        {
            xRect r = { item->hframe.pos.x - 0.5f * item->hframe.size.x,
                        item->hframe.pos.y - 0.5f * item->hframe.size.y,
                        item->hframe.size.x * widthAspect, item->hframe.size.y };
            xVec3 from = { -0.8f, 0.22f, 0.8f };
            xVec3 to = { 0.0f, 0.0f, 0.0f };
            xMat4x3 frame;

            xMat3x3Euler((xMat3x3*)&frame, item->hframe.ang.x, item->hframe.ang.y,
                         item->hframe.ang.z);
            frame.flags = 0;
            frame.pos.x = 0.0f;
            frame.pos.y = 0.0f;
            frame.pos.z = 0.0f;
            xModelSetMaterialAlpha(item->model, item->hframe.color.a);
            memcpy(item->model->Mat, &frame, sizeof(xMat4x3));
            xModelSetRender2DStyle(item->renderStyle, item->hframe.color.a);
            xModelRender2D(item->model, &r, &from, &to, PI / 2);
        }
    }
}

void zHudElementsCB_ChancesInit()
{
}

void zHudElementsCB_ChancesSetup(_xScene* sc)
{
    sHudChancesArray[0].zHudStatusPosTable[eHudStatusPosOnScreen].x = 75.0f;
    sHudChancesArray[0].zHudStatusPosTable[eHudStatusPosOnScreen].y = 40.0f;
    sHudChancesArray[0].zHudStatusSizeTable[eHudStatusSizeDefault].x = 150.0f;
    sHudChancesArray[0].zHudStatusSizeTable[eHudStatusSizeDefault].y = 150.0f;
    sHudChancesArray[0].zHudStatusAngTable[eHudStatusAngStop].x = 0.0f;
    sHudChancesArray[0].zHudStatusAngTable[eHudStatusAngStop].y = 0.0f;
    sHudChancesArray[0].zHudStatusAngTable[eHudStatusAngStop].z = 0.0f;
    sHudChancesArray[0].zHudStatusAngTable[eHudStatusAngIdle].x = 1.5915494f;
    sHudChancesArray[0].zHudStatusAngTable[eHudStatusAngIdle].y = 0.0f;
    sHudChancesArray[0].zHudStatusAngTable[eHudStatusAngIdle].z = 0.0f;
    sHudChancesArray[0].zHudStatusAngTable[eHudStatusAngTest1].x = 1.5915494f;
    sHudChancesArray[0].zHudStatusAngTable[eHudStatusAngTest1].y = 15.915494f;
    sHudChancesArray[0].zHudStatusAngTable[eHudStatusAngTest1].z = 0.0f;

    sHudChancesArray[0].statusPos = eHudStatusPosOnScreen;
    sHudChancesArray[0].statusAng = eHudStatusAngIdle;
    sHudChancesArray[0].statusSize = eHudStatusSizeDefault;

    sHudChancesArray[0].hframe.pos.x =
        sHudChancesArray[0].zHudStatusPosTable[eHudStatusPosOnScreen].x;
    sHudChancesArray[0].hframe.pos.y =
        sHudChancesArray[0].zHudStatusPosTable[eHudStatusPosOnScreen].y;
    sHudChancesArray[0].hframe.dang.x =
        sHudChancesArray[0].zHudStatusAngTable[eHudStatusAngStop].x;
    sHudChancesArray[0].hframe.dang.y =
        sHudChancesArray[0].zHudStatusAngTable[eHudStatusAngStop].y;
    sHudChancesArray[0].hframe.dang.z =
        sHudChancesArray[0].zHudStatusAngTable[eHudStatusAngStop].z;
    sHudChancesArray[0].hframe.size.x =
        sHudChancesArray[0].zHudStatusSizeTable[eHudStatusSizeDefault].x;
    sHudChancesArray[0].hframe.size.y =
        sHudChancesArray[0].zHudStatusSizeTable[eHudStatusSizeDefault].y;
}

void zHudElementsCB_ChancesUpdate(_xScene* sc, F32 dt)
{
    zHudItem* item = &sHudChancesArray[0];

    switch (item->statusPos)
    {
    case eHudStatusPosOnScreen:
        zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                     item->zHudStatusPosTable[eHudStatusPosOnScreen].x,
                     item->zHudStatusPosTable[eHudStatusPosOnScreen].y, 250.0f,
                     eHudInterpTypeDecel);
        break;
    case eHudStatusPosMiddleOfScreen:
        zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                     item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x,
                     item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y, 250.0f,
                     eHudInterpTypeDecel);
        break;
    }

    switch (item->statusAng)
    {
    case eHudStatusAngStop:
        zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngStop].x,
                     1.0f, eHudInterpTypeLinear);
        break;
    case eHudStatusAngIdle:
        zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngIdle].x,
                     1.0f, eHudInterpTypeLinear);
        break;
    case eHudStatusAngTest1:
        zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngTest1].x,
                     1.0f, eHudInterpTypeLinear);
        zHudInterp1d(dt, &item->hframe.dang.y, item->zHudStatusAngTable[eHudStatusAngTest1].y,
                     1.0f, eHudInterpTypeLinear);
        zHudInterp1d(dt, &item->hframe.dang.z, item->zHudStatusAngTable[eHudStatusAngTest1].z,
                     1.0f, eHudInterpTypeLinear);
        break;
    case eHudStatusAngTest2:
        break;
    }

    switch (item->statusSize)
    {
    case eHudStatusSizeDefault:
        zHudInterp1d(dt, &item->hframe.size.x,
                     item->zHudStatusSizeTable[eHudStatusSizeDefault].x, 15.0f,
                     eHudInterpTypeLinear);
        zHudInterp1d(dt, &item->hframe.size.y,
                     item->zHudStatusSizeTable[eHudStatusSizeDefault].y, 10.0f,
                     eHudInterpTypeLinear);
        break;
    case eHudStatusSizeZero:
    case eHudStatusSizeSmall:
    case eHudStatusSizeBig:
    case eHudStatusSizeSquash:
    case eHudStatusSizeZeroHoriz:
    case eHudStatusSizeZeroVert:
    case eHudStatusSizeHyper:
    case eHudStatusSizeHyperReverse:
        break;
    }

    item->hframe.ang.x += item->hframe.dang.x * dt;
    item->hframe.ang.y += item->hframe.dang.y * dt;
    item->hframe.ang.z += item->hframe.dang.z * dt;
}

void zHudElementsCB_ChancesRender()
{
}

// Loads the Scooby Snack model.
void zHudElementsCB_SnacksInit()
{
    sHudSnackArray[0].model = zHudLoadModel(eHudModelIndividualScoobySnack);
    sHudSnackArray[0].renderStyle = 9;
}

void zHudElementsCB_SnacksSetup(_xScene* sc)
{
    sHudDisplayTimer_snacks = 2.0f;

    sHudSnackArray[0].zHudStatusPosTable[eHudStatusPosOnScreen].x = 520.0f;
    sHudSnackArray[0].zHudStatusPosTable[eHudStatusPosOnScreen].y = 32.0f;
    sHudSnackArray[0].zHudStatusPosTable[eHudStatusPosOnScreenAlt1].x = 496.0f;
    sHudSnackArray[0].zHudStatusPosTable[eHudStatusPosOnScreenAlt1].y = 32.0f;
    sHudSnackArray[0].zHudStatusPosTable[eHudStatusPosOffScreenDefault].x = 600.0f;
    sHudSnackArray[0].zHudStatusPosTable[eHudStatusPosOffScreenDefault].y = -68.0f;
    sHudSnackArray[0].zHudStatusSizeTable[eHudStatusSizeDefault].x = 70.0f;
    sHudSnackArray[0].zHudStatusSizeTable[eHudStatusSizeDefault].y = 70.0f;
    sHudSnackArray[0].zHudStatusAngTable[eHudStatusAngStop].x = 0.0f;
    sHudSnackArray[0].zHudStatusAngTable[eHudStatusAngStop].y = 0.0f;
    sHudSnackArray[0].zHudStatusAngTable[eHudStatusAngStop].z = 0.0f;
    sHudSnackArray[0].zHudStatusAngTable[eHudStatusAngIdle].x = 1.5915494f;
    sHudSnackArray[0].zHudStatusAngTable[eHudStatusAngIdle].y = 0.0f;
    sHudSnackArray[0].zHudStatusAngTable[eHudStatusAngIdle].z = 0.0f;
    sHudSnackArray[0].zHudStatusAngTable[eHudStatusAngTest1].x = 1.5915494f;
    sHudSnackArray[0].zHudStatusAngTable[eHudStatusAngTest1].y = 15.915494f;
    sHudSnackArray[0].zHudStatusAngTable[eHudStatusAngTest1].z = 0.0f;
    sHudSnackArray[0].zHudStatusColorTable[eHudStatusColorSolid].rm = 1.0f;
    sHudSnackArray[0].zHudStatusColorTable[eHudStatusColorSolid].gm = 1.0f;
    sHudSnackArray[0].zHudStatusColorTable[eHudStatusColorSolid].bm = 1.0f;
    sHudSnackArray[0].zHudStatusColorTable[eHudStatusColorSolid].a = 255.0f;
    sHudSnackArray[0].zHudStatusColorTable[eHudStatusColorTransparent].rm = 1.0f;
    sHudSnackArray[0].zHudStatusColorTable[eHudStatusColorTransparent].gm = 1.0f;
    sHudSnackArray[0].zHudStatusColorTable[eHudStatusColorTransparent].bm = 1.0f;
    sHudSnackArray[0].zHudStatusColorTable[eHudStatusColorTransparent].a = 0.0f;

    sHudSnackArray[0].statusPos = eHudStatusPosOnScreen;
    sHudSnackArray[0].statusAng = eHudStatusAngIdle;
    sHudSnackArray[0].statusSize = eHudStatusSizeDefault;
    sHudSnackArray[0].statusColor = eHudStatusColorSolid;

    sHudSnackArray[0].hframe.pos.x = sHudSnackArray[0].zHudStatusPosTable[eHudStatusPosOnScreen].x;
    sHudSnackArray[0].hframe.pos.y = sHudSnackArray[0].zHudStatusPosTable[eHudStatusPosOnScreen].y;
    sHudSnackArray[0].hframe.dang.x = sHudSnackArray[0].zHudStatusAngTable[eHudStatusAngIdle].x;
    sHudSnackArray[0].hframe.dang.y = sHudSnackArray[0].zHudStatusAngTable[eHudStatusAngIdle].y;
    sHudSnackArray[0].hframe.dang.z = sHudSnackArray[0].zHudStatusAngTable[eHudStatusAngIdle].z;
    sHudSnackArray[0].hframe.size.x =
        sHudSnackArray[0].zHudStatusSizeTable[eHudStatusSizeDefault].x;
    sHudSnackArray[0].hframe.size.y =
        sHudSnackArray[0].zHudStatusSizeTable[eHudStatusSizeDefault].y;
    sHudSnackArray[0].hframe.color.rm =
        sHudSnackArray[0].zHudStatusColorTable[eHudStatusColorTransparent].rm;
    sHudSnackArray[0].hframe.color.gm =
        sHudSnackArray[0].zHudStatusColorTable[eHudStatusColorTransparent].gm;
    sHudSnackArray[0].hframe.color.bm =
        sHudSnackArray[0].zHudStatusColorTable[eHudStatusColorTransparent].bm;
    sHudSnackArray[0].hframe.color.a =
        sHudSnackArray[0].zHudStatusColorTable[eHudStatusColorTransparent].a;
}

void zHudElementsCB_SnacksUpdate(_xScene* sc, F32 dt)
{
    zHudItem* item = &sHudSnackArray[0];

    if (zGameModeGet() == eGameMode_Pause)
    {
        item->statusAng = eHudStatusAngStop;
    }
    else
    {
        item->statusAng = eHudStatusAngIdle;
    }

    if (sHudDisplayTimer_snacks == 0.0f)
    {
        sHudSnackArray[0].statusColor = eHudStatusColorTransparent;
        sHudSnackArray[0].statusPos = eHudStatusPosOffScreenDefault;
    }
    else
    {
        if (globals.player.Inv_ScoobySnacks > 999)
        {
            sHudSnackArray[0].statusColor = eHudStatusColorSolid;
            sHudSnackArray[0].statusPos = eHudStatusPosOnScreenAlt1;
        }
        else
        {
            sHudSnackArray[0].statusColor = eHudStatusColorSolid;
            sHudSnackArray[0].statusPos = eHudStatusPosOnScreen;
        }

        sHudDisplayTimer_snacks -= dt;
        if (sHudDisplayTimer_snacks < 0.0f)
        {
            sHudDisplayTimer_snacks = 0.0f;
        }
    }

    switch (item->statusPos)
    {
    case eHudStatusPosOnScreen:
        zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                     item->zHudStatusPosTable[eHudStatusPosOnScreen].x,
                     item->zHudStatusPosTable[eHudStatusPosOnScreen].y, 1000.0f,
                     eHudInterpTypeDecel);
        break;
    case eHudStatusPosOffScreenDefault:
        zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                     item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].x,
                     item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].y, 750.0f,
                     eHudInterpTypeDecel);
        break;
    case eHudStatusPosOnScreenAlt1:
        zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                     item->zHudStatusPosTable[eHudStatusPosOnScreenAlt1].x,
                     item->zHudStatusPosTable[eHudStatusPosOnScreenAlt1].y, 750.0f,
                     eHudInterpTypeDecel);
        break;
    case eHudStatusPosMiddleOfScreen:
        zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                     item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x,
                     item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y, 250.0f,
                     eHudInterpTypeDecel);
        break;
    }

    switch (item->statusAng)
    {
    case eHudStatusAngStop:
        zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngStop].x,
                     1.0f, eHudInterpTypeLinear);
        break;
    case eHudStatusAngIdle:
        zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngIdle].x,
                     1.0f, eHudInterpTypeLinear);
        break;
    case eHudStatusAngTest1:
        zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngTest1].x,
                     1.0f, eHudInterpTypeLinear);
        zHudInterp1d(dt, &item->hframe.dang.y, item->zHudStatusAngTable[eHudStatusAngTest1].y,
                     1.0f, eHudInterpTypeLinear);
        zHudInterp1d(dt, &item->hframe.dang.z, item->zHudStatusAngTable[eHudStatusAngTest1].z,
                     1.0f, eHudInterpTypeLinear);
        break;
    case eHudStatusAngTest2:
        break;
    }

    switch (item->statusSize)
    {
    case eHudStatusSizeDefault:
        zHudInterp1d(dt, &item->hframe.size.x,
                     item->zHudStatusSizeTable[eHudStatusSizeDefault].x, 10.0f,
                     eHudInterpTypeLinear);
        zHudInterp1d(dt, &item->hframe.size.y,
                     item->zHudStatusSizeTable[eHudStatusSizeDefault].y, 10.0f,
                     eHudInterpTypeLinear);
        break;
    case eHudStatusSizeZero:
    case eHudStatusSizeSmall:
    case eHudStatusSizeBig:
    case eHudStatusSizeSquash:
    case eHudStatusSizeZeroHoriz:
    case eHudStatusSizeZeroVert:
    case eHudStatusSizeHyper:
    case eHudStatusSizeHyperReverse:
        break;
    }

    switch (item->statusColor)
    {
    case eHudStatusColorSolid:
        zHudInterp1d(dt, &item->hframe.color.a,
                     item->zHudStatusColorTable[eHudStatusColorSolid].a, 600.0f,
                     eHudInterpTypeLinear);
        break;
    case eHudStatusColorTransparent:
        zHudInterp1d(dt, &item->hframe.color.a,
                     item->zHudStatusColorTable[eHudStatusColorTransparent].a, 300.0f,
                     eHudInterpTypeLinear);
        break;
    }

    item->hframe.ang.x += item->hframe.dang.x * dt;
    item->hframe.ang.y += item->hframe.dang.y * dt;
    item->hframe.ang.z += item->hframe.dang.z * dt;

    if (sShakeSnackCounter)
    {
        item->hframe.pos.x += item->shake;
        item->shake *= -0.8f;
    }
}

// Starts the snack counter shaking from side to side.
void zHud_ShakeSnackCounter()
{
    sShakeSnackCounter = 1;
    sHudSnackArray[0].shake = 5.0f;
}

void zHud_showSnack()
{
    sHudDisplayTimer_snacks = 2.0f;
}

void zHud_showSnackSpecial()
{
    sHudDisplayTimer_snacks = 3.0f;
}

// Shows the "all snacks" message for a moment and tells the player.
void zHudShowAllSnacksArea()
{
    sHudDisplayTimer_allSnacksArea = 2.5f;
    zEntPlayer_Notify(0x20);
}

// Shows the "save ok" message for a moment.
void zHudShowSaveSuccess()
{
    sHudDisplayTimer_saveDone = 1.5f;
}

// The Scooby Snack count at the top right: the snack model with the count beside it.
void zHudElementsCB_SnacksRender()
{
    char str[256] = "";

    sprintf(str, "x%d", globals.player.Inv_ScoobySnacks);
    zHud_DrawText(sHudSnackArray[0].hframe.pos.x, sHudSnackArray[0].hframe.pos.y, 150.0f, 100.0f,
                  str, 0x10, 1.0f, sHudSnackArray[0].hframe.color.a, DROPSHADOW_NORMAL);

    xRect r = { -45.0f + (sHudSnackArray[0].hframe.pos.x - 0.5f * sHudSnackArray[0].hframe.size.x),
                22.0f + (sHudSnackArray[0].hframe.pos.y - 0.5f * sHudSnackArray[0].hframe.size.y),
                sHudSnackArray[0].hframe.size.x * widthAspect, sHudSnackArray[0].hframe.size.y };
    xVec3 from = { 0.0f, 0.0f, 0.8f };
    xVec3 to = { 0.0f, 0.0f, 0.0f };
    xMat4x3 frame;

    xMat3x3Euler((xMat3x3*)&frame, sHudSnackArray[0].hframe.ang.x, sHudSnackArray[0].hframe.ang.y,
                 sHudSnackArray[0].hframe.ang.z);
    frame.pos.x = 0.0f;
    frame.pos.y = 0.0f;
    frame.pos.z = 0.0f;
    frame.flags = 0;
    xModelSetMaterialAlpha(sHudSnackArray[0].model, sHudSnackArray[0].hframe.color.a);
    memcpy(sHudSnackArray[0].model->Mat, &frame, sizeof(xMat4x3));
    xModelSetRender2DStyle(sHudSnackArray[0].renderStyle, sHudSnackArray[0].hframe.color.a);
    xModelRender2D(sHudSnackArray[0].model, &r, &from, &to, PI / 2);
}

// Loads the five gum piece models, one per gum in a pack, and their idle animations.
void zHudElementsCB_GumInit()
{
    xAnimFile* afile;
    U32 size;
    void* buff;
    S32 i;

    sHudGumArray[0].model = zHudLoadModel(eHudModelBubblegumPiece);
    sHudGumArray[0].renderStyle = 8;
    sHudGumArray[1].model = zHudLoadModel(eHudModelBubblegumPiece5);
    sHudGumArray[1].renderStyle = 8;
    sHudGumArray[2].model = zHudLoadModel(eHudModelBubblegumPiece4);
    sHudGumArray[2].renderStyle = 8;
    sHudGumArray[3].model = zHudLoadModel(eHudModelBubblegumPiece3);
    sHudGumArray[3].renderStyle = 8;
    sHudGumArray[4].model = zHudLoadModel(eHudModelBubblegumPiece2);
    sHudGumArray[4].renderStyle = 8;

    for (i = 0; i < 5; i++)
    {
        zHudItem* item = &sHudGumArray[i];

        buff = xSTFindAsset(xStrHash("bat_fly.anm"), &size);
        if (buff)
        {
            afile = xAnimFileNew(buff, "", 0, NULL);
            item->atbl = xAnimTableNew("", NULL);
            xAnimTableNewState(item->atbl, "Idle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
            xAnimTableAddFile(item->atbl, afile, "Idle");
            xAnimPoolAlloc(&globals.sceneCur->mempool, item, item->atbl, item->model);
        }
    }
}

void zHudElementsCB_GumSetup(_xScene* sc)
{
    S32 i;
    xAnimPlay* aplay;
    xAnimSingle* asing;

    oldGumCnt = globals.player.Inv_Ammo[0];

    for (i = 0; i < 5; i++)
    {
        zHudItem* item = &sHudGumArray[i];

        item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 80.0f;
        item->zHudStatusPosTable[eHudStatusPosOnScreen].y = 435.0f;
        item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x = 100.0f;
        item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y = 300.0f;
        item->zHudStatusPosTable[eHudStatusPosOnScreenAlt1].x = 40.0f;
        item->zHudStatusPosTable[eHudStatusPosOnScreenAlt1].y = 435.0f;
        item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 50.0f;
        item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 50.0f;
        item->zHudStatusAngTable[eHudStatusAngStop].x = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngStop].y = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngStop].z = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngIdle].x = 1.5915494f;
        item->zHudStatusAngTable[eHudStatusAngIdle].y = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngIdle].z = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngTest1].x = 1.5915494f;
        item->zHudStatusAngTable[eHudStatusAngTest1].y = 15.915494f;
        item->zHudStatusAngTable[eHudStatusAngTest1].z = 0.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].rm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].gm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].bm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].a = 255.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].rm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].gm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].bm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].a = 0.0f;

        item->statusPos = eHudStatusPosOnScreen;
        item->statusAng = eHudStatusAngStop;
        item->statusSize = eHudStatusSizeDefault;
        item->statusColor = eHudStatusColorSolid;

        item->hframe.pos.x = item->zHudStatusPosTable[eHudStatusPosOnScreen].x;
        item->hframe.pos.y = item->zHudStatusPosTable[eHudStatusPosOnScreen].y;
        item->hframe.dang.x = item->zHudStatusAngTable[eHudStatusAngIdle].x;
        item->hframe.dang.y = item->zHudStatusAngTable[eHudStatusAngIdle].y;
        item->hframe.dang.z = item->zHudStatusAngTable[eHudStatusAngIdle].z;
        item->hframe.size.x = item->zHudStatusSizeTable[eHudStatusSizeDefault].x;
        item->hframe.size.y = item->zHudStatusSizeTable[eHudStatusSizeDefault].y;
        item->hframe.color.a = item->zHudStatusColorTable[eHudStatusColorTransparent].a;

        aplay = item->model->Anim;
        if (aplay)
        {
            asing = aplay->Single;
            asing->Time = 0.0f;
            asing->CurrentSpeed = 1.0f;
            xAnimPlaySetState(aplay->Single, item->model->Anim->Table->StateList, 0.0f);
        }
    }
}

void zHudElementsCB_GumUpdate(_xScene* sc, F32 dt)
{
    S32 i;
    S32 num;

    if (!(globals.player.Inv_Specials & 1))
    {
        for (S32 i = 0; i < 5; i++)
        {
            sHudGumArray[i].statusSize = eHudStatusSizeZero;
        }
    }
    else
    {
        num = globals.player.Inv_Ammo[0] % 5;

        switch (num)
        {
        case 0:
            if (globals.player.Inv_Ammo[0] == 0)
            {
                for (S32 i = 0; i < 5; i++)
                {
                    sHudGumArray[i].statusSize = eHudStatusSizeZero;
                }
            }
            else
            {
                sHudGumArray[0].statusSize = eHudStatusSizeDefault;
                for (S32 i = 1; i < 5; i++)
                {
                    sHudGumArray[i].statusSize = eHudStatusSizeZero;
                }
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            for (S32 i = 0; i < 5; i++)
            {
                if (i == num)
                {
                    sHudGumArray[num].statusSize = eHudStatusSizeDefault;
                }
                else
                {
                    sHudGumArray[i].statusSize = eHudStatusSizeZero;
                }
            }
            break;
        }
    }

    if (globals.player.Inv_Ammo[0] != oldGumCnt)
    {
        oldGumCnt = globals.player.Inv_Ammo[0];
        sHudDisplayTimer_gum = 1.5f;
    }

    if (sHudDisplayTimer_gum == 0.0f)
    {
        for (S32 i = 0; i < 5; i++)
        {
            sHudGumArray[i].statusColor = eHudStatusColorTransparent;
        }
    }
    else
    {
        for (S32 i = 0; i < 5; i++)
        {
            sHudGumArray[i].statusColor = eHudStatusColorSolid;
        }
        sHudDisplayTimer_gum -= dt;
        if (sHudDisplayTimer_gum < 0.0f)
        {
            sHudDisplayTimer_gum = 0.0f;
        }
    }

    for (i = 0; i < 5; i++)
    {
        zHudItem* item = &sHudGumArray[i];

        if ((globals.player.Inv_Ammo[0] - 1) / 5 > 0)
        {
            item->statusPos = eHudStatusPosOnScreen;
        }
        else
        {
            item->statusPos = eHudStatusPosOnScreenAlt1;
        }

        switch (item->statusPos)
        {
        case eHudStatusPosOnScreen:
            zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                         item->zHudStatusPosTable[eHudStatusPosOnScreen].x,
                         item->zHudStatusPosTable[eHudStatusPosOnScreen].y, 250.0f,
                         eHudInterpTypeDecel);
            break;
        case eHudStatusPosOnScreenAlt1:
            zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                         item->zHudStatusPosTable[eHudStatusPosOnScreenAlt1].x,
                         item->zHudStatusPosTable[eHudStatusPosOnScreenAlt1].y, 250.0f,
                         eHudInterpTypeDecel);
            break;
        case eHudStatusPosMiddleOfScreen:
            zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                         item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x,
                         item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y, 250.0f,
                         eHudInterpTypeDecel);
            break;
        }

        switch (item->statusAng)
        {
        case eHudStatusAngStop:
            zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngStop].x,
                         1.0f, eHudInterpTypeLinear);
            break;
        case eHudStatusAngIdle:
            zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngIdle].x,
                         1.0f, eHudInterpTypeLinear);
            break;
        case eHudStatusAngTest1:
            zHudInterp1d(dt, &item->hframe.dang.x,
                         item->zHudStatusAngTable[eHudStatusAngTest1].x, 1.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.dang.y,
                         item->zHudStatusAngTable[eHudStatusAngTest1].y, 1.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.dang.z,
                         item->zHudStatusAngTable[eHudStatusAngTest1].z, 1.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusAngTest2:
            break;
        }

        switch (item->statusSize)
        {
        case eHudStatusSizeDefault:
            item->hframe.size.x = item->zHudStatusSizeTable[eHudStatusSizeDefault].x;
            item->hframe.size.y = item->zHudStatusSizeTable[eHudStatusSizeDefault].y;
            break;
        case eHudStatusSizeZero:
            item->hframe.size.x = item->zHudStatusSizeTable[eHudStatusSizeZero].x;
            item->hframe.size.y = item->zHudStatusSizeTable[eHudStatusSizeZero].y;
            break;
        case eHudStatusSizeSmall:
            break;
        }

        switch (item->statusColor)
        {
        case eHudStatusColorSolid:
            zHudInterp1d(dt, &item->hframe.color.a,
                         item->zHudStatusColorTable[eHudStatusColorSolid].a, 300.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusColorTransparent:
            zHudInterp1d(dt, &item->hframe.color.a,
                         item->zHudStatusColorTable[eHudStatusColorTransparent].a, 120.0f,
                         eHudInterpTypeLinear);
            break;
        }

        item->hframe.ang.x += item->hframe.dang.x * dt;
        item->hframe.ang.y += item->hframe.dang.y * dt;
        item->hframe.ang.z += item->hframe.dang.z * dt;
        xModelUpdate(item->model, dt);
        xModelEval(item->model);
    }
}

// The gum count at the bottom left: one piece of gum for each gum in the current pack of five.
void zHudElementsCB_GumRender()
{
    S32 i;

    if (globals.player.Inv_Specials & 1)
    {
        for (i = 0; i < 5; i++)
        {
            zHudItem* item = &sHudGumArray[i];

            if (item->hframe.color.a != 0.0f)
            {
                char str[256] = "";

                sprintf(str, "%dx", globals.player.Inv_Ammo[0]);

                xRect r = { item->hframe.pos.x - 0.5f * item->hframe.size.x,
                            item->hframe.pos.y - 0.5f * item->hframe.size.y,
                            item->hframe.size.x * widthAspect, item->hframe.size.y };
                xVec3 from = { 2.0f, 0.08f, 0.0f };
                xVec3 to = { 0.0f, 0.0f, 0.0f };
                xMat4x3 frame;

                xMat3x3Euler((xMat3x3*)&frame, item->hframe.ang.x, item->hframe.ang.y,
                             item->hframe.ang.z);
                frame.flags = 0;
                frame.pos.x = 0.0f;
                frame.pos.y = 0.0f;
                frame.pos.z = 0.0f;
                xModelSetMaterialAlpha(item->model, item->hframe.color.a);
                memcpy(item->model->Mat, &frame, sizeof(xMat4x3));
                xModelSetRender2DStyle(item->renderStyle, item->hframe.color.a);
                xModelRender2D(item->model, &r, &from, &to, PI / 2);
            }
        }
    }
}

// Loads the soap bar model and the bubble model the five soap counters share.
void zHudElementsCB_SoapInit()
{
    S32 i;
    xModelInstance* model;

    sHudSoapArray[0].model = zHudLoadModel(eHudModelBarSoap);
    sHudSoapArray[0].renderStyle = 11;

    model = zHudLoadModel(eHudModelSphere);
    for (i = 1; i < 6; i++)
    {
        sHudSoapArray[i].model = model;
        sHudSoapArray[i].renderStyle = 12;
    }
}

void zHudElementsCB_SoapSetup(_xScene* sc)
{
    S32 i;

    oldSoapCnt = globals.player.Inv_Ammo[1];
    sHudDisplayTimer_soap = 1.5f;

    sHudSoapArray[0].zHudStatusPosTable[eHudStatusPosOnScreen].x = 590.0f;
    sHudSoapArray[0].zHudStatusPosTable[eHudStatusPosOnScreen].y = 437.0f;
    sHudSoapArray[0].zHudStatusPosTable[eHudStatusPosOffScreenDefault].x = 586.0f;
    sHudSoapArray[0].zHudStatusPosTable[eHudStatusPosOffScreenDefault].y = 490.0f;
    sHudSoapArray[0].zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x = 200.0f;
    sHudSoapArray[0].zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y = 220.0f;
    sHudSoapArray[0].zHudStatusSizeTable[eHudStatusSizeDefault].x = 35.0f;
    sHudSoapArray[0].zHudStatusSizeTable[eHudStatusSizeDefault].y = 35.0f;
    sHudSoapArray[0].zHudStatusSizeTable[eHudStatusSizeZeroHoriz].x = 20.0f;
    sHudSoapArray[0].zHudStatusSizeTable[eHudStatusSizeZeroHoriz].y = 0.0f;
    sHudSoapArray[0].zHudStatusAngTable[eHudStatusAngStop].x = 0.0f;
    sHudSoapArray[0].zHudStatusAngTable[eHudStatusAngStop].y = 0.0f;
    sHudSoapArray[0].zHudStatusAngTable[eHudStatusAngStop].z = 0.0f;
    sHudSoapArray[0].zHudStatusAngTable[eHudStatusAngIdle].x = 1.5915494f;
    sHudSoapArray[0].zHudStatusAngTable[eHudStatusAngIdle].y = 0.0f;
    sHudSoapArray[0].zHudStatusAngTable[eHudStatusAngIdle].z = 0.0f;
    sHudSoapArray[0].zHudStatusAngTable[eHudStatusAngTest1].x = 1.5915494f;
    sHudSoapArray[0].zHudStatusAngTable[eHudStatusAngTest1].y = 15.915494f;
    sHudSoapArray[0].zHudStatusAngTable[eHudStatusAngTest1].z = 0.0f;
    sHudSoapArray[0].zHudStatusColorTable[eHudStatusColorSolid].rm = 1.0f;
    sHudSoapArray[0].zHudStatusColorTable[eHudStatusColorSolid].gm = 1.0f;
    sHudSoapArray[0].zHudStatusColorTable[eHudStatusColorSolid].bm = 1.0f;
    sHudSoapArray[0].zHudStatusColorTable[eHudStatusColorSolid].a = 255.0f;
    sHudSoapArray[0].zHudStatusColorTable[eHudStatusColorTransparent].rm = 1.0f;
    sHudSoapArray[0].zHudStatusColorTable[eHudStatusColorTransparent].gm = 1.0f;
    sHudSoapArray[0].zHudStatusColorTable[eHudStatusColorTransparent].bm = 1.0f;
    sHudSoapArray[0].zHudStatusColorTable[eHudStatusColorTransparent].a = 0.0f;

    sHudSoapArray[0].statusPos = eHudStatusPosOnScreen;
    sHudSoapArray[0].statusAng = eHudStatusAngStop;
    sHudSoapArray[0].statusSize = eHudStatusSizeDefault;

    sHudSoapArray[0].hframe.pos.x = sHudSoapArray[0].zHudStatusPosTable[eHudStatusPosOnScreen].x;
    sHudSoapArray[0].hframe.pos.y = sHudSoapArray[0].zHudStatusPosTable[eHudStatusPosOnScreen].y;
    sHudSoapArray[0].hframe.dang.x = sHudSoapArray[0].zHudStatusAngTable[eHudStatusAngStop].x;
    sHudSoapArray[0].hframe.dang.y = sHudSoapArray[0].zHudStatusAngTable[eHudStatusAngStop].y;
    sHudSoapArray[0].hframe.dang.z = sHudSoapArray[0].zHudStatusAngTable[eHudStatusAngStop].z;
    sHudSoapArray[0].hframe.size.x =
        sHudSoapArray[0].zHudStatusSizeTable[eHudStatusSizeDefault].x;
    sHudSoapArray[0].hframe.size.y =
        sHudSoapArray[0].zHudStatusSizeTable[eHudStatusSizeDefault].y;

    // The five bubbles curve up the right edge, each smaller than the one before.
    for (i = 1; i < 6; i++)
    {
        zHudItem* item = &sHudSoapArray[i];
        switch (i)
        {
        case 1:
            item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 586.0f;
            item->zHudStatusPosTable[eHudStatusPosOnScreen].y = 419.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 16.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 16.0f;
            break;
        case 2:
            item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 606.0f;
            item->zHudStatusPosTable[eHudStatusPosOnScreen].y = 415.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 14.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 14.0f;
            break;
        case 3:
            item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 592.0f;
            item->zHudStatusPosTable[eHudStatusPosOnScreen].y = 403.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 12.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 12.0f;
            break;
        case 4:
            item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 604.0f;
            item->zHudStatusPosTable[eHudStatusPosOnScreen].y = 398.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 10.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 10.0f;
            break;
        case 5:
            item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 597.0f;
            item->zHudStatusPosTable[eHudStatusPosOnScreen].y = 388.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 8.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 8.0f;
            break;
        }

        item->zHudStatusColorTable[eHudStatusColorSolid].rm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].gm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].bm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].a = 255.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].rm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].gm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].bm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].a = 0.0f;
        item->zHudStatusColorTable[eHudStatusColorLightBlue].rm = 0.8f;
        item->zHudStatusColorTable[eHudStatusColorLightBlue].gm = 0.8f;
        item->zHudStatusColorTable[eHudStatusColorLightBlue].bm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorLightBlue].a = 100.0f;
        item->zHudStatusSizeTable[eHudStatusSizeZero].x = 0.0f;
        item->zHudStatusSizeTable[eHudStatusSizeZero].y = 0.0f;

        item->statusPos = eHudStatusPosOnScreen;
        item->statusAng = eHudStatusAngStop;
        item->statusSize = eHudStatusSizeDefault;
        item->statusColor = eHudStatusColorLightBlue;

        item->hframe.pos.x =
            item->zHudStatusPosTable[eHudStatusPosOnScreen].x;
        item->hframe.pos.y =
            item->zHudStatusPosTable[eHudStatusPosOnScreen].y;
        item->hframe.size.x =
            item->zHudStatusSizeTable[eHudStatusSizeDefault].x;
        item->hframe.size.y =
            item->zHudStatusSizeTable[eHudStatusSizeDefault].y;
    }
}

void zHudElementsCB_SoapUpdate(_xScene* sc, F32 dt)
{
    S32 i;
    S32 num;

    if (!(globals.player.Inv_Specials & 2))
    {
        for (S32 i = 0; i < 6; i++)
        {
            sHudSoapArray[i].statusSize = eHudStatusSizeZero;
        }
    }
    else
    {
        num = globals.player.Inv_Ammo[1] % 5;

        switch (num)
        {
        case 0:
            if (globals.player.Inv_Ammo[1] == 0)
            {
                for (S32 i = 1; i < 6; i++)
                {
                    sHudSoapArray[i].statusSize = eHudStatusSizeZero;
                }
                sHudSoapArray[0].statusPos = eHudStatusPosOffScreenDefault;
            }
            else
            {
                for (S32 i = 1; i < 6; i++)
                {
                    sHudSoapArray[i].statusSize = eHudStatusSizeDefault;
                }
                sHudSoapArray[0].statusColor = eHudStatusColorSolid;
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            for (S32 i = 1; i <= num; i++)
            {
                sHudSoapArray[i].statusSize = eHudStatusSizeDefault;
            }
            for (S32 i = num + 1; i < 6; i++)
            {
                sHudSoapArray[i].statusSize = eHudStatusSizeZero;
            }
            sHudSoapArray[0].statusColor = eHudStatusColorSolid;
            break;
        }
    }

    if (globals.player.Inv_Ammo[1] != oldSoapCnt)
    {
        oldSoapCnt = globals.player.Inv_Ammo[1];
        sHudDisplayTimer_soap = 1.5f;
    }

    if (!(globals.player.Inv_Specials & 2))
    {
        sHudSoapArray[0].statusColor = eHudStatusColorTransparent;
        sHudDisplayTimer_soap = 0.0f;
    }

    if (sHudDisplayTimer_soap == 0.0f)
    {
        for (S32 i = 0; i < 6; i++)
        {
            sHudSoapArray[i].statusColor = eHudStatusColorTransparent;
        }
    }
    else
    {
        for (S32 i = 0; i < 6; i++)
        {
            sHudSoapArray[i].statusColor = eHudStatusColorSolid;
        }
        sHudDisplayTimer_soap -= dt;
        if (sHudDisplayTimer_soap < 0.0f)
        {
            sHudDisplayTimer_soap = 0.0f;
        }
    }

    for (i = 0; i < 6; i++)
    {
        zHudItem* item = &sHudSoapArray[i];
        switch (item->statusPos)
        {
        case eHudStatusPosOnScreen:
            zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                         item->zHudStatusPosTable[eHudStatusPosOnScreen].x,
                         item->zHudStatusPosTable[eHudStatusPosOnScreen].y, 250.0f,
                         eHudInterpTypeDecel);
            break;
        case eHudStatusPosOffScreenDefault:
            zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                         item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].x,
                         item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].y,
                         250.0f, eHudInterpTypeDecel);
            break;
        case eHudStatusPosMiddleOfScreen:
            zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                         item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x,
                         item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y,
                         250.0f, eHudInterpTypeDecel);
            break;
        }

        switch (item->statusAng)
        {
        case eHudStatusAngStop:
            zHudInterp1d(dt, &item->hframe.dang.x,
                         item->zHudStatusAngTable[eHudStatusAngStop].x, 1.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusAngIdle:
            zHudInterp1d(dt, &item->hframe.dang.x,
                         item->zHudStatusAngTable[eHudStatusAngIdle].x, 1.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusAngTest1:
            zHudInterp1d(dt, &item->hframe.dang.x,
                         item->zHudStatusAngTable[eHudStatusAngTest1].x, 1.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.dang.y,
                         item->zHudStatusAngTable[eHudStatusAngTest1].y, 1.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.dang.z,
                         item->zHudStatusAngTable[eHudStatusAngTest1].z, 1.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusAngTest2:
            break;
        }

        switch (item->statusSize)
        {
        case eHudStatusSizeDefault:
            zHudInterp1d(dt, &item->hframe.size.x,
                         item->zHudStatusSizeTable[eHudStatusSizeDefault].x, 90.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.size.y,
                         item->zHudStatusSizeTable[eHudStatusSizeDefault].y, 90.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusSizeZero:
            zHudInterp1d(dt, &item->hframe.size.x,
                         item->zHudStatusSizeTable[eHudStatusSizeZero].x, 90.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.size.y,
                         item->zHudStatusSizeTable[eHudStatusSizeZero].y, 90.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusSizeSquash:
            break;
        case eHudStatusSizeZeroHoriz:
            zHudInterp1d(dt, &item->hframe.size.y,
                         item->zHudStatusSizeTable[eHudStatusSizeZeroHoriz].y, 7.2f,
                         eHudInterpTypeDecel);
            break;
        }

        switch (item->statusColor)
        {
        case eHudStatusColorSolid:
            zHudInterp1d(dt, &item->hframe.color.a,
                         item->zHudStatusColorTable[eHudStatusColorSolid].a, 300.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusColorTransparent:
            zHudInterp1d(dt, &item->hframe.color.a,
                         item->zHudStatusColorTable[eHudStatusColorTransparent].a,
                         150.0f, eHudInterpTypeLinear);
            break;
        case eHudStatusColorLightBlue:
        case eHudStatusColorOpaque2:
            item->hframe.color.a =
                item->zHudStatusColorTable[item->statusColor].a;
            break;
        }

        item->hframe.ang.x += item->hframe.dang.x * dt;
        item->hframe.ang.y += item->hframe.dang.y * dt;
        item->hframe.ang.z += item->hframe.dang.z * dt;
        xModelSetMaterialAlpha(item->model, item->hframe.color.a);
    }
}

// The soap count at the bottom right: the soap bar with the count beside it, and a bubble for
// each soap in the current box of five.
void zHudElementsCB_SoapRender()
{
    S32 i;
    char str[256] = "";

    sprintf(str, "%dx", globals.player.Inv_Ammo[1]);
    zHud_DrawText(sHudSoapArray[0].hframe.pos.x, sHudSoapArray[0].hframe.pos.y, 150.0f, 100.0f,
                  str, 0x20, 1.0f, 200, DROPSHADOW_NORMAL);

    xRect r = { sHudSoapArray[0].hframe.pos.x - 0.5f * sHudSoapArray[0].hframe.size.x,
                sHudSoapArray[0].hframe.pos.y - 0.5f * sHudSoapArray[0].hframe.size.y,
                sHudSoapArray[0].hframe.size.x * widthAspect, sHudSoapArray[0].hframe.size.y };
    xVec3 from = { 0.0f, -0.35f, 0.4f };
    xVec3 to = { 0.0f, 0.0f, 0.0f };
    xMat4x3 frame;

    xMat3x3Euler((xMat3x3*)&frame, sHudSoapArray[0].hframe.ang.x, sHudSoapArray[0].hframe.ang.y,
                 sHudSoapArray[0].hframe.ang.z);
    frame.pos.x = 0.0f;
    frame.pos.y = 0.0f;
    frame.pos.z = 0.0f;
    frame.flags = 0;
    xModelSetMaterialAlpha(sHudSoapArray[0].model, sHudSoapArray[0].hframe.color.a);
    memcpy(sHudSoapArray[0].model->Mat, &frame, sizeof(xMat4x3));
    xModelSetRender2DStyle(sHudSoapArray[0].renderStyle, sHudSoapArray[0].hframe.color.a);
    xModelRender2D(sHudSoapArray[0].model, &r, &from, &to, PI / 2);

    for (i = 1; i < 6; i++)
    {
        zHudItem* item = &sHudSoapArray[i];
        if (item->hframe.color.a != 0.0f)
        {
            xRect r = { item->hframe.pos.x - 0.5f * item->hframe.size.x,
                        item->hframe.pos.y - 0.5f * item->hframe.size.y,
                        item->hframe.size.x * widthAspect,
                        item->hframe.size.y };
            xVec3 from = { 0.0f, 0.0f, 2.0f };
            xVec3 to = { 0.0f, 0.0f, 0.0f };
            xMat4x3 frame;

            xMat3x3Euler((xMat3x3*)&frame, item->hframe.ang.x,
                         item->hframe.ang.y, item->hframe.ang.z);
            frame.flags = 0;
            frame.pos.x = 0.0f;
            frame.pos.y = 0.0f;
            frame.pos.z = 0.0f;
            memcpy(item->model->Mat, &frame, sizeof(xMat4x3));

            RwRGBA colsoap = { 158, 215, 224, 0 };
            colsoap.alpha = item->hframe.color.a;
            item->model->Data->geometry->matList.materials[0]->color = colsoap;
            xModelSetRender2DStyle(item->renderStyle, item->hframe.color.a);
            xModelRender2D(item->model, &r, &from, &to, PI / 2);
        }
    }
}

void zHudElementsCB_MessageInit()
{
}

void zHudElementsCB_MessageSetup(_xScene* sc)
{
}

void zHudElementsCB_MessageUpdate(_xScene* sc, F32 dt)
{
}

void zHudElementsCB_MessageRender()
{
}

// Loads the soap box model and its idle animation.
void zHudElementsCB_SoapboxInit()
{
    xAnimFile* afile;
    U32 size;
    void* buff;

    sHudSoapboxArray[0].model = zHudLoadModel(eHudModelBoxSoap);
    sHudSoapboxArray[0].renderStyle = 13;

    buff = xSTFindAsset(xStrHash("bat_fly.anm"), &size);
    if (buff)
    {
        afile = xAnimFileNew(buff, "", 0, NULL);
        sHudSoapboxArray[0].atbl = xAnimTableNew("", NULL);
        xAnimTableNewState(sHudSoapboxArray[0].atbl, "Idle", 0x10, 0, 1.0f, NULL, NULL, 0.0f,
                           NULL);
        xAnimTableAddFile(sHudSoapboxArray[0].atbl, afile, "Idle");
        xAnimPoolAlloc(&globals.sceneCur->mempool, &sHudSoapboxArray[0],
                       sHudSoapboxArray[0].atbl, sHudSoapboxArray[0].model);
    }
}

void zHudElementsCB_SoapboxSetup(_xScene* sc)
{
    zHudItem* item = &sHudSoapboxArray[0];
    xAnimPlay* aplay;
    xAnimSingle* asing;

    oldSoapCnt2 = globals.player.Inv_Ammo[1];
    sHudDisplayTimer_soap = 1.5f;

    item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 492.0f;
    item->zHudStatusPosTable[eHudStatusPosOnScreen].y = 475.0f;
    item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].x = 486.0f;
    item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].y = 575.0f;
    item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x = 200.0f;
    item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y = 220.0f;
    item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 105.0f;
    item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 105.0f;
    item->zHudStatusAngTable[eHudStatusAngStop].x = 0.0f;
    item->zHudStatusAngTable[eHudStatusAngStop].y = 0.0f;
    item->zHudStatusAngTable[eHudStatusAngStop].z = 0.0f;
    item->zHudStatusAngTable[eHudStatusAngIdle].x = 1.5915494f;
    item->zHudStatusAngTable[eHudStatusAngIdle].y = 0.0f;
    item->zHudStatusAngTable[eHudStatusAngIdle].z = 0.0f;
    item->zHudStatusAngTable[eHudStatusAngTest1].x = 1.5915494f;
    item->zHudStatusAngTable[eHudStatusAngTest1].y = 15.915494f;
    item->zHudStatusAngTable[eHudStatusAngTest1].z = 0.0f;
    item->zHudStatusColorTable[eHudStatusColorSolid].rm = 1.0f;
    item->zHudStatusColorTable[eHudStatusColorSolid].gm = 1.0f;
    item->zHudStatusColorTable[eHudStatusColorSolid].bm = 1.0f;
    item->zHudStatusColorTable[eHudStatusColorSolid].a = 255.0f;
    item->zHudStatusColorTable[eHudStatusColorTransparent].rm = 1.0f;
    item->zHudStatusColorTable[eHudStatusColorTransparent].gm = 1.0f;
    item->zHudStatusColorTable[eHudStatusColorTransparent].bm = 1.0f;
    item->zHudStatusColorTable[eHudStatusColorTransparent].a = 0.0f;

    item->statusPos = eHudStatusPosOnScreen;
    item->statusAng = eHudStatusAngStop;
    item->statusSize = eHudStatusSizeDefault;
    item->statusColor = eHudStatusColorSolid;

    if ((globals.player.Inv_Specials & 2) && (globals.player.Inv_Ammo[1] - 1) / 5 > 0)
    {
        item->hframe.pos.x = item->zHudStatusPosTable[eHudStatusPosOnScreen].x;
        item->hframe.pos.y = item->zHudStatusPosTable[eHudStatusPosOnScreen].y;
    }
    else
    {
        item->hframe.pos.x = item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].x;
        item->hframe.pos.y = item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].y;
    }

    item->hframe.dang.x = item->zHudStatusAngTable[eHudStatusAngStop].x;
    item->hframe.dang.y = item->zHudStatusAngTable[eHudStatusAngStop].y;
    item->hframe.dang.z = item->zHudStatusAngTable[eHudStatusAngStop].z;
    item->hframe.size.x = item->zHudStatusSizeTable[eHudStatusSizeDefault].x;
    item->hframe.size.y = item->zHudStatusSizeTable[eHudStatusSizeDefault].y;
    item->hframe.color.a = item->zHudStatusColorTable[eHudStatusColorTransparent].a;

    aplay = item->model->Anim;
    if (aplay)
    {
        asing = aplay->Single;
        asing->Time = 0.0f;
        asing->CurrentSpeed = 1.0f;
        xAnimPlaySetState(aplay->Single, item->model->Anim->Table->StateList, 0.0f);
    }
}

void zHudElementsCB_SoapboxUpdate(_xScene* sc, F32 dt)
{
    zHudItem* item = &sHudSoapboxArray[0];

    if (!(globals.player.Inv_Specials & 2))
    {
        item->statusPos = eHudStatusPosOffScreenDefault;
    }
    else
    {
        item->statusPos = eHudStatusPosOffScreenDefault;
    }

    if ((globals.player.Inv_Specials & 2) && (globals.player.Inv_Ammo[1] - 1) / 5 > 0)
    {
        item->statusPos = eHudStatusPosOnScreen;
    }
    else
    {
        item->statusPos = eHudStatusPosOffScreenDefault;
    }

    if (globals.player.Inv_Ammo[1] != oldSoapCnt2)
    {
        oldSoapCnt2 = globals.player.Inv_Ammo[1];
    }

    if (!(globals.player.Inv_Specials & 2))
    {
        sHudSoapboxArray[0].statusColor = eHudStatusColorTransparent;
    }
    else if (sHudDisplayTimer_soap == 0.0f)
    {
        sHudSoapboxArray[0].statusColor = eHudStatusColorTransparent;
    }
    else
    {
        sHudSoapboxArray[0].statusColor = eHudStatusColorSolid;
    }

    switch (item->statusPos)
    {
    case eHudStatusPosOnScreen:
        zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                     item->zHudStatusPosTable[eHudStatusPosOnScreen].x,
                     item->zHudStatusPosTable[eHudStatusPosOnScreen].y, 2500.0f,
                     eHudInterpTypeDecel);
        break;
    case eHudStatusPosOffScreenDefault:
        zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                     item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].x,
                     item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].y, 40.0f,
                     eHudInterpTypeAccel);
        break;
    case eHudStatusPosMiddleOfScreen:
        zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                     item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x,
                     item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y, 250.0f,
                     eHudInterpTypeDecel);
        break;
    }

    switch (item->statusAng)
    {
    case eHudStatusAngStop:
        zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngStop].x,
                     1.0f, eHudInterpTypeLinear);
        break;
    case eHudStatusAngIdle:
        zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngIdle].x,
                     1.0f, eHudInterpTypeLinear);
        break;
    case eHudStatusAngTest1:
        zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngTest1].x,
                     1.0f, eHudInterpTypeLinear);
        zHudInterp1d(dt, &item->hframe.dang.y, item->zHudStatusAngTable[eHudStatusAngTest1].y,
                     1.0f, eHudInterpTypeLinear);
        zHudInterp1d(dt, &item->hframe.dang.z, item->zHudStatusAngTable[eHudStatusAngTest1].z,
                     1.0f, eHudInterpTypeLinear);
        break;
    case eHudStatusAngTest2:
        break;
    }

    switch (item->statusSize)
    {
    case eHudStatusSizeDefault:
        zHudInterp1d(dt, &item->hframe.size.x,
                     item->zHudStatusSizeTable[eHudStatusSizeDefault].x, 10.0f,
                     eHudInterpTypeLinear);
        zHudInterp1d(dt, &item->hframe.size.y,
                     item->zHudStatusSizeTable[eHudStatusSizeDefault].y, 10.0f,
                     eHudInterpTypeLinear);
        break;
    case eHudStatusSizeZero:
        item->hframe.size.x = item->zHudStatusSizeTable[eHudStatusSizeZero].x;
        item->hframe.size.y = item->zHudStatusSizeTable[eHudStatusSizeZero].y;
        break;
    case eHudStatusSizeSmall:
        break;
    }

    switch (item->statusColor)
    {
    case eHudStatusColorSolid:
        zHudInterp1d(dt, &item->hframe.color.a,
                     item->zHudStatusColorTable[eHudStatusColorSolid].a, 300.0f,
                     eHudInterpTypeLinear);
        break;
    case eHudStatusColorTransparent:
        zHudInterp1d(dt, &item->hframe.color.a,
                     item->zHudStatusColorTable[eHudStatusColorTransparent].a, 150.0f,
                     eHudInterpTypeLinear);
        break;
    case eHudStatusColorLightBlue:
        break;
    case eHudStatusColorOpaque2:
        break;
    }

    item->hframe.ang.x += item->hframe.dang.x * dt;
    item->hframe.ang.y += item->hframe.dang.y * dt;
    item->hframe.ang.z += item->hframe.dang.z * dt;
    xModelUpdate(item->model, dt);
    xModelEval(item->model);
}

// The soap box under the soap count while the player carries one: the box model and the
// number of full boxes left.
void zHudElementsCB_SoapboxRender()
{
    zHudItem* item = &sHudSoapboxArray[0];

    if (globals.player.Inv_Specials & 2)
    {
        xRect r = { 23.0f + (item->hframe.pos.x - 0.5f * item->hframe.size.x),
                    17.0f + (item->hframe.pos.y - 0.5f * item->hframe.size.y),
                    item->hframe.size.x * widthAspect, item->hframe.size.y };
        xVec3 from = { -1.3f, 0.3f, 2.0f };
        xVec3 to = { 0.0f, 0.0f, 0.0f };
        xMat4x3 frame;

        xMat3x3Euler((xMat3x3*)&frame, item->hframe.ang.x, item->hframe.ang.y,
                     item->hframe.ang.z);
        frame.flags = 0;
        frame.pos.x = 0.0f;
        frame.pos.y = 0.0f;
        frame.pos.z = 0.0f;
        xModelSetMaterialAlpha(item->model, item->hframe.color.a);
        memcpy(item->model->Mat, &frame, sizeof(xMat4x3));

        if (globals.player.Inv_Specials & 2)
        {
            xModelSetRender2DStyle(item->renderStyle, item->hframe.color.a);
            xModelRender2D(item->model, &r, &from, &to, PI / 2);
        }

        if (globals.player.Inv_Specials & 2)
        {
            char str[256] = "";
            S32 textOffset[2] = { -20, -70 };

            sprintf(str, "%d", (globals.player.Inv_Ammo[1] - 1) / 5);
            zHud_DrawText(item->hframe.pos.x + textOffset[0],
                          item->hframe.pos.y + textOffset[1], 90.0f, 100.0f, str, 0x40, 1.0f,
                          item->hframe.color.a, DROPSHADOW_NORMAL);
        }
    }
}

// Loads the gum pack model and its idle animation.
void zHudElementsCB_GumpackInit()
{
    xAnimFile* afile;
    U32 size;
    void* buff;

    sHudGumpackArray[0].model = zHudLoadModel(eHudModelSphere);
    sHudGumpackArray[0].renderStyle = 7;

    buff = xSTFindAsset(xStrHash("bat_fly.anm"), &size);
    if (buff)
    {
        afile = xAnimFileNew(buff, "", 0, NULL);
        sHudGumpackArray[0].atbl = xAnimTableNew("", NULL);
        xAnimTableNewState(sHudGumpackArray[0].atbl, "Idle", 0x10, 0, 1.0f, NULL, NULL, 0.0f,
                           NULL);
        xAnimTableAddFile(sHudGumpackArray[0].atbl, afile, "Idle");
        xAnimPoolAlloc(&globals.sceneCur->mempool, &sHudGumpackArray[0],
                       sHudGumpackArray[0].atbl, sHudGumpackArray[0].model);
    }
}

void zHudElementsCB_GumpackSetup(_xScene* sc)
{
    zHudItem* item = &sHudGumpackArray[0];
    xAnimPlay* aplay;
    xAnimSingle* asing;

    oldGumCnt2 = globals.player.Inv_Ammo[0];
    sHudDisplayTimer_gum = 1.5f;

    item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 1.0f;
    item->zHudStatusPosTable[eHudStatusPosOnScreen].y = 480.0f;
    item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].x = 1.0f;
    item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].y = 580.0f;
    item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x = 200.0f;
    item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y = 220.0f;
    item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 60.0f;
    item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 60.0f;
    item->zHudStatusAngTable[eHudStatusAngStop].x = 0.0f;
    item->zHudStatusAngTable[eHudStatusAngStop].y = 0.0f;
    item->zHudStatusAngTable[eHudStatusAngStop].z = 0.0f;
    item->zHudStatusAngTable[eHudStatusAngIdle].x = 1.5915494f;
    item->zHudStatusAngTable[eHudStatusAngIdle].y = 0.0f;
    item->zHudStatusAngTable[eHudStatusAngIdle].z = 0.0f;
    item->zHudStatusAngTable[eHudStatusAngTest1].x = 1.5915494f;
    item->zHudStatusAngTable[eHudStatusAngTest1].y = 15.915494f;
    item->zHudStatusAngTable[eHudStatusAngTest1].z = 0.0f;
    item->zHudStatusColorTable[eHudStatusColorSolid].rm = 1.0f;
    item->zHudStatusColorTable[eHudStatusColorSolid].gm = 1.0f;
    item->zHudStatusColorTable[eHudStatusColorSolid].bm = 1.0f;
    item->zHudStatusColorTable[eHudStatusColorSolid].a = 255.0f;
    item->zHudStatusColorTable[eHudStatusColorTransparent].rm = 1.0f;
    item->zHudStatusColorTable[eHudStatusColorTransparent].gm = 1.0f;
    item->zHudStatusColorTable[eHudStatusColorTransparent].bm = 1.0f;
    item->zHudStatusColorTable[eHudStatusColorTransparent].a = 0.0f;

    item->statusPos = eHudStatusPosOnScreen;
    item->statusAng = eHudStatusAngStop;
    item->statusSize = eHudStatusSizeDefault;
    item->statusColor = eHudStatusColorSolid;

    if ((globals.player.Inv_Specials & 1) && (globals.player.Inv_Ammo[0] - 1) / 5 > 0)
    {
        item->hframe.pos.x = item->zHudStatusPosTable[eHudStatusPosOnScreen].x;
        item->hframe.pos.y = item->zHudStatusPosTable[eHudStatusPosOnScreen].y;
    }
    else
    {
        item->hframe.pos.x = item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].x;
        item->hframe.pos.y = item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].y;
    }

    item->hframe.dang.x = item->zHudStatusAngTable[eHudStatusAngStop].x;
    item->hframe.dang.y = item->zHudStatusAngTable[eHudStatusAngStop].y;
    item->hframe.dang.z = item->zHudStatusAngTable[eHudStatusAngStop].z;
    item->hframe.size.x = item->zHudStatusSizeTable[eHudStatusSizeDefault].x;
    item->hframe.size.y = item->zHudStatusSizeTable[eHudStatusSizeDefault].y;
    item->hframe.color.a = item->zHudStatusColorTable[eHudStatusColorTransparent].a;

    aplay = item->model->Anim;
    if (aplay)
    {
        asing = aplay->Single;
        asing->Time = 0.0f;
        asing->CurrentSpeed = 1.0f;
        xAnimPlaySetState(aplay->Single, item->model->Anim->Table->StateList, 0.0f);
    }
}

void zHudElementsCB_GumpackUpdate(_xScene* sc, F32 dt)
{
    zHudItem* item = &sHudGumpackArray[0];

    if (!(globals.player.Inv_Specials & 1))
    {
        item->statusSize = eHudStatusSizeZero;
    }
    else
    {
        item->statusSize = eHudStatusSizeDefault;
    }

    if ((globals.player.Inv_Ammo[0] - 1) / 5 > 0)
    {
        item->statusPos = eHudStatusPosOnScreen;
    }
    else
    {
        item->statusPos = eHudStatusPosOffScreenDefault;
    }

    if (globals.player.Inv_Ammo[0] != oldGumCnt2)
    {
        oldGumCnt2 = globals.player.Inv_Ammo[0];
        sHudDisplayTimer_gum = 1.5f;
    }

    if (sHudDisplayTimer_gum == 0.0f)
    {
        sHudGumpackArray[0].statusColor = eHudStatusColorTransparent;
    }
    else
    {
        sHudGumpackArray[0].statusColor = eHudStatusColorSolid;
        sHudDisplayTimer_gum -= dt;
        if (sHudDisplayTimer_gum < 0.0f)
        {
            sHudDisplayTimer_gum = 0.0f;
        }
    }

    switch (item->statusPos)
    {
    case eHudStatusPosOnScreen:
        zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                     item->zHudStatusPosTable[eHudStatusPosOnScreen].x,
                     item->zHudStatusPosTable[eHudStatusPosOnScreen].y, 2500.0f,
                     eHudInterpTypeDecel);
        break;
    case eHudStatusPosOffScreenDefault:
        zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                     item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].x,
                     item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].y, 40.0f,
                     eHudInterpTypeAccel);
        break;
    case eHudStatusPosMiddleOfScreen:
        zHudInterp2d(dt, &item->hframe.pos.x, &item->hframe.pos.y,
                     item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x,
                     item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y, 250.0f,
                     eHudInterpTypeDecel);
        break;
    }

    switch (item->statusAng)
    {
    case eHudStatusAngStop:
        zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngStop].x,
                     1.0f, eHudInterpTypeLinear);
        break;
    case eHudStatusAngIdle:
        zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngIdle].x,
                     1.0f, eHudInterpTypeLinear);
        break;
    case eHudStatusAngTest1:
        zHudInterp1d(dt, &item->hframe.dang.x, item->zHudStatusAngTable[eHudStatusAngTest1].x,
                     1.0f, eHudInterpTypeLinear);
        zHudInterp1d(dt, &item->hframe.dang.y, item->zHudStatusAngTable[eHudStatusAngTest1].y,
                     1.0f, eHudInterpTypeLinear);
        zHudInterp1d(dt, &item->hframe.dang.z, item->zHudStatusAngTable[eHudStatusAngTest1].z,
                     1.0f, eHudInterpTypeLinear);
        break;
    case eHudStatusAngTest2:
        break;
    }

    switch (item->statusSize)
    {
    case eHudStatusSizeDefault:
        zHudInterp1d(dt, &item->hframe.size.x,
                     item->zHudStatusSizeTable[eHudStatusSizeDefault].x, 150.0f,
                     eHudInterpTypeLinear);
        zHudInterp1d(dt, &item->hframe.size.y,
                     item->zHudStatusSizeTable[eHudStatusSizeDefault].y, 150.0f,
                     eHudInterpTypeLinear);
        break;
    case eHudStatusSizeZero:
        item->hframe.size.x = item->zHudStatusSizeTable[eHudStatusSizeZero].x;
        item->hframe.size.y = item->zHudStatusSizeTable[eHudStatusSizeZero].y;
        break;
    case eHudStatusSizeSmall:
        break;
    }

    switch (item->statusColor)
    {
    case eHudStatusColorSolid:
        zHudInterp1d(dt, &item->hframe.color.a,
                     item->zHudStatusColorTable[eHudStatusColorSolid].a, 300.0f,
                     eHudInterpTypeLinear);
        break;
    case eHudStatusColorTransparent:
        zHudInterp1d(dt, &item->hframe.color.a,
                     item->zHudStatusColorTable[eHudStatusColorTransparent].a, 120.0f,
                     eHudInterpTypeLinear);
        break;
    case eHudStatusColorOpaque2:
        break;
    }

    item->hframe.ang.x += item->hframe.dang.x * dt;
    item->hframe.ang.y += item->hframe.dang.y * dt;
    item->hframe.ang.z += item->hframe.dang.z * dt;
    xModelUpdate(item->model, dt);
    xModelEval(item->model);
}

// The gum pack under the gum count while the player carries one: the pack model and the
// number of full packs left.
void zHudElementsCB_GumpackRender()
{
    zHudItem* item = &sHudGumpackArray[0];
    xRect r = { 33.0f + (item->hframe.pos.x - 0.5f * item->hframe.size.x),
                21.0f + (item->hframe.pos.y - 0.5f * item->hframe.size.y),
                item->hframe.size.x * widthAspect, item->hframe.size.y };
    xVec3 from = { 1.8f, 0.0f, 0.0f };
    xVec3 to = { 0.0f, 0.0f, 0.0f };
    xMat4x3 frame;

    xMat3x3Euler((xMat3x3*)&frame, item->hframe.ang.x, item->hframe.ang.y, item->hframe.ang.z);
    frame.pos.x = 0.0f;
    frame.pos.y = 0.0f;
    frame.pos.z = 0.0f;
    frame.flags = 0;
    memcpy(sHudGumpackArray[0].model->Mat, &frame, sizeof(xMat4x3));

    RwRGBA colgum = { 245, 188, 188, 0 };
    colgum.alpha = sHudGumpackArray[0].hframe.color.a;
    sHudGumpackArray[0].model->Data->geometry->matList.materials[0]->color = colgum;

    if (globals.player.Inv_Specials & 1)
    {
        xModelSetRender2DStyle(item->renderStyle, item->hframe.color.a);
        xModelRender2D(item->model, &r, &from, &to, PI / 2);
    }

    if (globals.player.Inv_Specials & 1)
    {
        char str[256] = "";
        S32 textOffset[2] = { 17, -70 };

        sprintf(str, "%d", (globals.player.Inv_Ammo[0] - 1) / 5);
        zHud_DrawText(item->hframe.pos.x + textOffset[0], item->hframe.pos.y + textOffset[1],
                      90.0f, 70.0f, str, 0x40, 1.0f, item->hframe.color.a, DROPSHADOW_NORMAL);
    }
}

// Loads the collar model the health bar shows and its idle animation.
void zHudElementsCB_HealthbarInit()
{
    xAnimFile* afile;
    U32 size;
    void* buff;
    S32 i;
    xModelInstance* model;

    model = zHudLoadModel(eHudModelScoobyCollar);

    for (i = 0; i < 5; i++)
    {
        sHudHealthbarArray[i].model = model;
        sHudHealthbarArray[i].renderStyle = 14;
    }

    for (i = 0; i < 5; i++)
    {
        switch (i)
        {
        case 0:
            buff = xSTFindAsset(xStrHash("rhsa0001.anm"), &size);
            break;
        case 1:
            buff = xSTFindAsset(xStrHash("rhsa0001.anm"), &size);
            break;
        case 2:
            buff = xSTFindAsset(xStrHash("rhsa0001.anm"), &size);
            break;
        case 3:
            buff = xSTFindAsset(xStrHash("rhsa0001.anm"), &size);
            break;
        case 4:
            buff = xSTFindAsset(xStrHash("rhsa0001.anm"), &size);
            break;
        }

        if (buff)
        {
            afile = xAnimFileNew(buff, "", 0, NULL);
            sHudHealthbarArray[i].atbl = xAnimTableNew("", NULL);
            xAnimTableNewState(sHudHealthbarArray[i].atbl, "Idle", 0x10, 0, 1.0f, NULL, NULL,
                               0.0f, NULL);
            xAnimTableAddFile(sHudHealthbarArray[i].atbl, afile, "Idle");
            xAnimPoolAlloc(&globals.sceneCur->mempool, &sHudHealthbarArray[i],
                           sHudHealthbarArray[i].atbl, sHudHealthbarArray[i].model);
        }
    }
}

void zHudElementsCB_HealthbarSetup(_xScene* sc)
{
    S32 i;
    xAnimPlay* aplay;
    xAnimSingle* asing;

    oldHealthCnt2 = globals.player.Health;

    for (i = 0; i < 5; i++)
    {
        sHudHealthbarArray[i].zHudStatusPosTable[eHudStatusPosOnScreen].x = 17.0f * i + 20.0f;
        sHudHealthbarArray[i].zHudStatusPosTable[eHudStatusPosOnScreen].y = 115.0f;
        sHudHealthbarArray[i].zHudStatusPosTable[eHudStatusPosOffScreenDefault].x =
            sHudHealthbarArray[i].zHudStatusPosTable[eHudStatusPosOnScreen].x - 100.0f;
        sHudHealthbarArray[i].zHudStatusPosTable[eHudStatusPosOffScreenDefault].y = 115.0f;
        sHudHealthbarArray[i].zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x = 300.0f;
        sHudHealthbarArray[i].zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y = 200.0f;
        sHudHealthbarArray[i].zHudStatusSizeTable[eHudStatusSizeDefault].x = 17.0f;
        sHudHealthbarArray[i].zHudStatusSizeTable[eHudStatusSizeDefault].y = 17.0f;
        sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngStop].x = 0.0f;
        sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngStop].y = 0.0f;
        sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngStop].z = 0.0f;
        sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngIdle].x = 1.5915494f;
        sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngIdle].y = 0.0f;
        sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngIdle].z = 0.0f;
        sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngFast].x = 6.3661976f;
        sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngFast].y = 0.0f;
        sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngFast].z = 0.0f;
        sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngTest1].x = 1.5915494f;
        sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngTest1].y = 15.915494f;
        sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngTest1].z = 0.0f;
        sHudHealthbarArray[i].zHudStatusColorTable[eHudStatusColorSolid].rm = 1.0f;
        sHudHealthbarArray[i].zHudStatusColorTable[eHudStatusColorSolid].gm = 1.0f;
        sHudHealthbarArray[i].zHudStatusColorTable[eHudStatusColorSolid].bm = 1.0f;
        sHudHealthbarArray[i].zHudStatusColorTable[eHudStatusColorSolid].a = 255.0f;
        sHudHealthbarArray[i].zHudStatusColorTable[eHudStatusColorTransparent].rm = 1.0f;
        sHudHealthbarArray[i].zHudStatusColorTable[eHudStatusColorTransparent].gm = 1.0f;
        sHudHealthbarArray[i].zHudStatusColorTable[eHudStatusColorTransparent].bm = 1.0f;
        sHudHealthbarArray[i].zHudStatusColorTable[eHudStatusColorTransparent].a = 0.0f;

        sHudHealthbarArray[i].statusPos = eHudStatusPosOnScreen;
        sHudHealthbarArray[i].statusAng = eHudStatusAngStop;
        sHudHealthbarArray[i].statusSize = eHudStatusSizeDefault;
        sHudHealthbarArray[i].statusColor = eHudStatusColorSolid;

        sHudHealthbarArray[i].hframe.pos.x =
            sHudHealthbarArray[i].zHudStatusPosTable[eHudStatusPosOnScreen].x;
        sHudHealthbarArray[i].hframe.pos.y =
            sHudHealthbarArray[i].zHudStatusPosTable[eHudStatusPosOnScreen].y;
        sHudHealthbarArray[i].hframe.dang.x =
            sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngStop].x;
        sHudHealthbarArray[i].hframe.dang.y =
            sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngStop].y;
        sHudHealthbarArray[i].hframe.dang.z =
            sHudHealthbarArray[i].zHudStatusAngTable[eHudStatusAngStop].z;
        sHudHealthbarArray[i].hframe.size.x =
            sHudHealthbarArray[i].zHudStatusSizeTable[eHudStatusSizeDefault].x;
        sHudHealthbarArray[i].hframe.size.y =
            sHudHealthbarArray[i].zHudStatusSizeTable[eHudStatusSizeDefault].y;
        sHudHealthbarArray[i].hframe.color.a =
            sHudHealthbarArray[i].zHudStatusColorTable[eHudStatusColorSolid].a;

        aplay = sHudHealthbarArray[i].model->Anim;
        if (aplay)
        {
            asing = aplay->Single;
            asing->Time = 0.0f;
            asing->CurrentSpeed = 1.0f;
            xAnimPlaySetState(aplay->Single, sHudHealthbarArray[i].model->Anim->Table->StateList,
                              0.0f);
        }
    }
}

void zHudElementsCB_HealthbarUpdate(_xScene* sc, F32 dt)
{
    S32 i;
    S32 num = globals.player.Health - 1;

    switch (num)
    {
    case -1:
        for (S32 i = 0; i < 5; i++)
        {
            sHudHealthbarArray[i].statusSize = eHudStatusSizeZero;
        }
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        for (S32 i = 0; i < 5; i++)
        {
            for (S32 i = 0; i <= num; i++)
            {
                sHudHealthbarArray[i].statusSize = eHudStatusSizeDefault;
            }
            for (S32 i = num + 1; i < 5; i++)
            {
                sHudHealthbarArray[i].statusSize = eHudStatusSizeZero;
            }
        }
        break;
    }

    if (globals.player.Health != oldHealthCnt2)
    {
        oldHealthCnt2 = globals.player.Health;
        sHudDisplayTimer_healthBar = 2.0f;
    }

    if (sHudDisplayTimer_healthBar == 0.0f)
    {
        for (i = 0; i < 5; i++)
        {
            sHudHealthbarArray[i].statusColor = eHudStatusColorTransparent;
            sHudHealthbarArray[i].statusPos = eHudStatusPosOffScreenDefault;
        }
    }
    else
    {
        for (i = 0; i < 5; i++)
        {
            sHudHealthbarArray[i].statusColor = eHudStatusColorSolid;
            sHudHealthbarArray[i].statusPos = eHudStatusPosOnScreen;
        }
        sHudDisplayTimer_healthBar -= dt;
        if (sHudDisplayTimer_healthBar < 0.0f)
        {
            sHudDisplayTimer_healthBar = 0.0f;
        }
    }

    dynOffset = (5 - globals.player.Health) * sHudHealthbarArray[0].hframe.size.x * 0.5f;

    for (i = 0; i < 5; i++)
    {
        zHudItem* item = &sHudHealthbarArray[i];
        item->zHudStatusPosTable[eHudStatusPosOnScreen].x =
            20.0f + dynOffset + 17.0f * i;

        switch (item->statusPos)
        {
        case eHudStatusPosOnScreen:
            zHudInterp2d(dt, &item->hframe.pos.x,
                         &item->hframe.pos.y,
                         item->zHudStatusPosTable[eHudStatusPosOnScreen].x,
                         item->zHudStatusPosTable[eHudStatusPosOnScreen].y,
                         1000.0f, eHudInterpTypeDecel);
            break;
        case eHudStatusPosOffScreenDefault:
            zHudInterp2d(
                dt, &item->hframe.pos.x, &item->hframe.pos.y,
                item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].x,
                item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].y, 125.0f,
                eHudInterpTypeLinear);
            break;
        case eHudStatusPosMiddleOfScreen:
            zHudInterp2d(
                dt, &item->hframe.pos.x, &item->hframe.pos.y,
                item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x,
                item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y, 250.0f,
                eHudInterpTypeDecel);
            break;
        }

        switch (item->statusAng)
        {
        case eHudStatusAngStop:
            zHudInterp1d(dt, &item->hframe.dang.x,
                         item->zHudStatusAngTable[eHudStatusAngStop].x, 1.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusAngIdle:
            zHudInterp1d(dt, &item->hframe.dang.x,
                         item->zHudStatusAngTable[eHudStatusAngIdle].x, 1.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusAngFast:
            zHudInterp1d(dt, &item->hframe.dang.x,
                         item->zHudStatusAngTable[eHudStatusAngFast].x, 1.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusAngTest1:
            zHudInterp1d(dt, &item->hframe.dang.x,
                         item->zHudStatusAngTable[eHudStatusAngTest1].x, 1.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.dang.y,
                         item->zHudStatusAngTable[eHudStatusAngTest1].y, 1.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.dang.z,
                         item->zHudStatusAngTable[eHudStatusAngTest1].z, 1.0f,
                         eHudInterpTypeLinear);
            break;
        }

        switch (item->statusSize)
        {
        case eHudStatusSizeDefault:
            zHudInterp1d(dt, &item->hframe.size.x,
                         item->zHudStatusSizeTable[eHudStatusSizeDefault].x, 50.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.size.y,
                         item->zHudStatusSizeTable[eHudStatusSizeDefault].y, 50.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusSizeZero:
            zHudInterp1d(dt, &item->hframe.size.x,
                         item->zHudStatusSizeTable[eHudStatusSizeZero].x, 50.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.size.y,
                         item->zHudStatusSizeTable[eHudStatusSizeZero].y, 50.0f,
                         eHudInterpTypeLinear);
            break;
        }

        switch (item->statusColor)
        {
        case eHudStatusColorSolid:
            zHudInterp1d(dt, &item->hframe.color.a,
                         item->zHudStatusColorTable[eHudStatusColorSolid].a,
                         450.0f, eHudInterpTypeLinear);
            break;
        case eHudStatusColorTransparent:
            zHudInterp1d(dt, &item->hframe.color.a,
                         item->zHudStatusColorTable[eHudStatusColorTransparent].a,
                         600.0f, eHudInterpTypeLinear);
            break;
        }

        item->hframe.ang.x += item->hframe.dang.x * dt;
        item->hframe.ang.y += item->hframe.dang.y * dt;
        item->hframe.ang.z += item->hframe.dang.z * dt;
        xModelUpdate(item->model, dt);
        xModelEval(item->model);
    }
}

// The health bar: up to five collars along the top left, shown for a moment whenever the
// health changes.
void zHudElementsCB_HealthbarRender()
{
    S32 i;

    for (i = 0; i < 5; i++)
    {
        zHudItem* item = &sHudHealthbarArray[i];
        if (item->hframe.color.a)
        {
            xRect r = { item->hframe.pos.x -
                            0.5f * item->hframe.size.x,
                        item->hframe.pos.y -
                            0.5f * item->hframe.size.y,
                        item->hframe.size.x * widthAspect,
                        item->hframe.size.y };
            xVec3 from = { 0.0f, 0.0f, 1.0f };
            xVec3 to = { 0.0f, 0.0f, 0.0f };
            xMat4x3 frame;

            xMat3x3Euler((xMat3x3*)&frame, item->hframe.ang.x,
                         item->hframe.ang.y, item->hframe.ang.z);
            frame.flags = 0;
            frame.pos.x = 0.0f;
            frame.pos.y = 0.0f;
            frame.pos.z = 0.0f;
            xModelSetMaterialAlpha(item->model,
                                   item->hframe.color.a);
            memcpy(item->model->Mat, &frame, sizeof(xMat4x3));
            xModelSetRender2DStyle(item->renderStyle, item->hframe.color.a);
            xModelRender2D(item->model, &r, &from, &to, PI / 2);
        }
    }
}

// Loads the stats icons (a snack, a clue and a second snack) and gives two of the player
// status items an idle animation.
void zHudElementsCB_GameStatsInit()
{
    xAnimFile* afile;
    U32 size;
    void* buff;
    S32 i;

    sHudGameStatsArray[4].model = zHudLoadModel(eHudModelIndividualScoobySnack);
    sHudGameStatsArray[4].renderStyle = 10;

    sHudGameStatsArray[5].model = zHudLoadModel(eHudModelClue);
    sHudGameStatsArray[5].renderStyle = 15;

    sHudGameStatsArray[6].model = zHudLoadModel(eHudModelIndividualScoobySnack);
    sHudGameStatsArray[6].renderStyle = 10;

    for (i = 4; i < 6; i++)
    {
        switch (i)
        {
        case 4:
            buff = xSTFindAsset(xStrHash("rhsd0001.anm"), &size);
            break;
        case 5:
            buff = xSTFindAsset(xStrHash("rhsd0001.anm"), &size);
            break;
        case 6:
            buff = xSTFindAsset(xStrHash("rhsd0001.anm"), &size);
            break;
        }

        if (buff)
        {
            afile = xAnimFileNew(buff, "", 0, NULL);
            sHudPlayerStatusArray[i].atbl = xAnimTableNew("", NULL);
            xAnimTableNewState(sHudPlayerStatusArray[i].atbl, "Idle", 0x10, 0, 1.0f, NULL, NULL,
                               0.0f, NULL);
            xAnimTableAddFile(sHudPlayerStatusArray[i].atbl, afile, "Idle");
            xAnimPoolAlloc(&globals.sceneCur->mempool, &sHudPlayerStatusArray[i],
                           sHudPlayerStatusArray[i].atbl, sHudPlayerStatusArray[i].model);
        }
    }
}

// Lays out the stats screen: four text rows down the middle that slide in from the sides,
// and three icons.
void zHudElementsCB_GameStatsSetup(_xScene* sc)
{
    S32 i;

    for (i = 0; i < 7; i++)
    {
        zHudItem* item = &sHudGameStatsArray[i];
        if (i >= 0 && i < 4)
        {
            item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 260.0f;
            item->zHudStatusPosTable[eHudStatusPosOnScreen].y =
                60.0f * i + 150.0f;
        }
        else
        {
            switch (i)
            {
            case 4:
                item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 205.0f;
                item->zHudStatusPosTable[eHudStatusPosOnScreen].y =
                    60.0f * (i - 4) + 170.0f;
                break;
            case 5:
                item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 205.0f;
                item->zHudStatusPosTable[eHudStatusPosOnScreen].y =
                    60.0f * (i - 4) + 170.0f;
                break;
            case 6:
                item->zHudStatusPosTable[eHudStatusPosOnScreen].x = 205.0f;
                item->zHudStatusPosTable[eHudStatusPosOnScreen].y =
                    60.0f * (i - 4) + 170.0f;
                break;
            }
        }

        switch (i)
        {
        case 0:
        case 2:
            item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].x = 20.0f;
            item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].y =
                60.0f * i + 150.0f;
            break;
        case 1:
        case 3:
            item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].x = 500.0f;
            item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].y =
                60.0f * i + 150.0f;
            break;
        case 4:
            item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].x = -35.0f;
            item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].y = 170.0f;
            break;
        case 5:
            item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].x = 445.0f;
            item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].y = 230.0f;
            break;
        case 6:
            item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].x = -35.0f;
            item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].y = 290.0f;
            break;
        }

        switch (i)
        {
        case 4:
            item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 80.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 80.0f;
            break;
        case 5:
            item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 30.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 30.0f;
            break;
        case 6:
            item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 80.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 80.0f;
            break;
        default:
            item->zHudStatusSizeTable[eHudStatusSizeDefault].x = 70.0f;
            item->zHudStatusSizeTable[eHudStatusSizeDefault].y = 70.0f;
            break;
        }

        item->zHudStatusAngTable[eHudStatusAngStop].x = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngStop].y = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngStop].z = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngIdle].x = 1.5915494f;
        item->zHudStatusAngTable[eHudStatusAngIdle].y = 0.0f;
        item->zHudStatusAngTable[eHudStatusAngIdle].z = 0.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].rm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].gm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].bm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorSolid].a = 200.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].rm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].gm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].bm = 1.0f;
        item->zHudStatusColorTable[eHudStatusColorTransparent].a = 0.0f;

        item->statusPos = eHudStatusPosOffScreenAlternate1;
        item->statusAng = eHudStatusAngIdle;
        item->statusSize = eHudStatusSizeDefault;
        item->statusColor = eHudStatusColorTransparent;

        item->hframe.pos.x =
            item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].x;
        item->hframe.pos.y =
            item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].y;
        item->hframe.size.x =
            item->zHudStatusSizeTable[eHudStatusSizeDefault].x;
        item->hframe.size.y =
            item->zHudStatusSizeTable[eHudStatusSizeDefault].y;
        item->hframe.color.a =
            item->zHudStatusColorTable[eHudStatusColorTransparent].a;
    }
}

void zHudElementsCB_GameStatsUpdate(_xScene* sc, F32 dt)
{
    S32 i;
    F32 collected;
    F32 total;

    if (zGameStateGet() == 4)
    {
        if (!sStatInit)
        {
            iColor_tag clear = { 0, 0, 0, 0 };
            iColor_tag black = { 0, 0, 0, 255 };

            sStatInit = 1;
            xScrFxFade(&clear, &black, 0.9f, NULL, 1, 1);
            zEntEventAllOfType(8, 3);
        }

        if (!sHudGameStatsTimer)
        {
            sHudGameStatsTimer = 30;
        }
        else
        {
            sHudGameStatsTimer = sHudGameStatsTimer - dt;
            if (sHudGameStatsTimer <= 0)
            {
                sHudGameStatsTimer = 0;
                zGameStateSwitch(5);
            }
        }

        if (tmpRoomTime == 0.0f)
        {
            tmpRoomTime = globals.stats.roomTime;
        }
    }
    else
    {
        sStatInit = 0;
    }

    for (i = 0; i < 7; i++)
    {
        zHudItem* item = &sHudGameStatsArray[i];
        switch (item->statusPos)
        {
        case eHudStatusPosOnScreen:
            zHudInterp2d(dt, &item->hframe.pos.x,
                         &item->hframe.pos.y,
                         item->zHudStatusPosTable[eHudStatusPosOnScreen].x,
                         item->zHudStatusPosTable[eHudStatusPosOnScreen].y,
                         500.0f, eHudInterpTypeDecel);
            break;
        case eHudStatusPosOffScreenDefault:
            zHudInterp2d(
                dt, &item->hframe.pos.x, &item->hframe.pos.y,
                item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].x,
                item->zHudStatusPosTable[eHudStatusPosOffScreenDefault].y,
                500.0f, eHudInterpTypeDecel);
            break;
        case eHudStatusPosOffScreenAlternate1:
            zHudInterp2d(
                dt, &item->hframe.pos.x, &item->hframe.pos.y,
                item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].x,
                item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate1].y,
                500.0f, eHudInterpTypeDecel);
            break;
        case eHudStatusPosOffScreenAlternate2:
            zHudInterp2d(
                dt, &item->hframe.pos.x, &item->hframe.pos.y,
                item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate2].x,
                item->zHudStatusPosTable[eHudStatusPosOffScreenAlternate2].y,
                500.0f, eHudInterpTypeDecel);
            break;
        case eHudStatusPosMiddleOfScreen:
            zHudInterp2d(
                dt, &item->hframe.pos.x, &item->hframe.pos.y,
                item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].x,
                item->zHudStatusPosTable[eHudStatusPosMiddleOfScreen].y,
                250.0f, eHudInterpTypeDecel);
            break;
        }

        switch (item->statusAng)
        {
        case eHudStatusAngStop:
            zHudInterp1d(dt, &item->hframe.dang.x,
                         item->zHudStatusAngTable[eHudStatusAngStop].x, 1.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusAngIdle:
            zHudInterp1d(dt, &item->hframe.dang.x,
                         item->zHudStatusAngTable[eHudStatusAngIdle].x, 1.0f,
                         eHudInterpTypeLinear);
            break;
        case eHudStatusAngTest1:
            zHudInterp1d(dt, &item->hframe.dang.x,
                         item->zHudStatusAngTable[eHudStatusAngTest1].x, 1.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.dang.y,
                         item->zHudStatusAngTable[eHudStatusAngTest1].y, 1.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.dang.z,
                         item->zHudStatusAngTable[eHudStatusAngTest1].z, 1.0f,
                         eHudInterpTypeLinear);
            break;
        }

        switch (item->statusSize)
        {
        case eHudStatusSizeDefault:
            zHudInterp1d(dt, &item->hframe.size.x,
                         item->zHudStatusSizeTable[eHudStatusSizeDefault].x,
                         90.0f, eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.size.y,
                         item->zHudStatusSizeTable[eHudStatusSizeDefault].y,
                         90.0f, eHudInterpTypeLinear);
            break;
        case eHudStatusSizeZero:
            zHudInterp1d(dt, &item->hframe.size.x,
                         item->zHudStatusSizeTable[eHudStatusSizeZero].x, 90.0f,
                         eHudInterpTypeLinear);
            zHudInterp1d(dt, &item->hframe.size.y,
                         item->zHudStatusSizeTable[eHudStatusSizeZero].y, 90.0f,
                         eHudInterpTypeLinear);
            break;
        }

        switch (item->statusColor)
        {
        case eHudStatusColorSolid:
            zHudInterp1d(dt, &item->hframe.color.a,
                         item->zHudStatusColorTable[eHudStatusColorSolid].a,
                         300.0f, eHudInterpTypeLinear);
            break;
        case eHudStatusColorTransparent:
            zHudInterp1d(dt, &item->hframe.color.a,
                         item->zHudStatusColorTable[eHudStatusColorTransparent].a,
                         300.0f, eHudInterpTypeLinear);
            break;
        case eHudStatusColorLightBlue:
            break;
        case eHudStatusColorOpaque2:
            break;
        }

        switch (i)
        {
        case 0:
            collected = globals.stats.snacks;
            total = globals.stats.snacks_tot;
            sprintf(item->text, "x %d%/%d", (S32)collected, (S32)total);
            break;
        case 1:
            collected = globals.stats.clues;
            total = globals.stats.clues_tot;
            sprintf(item->text, "x %d%/%d", (S32)collected, (S32)total);
            break;
        case 2:
            collected = globals.stats.enemyKills;
            total = globals.stats.enemyCount;
            sprintf(item->text, "x %d%/%d", (S32)collected, (S32)total);
            break;
        case 3:
            sprintf(item->text, "Time - %dh %dm %ds",
                    (S32)(tmpRoomTime / 3600.0f), (S32)(tmpRoomTime / 60.0f),
                    (S32)tmpRoomTime % 60);
            break;
        }

        item->hframe.ang.x += item->hframe.dang.x * dt;
        item->hframe.ang.y += item->hframe.dang.y * dt;
        item->hframe.ang.z += item->hframe.dang.z * dt;
    }
}

// The end-of-area stats screen: the snack, clue and enemy counts and the time, with their
// icons.
void zHudElementsCB_GameStatsRender()
{
    S32 i;

    if (zGameStateGet() == 4)
    {
        for (i = 0; i < 7; i++)
        {
            zHudItem* item = &sHudGameStatsArray[i];
            if (!item->hframe.color.a)
            {
                if (i != 3)
                {
                    zHud_DrawText(item->hframe.pos.x,
                                  item->hframe.pos.y, 300.0f, 50.0f,
                                  item->text, 0x10, 1.0f,
                                  item->hframe.color.a, DROPSHADOW_NORMAL);
                }

                if (i == 4 || i == 5 || i == 6)
                {
                    xRect r = { item->hframe.pos.x -
                                    0.5f * item->hframe.size.x,
                                item->hframe.pos.y -
                                    0.5f * item->hframe.size.y,
                                item->hframe.size.x * widthAspect,
                                item->hframe.size.y };
                    xVec3 from = { 0.0f, 0.0f, 0.8f };
                    xVec3 to = { 0.0f, 0.0f, 0.0f };
                    xMat4x3 frame;

                    xMat3x3Euler((xMat3x3*)&frame, item->hframe.ang.x,
                                 item->hframe.ang.y,
                                 item->hframe.ang.z);
                    frame.flags = 0;
                    frame.pos.x = 0.0f;
                    frame.pos.y = 0.0f;
                    frame.pos.z = 0.0f;
                    xModelSetMaterialAlpha(item->model,
                                           item->hframe.color.a);
                    memcpy(item->model->Mat, &frame, sizeof(xMat4x3));
                    xModelSetRender2DStyle(item->renderStyle,
                                item->hframe.color.a);
                    xModelRender2D(item->model, &r, &from, &to, PI / 2);
                }
            }
        }
    }
}

// Draws s in the HUD font, first offset as a drop shadow when asked.
void zHud_DrawText(F32 x, F32 y, F32 w, F32 h, char* s, S32 just, F32 size, U8 alpha,
                   eHudDropShadowMode shadow)
{
    F32 drawSizeX = 24.0f * size;
    F32 drawSizeY = 32.0f * size;

    clr.a = alpha;
    clrDropShadow.a = alpha;

    if (shadow == DROPSHADOW_NORMAL)
    {
        fontBox = xFontBoxCreate(x + -3.0f, y + 3.0f, w, h, 0, clrDropShadow);
        xFontBoxSetJustification(fontBox, just);
        xFontBoxSetDrawSize(fontBox, drawSizeX, drawSizeY);
        xFontBoxSetDrawSpacing(fontBox, drawSizeX, drawSizeY);
        xFontBoxSetColor(fontBox, clrDropShadow);
        xFontBoxSetText(fontBox, s);
        xFontBoxRender(fontBox);
        xFontBoxDestroy(fontBox);
    }

    fontBox = xFontBoxCreate(x, y, w, h, 0, clr);
    xFontBoxSetJustification(fontBox, just);
    xFontBoxSetDrawSize(fontBox, drawSizeX, drawSizeY);
    xFontBoxSetDrawSpacing(fontBox, drawSizeX, drawSizeY);
    xFontBoxSetColor(fontBox, clr);
    xFontBoxSetText(fontBox, s);
    xFontBoxRender(fontBox);
    xFontBoxDestroy(fontBox);
}

// Draws the texture asset as a flat rectangle in screen space.
void zHud_UIRender(char* asset, F32 xPos, F32 yPos, F32 w, F32 h, U8 alpha)
{
    RwTexture* texture;
    F32 x2;
    F32 y2;
    F32 z;

    texture = (RwTexture*)xSTFindAsset(xStrHash(asset), NULL);
    if (texture && texture->raster)
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, texture->raster);
    }

    x2 = xPos + w;
    y2 = yPos + h;
    z = RwIm2DGetNearScreenZ();

    RwIm2DVertexSetIntRGBA(&Vert[0], 255, 255, 255, 255);
    RwIm2DVertexSetIntRGBA(&Vert[1], 255, 255, 255, 255);
    RwIm2DVertexSetIntRGBA(&Vert[2], 255, 255, 255, 255);
    RwIm2DVertexSetIntRGBA(&Vert[3], 255, 255, 255, 255);

    RwIm2DVertexSetScreenX(&Vert[0], xPos);
    RwIm2DVertexSetScreenY(&Vert[0], yPos);
    RwIm2DVertexSetScreenZ(&Vert[0], z);
    Vert[0].u = 0.0f;
    Vert[0].v = 0.0f;

    RwIm2DVertexSetScreenX(&Vert[1], xPos);
    RwIm2DVertexSetScreenY(&Vert[1], y2);
    RwIm2DVertexSetScreenZ(&Vert[1], z);
    Vert[1].u = 0.0f;
    Vert[1].v = 1.0f;

    RwIm2DVertexSetScreenX(&Vert[2], x2);
    RwIm2DVertexSetScreenY(&Vert[2], y2);
    RwIm2DVertexSetScreenZ(&Vert[2], z);
    Vert[2].u = 1.0f;
    Vert[2].v = 1.0f;

    RwIm2DVertexSetScreenX(&Vert[3], x2);
    RwIm2DVertexSetScreenY(&Vert[3], yPos);
    RwIm2DVertexSetScreenZ(&Vert[3], z);
    Vert[3].u = 1.0f;
    Vert[3].v = 0.0f;

    RwIm2DVertexSetIntRGBA(&Vert[0], 255, 255, 255, alpha);
    RwIm2DVertexSetIntRGBA(&Vert[1], 255, 255, 255, alpha);
    RwIm2DVertexSetIntRGBA(&Vert[2], 255, 255, 255, alpha);
    RwIm2DVertexSetIntRGBA(&Vert[3], 255, 255, 255, alpha);

    zRenderState(SDRS_Default);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
    RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, Vert, 4, Idx, 6);
}

void zHud_sceneReset()
{
    tmpRoomTime = 0.0f;
}

// Loads the HUD model for modelEnum, or the fallback model when its asset is missing.
static xModelInstance* zHudLoadModel(U32 modelEnum)
{
    U32 size;
    void* buff = xSTFindAsset(zHudModelGetID(modelEnum), &size);

    if (!buff)
    {
        buff = xSTFindAsset(zHudModelGetID(eHudModel_err), &size);
    }
    if (*(U32*)buff == 'FNIM')
    {
        return zEntRecurseModelInfo(buff, NULL);
    }
    else
    {
        return xEntLoadModel(NULL, (RpAtomic*)buff);
    }
}

U32 zHudModelGetID(U32 modelEnum)
{
    S32 i;

    for (i = 0; i < eHudModelCount; i++)
    {
        if (zHudModelTable[i].type == modelEnum)
        {
            return zHudModelTable[i].modelNameID;
        }
    }

    return 0;
}

void zHudShow()
{
    sHudDisplayTimer_plrStatus = 2.0f;
    sHudDisplayTimer_snacks = 2.0f;
    sHudDisplayTimer_gum = 1.5f;
    sHudDisplayTimer_soap = 1.5f;
    sHudDisplayTimer_healthBar = 2.0f;
}

void zHudHide()
{
    sHudDisplayTimer_plrStatus = 0.0f;
    sHudDisplayTimer_snacks = 0.0f;
    sHudDisplayTimer_gum = 0.0f;
    sHudDisplayTimer_soap = 0.0f;
    sHudDisplayTimer_healthBar = 0.0f;
}

// Moves *src toward dst without overshooting it: by speed units a second (linear), or by
// speed times the remaining distance a second (decel).
void zHudInterp1d(F32 dt, F32* src, F32 dst, F32 speed, S32 type)
{
    F32 diff = dst - *src;

    switch (type)
    {
    case eHudInterpTypeLinear:
        if (diff > 0.0f)
        {
            *src += fabsf(speed * dt) > fabsf(diff) ? diff : speed * dt;
        }
        else
        {
            *src -= fabsf(speed * dt) > fabsf(diff) ? fabsf(diff) : speed * dt;
        }
        break;
    case eHudInterpTypeDecel:
        *src += fabsf(diff * speed * dt) > fabsf(diff) ? diff : diff * speed * dt;
        break;
    }
}

// Moves (*srcx, *srcy) toward (dstx, dsty) at speed units a second: at a steady rate
// (linear), slowing as it closes in (decel) or by the inverse square of the distance
// (accel). It never overshoots.
void zHudInterp2d(F32 dt, F32* srcx, F32* srcy, F32 dstx, F32 dsty, F32 speed, S32 type)
{
    if (speed)
    {
        if (*srcx == dstx && *srcy == dsty)
        {
            return;
        }

        F32 dx = dstx - *srcx;
        F32 dy = dsty - *srcy;
        xVec2 t = { dx, dy };
        xVec2 tNorm = { 0.0f, 0.0f };
        F32 d2;

        xVec2Normalize(&tNorm, &t);

        switch (type)
        {
        case eHudInterpTypeLinear:
            dstx = tNorm.x * speed * dt;
            if (fabsf(dstx) > fabsf(dx))
            {
                dstx = dx;
            }
            *srcx += dstx;
            dsty = tNorm.y * speed * dt;
            if (fabsf(dsty) > fabsf(dy))
            {
                dsty = dy;
            }
            *srcy += dsty;
            break;
        case eHudInterpTypeDecel:
            if (fabsf(dx) < 0.1f)
            {
                dstx = dx;
            }
            else
            {
                dstx = 0.05f * dx * speed / 10.0f * dt;
            }
            *srcx += dstx;
            if (fabsf(dy) < 0.1f)
            {
                dsty = dy;
            }
            else
            {
                dsty = 0.05f * dy * speed / 10.0f * dt;
            }
            *srcy += dsty;
            break;
        case eHudInterpTypeAccel:
            d2 = 10000.0f / (dx * dx + dy * dy);
            if (fabsf(tNorm.x * d2) * speed * dt > fabsf(dx))
            {
                dstx = dx;
            }
            else
            {
                dstx = tNorm.x * fabsf(d2) * speed * dt;
            }
            *srcx += dstx;
            if (fabsf(tNorm.y * d2) * speed * dt > fabsf(dy))
            {
                dsty = dy;
            }
            else
            {
                dsty = tNorm.y * fabsf(d2) * speed * dt;
            }
            *srcy += dsty;
            break;
        }
    }
    else
    {
        *srcx = dstx;
        *srcy = dsty;
    }
}
