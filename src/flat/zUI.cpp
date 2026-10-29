#include "zEnt.h"
#include "xPad.h"
#include "xEvent.h"
#include "xString.h"
#include "xScrFx.h"
#include "xstransvc.h"
#include "rwcore.h"
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
extern "C" void fn_800D77CC(U32 sceneID);
extern "C" void fn_800CDB7C(xBase* (*callback)(xBase*, zScene*, void*), S32 type, void* data);
void zUI_Render(_xEnt* ent);
extern "C" void fn_800D47E8(_xEnt* ent);

static _zUI* sSorted[768];
static U32 sSortedCount;
static F32 sTimer;
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

// Partial: textured quad and model rendering are not yet reconstructed.
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
        // Draw the textured quad.
    }
    else if (ent->model)
    {
        // Draw the UI model.
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
