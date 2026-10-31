#include "xDraw.h"

#include "iDraw.h"

#include <rwcore.h>

extern "C" S32 RwIm2DRenderLine(RwIm2DVertex* vertices, S32 numVertices, S32 vert1, S32 vert2);

// The colour of the debug lines drawn below; white until set.
static iColor_tag sgDrawColor = { 0xFF, 0xFF, 0xFF, 0xFF };

// The functions are in the reverse of their target order, the order GC/1.3.2 emits them in.

void xDrawBegin()
{
    iDrawBegin();
}

void xDrawEnd()
{
    iDrawEnd();
}

void xDrawSetColor(iColor_tag color)
{
    sgDrawColor = color;
}

void xDrawSetColor(U8 r, U8 g, U8 b, U8 a)
{
    iColor_tag color;

    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;

    xDrawSetColor(color);
}

// A world-space line from a to b in the current colour, queued for iDraw.
void xDrawLine(const xVec3* a, const xVec3* b)
{
    RwV3d verts[2];
    U32 indices[2];

    indices[0] = 0;
    indices[1] = 1;

    verts[0].x = a->x;
    verts[0].y = a->y;
    verts[0].z = a->z;
    verts[1].x = b->x;
    verts[1].y = b->y;
    verts[1].z = b->z;

    iQueueLineList(sgDrawColor, verts, indices, NULL, 2, 2);
}

// GC only: a screen-space line from (x1, y1) to (x2, y2) in the current colour, at the near
// screen depth.
extern "C" void fn_80021CB0(F32 x1, F32 y1, F32 x2, F32 y2)
{
    RwIm2DVertex verts[2];
    F32 z = RwIm2DGetNearScreenZ();

    RwIm2DVertexSetScreenX(&verts[0], x1);
    RwIm2DVertexSetScreenY(&verts[0], y1);
    RwIm2DVertexSetScreenZ(&verts[0], z);
    RwIm2DVertexSetIntRGBA(&verts[0], sgDrawColor.r, sgDrawColor.g, sgDrawColor.b, sgDrawColor.a);

    RwIm2DVertexSetScreenX(&verts[1], x2);
    RwIm2DVertexSetScreenY(&verts[1], y2);
    RwIm2DVertexSetScreenZ(&verts[1], z);
    RwIm2DVertexSetIntRGBA(&verts[1], sgDrawColor.r, sgDrawColor.g, sgDrawColor.b, sgDrawColor.a);

    RwIm2DRenderLine(verts, 2, 0, 1);
}
