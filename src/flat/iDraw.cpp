#include "iDraw.h"

#include "xMath3.h"

#include <dolphin/gx.h>

// A mask of all ones stops colour and alpha writes; zero turns them back on. Other masks are
// ignored on this platform.
void iDrawSetFBMSK(U32 FBMSK)
{
    if (FBMSK == 0xFFFFFFFF)
    {
        GXSetColorUpdate(GX_FALSE);
        GXSetAlphaUpdate(GX_FALSE);
    }
    else if (FBMSK == 0)
    {
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_TRUE);
    }
}

// Draws count camera-facing quads of one texture, width by height, centred on the points and
// pulled a quarter unit towards the camera. Each quad is two triangles of the vertex list.
void iDrawBillboards(xVec3* pts, U32 count, RwRaster* raster, F32 width, F32 height)
{
    RwIm3DVertex verts[384];
    RwIm3DVertex* v;
    RwMatrix* cam;
    xVec3 right;
    xVec3 up;
    xVec3 at;
    U32 i;

    cam = RwFrameGetMatrix(RwCameraGetFrame(RwCameraGetCurrentCamera()));

    if (!pts || !cam || !raster)
    {
        return;
    }

    v = verts;

    xVec3SMul(&right, (xVec3*)&cam->right, 0.5f * width);
    xVec3SMul(&up, (xVec3*)&cam->up, -0.5f * height);
    xVec3SMul(&at, (xVec3*)&cam->at, -0.25f);

    for (i = 0; i < count; i++)
    {
        v[0].x = pts->x - right.x - up.x;
        v[0].y = pts->y - right.y - up.y;
        v[0].z = pts->z - right.z - up.z;
        *(U32*)&v[0].r = 0x80808080;
        v[0].u = 0.0f;
        v[0].v = 0.0f;
        v[0].x += at.x;
        v[0].y += at.y;
        v[0].z += at.z;

        v[1].x = pts->x + right.x - up.x;
        v[1].y = pts->y + right.y - up.y;
        v[1].z = pts->z + right.z - up.z;
        *(U32*)&v[1].r = 0x80808080;
        v[1].u = 0.999f;
        v[1].v = 0.0f;
        v[1].x += at.x;
        v[1].y += at.y;
        v[1].z += at.z;

        v[2].x = pts->x - right.x + up.x;
        v[2].y = pts->y - right.y + up.y;
        v[2].z = pts->z - right.z + up.z;
        *(U32*)&v[2].r = 0x80808080;
        v[2].u = 0.0f;
        v[2].v = 0.999f;
        v[2].x += at.x;
        v[2].y += at.y;
        v[2].z += at.z;

        v[3].x = pts->x - right.x + up.x;
        v[3].y = pts->y - right.y + up.y;
        v[3].z = pts->z - right.z + up.z;
        *(U32*)&v[3].r = 0x80808080;
        v[3].u = 0.0f;
        v[3].v = 0.999f;
        v[3].x += at.x;
        v[3].y += at.y;
        v[3].z += at.z;

        v[4].x = pts->x + right.x - up.x;
        v[4].y = pts->y + right.y - up.y;
        v[4].z = pts->z + right.z - up.z;
        *(U32*)&v[4].r = 0x80808080;
        v[4].u = 0.999f;
        v[4].v = 0.0f;
        v[4].x += at.x;
        v[4].y += at.y;
        v[4].z += at.z;

        v[5].x = pts->x + right.x + up.x;
        v[5].y = pts->y + right.y + up.y;
        v[5].z = pts->z + right.z + up.z;
        *(U32*)&v[5].r = 0x80808080;
        v[5].u = 0.999f;
        v[5].v = 0.999f;
        v[5].x += at.x;
        v[5].y += at.y;
        v[5].z += at.z;

        pts++;
        v += 6;
    }

    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);

    RwIm3DTransform(verts, count * 6, NULL, rwIM3D_VERTEXUV | rwIM3D_VERTEXXYZ);
    RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
    RwIm3DEnd();

    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
}

// The debug line queue does nothing on this platform.
void iDrawBegin()
{
}

void iQueueLineList(iColor_tag col, RwV3d* verts, U32* indices, RwMatrixTag* mat, U32 nverts,
                    U32 nindices)
{
}

void iDrawEnd()
{
}
