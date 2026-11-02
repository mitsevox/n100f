#include "xFont.h"
#include "xString.h"
#include "xstransvc.h"
#include "rwcore.h"

#include <math.h>
#include <string.h>

extern xFontInfo gFontList[4];

U32 iFontInitFont(RwTexture* texture, const char* characters, U32 layout, U16 u, U16 v,
                  U16 char_width, U16 char_height, U16 spacing);

U32 gDBfont;
static RwRaster* sButtonRaster;

void iFontInit();
U32 iFontInitFont(RwTexture* texture, const char* characters, U32 layout, U16 u, U16 v,
                  U16 char_width, U16 char_height, U16 flags);
void iFontDrawChar(char c, F32 depth, iColor_tag color, xFontBox* box);
extern "C" void fn_8000B538(S32 c, F32 x, F32 y, F32 w, F32 h);
void iFontDrawBackdrop(xFontBox* box);
void iFontRestoreRenderState();
void iFontSetCharRenderState(xFontBox* box);
void iFontSetBackdropRenderState(xFontBox*);
void iFontSaveRenderState();

static RwIm2DVertex ourGlobals[120];

static RxRenderStateVector rsv;

static RwIm2DVertex lbl_801E1428[4];

static RwIm2DVertex* im2d_vert;

static S32 im2d_vert_ct;

static F32 rcz;

static F32 nsz;

static RwRaster* sRaster;

extern "C" {
// How far characters bob while they wiggle (0 for no wiggle), and the wiggle clock.
F32 lbl_8025E8C0;
F32 gFontWiggleTime;
}

extern "C" S32 fn_800CD170();

extern "C" {
RwImage* RwImageCreate(S32 width, S32 height, S32 depth);
RwImage* RwImageAllocatePixels(RwImage* image);
RwImage* RwImageSetFromRaster(RwImage* image, RwRaster* raster);
}

extern "C" void fn_8000B538(S32 c, F32 x, F32 y, F32 w, F32 h);

// GameCube only: the texture cell of each controller-button glyph, 'T' to 'W', as (u, v).
static F32 lbl_801AD420[4][2] = {
    { 0.5f, 0.0f },
    { 0.5f, 0.5f },
    { 0.0f, 0.0f },
    { 0.0f, 0.5f },
};

// Saves the render states the font code changes and the camera depth values its quads use.
void iFontSaveRenderState()
{
    RxRenderStateVectorLoadDriverState(&rsv);
    rcz = 1.0f / RwCameraGetCurrentCamera()->nearPlane;
    nsz = RwIm2DGetNearScreenZ();
}

void iFontSetBackdropRenderState(xFontBox*)
{
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
}

void iFontSetCharRenderState(xFontBox* box)
{
    xFontInfo* font = &gFontList[box->font];

    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, font->ps.raster);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
    sRaster = font->ps.raster;
    im2d_vert = ourGlobals;
}

// Character quads are batched in ourGlobals between iFontSetCharRenderState and
// iFontRestoreRenderState, which draws whatever is left before restoring the saved states.
void iFontRestoreRenderState()
{
    if (im2d_vert_ct > 0)
    {
        RwIm2DRenderPrimitive(rwPRIMTYPETRILIST, im2d_vert, im2d_vert_ct);
        im2d_vert_ct = 0;
    }

    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)((rsv.Flags >> 4) & 1));
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rsv.SrcBlend);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rsv.DestBlend);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)((rsv.Flags >> 3) & 1));
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, rsv.TextureRaster);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rsv.ShadeMode);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)((rsv.Flags >> 2) & 1));
    im2d_vert = NULL;
}

