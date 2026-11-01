#include "zUIFont.h"
#include "zGlobals.h"
#include "xPad.h"
#include "xString.h"
#include "xFont.h"
#include "xColor.h"
#include "xstransvc.h"
#include "zVar.h"
#include "version.h"

U32 zSaveLoad_slotIsEmpty(U32 i);
#define FONT_SLOT_EMPTY zSaveLoad_slotIsEmpty
#define UI_RESET zUI_Reset
#define UI_EVENT zUIEventCB
#define UI_INIT zUI_Init

#if VERSION_GIHE78_REV0
#define FONT_UPDATE fn_800D4C5C
#define FONT_EVENT fn_800D4B0C
#define FONT_RENDER fn_800D47E8
#define FONT_WIGGLE lbl_8025E8C0
#elif VERSION_GIHE78_REV1
#define FONT_UPDATE fn_800D4850
#define FONT_EVENT fn_800D4700
#define FONT_RENDER fn_800D43DC
#define FONT_WIGGLE lbl_80270758
#endif

extern "C" F32 FONT_WIGGLE;
void xFontBoxSetInset(xFontBox*, F32, F32, F32, F32);
void xFontBoxSetBackdrop(xFontBox*, iColor_tag);
void zUI_Init(_zUI*, p2EntAsset*);
void zUI_Reset(_zUI*);
S32 zUIEventCB(xBase*, xBase*, U32, F32*, xBase*);

// Defined in reverse address order: the deferred profile emits the last-defined function first.
extern "C" void FONT_RENDER(_xEnt* base);
extern "C" S32 FONT_EVENT(xBase* from, xBase* to, U32 event, F32* param, xBase* widget);
extern "C" void FONT_UPDATE(_xEnt* base, _xScene* sc, F32 dt);
void zUIFont_PreUpdate(_zUIFont* ent, _xScene*, F32);
void zUIFont_Save(_zUIFont* ent, xSerial* s);
void zUIFont_Load(_zUIFont* ent, xSerial* s);
void zUIFont_Init(void* object, void* asset);

// Initialize the widget and apply the font mode's presentation defaults.
void zUIFont_Init(void* object, void* asset)
{
    _zUIFont* ent = (_zUIFont*)object;
    zUIFontAsset* font = (zUIFontAsset*)asset;
    UI_INIT(ent, font);
    ent->render = FONT_RENDER;
    ent->fasset = font;
    ent->update = FONT_UPDATE;
    ent->eventFunc = FONT_EVENT;
    if (ent->linkCount) ent->link = (p2LinkAsset*)(ent->fasset + 1);
    ent->uiFontFlags = ent->fasset->uiFontFlags;
    U16 flags = 0;
    if (ent->id == xStrHash("MNU3 PRESS START")) flags |= 1;
    if (ent->id == xStrHash("MNU3 PRESS START")) flags |= 2;
    ent->uiFontHackFlags = flags;
    ent->uiButton = 0;
    if (ent->flags & 0x40) ent->flags ^= 0x40;
    switch (ent->fasset->mode)
    {
    case 1:
    case 2:
        ent->fasset->pos.x = 50.0f;
        ent->fasset->dim[0] = 0x21C;
    case 3:
    case 4:
    case 5:
    case 6:
        ent->fasset->fontID = 1;
        ent->fasset->color[0] = 255;
        ent->fasset->color[1] = 230;
        ent->fasset->color[2] = 0;
        ent->fasset->color[3] = 255;
    }
}

// Load the base UI state.
void zUIFont_Load(_zUIFont* ent, xSerial* s)
{
    zUI_Load(ent, s);
}

// Save the base UI state.
void zUIFont_Save(_zUIFont* ent, xSerial* s)
{
    zUI_Save(ent, s);
}

