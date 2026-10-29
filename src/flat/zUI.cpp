#include "zEnt.h"
#include "xPad.h"
#include "xEvent.h"
#include "xString.h"
#include "xScrFx.h"
#include "xstransvc.h"
#include "rwcore.h"
#include "xMath2.h"
#include "zRenderState.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "zGlobals.h"
#include "zGame.h"
#include <stdlib.h>

struct zUIAsset : p2EntAsset
{
    U32 uiFlags;
    U16 dim[2];
    U32 textureID;
    F32 uva[2];
    F32 uvb[2];
    F32 uvc[2];
    F32 uvd[2];
};

struct _zUI : _zEnt
{
    zUIAsset* sasset;
    U32 uiFlags;
    U32 uiButton;
};

struct zScene;

// Only the font flags used during rendering are declared here.
struct zUIFontAsset : zUIAsset
{
    U16 uiFontFlags;
};

struct zSceneLevel
{
    char* desc;
    char name;
    S32 index;
    U32 totalClues;
};

extern "C" zSceneLevel lbl_801C8BCC[];
extern "C" F32 lbl_8025E8C4;
extern "C" void fn_800D77CC(U32 sceneID);
extern "C" void fn_800CDB7C(xBase* (*callback)(xBase*, zScene*, void*), S32 type, void* data);
void zUI_Render(_xEnt* ent);
extern "C" void fn_800D47E8(_xEnt* ent);

static _zUI* sSorted[768];
static U32 sSortedCount;
static F32 sTimer;
static RwIm2DVertex Vertex[4];
static RwImVertexIndex Index[6] = { 0, 1, 2, 0, 2, 3 };
static U32 cKeyUIid1off;
static U32 cKeyUIid2off;
static U32 cKeyUIid3off;
static U32 cKeyUIid4off;
static U32 cKeyUIid1on;
static U32 cKeyUIid2on;
static U32 cKeyUIid3on;
static U32 cKeyUIid4on;

void zUI_Reset(_zUI* ent);

// Handle visibility and focus changes for a UI element.
S32 zUIEventCB(xBase*, xBase* to, U32 event, F32*, xBase*)
{
    _zUI* ent = static_cast<_zUI*>(to);
    switch (event)
    {
    case eEventVisible:
        ent->flags |= 1;
        break;
    case eEventInvisible:
        ent->flags &= ~1;
        break;
    case eEventReset:
        zUI_Reset(ent);
        break;
    case 0x4F:
        ent->uiFlags |= 2;
        for (S32 i = 0; i < 13; ++i)
        {
            char level = lbl_801C8BCC[i].name;
            for (S32 j = 0; j < 30; ++j)
            {
                char uiName[256];
                sprintf(uiName, "%c %02d SELECTED", level, j);
                if (ent->id == xStrHash(uiName))
                {
                    zEntEvent(ent, ent, eEventVisible);
                    char scene[5] = { 0 };
                    strncpy(scene, uiName, 4);
                    scene[1] = '0';
                    U32 sceneID = (U32(scene[0]) << 24) | (U32(scene[1]) << 16) |
                                  (U32(scene[2]) << 8) | U32(scene[3]);
                    fn_800D77CC(sceneID);
                }
            }
        }
        break;
    case 0x50:
        if (ent->uiFlags & 2) ent->uiFlags ^= 2;
        for (S32 i = 0; i < 13; ++i)
        {
            char level = lbl_801C8BCC[i].name;
            for (S32 j = 0; j < 30; ++j)
            {
                char uiName[256];
                sprintf(uiName, "%c %02d SELECTED", level, j);
                if (ent->id == xStrHash(uiName))
                    zEntEvent(ent, ent, eEventInvisible);
            }
        }
        break;
    case eEventUIFocusOn:
        ent->uiFlags |= 8;
        if (ent->uiFlags & 0x10) zEntEvent(ent, ent, eEventVisible);
        break;
    case eEventUIFocusOff:
        if (ent->uiFlags & 8) ent->uiFlags ^= 8;
        if (ent->uiFlags & 0x20) zEntEvent(ent, ent, eEventInvisible);
        break;
    case 0x5E:
        zEntEvent(ent, ent, eEventUIFocusOn);
        zEntEvent(ent, ent, 0x4F);
        break;
    case 0x5F:
        zEntEvent(ent, ent, eEventUIFocusOff);
        zEntEvent(ent, ent, 0x50);
        break;
    }
    return 1;
}

