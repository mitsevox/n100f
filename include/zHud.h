#ifndef ZHUD_H
#define ZHUD_H

#include "types.h"
#include "xMath3.h"

struct _xScene;
struct xModelInstance;
struct xAnimTable;

enum eHudElement
{
    eHudPlayerStatus,
    eHudChances,
    eHudSnacks,
    eHudGum,
    eHudSoap,
    eHudMessage,
    eHudGameStats,
    eHudSoapbox,
    eHudGumpack,
    eHudHealthbar,
    eHudCount
};

enum eHudDropShadowMode
{
    DROPSHADOW_NONE,
    DROPSHADOW_NORMAL
};

enum eHudInterpType
{
    eHudInterpTypeLinear,
    eHudInterpTypeDecel,
    eHudInterpTypeAccel,
    eHudInterpTypeCount
};

enum eHudModel
{
    eHudModelBubblegumPiece,
    eHudModelBubblegumPack,
    eHudModelBubblegumMachine,
    eHudModelPinkBunnySlippers,
    eHudModelBarSoap,
    eHudModelBoxSoap,
    eHudModelBubblePowerUp,
    eHudModelLampShade,
    eHudModelFlowerPot,
    eHudModelPlungerSuctionCup,
    eHudModelBlackKnightHelmut,
    eHudModelSpring,
    eHudModelDivingHelmut,
    eHudModelSkull,
    eHudModelLightingBolt,
    eHudModelFootballHelmut,
    eHudModelUmbrella_closed,
    eHudModelUmbrella_open,
    eHudModelShovel,
    eHudModelIndividualScoobySnack,
    eHudModelScoobyCollar,
    eHudModelMagnifyingGlass,
    eHudModelHotSauce,
    eHudModelSphere,
    eHudModelHead0,
    eHudModelHead1,
    eHudModelHead2,
    eHudModelHead3,
    eHudModelHead4,
    eHudModelHead5,
    eHudModelSandwich1,
    eHudModelSandwich2,
    eHudModelSandwich3,
    eHudModelSandwich4,
    eHudModelSandwich5,
    eHudModelEnemyIcon,
    eHudModelClue,
    eHudModel_err,
    eHudModelBubblegumPiece2,
    eHudModelBubblegumPiece3,
    eHudModelBubblegumPiece4,
    eHudModelBubblegumPiece5,
    eHudModelCount
};

// Where an item sits, as a row of its position table.
enum eHudStatusPos
{
    eHudStatusPosOnScreen,
    eHudStatusPosOffScreenDefault,
    eHudStatusPosOffScreenAlternate1,
    eHudStatusPosOffScreenAlternate2,
    eHudStatusPosNudgeUp,
    eHudStatusPosNudgeDown,
    eHudStatusPosNudgeLeft,
    eHudStatusPosNudgeRight,
    eHudStatusPosMiddleOfScreen,
    eHudStatusPosOnScreenAlt1,
    eHudStatusPosCount
};

enum eHudStatusSize
{
    eHudStatusSizeDefault,
    eHudStatusSizeZero,
    eHudStatusSizeSmall,
    eHudStatusSizeBig,
    eHudStatusSizeSquash,
    eHudStatusSizeZeroHoriz,
    eHudStatusSizeZeroVert,
    eHudStatusSizeHyper,
    eHudStatusSizeHyperReverse,
    eHudStatusSizeCount
};

// How fast an item spins, as a row of its spin table.
enum eHudStatusAng
{
    eHudStatusAngStop,
    eHudStatusAngIdle,
    eHudStatusAngIdleReverse,
    eHudStatusAngSlow,
    eHudStatusAngSlowReverse,
    eHudStatusAngFast,
    eHudStatusAngFastReverse,
    eHudStatusAngHyper,
    eHudStatusAngHyperReverse,
    eHudStatusAngTest1,
    eHudStatusAngTest2,
    eHudStatusAngCount
};

enum eHudStatusColor
{
    eHudStatusColorSolid,
    eHudStatusColorTransparent,
    eHudStatusColorLightBlue,
    eHudStatusColorOpaque1,
    eHudStatusColorOpaque2,
    eHudStatusColorOpaque3,
    eHudStatusColorCount
};

struct zHudColor
{
    F32 rm;
    F32 gm;
    F32 bm;
    F32 a;
};

// The current placement of an item, which its update moves toward the table rows its
// status names.
struct zHudFrame
{
    xVec3 pos;
    xVec3 size;
    xVec3 ang;
    xVec3 dang;
    zHudColor color;
};

struct zHudStatusPos
{
    F32 x;
    F32 y;
};

struct zHudStatusAng
{
    F32 x;
    F32 y;
    F32 z;
};

struct zHudStatusSize
{
    F32 x;
    F32 y;
};

struct zHudStatusColor
{
    F32 rm;
    F32 gm;
    F32 bm;
    F32 a;
    eHudInterpType interp;
};

// One model on the HUD. renderStyle picks the lighting preset the model is drawn with.
struct zHudItem
{
    xModelInstance* model;
    xAnimTable* atbl;
    U32 renderStyle;
    zHudFrame hframe;
    eHudStatusPos statusPos;
    eHudStatusSize statusSize;
    eHudStatusAng statusAng;
    eHudStatusColor statusColor;
    char text[64];
    S32 statusFX;
    F32 shake;
    zHudStatusPos zHudStatusPosTable[eHudStatusPosCount];
    zHudStatusAng zHudStatusAngTable[eHudStatusAngCount];
    zHudStatusSize zHudStatusSizeTable[eHudStatusSizeCount];
    zHudStatusColor zHudStatusColorTable[eHudStatusColorCount];
};

struct _zHudElement
{
    eHudElement hudType;
    void (*hudInit)();
    void (*hudSetup)(_xScene*);
    void (*hudUpdate)(_xScene*, F32);
    void (*hudRender)();
    U32 count;
    char text[256];
};

struct zHudModel
{
    eHudModel type;
    char* modelName;
    U32 modelNameID;
};

void zHudInterp2d(F32 dt, F32* srcx, F32* srcy, F32 dstx, F32 dsty, F32 speed, S32 type);
void zHudInterp1d(F32 dt, F32* src, F32 dst, F32 speed, S32 type);
void zHudHide();
void zHudShow();
U32 zHudModelGetID(U32 modelEnum);
void zHud_sceneReset();
void zHud_UIRender(char* asset, F32 xPos, F32 yPos, F32 w, F32 h, U8 alpha);
void zHud_DrawText(F32 x, F32 y, F32 w, F32 h, char* s, S32 just, F32 size, U8 alpha,
                   eHudDropShadowMode shadow);
void zHudShowSaveSuccess();
void zHudShowAllSnacksArea();
void zHud_showSnackSpecial();
void zHud_showSnack();
void zHud_ShakeSnackCounter();
void zHud_showHealthandStatus();
void zHud_Render();
void zHud_Update(_xScene* sc, F32 dt);
void zHud_Setup(_xScene* sc);
void zHud_Init();

#endif
