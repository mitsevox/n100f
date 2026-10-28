#include "zLightning.h"

#include "iMath.h"
#include "xMath3.h"
#include "xMemMgr.h"
#include "xString.h"
#include "xstransvc.h"
#include "zParEmitter.h"
#include "zScene.h"

#include "rwcore.h"

#include <stdio.h>

static const U8 cSparkColors[24] = {
    0xFF, 0x00, 0x00, 0xFF, 0x80, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFF,
    0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x80, 0x80, 0x00, 0xFF,
};

static _tagLightning* sLightning[48];

void zLightningInit()
{
    S32 i;

    for (i = 0; i < 48; i++)
    {
        sLightning[i] = NULL;
    }
}

static inline _tagLightning* FindFreeLightning()
{
    S32 i;

    for (i = 0; i < 48; i++)
    {
        if (sLightning[i])
        {
            if (!(sLightning[i]->flags & 0x1))
            {
                return sLightning[i];
            }
        }
        else
        {
            sLightning[i] = (_tagLightning*)xMemAlloc(gActiveHeap, sizeof(_tagLightning));
            return sLightning[i];
        }
    }

    return NULL;
}

_tagLightning* zLightningAdd(_tagLightningAdd* add)
{
    F32 currot;
    S32 i;
    F32 pos;
    F32 scalar;
    _tagLightning* l;

    l = FindFreeLightning();

    l->type = add->type;
    l->flags = add->flags | 0x41;
    l->total_points = add->total_points;
    l->arc_height = add->arc_height;
    l->thickness = 0.5f * add->thickness;
    l->rand_radius = add->rand_radius;
    l->r = add->r;
    l->g = add->g;
    l->b = add->b;

    currot = 0.0f;

    switch (l->type)
    {
    case eLT_Line:
        break;
    case eLT_Rotating:
        l->rot.degrees = add->move_degrees;
        l->rot.height = add->rot_radius;
        break;
    }

    for (i = 0; i < l->total_points; i++)
    {
        pos = i / (l->total_points - 1.0f);

        xVec3Lerp(&l->point[i], add->start, add->end, pos);

        scalar = 4.0f * pos + -4.0f * (pos * pos);
        if (scalar < 0.0f)
        {
            scalar = 0.0f;
        }

        switch (l->type)
        {
        case eLT_Line:
            break;
        case eLT_Rotating:
            l->rot.deg[i] = currot;
            currot += add->setup_degrees;
            break;
        }

        if (l->flags & 0x20)
        {
            l->point[i].y += scalar * l->arc_height;
        }

        l->base_point[i] = l->point[i];
    }

    if (l->flags & 0x10)
    {
        l->time_left = l->time_total = 0.1f;
    }

    return l;
}

