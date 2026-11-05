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
#define FONT_WIGGLE lbl_8025E8C0
#elif VERSION_GIHE78_REV1
#define FONT_WIGGLE lbl_80270758
#endif

extern "C" F32 FONT_WIGGLE;
void xFontBoxSetInset(xFontBox*, F32, F32, F32, F32);
void xFontBoxSetBackdrop(xFontBox*, iColor_tag);
void zUI_Init(_zUI*, p2EntAsset*);
void zUI_Reset(_zUI*);
S32 zUIEventCB(xBase*, xBase*, U32, F32*, xBase*);

void zUIFont_Render(_xEnt* e);
S32 zUIFontEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
void zUIFont_Update(_zUIFont* ent, _xScene* sc, F32 dt);
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
    ent->render = zUIFont_Render;
    ent->fasset = font;
    ent->update = (void (*)(_xEnt*, _xScene*, F32))zUIFont_Update;
    ent->eventFunc = zUIFontEventCB;
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

// Save the base UI state.
void zUIFont_Save(_zUIFont* ent, xSerial* s)
{
    zUI_Save(ent, s);
}

// Load the base UI state.
void zUIFont_Load(_zUIFont* ent, xSerial* s)
{
    zUI_Load(ent, s);
}

// Translate controller presses into the font widget's input flags.
void zUIFont_PreUpdate(_zUIFont* ent, _xScene*, F32)
{
    _zUI* ui = ent;

    ui->uiButton = 0;
    if (globals.pad0 && globals.pad0->pressed && (ui->uiFlags & 8) && ((ui->uiFlags & 2) || (ui->uiFlags & 1)))
    {
        if (globals.pad0->pressed & 0x10) ui->uiButton |= 0x10;
        else if (globals.pad0->pressed & 0x40) ui->uiButton |= 0x40;
        else if (globals.pad0->pressed & 0x80) ui->uiButton |= 0x80;
        else if (globals.pad0->pressed & 0x20) ui->uiButton |= 0x20;
        if (globals.pad0->pressed & 0x1) ui->uiButton |= 0x1;
        if (globals.pad0->pressed & 0x2) ui->uiButton |= 0x2;
        if (globals.pad0->pressed & 0x1000) ui->uiButton |= 0x1000;
        if (globals.pad0->pressed & 0x2000) ui->uiButton |= 0x2000;
        if (globals.pad0->pressed & 0x100) ui->uiButton |= 0x100;
        if (globals.pad0->pressed & 0x200) ui->uiButton |= 0x200;
        if (globals.pad0->pressed & 0x10000) ui->uiButton |= 0x10000;
        if (globals.pad0->pressed & 0x20000) ui->uiButton |= 0x20000;
        if (globals.pad0->pressed & 0x40000) ui->uiButton |= 0x40000;
        if (globals.pad0->pressed & 0x80000) ui->uiButton |= 0x80000;
        if (ui->uiButton) ui->uiFlags |= 0x40;
    }
}

// Dispatch the widget input as UI events.
void zUIFont_Update(_zUIFont* ent, _xScene* sc, F32 dt)
{
    _zUI* ui = ent;

    xEntUpdate(ui, sc, dt);
    if (!ui->uiButton) return;
    if (ui->uiButton & 0x10)
    {
        zEntEvent(ent, ent, 0x49);
        if ((ent->id == xStrHash("LD GAMESLOT 0") || ent->id == xStrHash("LD GAMESLOT 2")) &&
            !FONT_SLOT_EMPTY(0) && FONT_SLOT_EMPTY(1) && !FONT_SLOT_EMPTY(2))
            zEntEvent(ent, ent, 0x4A);
    }
    else if (ui->uiButton & 0x40)
    {
        zEntEvent(ent, ent, 0x4A);
        if ((ent->id == xStrHash("LD GAMESLOT 0") || ent->id == xStrHash("LD GAMESLOT 2")) &&
            !FONT_SLOT_EMPTY(0) && FONT_SLOT_EMPTY(1) && !FONT_SLOT_EMPTY(2))
            zEntEvent(ent, ent, 0x49);
    }
    else if (ui->uiButton & 0x80) zEntEvent(ent, ent, 0x4C);
    else if (ui->uiButton & 0x20) zEntEvent(ent, ent, 0x4B);
    else if (ui->uiButton & 0x1) zEntEvent(ent, ent, 0x47);
    else if (ui->uiButton & 0x2) zEntEvent(ent, ent, 0x48);
    else if (ui->uiButton & 0x1000) { zEntEvent(ent, ent, 0x45); ui->uiButton = 0x1000; }
    else if (ui->uiButton & 0x2000) { zEntEvent(ent, ent, 0x46); ui->uiButton = 0x2000; }
    else if (ui->uiButton & 0x100) zEntEvent(ent, ent, 0x43);
    else if (ui->uiButton & 0x200) zEntEvent(ent, ent, 0x44);
    else if (ui->uiButton & 0x10000) zEntEvent(ent, ent, 0x3F);
    else if (ui->uiButton & 0x20000) zEntEvent(ent, ent, 0x41);
    else if (ui->uiButton & 0x40000) zEntEvent(ent, ent, 0x42);
    else if (ui->uiButton & 0x80000) zEntEvent(ent, ent, 0x40);
    ui->uiFlags &= ~0x40;
}