// Fills the box with its backdrop colour.
void iFontDrawBackdrop(xFontBox* box)
{
    RwIm2DVertex vert[4];

    vert[0].x = box->x;
    vert[0].y = box->y + box->h;
    vert[0].z = nsz;
    vert[0].emissiveColor.red = box->backdrop_color.r;
    vert[0].emissiveColor.green = box->backdrop_color.g;
    vert[0].emissiveColor.blue = box->backdrop_color.b;
    vert[0].emissiveColor.alpha = box->backdrop_color.a;
    vert[1].x = box->x + box->w;
    vert[1].y = box->y + box->h;
    vert[1].z = nsz;
    vert[1].emissiveColor.red = box->backdrop_color.r;
    vert[1].emissiveColor.green = box->backdrop_color.g;
    vert[1].emissiveColor.blue = box->backdrop_color.b;
    vert[1].emissiveColor.alpha = box->backdrop_color.a;
    vert[2].x = box->x;
    vert[2].y = box->y;
    vert[2].z = nsz;
    vert[2].emissiveColor.red = box->backdrop_color.r;
    vert[2].emissiveColor.green = box->backdrop_color.g;
    vert[2].emissiveColor.blue = box->backdrop_color.b;
    vert[2].emissiveColor.alpha = box->backdrop_color.a;
    vert[3].x = box->x + box->w;
    vert[3].y = box->y;
    vert[3].z = nsz;
    vert[3].emissiveColor.red = box->backdrop_color.r;
    vert[3].emissiveColor.green = box->backdrop_color.g;
    vert[3].emissiveColor.blue = box->backdrop_color.b;
    vert[3].emissiveColor.alpha = box->backdrop_color.a;

    RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vert, 4);
}

// GameCube only: draws a controller-button glyph ('T' to 'W') from the buttons texture as a
// square of side h, centred in the w-wide cell at x. 'U' and 'V' trade places while
// fn_800CD170 says so.
extern "C" void fn_8000B538(S32 c, F32 x, F32 y, F32 w, F32 h)
{
    F32 half = 0.5f * h;
    F32 left = (0.5f * w + x) - half;
    F32 top = (y + half) - half;

    if (fn_800CD170())
    {
        if (c == 'U')
        {
            c = 'V';
        }
        else if (c == 'V')
        {
            c = 'U';
        }
    }

    F32 bu = lbl_801AD420[c - 'T'][0];
    F32 bv = lbl_801AD420[c - 'T'][1];
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, sButtonRaster);

    RwIm2DVertex* vert = lbl_801E1428;
    vert[0].x = left;
    vert[0].y = top + h;
    vert[0].z = nsz;
    vert[0].u = bu;
    vert[0].v = bv + 0.499999911f;
    RwIm2DVertexSetIntRGBA(&vert[0], 0xFF, 0xFF, 0xFF, 0xFF);
    vert[1].x = left + h;
    vert[1].y = top + h;
    vert[1].z = nsz;
    vert[1].u = bu + 0.499999911f;
    vert[1].v = bv + 0.499999911f;
    RwIm2DVertexSetIntRGBA(&vert[1], 0xFF, 0xFF, 0xFF, 0xFF);
    vert[2].x = left;
    vert[2].y = top;
    vert[2].z = nsz;
    vert[2].u = bu;
    vert[2].v = bv;
    RwIm2DVertexSetIntRGBA(&vert[2], 0xFF, 0xFF, 0xFF, 0xFF);
    vert[3].x = left + h;
    vert[3].y = top;
    vert[3].z = nsz;
    vert[3].u = bu + 0.499999911f;
    vert[3].v = bv;
    RwIm2DVertexSetIntRGBA(&vert[3], 0xFF, 0xFF, 0xFF, 0xFF);

    RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, lbl_801E1428, 4);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, sRaster);
}

