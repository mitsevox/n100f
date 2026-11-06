#include "xFont.h"
#include "xColor.h"
#include "xstransvc.h"
#include "xutil.h"

S32 xMemPushBase();

void iFontSaveRenderState();
void iFontRestoreRenderState();
void iFontSetCharRenderState(xFontBox* box);
void iFontSetBackdropRenderState(xFontBox* box);
void iFontDrawBackdrop(xFontBox* box);

xFontGlobals_t gFontGlobals = { 0.0f, 0.0f, 0, 0.0f };

static char* blankText = "";
xFontInfo gFontList[4];
xFontBox gBoxList[4];

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

// Reads a fixed-width decimal field into an integer. Single-digit escapes accept a literal
// character without consuming it; RGB fields consume three characters and narrow at color creation.
static inline S32 xFontReadDigits(char*& s, S32 count, S32 literalFallback)
{
    S32 n = 0;
    for (S32 i = 0; i < count; i++)
    {
        if (literalFallback && (*s < '0' || *s > '9'))
        {
            n = *s;
            break;
        }
        n = n * 10 + *s++ - '0';
    }
    return n;
}

// Measures, and when gFontGlobals.draw is set draws, one word: the characters up to white space,
// a line break ('\n' or '@') or the end, stopping early once the width would pass maxw (0 means no
// limit). Escape codes inside the text change the font ($B<digit>), the colour ($A<rrr><ggg><bbb>,
// $C for the box's saved colour, $D<digit> for one of the preset colours); a drawn word leaves the
// box with the last font and colour. Spacing within the word uses its initial font.
static char* ProcessWord(xFontBox* box, char* text, F32* width, F32* height, F32 maxw)
{
    char* s = text;
    U32 fontID = box->font;
    xFontInfo* font = &gFontList[fontID];
    iColor_tag color;
    color = box->color;
    F32 dx;

    *width = 0.0f;
    *height = 0.0f;

    for (;;)
    {
        if (s[0] == '$')
        {
            char c = s[1];
            s += 2;
            switch (c)
            {
            case 'B':
                fontID = xFontReadDigits(s, 1, 1);
                break;
            case 'A':
            {
                S32 r = xFontReadDigits(s, 3, 0);
                S32 g = xFontReadDigits(s, 3, 0);
                S32 b = xFontReadDigits(s, 3, 0);
                color = xColorFromRGBA(r, g, b, 0xFF);
                break;
            }
            case 'C':
                color = box->saved_color;
                break;
            case 'D':
                switch (xFontReadDigits(s, 1, 1))
                {
                case 0:
                case '#':
                {
                    color = xColorFromRGBA(0x0C, 0xB3, 0x5F, 0xFF);
                    break;
                }
                case 1:
                case '{':
                {
                    color = xColorFromRGBA(0xCD, 0x07, 0x00, 0xFF);
                    break;
                }
                case 2:
                case '}':
                case '~':
                {
                    color = xColorFromRGBA(0x99, 0xBD, 0xEE, 0xFF);
                    break;
                }
                case '|':
                default:
                {
                    color = xColorFromRGBA(0xFF, 0xFF, 0xFF, 0xFF);
                    break;
                }
                }
                break;
            }
        }

        if (font->flags & 4)
        {
            dx = box->x_spacing;
        }
        else
        {
            dx = font->pspc[*s].dx + 3U;
            dx *= box->x_spacing / font->char_width;
        }

        if (*s == '\n' || *s == '@' || *s == '\0' || *s == ' ' || *s == '\t')
        {
            break;
        }
        if (maxw != 0.0f && *width + dx > maxw)
        {
            break;
        }

        if (gFontGlobals.draw)
        {
            iFontDrawChar(*s, box->depth, color, box);
            gFontGlobals.dpx += dx;
        }

        *width += dx;
        *height = (*height > box->y_spacing) ? *height : box->y_spacing;
        s++;
    }

    if (gFontGlobals.draw)
    {
        box->font = fontID;
        box->color = color;
    }

    return s;
}

