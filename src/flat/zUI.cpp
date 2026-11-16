#include "zUI.h"
#include "zUIFont.h"
#include "zScene.h"
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
#include "zVar.h"
#include <stdlib.h>

extern F32 gFontWiggleTime;
void zUI_Render(_xEnt* ent);
void zUIFont_Render(_xEnt* e);

static _zUI* sSorted[768];
static U32 sSortedCount;
static RwIm2DVertex Vertex[4];
static RwImVertexIndex Index[6] = { 0, 1, 2, 0, 2, 3 };
static zSceneLevel sLevelTable2[14] = {
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
// Cache the key-image identifiers; UI initialization also refreshes them.
static U32 cKeyUIid1off = xStrHash("KEY OFF 1 UI");
static U32 cKeyUIid2off = xStrHash("KEY OFF 2 UI");
static U32 cKeyUIid3off = xStrHash("KEY OFF 3 UI");
static U32 cKeyUIid4off = xStrHash("KEY OFF 4 UI");
static U32 cKeyUIid1on = xStrHash("KEY ON 1 UI");
static U32 cKeyUIid2on = xStrHash("KEY ON 2 UI");
static U32 cKeyUIid3on = xStrHash("KEY ON 3 UI");
static U32 cKeyUIid4on = xStrHash("KEY ON 4 UI");
static F32 sTimer;

void zUI_Reset(_zUI* ent);

S32 zUIEventCB(xBase*, xBase* to, U32 event, F32*, xBase*);
S32 iRenderQSort_Face(const void* a, const void* b);
xBase* zUIRenderIteratorInsert(xBase* b, zScene*, void*);
void zUI_Render(_xEnt* e);
void zUIRenderAll();
void zUI_Update(_zUI* ent, _xScene* sc, F32 dt);
void zUI_PreUpdate(_zUI* ent, _xScene*, F32);
void zUI_Load(_zUI* ent, xSerial* s);
void zUI_Save(_zUI* ent, xSerial* s);
void zUI_Init(_zUI* ent, p2EntAsset* asset);
void zUI_Init(void* ent, void* asset);

// Route generic scene objects and assets through typed UI initialization.
void zUI_Init(void* ent, void* asset)
{
    zUI_Init(static_cast<_zUI*>(ent), static_cast<p2EntAsset*>(asset));
}

// Initialize the UI entity and its shared key-display identifiers.
void zUI_Init(_zUI* ent, p2EntAsset* asset)
{
    _zEnt* base = ent;
    zEntInit(base, asset, 'UI  ');
    zUIAsset* sasset = static_cast<zUIAsset*>(asset);
    ent->sasset = sasset;
    ent->pflags = 0;
    U8 penby = ent->penby;
    penby |= 0x10;
    ent->penby = penby;
    ent->chkby = 0;
    ent->update = (void (*)(_xEnt*, _xScene*, F32))zUI_Update;
    ent->move = 0;
    ent->eventFunc = zUIEventCB;
    if (ent->linkCount)
        ent->link = (p2LinkAsset*)(sasset + 1);
    ent->eventFunc = zUIEventCB;
    ent->uiFlags = sasset->uiFlags;
    if (ent->uiFlags & 4) ent->render = 0;
    zEntReset(base);
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

// UI persistence uses the base entity state.
void zUI_Save(_zUI* ent, xSerial* s)
{
    zEntSave(ent, s);
}

// UI persistence uses the base entity state.
void zUI_Load(_zUI* ent, xSerial* s)
{
    zEntLoad(ent, s);
}

// Reset the entity and discard the cached sorted list.
void zUI_Reset(_zUI* ent)
{
    zEntReset(ent);
    ent->pflags = 0;
    U8 penby = ent->penby;
    penby |= 0x10;
    ent->penby = penby;
    ent->chkby = 0;
    ent->uiFlags = ent->sasset->uiFlags;
    sSortedCount = 0;
}

// Capture input for a focused UI element before its update.
void zUI_PreUpdate(_zUI* ent, _xScene*, F32)
{
    ent->uiButton = 0;
    if (globals.pad0 && globals.pad0->pressed && (ent->uiFlags & 8) &&
        ((ent->uiFlags & 2) || (ent->uiFlags & 1)))
    {
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

// Collect each UI element for the later depth sort.
xBase* zUIRenderIteratorInsert(xBase* b, zScene*, void*)
{
    sSorted[sSortedCount++] = static_cast<_zUI*>(b);
    return b;
}

// Draw UI elements with larger asset depth first.
S32 iRenderQSort_Face(const void* a, const void* b)
{
    if ((*static_cast<_zUI* const*>(a))->sasset->pos.z < (*static_cast<_zUI* const*>(b))->sasset->pos.z)
        return 1;
    if ((*static_cast<_zUI* const*>(a))->sasset->pos.z > (*static_cast<_zUI* const*>(b))->sasset->pos.z)
        return -1;
    return 0;
}

// Gather, sort and draw visible UI elements.
void zUIRenderAll()
{
    sTimer += 0.05f;
    if (sTimer >= 2.0f) sTimer -= 2.0f;

    if (!sSortedCount)
    {
        zSceneForAllBase(zUIRenderIteratorInsert, 0x20, 0);
        zSceneForAllBase(zUIRenderIteratorInsert, 0x21, 0);
        if (sSortedCount > 1)
            qsort(sSorted, sSortedCount, sizeof(sSorted[0]), iRenderQSort_Face);
    }

    for (S32 i = 0; i < (S32)sSortedCount; ++i)
    {
        if (!xEntIsVisible(sSorted[i])) continue;
        if (sSorted[i]->baseType == 0x20)
            zUI_Render(sSorted[i]);
        else if (zGameIsPaused())
        {
            if (!(static_cast<zUIFontAsset*>(sSorted[i]->sasset)->uiFontFlags & 0x100))
                zUIFont_Render(sSorted[i]);
        }
        else
        {
            zUIFont_Render(sSorted[i]);
        }
    }
}

// Draw UI images and model previews, including map pulses and warp-item motion.
void zUI_Render(_xEnt* e)
{
    _zUI* ui = static_cast<_zUI*>(e);
    if (xScrFxIsLetterbox() &&
        (ui->id == cKeyUIid1off || ui->id == cKeyUIid2off ||
         ui->id == cKeyUIid3off || ui->id == cKeyUIid4off ||
         ui->id == cKeyUIid1on || ui->id == cKeyUIid2on ||
         ui->id == cKeyUIid3on || ui->id == cKeyUIid4on))
        return;

    if (ui->uiFlags & 4)
    {
        if (!xEntIsVisible(ui)) return;
        if (ui->sasset->textureID)
        {
            RwTexture* texture = static_cast<RwTexture*>(xSTFindAsset(ui->sasset->textureID, 0));
            if (texture)
            {
                RwRaster* raster = texture->raster;
                if (raster)
                    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
            }
            F32 u1 = ui->sasset->uva[0];
            F32 v1 = ui->sasset->uva[1];
            F32 u2 = ui->sasset->uvb[0];
            F32 v2 = ui->sasset->uvb[1];
            F32 u3 = ui->sasset->uvc[0];
            F32 v3 = ui->sasset->uvc[1];
            F32 u4 = ui->sasset->uvd[0];
            F32 v4 = ui->sasset->uvd[1];
            F32 x1 = (640.0f * ui->sasset->pos.x) / 640.0f;
            F32 y1 = (480.0f * ui->sasset->pos.y) / 480.0f;
            F32 x2 = (640.0f * (ui->sasset->pos.x + ui->sasset->dim[0])) / 640.0f;
            F32 y2 = (480.0f * (ui->sasset->pos.y + ui->sasset->dim[1])) / 480.0f;

            static U32 hack_pulse1 = xStrHash("paws_map");
            static U32 hack_pulse2 = xStrHash("paws_corner");
            static U32 hack_pulse3 = xStrHash("sd_tag_map");
            if (ui->sasset->textureID == hack_pulse1 || ui->sasset->textureID == hack_pulse2 ||
                ui->sasset->textureID == hack_pulse3)
            {
                F32 wave = (F32)sin(15.0f * gFontWiggleTime);
                F32 pulse_w = (1.0f + wave) * (0.05f * (x2 - x1));
                F32 pulse_h = (1.0f + wave) * (0.05f * (y2 - y1));
                x1 -= pulse_w;
                x2 += pulse_w;
                y1 -= pulse_h;
                y2 += pulse_h;
            }

            static U32 hack_token1 = xStrHash("warp token ui");
            static U32 hack_token2 = xStrHash("warp snack ui");
            if (ui->id == hack_token1 || ui->id == hack_token2)
            {
                F32 wave = (F32)sin(5.0f * gFontWiggleTime + x1);
                F32 pulse_w = (0.5f * wave) * (x2 - x1);
                x1 = (640.0f * ui->sasset->pos.x) / 640.0f - pulse_w;
                x2 = (640.0f * ui->sasset->pos.x) / 640.0f + pulse_w;
            }

            F32 z = RwIm2DGetNearScreenZ();
            RwIm2DVertexSetIntRGBA(&Vertex[0], 255, 255, 255, 255);
            RwIm2DVertexSetIntRGBA(&Vertex[1], 255, 255, 255, 255);
            RwIm2DVertexSetIntRGBA(&Vertex[2], 255, 255, 255, 255);
            RwIm2DVertexSetIntRGBA(&Vertex[3], 255, 255, 255, 255);
            RwIm2DVertexSetScreenX(&Vertex[0], x1);
            RwIm2DVertexSetScreenY(&Vertex[0], y1);
            RwIm2DVertexSetScreenZ(&Vertex[0], z);
            Vertex[0].u = u1; Vertex[0].v = v1;
            RwIm2DVertexSetScreenX(&Vertex[1], x1);
            RwIm2DVertexSetScreenY(&Vertex[1], y2);
            RwIm2DVertexSetScreenZ(&Vertex[1], z);
            Vertex[1].u = u4; Vertex[1].v = v4;
            RwIm2DVertexSetScreenX(&Vertex[2], x2);
            RwIm2DVertexSetScreenY(&Vertex[2], y2);
            RwIm2DVertexSetScreenZ(&Vertex[2], z);
            Vertex[2].u = u3; Vertex[2].v = v3;
            RwIm2DVertexSetScreenX(&Vertex[3], x2);
            RwIm2DVertexSetScreenY(&Vertex[3], y1);
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
        else if (ui->model)
        {
            xVec3 relfrom = { 0.0f, 1.0f, 5.0f };
            xVec3 relto = { 0.0f, 0.0f, 0.0f };
            xVec3 from;
            xVec3 to;
            xVec3* pos = (xVec3*)&ui->model->Mat->pos;
            _xRect win1 = { 0.0f, 0.0f, 0.0f, 0.0f };
            win1.x = (640.0f * ui->sasset->pos.x) / 640.0f;
            win1.y = (480.0f * ui->sasset->pos.y) / 480.0f;
            win1.w = (640.0f * (ui->sasset->pos.x + ui->sasset->dim[0])) / 640.0f;
            win1.h = (480.0f * (ui->sasset->pos.y + ui->sasset->dim[1])) / 480.0f;
            xVec3Add(&from, &relfrom, pos);
            xVec3Add(&to, &relto, pos);
            xModelRender2D(ui->model, &win1, &from, &to, 1.5707964f);
        }
    }
    else
    {
        xEntRender(e);
    }
}

// Handle visibility and focus changes for a UI element.
S32 zUIEventCB(xBase*, xBase* to, U32 event, F32*, xBase*)
{
    _zUI* s = static_cast<_zUI*>(to);
    switch (event)
    {
    case eEventVisible:
        s->flags |= 1;
        break;
    case eEventInvisible:
        s->flags &= ~1;
        break;
    case eEventReset:
        zUI_Reset(s);
        break;
    case 0x4F:
        s->uiFlags |= 2;
        for (S32 i = 0; i < 13; ++i)
        {
            char level = sLevelTable2[i].name;
            for (S32 j = 0; j < 30; ++j)
            {
                char uiName[256];
                sprintf(uiName, "%c %02d SELECTED", level, j);
                if (s->id == xStrHash(uiName))
                {
                    zEntEvent(s, s, eEventVisible);
                    char scene[5] = { 0 };
                    strncpy(scene, uiName, 4);
                    scene[1] = '0';
                    zVarSetArea((U32(scene[0]) << 24) | (U32(scene[1]) << 16) |
                                (U32(scene[2]) << 8) | U32(scene[3]));
                }
            }
        }
        break;
    case 0x50:
        if (s->uiFlags & 2) s->uiFlags ^= 2;
        for (S32 i = 0; i < 13; ++i)
        {
            char level = sLevelTable2[i].name;
            for (S32 j = 0; j < 30; ++j)
            {
                char uiName[256];
                sprintf(uiName, "%c %02d SELECTED", level, j);
                if (s->id == xStrHash(uiName))
                    zEntEvent(s, s, eEventInvisible);
            }
        }
        break;
    case eEventUIFocusOn:
        s->uiFlags |= 8;
        if (s->uiFlags & 0x10) zEntEvent(s, s, eEventVisible);
        break;
    case eEventUIFocusOff:
        if (s->uiFlags & 8) s->uiFlags ^= 8;
        if (s->uiFlags & 0x20) zEntEvent(s, s, eEventInvisible);
        break;
    case 0x5E:
        zEntEvent(s, s, eEventUIFocusOn);
        zEntEvent(s, s, 0x4F);
        break;
    case 0x5F:
        zEntEvent(s, s, eEventUIFocusOff);
        zEntEvent(s, s, 0x50);
        break;
    }
    return 1;
}
