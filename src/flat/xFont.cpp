#include "xFont.h"
#include "xstransvc.h"
#include "xutil.h"

S32 xMemPushBase();

void iFontSaveRenderState();
void iFontRestoreRenderState();
void iFontSetCharRenderState(xFontBox* box);
void iFontSetBackdropRenderState(xFontBox* box);
void iFontDrawChar(char c, F32 depth, iColor_tag color, xFontBox* box);
void iFontDrawBackdrop(xFontBox* box);

xFontGlobals_t gFontGlobals = { 0.0f, 0.0f, 0, 0.0f };

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

// Reads one escape-code digit: a digit gives its value, anything else is taken as it is and left
// in the text.
static inline char xFontReadDigit(char*& s)
{
    char c = *s;
    if (c < '0' || c > '9')
    {
        return c;
    }
    s++;
    return c - '0';
}

static inline char xFontReadNumber3(char*& s)
{
    char n = 0;
    for (S32 i = 0; i < 3; i++)
    {
        n = n * 10 + (*s++ - '0');
    }
    return n;
}

// Measures, and when gFontGlobals.draw is set draws, one word: the characters up to white space,
// a line break ('\n' or '@') or the end, stopping early once the width would pass maxw (0 means no
// limit). Escape codes inside the text change the font ($B<digit>), the colour ($A<rrr><ggg><bbb>,
// $C for the box's saved colour, $D<digit> for one of the preset colours); a drawn word leaves the
// box with the last font and colour.
static char* ProcessWord(xFontBox* box, char* text, F32* width, F32* height, F32 maxw)
{
    char* s = text;
    U32 fontID = box->font;
    xFontInfo* font = &gFontList[fontID];
    iColor_tag color = box->color;
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
                fontID = xFontReadDigit(s);
                break;
            case 'A':
            {
                iColor_tag rgb;
                rgb.r = xFontReadNumber3(s);
                rgb.g = xFontReadNumber3(s);
                rgb.b = xFontReadNumber3(s);
                rgb.a = 0xFF;
                color = rgb;
                break;
            }
            case 'C':
                color = box->saved_color;
                break;
            case 'D':
                switch (xFontReadDigit(s))
                {
                case 0:
                case '#':
                {
                    iColor_tag green = { 0x0C, 0xB3, 0x5F, 0xFF };
                    color = green;
                    break;
                }
                case 1:
                case '{':
                {
                    iColor_tag red = { 0xCD, 0x07, 0x00, 0xFF };
                    color = red;
                    break;
                }
                case 2:
                {
                    iColor_tag blue = { 0x99, 0xBD, 0xEE, 0xFF };
                    color = blue;
                    break;
                }
                case '|':
                {
                    iColor_tag white = { 0xFF, 0xFF, 0xFF, 0xFF };
                    color = white;
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
            dx = (font->pspc[*s].dx + 3) * (box->x_spacing / font->char_width);
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

// The width of a space or a tab (eight spaces), or 0 for other characters.
static inline F32 xFontWhitespace(xFontBox* box, xFontInfo* font, char c, F32 width)
{
    if (font->flags & 4)
    {
        if (c == ' ')
        {
            width += box->x_spacing;
        }
        if (c == '\t')
        {
            width += 8.0f * box->x_spacing;
        }
    }
    else
    {
        F32 space = ((font->pspc[' '].dx + 3) * box->x_spacing) / font->char_width;
        if (c == ' ')
        {
            width += space;
        }
        if (c == '\t')
        {
            width += 8.0f * space;
        }
    }
    return width;
}

// Fits as many words as the line has room for, then draws them with the box's justification
// (flag 0x20 right, 0x40 centred). Returns where the next line starts; the text itself when not
// even one word fits.
static char* JustifyLine(xFontBox* box, char* text, F32 maxwidth, F32 maxheight)
{
    xFontInfo* font = &gFontList[box->font];
    char* s;
    char* end;
    F32 width;
    F32 height;
    F32 whitespace_width;
    F32 testwidth;
    F32 testheight;

    if (text == NULL)
    {
        return NULL;
    }

    height = 0.0f;
    width = 0.0f;
    gFontGlobals.draw = 0;
    end = text;
    s = text;

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
            whitespace_width = xFontWhitespace(box, font, *s, whitespace_width);
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

        char* t = ProcessWord(box, s, &testwidth, &testheight, 0.0f);
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

        end = t;
        s = end;
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
            whitespace_width = xFontWhitespace(box, font, *s, whitespace_width);
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

    s = box->text;
    if (s == NULL)
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