// Draw UI elements with larger asset depth first.
S32 iRenderQSort_Face(const void* a, const void* b)
{
    const _zUI* left = *static_cast<_zUI* const*>(a);
    const _zUI* right = *static_cast<_zUI* const*>(b);
    if (left->sasset->pos.z < right->sasset->pos.z)
        return 1;
    if (left->sasset->pos.z > right->sasset->pos.z)
        return -1;
    return 0;
}

// Collect each UI element for the later depth sort.
xBase* zUIRenderIteratorInsert(xBase* b, zScene*, void*)
{
    sSorted[sSortedCount++] = static_cast<_zUI*>(b);
    return b;
}

// Partial: textured-quad and model drawing behavior needs further reconstruction.
void zUI_Render(_xEnt* e)
{
    _zUI* ent = static_cast<_zUI*>(e);
    if (xScrFxIsLetterbox() &&
        (ent->id == cKeyUIid1off || ent->id == cKeyUIid2off ||
         ent->id == cKeyUIid3off || ent->id == cKeyUIid4off ||
         ent->id == cKeyUIid1on || ent->id == cKeyUIid2on ||
         ent->id == cKeyUIid3on || ent->id == cKeyUIid4on))
        return;

    if (!(ent->uiFlags & 4))
    {
        xEntRender(ent);
        return;
    }
    if (!xEntIsVisible(ent)) return;
    if (ent->sasset->textureID)
    {
        RwTexture* texture = static_cast<RwTexture*>(xSTFindAsset(ent->sasset->textureID, 0));
        if (texture && texture->raster)
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, texture->raster);
        zUIAsset* asset = ent->sasset;
        F32 u1 = asset->uva[0];
        F32 v1 = asset->uva[1];
        F32 u2 = asset->uvb[0];
        F32 v2 = asset->uvb[1];
        F32 u3 = asset->uvc[0];
        F32 v3 = asset->uvc[1];
        F32 u4 = asset->uvd[0];
        F32 v4 = asset->uvd[1];
        F32 left = (640.0f * asset->pos.x) / 640.0f;
        F32 top = (480.0f * asset->pos.y) / 480.0f;
        F32 right = (640.0f * (asset->pos.x + asset->dim[0])) / 640.0f;
        F32 bottom = (480.0f * (asset->pos.y + asset->dim[1])) / 480.0f;

        static U32 mapID = xStrHash("paws_map");
        static U32 cornerID = xStrHash("paws_corner");
        static U32 tagID = xStrHash("sd_tag_map");
        if (asset->textureID == mapID || asset->textureID == cornerID ||
            asset->textureID == tagID)
        {
            F32 pulse = 1.0f + (F32)sin(15.0f * lbl_8025E8C4);
            F32 dx = pulse * (0.05f * (right - left));
            F32 dy = pulse * (0.05f * (bottom - top));
            left -= dx;
            right += dx;
            top -= dy;
            bottom += dy;
        }

        static U32 warpTokenID = xStrHash("warp token ui");
        static U32 warpSnackID = xStrHash("warp snack ui");
        if (ent->id == warpTokenID || ent->id == warpSnackID)
        {
            F32 wave = 0.5f * (F32)sin(5.0f * lbl_8025E8C4 + left);
            F32 dx = wave * (right - left);
            F32 center = (640.0f * asset->pos.x) / 640.0f;
            left = center - dx;
            right = center + dx;
        }

        F32 z = RwIm2DGetNearScreenZ();
        RwIm2DVertexSetScreenX(&Vertex[0], left);
        RwIm2DVertexSetScreenY(&Vertex[0], top);
        RwIm2DVertexSetScreenZ(&Vertex[0], z);
        Vertex[0].u = u1; Vertex[0].v = v1;
        RwIm2DVertexSetScreenX(&Vertex[1], left);
        RwIm2DVertexSetScreenY(&Vertex[1], bottom);
        RwIm2DVertexSetScreenZ(&Vertex[1], z);
        Vertex[1].u = u4; Vertex[1].v = v4;
        RwIm2DVertexSetScreenX(&Vertex[2], right);
        RwIm2DVertexSetScreenY(&Vertex[2], bottom);
        RwIm2DVertexSetScreenZ(&Vertex[2], z);
        Vertex[2].u = u3; Vertex[2].v = v3;
        RwIm2DVertexSetScreenX(&Vertex[3], right);
        RwIm2DVertexSetScreenY(&Vertex[3], top);
        RwIm2DVertexSetScreenZ(&Vertex[3], z);
        Vertex[3].u = u2; Vertex[3].v = v2;
        RwIm2DVertexSetIntRGBA(&Vertex[0], 255, 255, 255, 255);
        RwIm2DVertexSetIntRGBA(&Vertex[1], 255, 255, 255, 255);
        RwIm2DVertexSetIntRGBA(&Vertex[2], 255, 255, 255, 255);
        RwIm2DVertexSetIntRGBA(&Vertex[3], 255, 255, 255, 255);

        zRenderState(SDRS_Default);
        RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)0);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
        RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)1);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)0);
        RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, Vertex, 4, Index, 6);
    }
    else if (ent->model)
    {
        zUIAsset* asset = ent->sasset;
        _xRect rect;
        rect.x = (640.0f * asset->pos.x) / 640.0f;
        rect.y = (480.0f * asset->pos.y) / 480.0f;
        rect.w = (640.0f * (asset->pos.x + asset->dim[0])) / 640.0f;
        rect.h = (480.0f * (asset->pos.y + asset->dim[1])) / 480.0f;
        RwV3d& pos = ent->model->Mat->pos;
        xVec3 from = { pos.x, pos.y + 1.0f, pos.z + 5.0f };
        xVec3 to = { pos.x, pos.y, pos.z };
        xModelRender2D(ent->model, &rect, &from, &to, 1.5707964f);
    }
}