static void UpdateLightning(_tagLightning* l, F32 seconds)
{
    if (!(l->flags & 0x10))
    {
        l->time_left -= seconds;
    }

    if (l->time_left <= 0.0f)
    {
        l->flags &= ~0x1;
    }

    if (l->type == eLT_Line)
    {
        F32 full;
        S32 i;
        F32 sc1;

        full = l->rand_radius * seconds;

        for (i = 1; i < l->total_points - 1; i++)
        {
            l->point[i].x = l->base_point[i].x + (full * iurand() + -(0.5f * full));
            l->point[i].y = l->base_point[i].y + (full * iurand() + -(0.5f * full));
            l->point[i].z = l->base_point[i].z + (full * iurand() + -(0.5f * full));

            if (l->flags & 0x20)
            {
                sc1 = (F32)i / (F32)l->total_points;
                sc1 = 4.0f * sc1 + -4.0f * (sc1 * sc1);
                l->point[i].y += sc1 * l->arc_height;
            }
        }
    }
    else if (l->type == eLT_Rotating)
    {
        xVec3 dir;
        xMat3x3 dirmat;
        xVec3 dir_euler;
        F32 full;
        S32 i;

        xVec3Sub(&dir, &l->base_point[l->total_points - 1], &l->base_point[0]);
        xVec3Normalize(&dir, &dir);
        xMat3x3LookVec(&dirmat, &dir);
        xMat3x3GetEuler(&dirmat, &dir_euler);

        full = l->rand_radius * seconds;

        for (i = 1; i < l->total_points - 1; i++)
        {
            xMat3x3 mat3;
            F32 sc2;
            F32 sc1;

            xMat3x3Euler(&mat3, dir_euler.x, 0.0f, 3.1415927f * l->rot.deg[i] / 180.0f);

            xVec3 vec = { 0.0f, 1.0f, 0.0f };

            sc1 = 1.0f;
            if (l->flags & 0x8 | 0x20)
            {
                sc2 = (F32)i / (F32)l->total_points;
                sc1 = 4.0f * sc2 + -4.0f * (sc2 * sc2);
                if (l->flags & 0x8)
                {
                    vec.y = l->rot.height * sc1;
                }
            }

            xMat3x3LMulVec(&vec, &mat3, &vec);

            l->rot.deg[i] += l->rot.degrees * seconds;

            l->point[i].x = l->base_point[i].x + (full * iurand() + -(0.5f * full));
            l->point[i].y = l->base_point[i].y + (full * iurand() + -(0.5f * full));
            l->point[i].z = l->base_point[i].z + (full * iurand() + -(0.5f * full));

            l->point[i].x += vec.x;
            l->point[i].y += vec.y;
            l->point[i].z += vec.z;

            if (l->flags & 0x20)
            {
                l->point[i].y += sc1 * l->arc_height;
            }
        }
    }

    if ((l->flags & 0x2) && (irand() & 0x3))
    {
        _zParEmitter* e;
        _zParEmitterCustomSettings info;
        S32 i;
        S32 col;

        e = (_zParEmitter*)zSceneFindObject(xStrHash("PAREMIT_EG_SPARK"));
        if (e)
        {
            info.custom_flags = 0x1A0;

            i = irand() % l->total_points;
            info.pos = l->point[i];

            col = irand();
            col = col % 6 * 4;
            info.color_death[0] = info.color_birth[0] = cSparkColors[col++];
            info.color_death[1] = info.color_birth[1] = cSparkColors[col++];
            info.color_death[2] = info.color_birth[2] = cSparkColors[col++];
            info.color_birth[3] = cSparkColors[col];
            info.color_death[3] = 0x80;

            zParEmitterEmitCustom(e, seconds, &info);
        }
    }
}

void zLightningUpdate(F32 seconds)
{
    S32 i;

    for (i = 0; i < 48; i++)
    {
        if (sLightning[i] && (sLightning[i]->flags & 0x1))
        {
            UpdateLightning(sLightning[i], seconds);
        }
    }
}

static void zLightning_UnknownTestText()
{
    printf("X to test lightning\n");
    printf("               ");
}