// Translate controller presses into the font widget's input flags.
void zUIFont_PreUpdate(_zUIFont* ent, _xScene*, F32)
{
    ent->uiButton = 0;
    if (!globals.pad0 || !globals.pad0->pressed || !(ent->uiFlags & 8))
        return;
    if (!(ent->uiFlags & 2))
    {
        if (!(ent->uiFlags & 1))
            return;
    }

    U32 buttons = globals.pad0->pressed;
    if (buttons & 0x10) ent->uiButton |= 0x10;
    else if (buttons & 0x40) ent->uiButton |= 0x40;
    else if (buttons & 0x80) ent->uiButton |= 0x80;
    else if (buttons & 0x20) ent->uiButton |= 0x20;
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

// Dispatch the widget input as UI events.
extern "C" void FONT_UPDATE(_xEnt* base, _xScene* sc, F32 dt)
{
    _zUIFont* ent = (_zUIFont*)base;
    xEntUpdate(ent, sc, dt);
    U32 buttons = ent->uiButton;
    if (!buttons) return;
    if (buttons & 0x10)
    {
        zEntEvent(ent, ent, 0x49);
        if ((ent->id == xStrHash("LD GAMESLOT 0") || ent->id == xStrHash("LD GAMESLOT 2")) &&
            !FONT_SLOT_EMPTY(0) && FONT_SLOT_EMPTY(1) && !FONT_SLOT_EMPTY(2))
            zEntEvent(ent, ent, 0x4A);
    }
    else if (buttons & 0x40)
    {
        zEntEvent(ent, ent, 0x4A);
        if ((ent->id == xStrHash("LD GAMESLOT 0") || ent->id == xStrHash("LD GAMESLOT 2")) &&
            !FONT_SLOT_EMPTY(0) && FONT_SLOT_EMPTY(1) && !FONT_SLOT_EMPTY(2))
            zEntEvent(ent, ent, 0x49);
    }
    else if (buttons & 0x80) zEntEvent(ent, ent, 0x4C);
    else if (buttons & 0x20) zEntEvent(ent, ent, 0x4B);
    else if (buttons & 0x1) zEntEvent(ent, ent, 0x47);
    else if (buttons & 0x2) zEntEvent(ent, ent, 0x48);
    else if (buttons & 0x1000) { zEntEvent(ent, ent, 0x45); ent->uiButton = 0x1000; }
    else if (buttons & 0x2000) { zEntEvent(ent, ent, 0x46); ent->uiButton = 0x2000; }
    else if (buttons & 0x100) zEntEvent(ent, ent, 0x43);
    else if (buttons & 0x200) zEntEvent(ent, ent, 0x44);
    else if (buttons & 0x10000) zEntEvent(ent, ent, 0x3F);
    else if (buttons & 0x20000) zEntEvent(ent, ent, 0x41);
    else if (buttons & 0x40000) zEntEvent(ent, ent, 0x42);
    else if (buttons & 0x80000) zEntEvent(ent, ent, 0x40);
    ent->uiFlags &= ~0x40;
}

// Handle visibility and font flags, forwarding other events to the base UI.
extern "C" S32 FONT_EVENT(xBase* from, xBase* to, U32 event, F32* param, xBase* widget)
{
    _zUIFont* ent = (_zUIFont*)to;
    S32 result = 1;
    switch (event)
    {
    case 3:
        ent->flags |= 1;
        switch (ent->fasset->mode)
        {
        case 3:
        case 4:
        case 5:
            zEntEvent("mnu4 xtocontinue", event);
        }
        break;
    case 4:
        ent->flags &= ~1;
        switch (ent->fasset->mode)
        {
        case 3:
        case 4:
        case 5:
            zEntEvent("mnu4 xtocontinue", event);
        }
        break;
    case 10:
        UI_RESET(ent);
        ent->uiFontFlags = ent->fasset->uiFontFlags;
        ent->uiButton = 0;
        if (ent->flags & 0x40) ent->flags ^= 0x40;
        break;
    case 0x4D:
        ent->uiFontFlags |= 8;
        break;
    case 0x4E:
        if (ent->uiFontFlags & 8) ent->uiFontFlags ^= 8;
        break;
    default:
        result = UI_EVENT(from, to, event, param, widget);
    }
    return result;
}

// Draw the font asset's processed text in its configured box.
extern "C" void FONT_RENDER(_xEnt* base)
{
    _zUIFont* ent = (_zUIFont*)base;
    if (!xEntIsVisible(ent)) return;

    iColor_tag color = xColorFromRGBA(ent->fasset->color[0], ent->fasset->color[1], ent->fasset->color[2], ent->fasset->color[3]);
    if (ent->uiFontFlags & 0x10)
    {
        if (ent->uiFontFlags & 0x80) ent->uiFontFlags ^= 0x80;
        if ((ent->uiFontFlags & 0x20) && !(ent->uiFlags & 2))
        {
            ent->uiFontFlags |= 0x80;
            color.a >>= 1;
        }
        if ((ent->uiFontFlags & 0x40) && !(ent->uiFlags & 8))
        {
            ent->uiFontFlags |= 0x80;
            color.a >>= 1;
        }
    }
    if (!(ent->uiFlags & 4)) return;
    xFontBox* box = xFontBoxCreate(ent->fasset->pos.x, ent->fasset->pos.y, ent->fasset->dim[0], ent->fasset->dim[1], ent->fasset->fontID, color);
    if (!box) return;
    xFontBoxSetDrawSpacing(box, ent->fasset->space[0], ent->fasset->space[1]);
    xFontBoxSetInset(box, ent->fasset->inset[2], ent->fasset->inset[3], ent->fasset->inset[0], ent->fasset->inset[1]);
    xFontBoxSetJustification(box, (ent->uiFontFlags & 7) << 4);
    xFontBoxSetDrawSize(box, ent->fasset->cdim[0], ent->fasset->cdim[1]);
    if (ent->uiFontFlags & 8)
        xFontBoxSetBackdrop(box, xColorFromRGBA(ent->fasset->bcolor[0], ent->fasset->bcolor[1], ent->fasset->bcolor[2], ent->fasset->bcolor[3]));
    char* text = (char*)xSTFindAsset(ent->fasset->textAssetID, 0);
    if (text)
    {
        char processed[512];
        zVarMagicProcessText(text + 4, processed);
        xFontBoxSetText(box, processed);
        if (((ent->uiFontHackFlags & 1) && (ent->uiFlags & 2)) ||
            (ent->uiFontHackFlags & 2) || ((ent->uiFontFlags & 0x200) && (ent->uiFlags & 2)))
            FONT_WIGGLE = 1.0f;
        else
            FONT_WIGGLE = 0.0f;
        xFontBoxRender(box);
        FONT_WIGGLE = 0.0f;
    }
    xFontBoxDestroy(box);
}