// Queues one character as two triangles at the pen position, clipped to the 640x480 screen. '^'
// draws the '~' cell. In font 0 the cells of '~', '{', '}' and '#' are controller buttons, drawn
// from the buttons texture with a gentle pulse instead. While the wiggle amount is set the
// characters bob up and down. depth is not used on this platform.
void iFontDrawChar(char c, F32 depth, iColor_tag color, xFontBox* box)
{
    xFontInfo* font = &gFontList[box->font];
    U32 draw_bg;
    U32 i;

    if ((font->flags & 2) && c >= 'a' && c <= 'z')
    {
        c ^= 0x20;
    }

    if (c == '^')
    {
        c = '~';
        draw_bg = 0;
        i = font->lookup['~'] & 0x7F;
    }
    else
    {
        i = font->lookup[c];
        if (i == 0xFF)
        {
            return;
        }
        draw_bg = i & 0x80;
        i &= 0x7F;
        if (draw_bg && box->font == 0)
        {
            i += 'T';
        }
    }

    F32 x = gFontGlobals.dpx;
    F32 y = gFontGlobals.dpy;
    F32 du = (font->char_width - 0.5f) * font->oo_tex_width;
    F32 u = font->oo_tex_width * (font->u + (F32)(font->char_width * (i % font->layout)));
    F32 dx = box->draw_width;
    F32 dy = box->draw_height;
    F32 v = font->oo_tex_height * (font->v + (F32)(font->char_height * (i / font->layout)));
    F32 dv = (font->char_height - 0.5f) * font->oo_tex_height;

    if (!(font->flags & 4))
    {
        dx *= (F32)font->pspc[c].dx / (F32)font->char_width;
        u += (F32)font->pspc[c].ox * font->oo_tex_width;
        du = (F32)font->pspc[c].dx * font->oo_tex_width;
    }

    if (0.0f != lbl_8025E8C0)
    {
        y += lbl_8025E8C0 * (F32)cos(10.0f * gFontWiggleTime + (x + y) / 10.0f);
    }

    F32 sx = font->oo_tex_width * (font->oo_tex_width * box->draw_width);
    F32 sy = font->oo_tex_height * (font->oo_tex_height * box->draw_height);

    if (x < 0.0f)
    {
        if (x + dx < 0.0f)
        {
            return;
        }
        F32 n = -x;
        x = 0.0f;
        dx -= n;
        u += n * sx;
        du -= n * sx;
    }
    else if (x + dx >= 640.0f)
    {
        if (x > 640.0f)
        {
            return;
        }
        F32 n = (x + dx) - 640.0f;
        dx -= n;
        du -= n * sx;
    }

    if (y < 0.0f)
    {
        if (y + dy < 0.0f)
        {
            return;
        }
        F32 n = -y;
        y = 0.0f;
        dy -= n;
        v += n * sy;
        dv -= n * sy;
    }
    else if (y + dy >= 480.0f)
    {
        if (y > 480.0f)
        {
            return;
        }
        F32 n = (y + dy) - 480.0f;
        dy -= n;
        dv -= n * sy;
    }

    if (draw_bg)
    {
        F32 _x = x - 0.24f * dx;
        F32 _y = y - 0.12f * dy;
        F32 _dx = 0.48f * dx + dx;
        F32 _dy = 0.32f * dy + dy;
        F32 wave = 1.0f + (F32)sin(5.0f * gFontWiggleTime);
        F32 pulse_w = 0.025f * _dx * wave;
        F32 pulse_h = 0.025f * _dy * wave;
        fn_8000B538(i, _x - pulse_w, _y - pulse_h, 2.0f * pulse_w + _dx, 2.0f * pulse_h + _dy);
        return;
    }

    if ((U32)(120 - im2d_vert_ct) < 6)
    {
        RwIm2DRenderPrimitive(rwPRIMTYPETRILIST, im2d_vert, im2d_vert_ct);
        im2d_vert_ct = 0;
    }

    RwIm2DVertex* vert = &im2d_vert[im2d_vert_ct];
    vert[0].x = x;
    vert[0].y = y + dy;
    vert[0].z = nsz;
    vert[0].u = u;
    vert[0].v = v + dv;
    RwIm2DVertexSetIntRGBA(&vert[0], color.r, color.g, color.b, color.a);
    vert[1].x = x + dx;
    vert[1].y = y + dy;
    vert[1].z = nsz;
    vert[1].u = u + du;
    vert[1].v = v + dv;
    RwIm2DVertexSetIntRGBA(&vert[1], color.r, color.g, color.b, color.a);
    vert[2].x = x;
    vert[2].y = y;
    vert[2].z = nsz;
    vert[2].u = u;
    vert[2].v = v;
    RwIm2DVertexSetIntRGBA(&vert[2], color.r, color.g, color.b, color.a);
    vert[3] = vert[2];
    vert[4] = vert[1];
    vert[5].x = x + dx;
    vert[5].y = y;
    vert[5].z = nsz;
    vert[5].u = u + du;
    vert[5].v = v;
    RwIm2DVertexSetIntRGBA(&vert[5], color.r, color.g, color.b, color.a);
    im2d_vert_ct += 6;
}

