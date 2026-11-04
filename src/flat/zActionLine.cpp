#include "types.h"
#include "xMath3.h"
#include "xString.h"
#include "xstransvc.h"

#include <rwcore.h>

struct _tagActionLine
{
    U32 flags;
    xVec3 pos[4];
    F32 time_left;
};

static _tagActionLine* sActionLine[8];
static RwRaster* sActionLineRaster;

// Clear the line slots and load their texture.
void zActionLineInit()
{
    for (S32 i = 0; i < 8; i++)
    {
        sActionLine[i] = NULL;
    }

    sActionLineRaster = NULL;
    RwTexture* tex = (RwTexture*)xSTFindAsset(xStrHash("ACTIONLINES"), NULL);
    if (tex != NULL)
    {
        sActionLineRaster = tex->raster;
    }
}

// Expire active lines as their remaining time runs out.
void zActionLineUpdate(F32 seconds)
{
    for (S32 i = 0; i < 8; i++)
    {
        _tagActionLine* l = sActionLine[i];
        if (l != NULL && (l->flags & 1))
        {
            l->time_left -= seconds;
            if (l->time_left <= 0.0f)
            {
                l->flags &= ~1;
            }
        }
    }
}

static inline void RenderActionLine(_tagActionLine* l)
{
    static RxObjSpace3DVertex sStripVert[4];
    RxObjSpace3DVertex* vert = sStripVert;
    for (S32 i = 0; i < 4; i++, vert++)
    {
        RwIm3DVertexSetPos(vert, l->pos[i].x, l->pos[i].y, l->pos[i].z);
        RwIm3DVertexSetU(vert, 0.0f);
        RwIm3DVertexSetV(vert, 0.0f);
        RwIm3DVertexSetRGBA(vert, 255, 255, 255, 128);
    }
    RwIm3DVertexSetU(&sStripVert[2], 1.0f);
    RwIm3DVertexSetU(&sStripVert[3], 1.0f);
    RwIm3DVertexSetV(&sStripVert[3], 1.0f);
    if (RwIm3DTransform(sStripVert, 4, NULL, rwIM3D_VERTEXRGBA | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV))
    {
        RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
        RwIm3DEnd();
    }
}

// Draw each active action line as a textured strip.
void zActionLineRender()
{
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, sActionLineRaster);
    for (S32 i = 0; i < 8; i++)
    {
        if (sActionLine[i] != NULL && (sActionLine[i]->flags & 1))
        {
            RenderActionLine(sActionLine[i]);
        }
    }
}