// Handle visibility and font flags, forwarding other events to the base UI.
S32 zUIFontEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    S32 rval = 1;
    _zUIFont* s = (_zUIFont*)to;
    switch (toEvent)
    {
    case 3:
        s->flags |= 1;
        switch (s->fasset->mode)
        {
        case 3:
        case 4:
        case 5:
            zEntEvent("mnu4 xtocontinue", toEvent);
            break;
        case 6:
            break;
        }
        break;
    case 4:
        s->flags &= ~1;
        switch (s->fasset->mode)
        {
        case 3:
        case 4:
        case 5:
            zEntEvent("mnu4 xtocontinue", toEvent);
            break;
        case 6:
            break;
        }
        break;
    case 10:
        UI_RESET(s);
        s->uiFontFlags = s->fasset->uiFontFlags;
        s->uiButton = 0;
        if (s->flags & 0x40) s->flags ^= 0x40;
        break;
    case 0x4D:
        s->uiFontFlags |= 8;
        break;
    case 0x4E:
        if (s->uiFontFlags & 8) s->uiFontFlags ^= 8;
        break;
    default:
        rval = UI_EVENT(from, to, toEvent, toParam, toParamWidget);
    }
    return rval;
}

// Draw the font asset's processed text in its configured box.
void zUIFont_Render(_xEnt* e)
{
    _zUIFont* ent = (_zUIFont*)e;
    iColor_tag c;
    xFontBox* b;
    U32 jflags;
    char* text;
    char processedText[512];

    if (!xEntIsVisible(ent))
        return;

    c.r = ent->fasset->color[0];
    c.g = ent->fasset->color[1];
    c.b = ent->fasset->color[2];
    c.a = ent->fasset->color[3];
    if (ent->uiFontFlags & 0x10)
    {
        if (ent->uiFontFlags & 0x80)
            ent->uiFontFlags ^= 0x80;
        if ((ent->uiFontFlags & 0x20) && !(ent->uiFlags & 2))
        {
            ent->uiFontFlags |= 0x80;
            c.a >>= 1;
        }
        if ((ent->uiFontFlags & 0x40) && !(ent->uiFlags & 8))
        {
            ent->uiFontFlags |= 0x80;
            c.a >>= 1;
        }
    }
    if (!(ent->uiFlags & 4))
        return;
    b = xFontBoxCreate(ent->fasset->pos.x, ent->fasset->pos.y, ent->fasset->dim[0], ent->fasset->dim[1], ent->fasset->fontID, c);
    if (!b)
        return;
    xFontBoxSetDrawSpacing(b, ent->fasset->space[0], ent->fasset->space[1]);
    xFontBoxSetInset(b, ent->fasset->inset[2], ent->fasset->inset[3], ent->fasset->inset[0], ent->fasset->inset[1]);
    jflags = 0;
    jflags |= (ent->uiFontFlags & 1) ? 0x10 : 0;
    jflags |= (ent->uiFontFlags & 2) ? 0x20 : 0;
    jflags |= (ent->uiFontFlags & 4) ? 0x40 : 0;
    xFontBoxSetJustification(b, jflags);
    xFontBoxSetDrawSize(b, ent->fasset->cdim[0], ent->fasset->cdim[1]);
    if (ent->uiFontFlags & 8)
    {
        c.r = ent->fasset->bcolor[0];
        c.g = ent->fasset->bcolor[1];
        c.b = ent->fasset->bcolor[2];
        c.a = ent->fasset->bcolor[3];
        xFontBoxSetBackdrop(b, c);
    }
    text = (char*)xSTFindAsset(ent->fasset->textAssetID, 0);
    if (text)
    {
        zVarMagicProcessText(text + 4, processedText);
        xFontBoxSetText(b, processedText);
        if (((ent->uiFontHackFlags & 1) && (ent->uiFlags & 2)) ||
            (ent->uiFontHackFlags & 2) || ((ent->uiFontFlags & 0x200) && (ent->uiFlags & 2)))
            FONT_WIGGLE = 1.0f;
        else
            FONT_WIGGLE = 0.0f;
        xFontBoxRender(b);
        FONT_WIGGLE = 0.0f;
    }
    xFontBoxDestroy(b);
}
