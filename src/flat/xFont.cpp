#include "xFont.h"
#include "xstransvc.h"
#include "xutil.h"

S32 xMemPushBase();

static char* blankText = "";
xFontBox gBoxList[4];
xFontInfo gFontList[4];

// Loads the FONT scene's assets in one go, clears the font and text box tables and hands over to
// the platform layer.
void xFontInit()
{
    F32 pdone;

    xMemPushBase();

    xUtil_idtag2string('FONT', 0);
    xSTPreLoadScene('FONT', NULL);
    xUtil_idtag2string('FONT', 0);
    xSTQueueSceneAssets('FONT');
    xUtil_idtag2string('FONT', 0);

    do
    {
        pdone = xSTLoadStep('FONT');
    } while (pdone < 1.0f);

    xUtil_idtag2string('FONT', 0);

    for (U32 i = 0; i < 4; i++)
    {
        gFontList[i].flags = 0;
    }

    for (U32 i = 0; i < 4; i++)
    {
        gBoxList[i].flags = 0;
    }

    iFontInit();
}

// Flag 0x1 marks a text box in use.
void xFontRender()
{
    U32 i;
    xFontBox* box = gBoxList;

    for (i = 0; i < 4; i++, box++)
    {
        if (box->flags & 0x1)
        {
            xFontBoxRender(box);
        }
    }
}

// Sizes are given on a 640x480 screen.
void xFontBoxSetDrawSize(xFontBox* box, F32 draw_width, F32 draw_height)
{
    box->draw_width = (640.0f * draw_width) / 640.0f;
    box->draw_height = (480.0f * draw_height) / 480.0f;
}

// Takes the first free text box (the scan runs off the end of the table when all four are in
// use); new boxes draw characters at the font's own size.
xFontBox* xFontBoxCreate(F32 x, F32 y, F32 w, F32 h, U32 id, iColor_tag color)
{
    xFontInfo* font;
    xFontBox* box = gBoxList;
    U32 i;

    for (i = 0; i < 4; i++, box++)
    {
        if (box->flags == 0)
        {
            break;
        }
    }

    box->x = (640.0f * x) / 640.0f;
    box->y = (480.0f * y) / 480.0f;
    box->w = (640.0f * w) / 640.0f;
    box->h = (480.0f * h) / 480.0f;
    box->color = color;
    box->saved_color = color;
    box->font = id;
    font = &gFontList[id];
    box->depth = 1.0f;
    box->inset_left = 4.0f;
    box->inset_right = 4.0f;
    box->inset_top = 4.0f;
    box->inset_bottom = 4.0f;
    box->x_spacing = (F32)(font->char_width * 640) / 640.0f;
    box->y_spacing = (F32)(font->char_height * 480) / 480.0f;
    box->draw_width = (F32)(font->char_width * 640) / 640.0f;
    box->draw_height = (F32)(font->char_height * 480) / 480.0f;
    box->flags = 0x11;

    return box;
}

void xFontBoxDestroy(xFontBox* box)
{
    box->flags = 0;
}

void xFontBoxSetText(xFontBox* box, char* text)
{
    if (text == NULL)
    {
        text = blankText;
    }
    box->text = text;
}

void xFontBoxSetFont(xFontBox* box, U32 id)
{
    xFontInfo* font;

    box->font = id;
    font = &gFontList[id];
    box->x_spacing = (F32)(font->char_width * 640) / 640.0f;
    box->y_spacing = (F32)(font->char_height * 480) / 480.0f;
}

// The text color can be changed by codes inside the text; saved_color is the one to go back to.
void xFontBoxSetColor(xFontBox* box, iColor_tag color)
{
    box->color = color;
    box->saved_color = color;
}

void xFontBoxSetDrawSpacing(xFontBox* box, F32 x_spacing, F32 y_spacing)
{
    box->x_spacing = (640.0f * x_spacing) / 640.0f;
    box->y_spacing = (480.0f * y_spacing) / 480.0f;
}

void xFontBoxSetInset(xFontBox* box, F32 inset_left, F32 inset_right, F32 inset_top,
                      F32 inset_bottom)
{
    box->inset_left = (640.0f * inset_left) / 640.0f;
    box->inset_right = (640.0f * inset_right) / 640.0f;
    box->inset_top = (480.0f * inset_top) / 480.0f;
    box->inset_bottom = (480.0f * inset_bottom) / 480.0f;
}

// Bits 0x10, 0x20 and 0x40 select the justification.
void xFontBoxSetJustification(xFontBox* box, S32 mode)
{
    box->flags &= ~0x10;
    box->flags &= ~0x20;
    box->flags &= ~0x40;
    box->flags |= mode;
}

// Flag 0x100 draws a backdrop behind the text.
void xFontBoxSetBackdrop(xFontBox* box, iColor_tag color)
{
    box->flags |= 0x100;
    box->backdrop_color = color;
}