// Fits as many words as the line has room for, then draws them with the box's justification
// (flag 0x20 right, 0x40 centred). Returns where the next line starts; the text itself when not
// even one word fits.
static char* JustifyLine(xFontBox* box, char* text, F32 maxwidth, F32 maxheight)
{
    F32 width;
    F32 height;
    F32 whitespace_width;
    F32 testwidth;
    F32 testheight;
    char* s;
    char* t;
    char* end;
    xFontInfo* font = &gFontList[box->font];

    if (text == NULL)
    {
        return NULL;
    }

    height = 0.0f;
    width = 0.0f;
    gFontGlobals.draw = 0;
    s = end = text;

    while (width <= maxwidth && height <= maxheight && s != NULL)
    {
        if (*s == '\0')
        {
            break;
        }
        if (*s == '\n' || *s == '@')
        {
            end = s + 1;
            break;
        }

        whitespace_width = 0.0f;
        while (*s == ' ' || *s == '\t')
        {
            if (font->flags & 4)
            {
                if (*s == ' ')
                {
                    whitespace_width += box->x_spacing;
                }
                if (*s == '\t')
                {
                    whitespace_width += 8.0f * box->x_spacing;
                }
            }
            else
            {
                F32 space = ((font->pspc[' '].dx + 3U) * box->x_spacing) / font->char_width;
                if (*s == ' ')
                {
                    whitespace_width += space;
                }
                if (*s == '\t')
                {
                    whitespace_width += 8.0f * space;
                }
            }
            s++;
        }

        if (*s == '\0')
        {
            break;
        }
        if (*s == '\n' || *s == '@')
        {
            end = s + 1;
            break;
        }

        if (width == 0.0f)
        {
            whitespace_width = 0.0f;
        }

        t = ProcessWord(box, s, &testwidth, &testheight, 0.0f);
        if (testwidth + (width + whitespace_width) > maxwidth)
        {
            if (testwidth > maxwidth)
            {
                end = ProcessWord(box, s, &testwidth, &testheight, maxwidth - (width + whitespace_width));
            }
            break;
        }
        if (testheight > maxheight)
        {
            break;
        }

        s = end = t;
        width += whitespace_width + testwidth;
        height = (height > testheight) ? height : testheight;
    }

    if (end == text)
    {
        return text;
    }

    if (box->flags & 0x20)
    {
        gFontGlobals.dpx += maxwidth - width;
    }
    if (box->flags & 0x40)
    {
        gFontGlobals.dpx += 0.5f * (maxwidth - width);
    }

    width = 0.0f;
    s = text;
    while (s != end)
    {
        if (*s == '\n' || *s == '@')
        {
            break;
        }

        whitespace_width = 0.0f;
        while (*s == ' ' || *s == '\t')
        {
            if (font->flags & 4)
            {
                if (*s == ' ')
                {
                    whitespace_width += box->x_spacing;
                }
                if (*s == '\t')
                {
                    whitespace_width += 8.0f * box->x_spacing;
                }
            }
            else
            {
                F32 space = ((font->pspc[' '].dx + 3U) * box->x_spacing) / font->char_width;
                if (*s == ' ')
                {
                    whitespace_width += space;
                }
                if (*s == '\t')
                {
                    whitespace_width += 8.0f * space;
                }
            }
            s++;
        }

        if (width == 0.0f)
        {
            whitespace_width = 0.0f;
        }

        gFontGlobals.draw = 0;
        gFontGlobals.dpx += whitespace_width;
        ProcessWord(box, s, &testwidth, &testheight, 0.0f);
        F32 start = width + whitespace_width;
        gFontGlobals.draw = 1;
        if (testwidth + start > maxwidth)
        {
            if (testwidth > maxwidth)
            {
                ProcessWord(box, s, &testwidth, &testheight, maxwidth - start);
            }
            break;
        }

        s = ProcessWord(box, s, &testwidth, &testheight, 0.0f);
        width += testwidth + whitespace_width;
    }

    return end;
}

// Draws the box's text line by line inside its insets. Returns NULL when all of it fitted, or
// where the text that did not fit starts.
char* xFontBoxRender(xFontBox* box)
{
    char* s;
    char* t;
    char* ret;
    F32 minx;
    F32 miny;
    F32 maxx;
    F32 maxy;

    iFontSaveRenderState();
    if (box->flags & 0x100)
    {
        iFontSetBackdropRenderState(box);
        iFontDrawBackdrop(box);
    }

    if (box->text == NULL)
    {
        return NULL;
    }

    minx = box->x + box->inset_left;
    maxx = (box->x + box->w) - box->inset_right;
    miny = box->y + box->inset_top;
    maxy = (box->y + box->h) - box->inset_bottom;
    if (maxx < 0.0f || maxy < 0.0f || minx > 640.0f || miny > 480.0f)
    {
        return NULL;
    }

    s = box->text;
    gFontGlobals.dpx = minx;
    gFontGlobals.dpy = miny;
    iFontSetCharRenderState(box);

    while (s != NULL)
    {
        if (*s == '\0')
        {
            ret = NULL;
            break;
        }

        t = JustifyLine(box, s, maxx - minx, maxy - gFontGlobals.dpy);
        if (t == s)
        {
            ret = s;
            break;
        }

        gFontGlobals.dpx = minx;
        s = t;
        gFontGlobals.dpy += box->y_spacing;
    }

    iFontRestoreRenderState();
    return ret;
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