// Gather, sort and draw visible UI elements.
void zUIRenderAll()
{
    sTimer += 0.05f;
    if (sTimer >= 2.0f) sTimer -= 2.0f;

    if (!sSortedCount)
    {
        fn_800CDB7C(zUIRenderIteratorInsert, 0x20, 0);
        fn_800CDB7C(zUIRenderIteratorInsert, 0x21, 0);
        if (sSortedCount > 1)
            qsort(sSorted, sSortedCount, sizeof(sSorted[0]), iRenderQSort_Face);
    }

    for (S32 i = 0; i < sSortedCount; ++i)
    {
        _zUI* ent = sSorted[i];
        if (!xEntIsVisible(ent)) continue;
        if (ent->baseType == 0x20)
            zUI_Render(ent);
        else if (!zGameIsPaused() ||
                 !(static_cast<zUIFontAsset*>(ent->sasset)->uiFontFlags & 0x100))
            fn_800D47E8(ent);
    }
}

// Dispatch the first pressed UI button in priority order.
void zUI_Update(_zUI* ent, _xScene* sc, F32 dt)
{
    xEntUpdate(ent, sc, dt);
    if (ent->uiButton)
    {
        if (ent->uiButton & 0x10) zEntEvent(ent, ent, 0x49);
        else if (ent->uiButton & 0x40) zEntEvent(ent, ent, 0x4A);
        else if (ent->uiButton & 0x80) zEntEvent(ent, ent, 0x4C);
        else if (ent->uiButton & 0x20) zEntEvent(ent, ent, 0x4B);
        else if (ent->uiButton & 0x1) zEntEvent(ent, ent, 0x47);
        else if (ent->uiButton & 0x2) zEntEvent(ent, ent, 0x48);
        else if (ent->uiButton & 0x1000)
        {
            zEntEvent(ent, ent, 0x45);
            ent->uiButton = 0x1000;
        }
        else if (ent->uiButton & 0x2000)
        {
            zEntEvent(ent, ent, 0x46);
            ent->uiButton = 0x2000;
        }
        else if (ent->uiButton & 0x100) zEntEvent(ent, ent, 0x43);
        else if (ent->uiButton & 0x200) zEntEvent(ent, ent, 0x44);
        else if (ent->uiButton & 0x10000) zEntEvent(ent, ent, 0x3F);
        else if (ent->uiButton & 0x20000) zEntEvent(ent, ent, 0x41);
        else if (ent->uiButton & 0x40000) zEntEvent(ent, ent, 0x42);
        else if (ent->uiButton & 0x80000) zEntEvent(ent, ent, 0x40);
        ent->uiFlags &= ~0x40;
    }
}