// Takes the next free font slot. lookup maps a character to its cell, in reading order, layout
// cells to a row; the cells of '{', '}', '~' and '#' are flagged 0x80. Unless flags has 0x4
// (fixed width) each character's visible columns are measured from the texture's alpha so text
// can be drawn proportionally; with 0x2 the lower-case letters take the upper-case spacing.
U32 iFontInitFont(RwTexture* texture, const char* characters, U32 layout, U16 u, U16 v,
                  U16 char_width, U16 char_height, U16 flags)
{
    xFontInfo* font = gFontList;
    U32 id = 0;
    U32 tex_width = texture->raster->width;
    U32 tex_height = texture->raster->height;
    const char* s;
    char t;
    RwImage* image;
    U8 c;

    for (; id < 4; id++, font++)
    {
        if (font->flags == 0)
        {
            break;
        }
    }

    font->ps.texture = texture;
    RwTextureSetFilterMode(font->ps.texture, rwFILTERLINEAR);
    font->ps.raster = font->ps.texture->raster;

    memset(font->lookup, 0xFF, sizeof(font->lookup));
    for (s = characters, t = 0; *s != '\0'; s++, t++)
    {
        if ((U8)*s < 0x80)
        {
            U8 val = t;
            if (*s == '{' || *s == '}' || *s == '~' || *s == '#')
            {
                val |= 0x80;
            }
            font->lookup[*s] = val;
        }
    }

    for (c = 0; c < 128; c++)
    {
        font->pspc[c].ox = 0;
        font->pspc[c].dx = char_width;
    }

    image = RwImageCreate(256, 256, 32);
    if (image != NULL)
    {
        RwImageAllocatePixels(image);
        RwImageSetFromRaster(image, font->ps.raster);

        for (s = characters; *s != '\0'; s++)
        {
            U8 minu = 0xFF;
            U8 maxu = 0;
            U8 val = font->lookup[(U8)*s];
            U32 ou = u + char_width * ((val & 0x7F) % layout);
            U32 ov = v + char_height * ((val & 0x7F) / layout);

            for (U8 vv = 0; vv < char_height; vv++)
            {
                for (U8 uu = 0; uu < char_width; uu++)
                {
                    U8* imp = &image->cpPixels[((ou + uu) + ((ov + vv) << 8)) * 4];
                    if (imp[3] != 0)
                    {
                        minu = (minu < uu) ? minu : uu;
                        maxu = (maxu > uu) ? maxu : uu;
                    }
                }
            }

            font->pspc[(U8)*s].ox = minu;
            font->pspc[(U8)*s].dx = maxu + 1 - minu;
            if ((flags & 2) && (U8)*s >= 'A' && (U8)*s <= 'Z')
            {
                font->pspc[(U8)*s | 0x20].ox = minu;
                font->pspc[(U8)*s | 0x20].dx = maxu + 1 - minu;
            }
        }

        RwImageDestroy(image);
    }

    font->pspc[' '].ox = 0;
    font->pspc[' '].dx = char_width >> 1;
    font->pspc['>'].ox = 0;
    font->pspc['<'].ox = 0;
    font->pspc['>'].dx = 18;
    font->pspc['<'].dx = 18;
    font->oo_tex_width = 1.0f / tex_width;
    font->oo_tex_height = 1.0f / tex_height;
    font->char_width = char_width;
    font->char_height = char_height;
    font->layout = layout;
    font->characters = (char*)characters;
    font->flags |= 1;
    font->flags |= flags;
    font->u = u;
    font->v = v;

    return id;
}

// All three fonts are cells of the one "font" texture; the third, the small debug font, is
// kept in gDBfont.
void iFontInit()
{
    RwTexture* tex = (RwTexture*)xSTFindAsset(xStrHash("font"), NULL);

    iFontInitFont(tex, "~{}#ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!.,-:_\"'&()<>/%", 14, 0, 0x84,
                  18, 22, 2);
    iFontInitFont(tex,
                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?!.,;:+-=/&()%"
                  "\"'_<>*[]~{}#",
                  14, 0, 0, 18, 22, 0);
    gDBfont = iFontInitFont(tex, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ,./*+-=:;%<>[]|()_", 18,
                            0x94, 0xE8, 6, 8, 6);

    sButtonRaster = *(RwRaster**)xSTFindAsset(xStrHash("buttons_all"), NULL);
}