static void RenderLightning(_tagLightning* l)
{
    static RxObjSpace3DVertex sStripVert[128];
    F32 alphaf;
    U8 alphau8;
    U32 verts;

    alphaf = l->time_left / l->time_total;
    alphaf *= 255.0f;
    alphau8 = alphaf;

    RwIm3DVertexSetPos(&sStripVert[0], l->point[0].x, l->point[0].y + l->thickness,
                       l->point[0].z);
    RwIm3DVertexSetU(&sStripVert[0], 0.0f);
    RwIm3DVertexSetV(&sStripVert[0], 0.0f);
    RwIm3DVertexSetRGBA(&sStripVert[0], l->r, l->g, l->b, alphau8);
    RwIm3DVertexSetPos(&sStripVert[1], l->point[0].x, l->point[0].y - l->thickness,
                       l->point[0].z);
    RwIm3DVertexSetU(&sStripVert[1], 0.0f);
    RwIm3DVertexSetV(&sStripVert[1], 1.0f);
    RwIm3DVertexSetRGBA(&sStripVert[1], l->r, l->g, l->b, alphau8);

    verts = 2;
    for (S32 i = 1; i < l->total_points; i++)
    {
        RwIm3DVertexSetPos(&sStripVert[verts], l->point[i].x, l->point[i].y + l->thickness,
                           l->point[i].z);
        if (i & 1)
        {
            RwIm3DVertexSetU(&sStripVert[verts], 1.0f);
        }
        else
        {
            RwIm3DVertexSetU(&sStripVert[verts], 0.0f);
        }
        RwIm3DVertexSetV(&sStripVert[verts], 0.0f);
        RwIm3DVertexSetRGBA(&sStripVert[verts], l->r, l->g, l->b, alphau8);
        verts++;

        RwIm3DVertexSetPos(&sStripVert[verts], l->point[i].x, l->point[i].y - l->thickness,
                           l->point[i].z);
        if (i & 1)
        {
            RwIm3DVertexSetU(&sStripVert[verts], 1.0f);
        }
        else
        {
            RwIm3DVertexSetU(&sStripVert[verts], 0.0f);
        }
        RwIm3DVertexSetV(&sStripVert[verts], 1.0f);
        RwIm3DVertexSetRGBA(&sStripVert[verts], l->r, l->g, l->b, alphau8);
        verts++;

        if (verts >= 128)
        {
            verts = 128;
            break;
        }
    }

    if (RwIm3DTransform(sStripVert, verts, NULL,
                        rwIM3D_VERTEXRGBA | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV))
    {
        RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
        RwIm3DEnd();
    }

    alphaf = l->time_left / l->time_total;
    alphaf *= 128.0f;
    alphau8 = alphaf;

    F32 oldthickness = l->thickness;
    l->thickness *= 3.0f + iurand();

    RwIm3DVertexSetPos(&sStripVert[0], l->point[0].x, l->point[0].y + l->thickness,
                       l->point[0].z);
    RwIm3DVertexSetU(&sStripVert[0], 0.0f);
    RwIm3DVertexSetV(&sStripVert[0], 0.0f);
    RwIm3DVertexSetRGBA(&sStripVert[0], l->r, l->g, l->b, alphau8);
    RwIm3DVertexSetPos(&sStripVert[1], l->point[0].x, l->point[0].y - l->thickness,
                       l->point[0].z);
    RwIm3DVertexSetU(&sStripVert[1], 0.0f);
    RwIm3DVertexSetV(&sStripVert[1], 1.0f);
    RwIm3DVertexSetRGBA(&sStripVert[1], l->r, l->g, l->b, alphau8);

    verts = 2;
    for (S32 i = 1; i < l->total_points; i++)
    {
        RwIm3DVertexSetPos(&sStripVert[verts], l->point[i].x, l->point[i].y + l->thickness,
                           l->point[i].z);
        if (i & 1)
        {
            RwIm3DVertexSetU(&sStripVert[verts], 1.0f);
        }
        else
        {
            RwIm3DVertexSetU(&sStripVert[verts], 0.0f);
        }
        RwIm3DVertexSetV(&sStripVert[verts], 0.0f);
        RwIm3DVertexSetRGBA(&sStripVert[verts], l->r, l->g, l->b, alphau8);
        verts++;

        RwIm3DVertexSetPos(&sStripVert[verts], l->point[i].x, l->point[i].y - l->thickness,
                           l->point[i].z);
        if (i & 1)
        {
            RwIm3DVertexSetU(&sStripVert[verts], 1.0f);
        }
        else
        {
            RwIm3DVertexSetU(&sStripVert[verts], 0.0f);
        }
        RwIm3DVertexSetV(&sStripVert[verts], 1.0f);
        RwIm3DVertexSetRGBA(&sStripVert[verts], l->r, l->g, l->b, alphau8);
        verts++;

        if (verts >= 128)
        {
            verts = 128;
            break;
        }
    }

    if (RwIm3DTransform(sStripVert, verts, NULL,
                        rwIM3D_VERTEXRGBA | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV))
    {
        RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
        RwIm3DEnd();
    }

    l->thickness = oldthickness;
}

void zLightningRender()
{
    RwTexture* tex;
    S32 i;

    tex = (RwTexture*)xSTFindAsset(xStrHash("LIGHTNING"), NULL);
    if (tex)
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, tex->raster);
    }

    for (i = 0; i < 48; i++)
    {
        if (sLightning[i] && (sLightning[i]->flags & 0x41) == 0x41)
        {
            RenderLightning(sLightning[i]);
        }
    }
}

void zLightningKill(_tagLightning* l)
{
    l->flags &= ~0x1;
}

void zLightningModifyEndpoints(_tagLightning* l, xVec3* start, xVec3* end)
{
    S32 i;
    F32 pos;
    F32 scalar;

    for (i = 0; i < l->total_points; i++)
    {
        pos = i / (l->total_points - 1.0f);

        xVec3Lerp(&l->base_point[i], start, end, pos);

        scalar = 4.0f * pos + -4.0f * (pos * pos);
        if (scalar < 0.0f)
        {
            scalar = 0.0f;
        }

        if (l->flags & 0x20)
        {
            l->base_point[i].y += scalar * l->arc_height;
        }
    }

    l->point[l->total_points - 1] = l->base_point[l->total_points - 1];
}