// Capture input for a focused UI element before its update.
void zUI_PreUpdate(_zUI* ent, _xScene*, F32)
{
    ent->uiButton = 0;
    if (!globals.pad0 || !globals.pad0->pressed || !(ent->uiFlags & 8) ||
        !(ent->uiFlags & 3))
        return;

    if (globals.pad0->pressed & 0x10) ent->uiButton |= 0x10;
    else if (globals.pad0->pressed & 0x40) ent->uiButton |= 0x40;
    else if (globals.pad0->pressed & 0x80) ent->uiButton |= 0x80;
    else if (globals.pad0->pressed & 0x20) ent->uiButton |= 0x20;
    if (globals.pad0->pressed & 0x1) ent->uiButton |= 0x1;
    if (globals.pad0->pressed & 0x2) ent->uiButton |= 0x2;
    if (globals.pad0->pressed & 0x1000) ent->uiButton |= 0x1000;
    if (globals.pad0->pressed & 0x2000) ent->uiButton |= 0x2000;
    if (globals.pad0->pressed & 0x100) ent->uiButton |= 0x100;
    if (globals.pad0->pressed & 0x200) ent->uiButton |= 0x200;
    if (globals.pad0->pressed & 0x10000) ent->uiButton |= 0x10000;
    if (globals.pad0->pressed & 0x20000) ent->uiButton |= 0x20000;
    if (globals.pad0->pressed & 0x40000) ent->uiButton |= 0x40000;
    if (globals.pad0->pressed & 0x80000) ent->uiButton |= 0x80000;
    if (ent->uiButton) ent->uiFlags |= 0x40;
}

// Reset the entity and discard the cached sorted list.
void zUI_Reset(_zUI* ent)
{
    zEntReset(ent);
    ent->pflags = 0;
    ent->penby |= 0x10;
    ent->chkby = 0;
    ent->uiFlags = ent->sasset->uiFlags;
    sSortedCount = 0;
}

// UI persistence uses the base entity state.
void zUI_Load(_zUI* ent, xSerial* s)
{
    zEntLoad(ent, s);
}

// UI persistence uses the base entity state.
void zUI_Save(_zUI* ent, xSerial* s)
{
    zEntSave(ent, s);
}

// Initialize the UI entity and its shared key-display identifiers.
void zUI_Init(_zUI* ent, p2EntAsset* asset)
{
    zEntInit(ent, asset, 'UI  ');
    ent->sasset = static_cast<zUIAsset*>(asset);
    ent->pflags = 0;
    ent->penby |= 0x10;
    ent->chkby = 0;
    ent->update = (void (*)(_xEnt*, _xScene*, F32))zUI_Update;
    ent->move = 0;
    ent->eventFunc = zUIEventCB;
    if (ent->linkCount)
        ent->link = (p2LinkAsset*)(ent->sasset + 1);
    ent->eventFunc = zUIEventCB;
    ent->uiFlags = ent->sasset->uiFlags;
    if (ent->uiFlags & 4) ent->render = 0;
    zEntReset(ent);
    sSortedCount = 0;
    cKeyUIid1off = xStrHash("KEY OFF 1 UI");
    cKeyUIid2off = xStrHash("KEY OFF 2 UI");
    cKeyUIid3off = xStrHash("KEY OFF 3 UI");
    cKeyUIid4off = xStrHash("KEY OFF 4 UI");
    cKeyUIid1on = xStrHash("KEY ON 1 UI");
    cKeyUIid2on = xStrHash("KEY ON 2 UI");
    cKeyUIid3on = xStrHash("KEY ON 3 UI");
    cKeyUIid4on = xStrHash("KEY ON 4 UI");
}

void zUI_Init(void* ent, void* asset)
{
    zUI_Init(static_cast<_zUI*>(ent), static_cast<p2EntAsset*>(asset));
}
