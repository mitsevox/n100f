#include "iParMgr.h"

#include "xMath3.h"
#include "xPar.h"
#include "xParGroup.h"
#include "xstransvc.h"
#include "zGlobals.h"
#include "zParCmd.h"
#include "zParSys.h"

#include <rwcore.h>
#include <string.h>

// Particle rendering. Each particle becomes a camera-facing quad, a streak along its velocity, or
// a quad laid flat or rotated in the world. The quads queue in one shared batch (gRenderBuffer)
// that is drawn as an indexed triangle list whenever it fills, and at the end of each system.

tagiRenderArrays gRenderArr;
tagiRenderInput gRenderBuffer;

void iRenderInit()
{
    gRenderBuffer.m_mode = 0;
    gRenderBuffer.m_indexCount = 0;
    gRenderBuffer.m_vertexCount = 0;
    gRenderBuffer.m_vertexTypeSize = sizeof(RxObjSpace3DVertex);
    gRenderBuffer.m_index = &gRenderArr.m_index[0];
    gRenderBuffer.m_vertex = &gRenderArr.m_vertex[0];
    gRenderBuffer.m_vertexTZ = gRenderArr.m_vertexTZ;
}

// Takes the view matrix (the current camera's frame by default) and keeps its right and up
// vectors negated, the directions camera-facing quads are built along.
void iRenderSetCameraViewMatrix(xMat4x3* m)
{
    if (m == NULL && globals.camera.lo_cam != NULL)
    {
        gRenderBuffer.m_camViewMatrix =
            *(xMat4x3*)RwFrameGetMatrix(RwCameraGetFrame(RwCameraGetCurrentCamera()));
    }
    else
    {
        gRenderBuffer.m_camViewMatrix = *m;
    }

    xVec3Inv((xVec3*)&gRenderBuffer.m_camViewR, &gRenderBuffer.m_camViewMatrix.right);
    xVec3Inv((xVec3*)&gRenderBuffer.m_camViewU, &gRenderBuffer.m_camViewMatrix.up);
}

void iRenderTrianglesImmediate(S32 vertType, S32 vertTypeSize, void* data, S32 dataSize, U16* index,
                               S32 indexSize)
{
    if (RwIm3DTransform((RwIm3DVertex*)data, dataSize, NULL, rwIM3D_VERTEXUV) != NULL)
    {
        if (indexSize != 0)
        {
            RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, index, indexSize);
        }
        else
        {
            RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
        }

        RwIm3DEnd();
    }
}

// Draws whatever the batch holds and empties it.
void iRenderFlush()
{
    if (gRenderBuffer.m_vertexCount > 0)
    {
        iRenderTrianglesImmediate(gRenderBuffer.m_vertexType, gRenderBuffer.m_vertexTypeSize,
                                  gRenderBuffer.m_vertex, gRenderBuffer.m_vertexCount,
                                  gRenderBuffer.m_index, gRenderBuffer.m_indexCount);
    }

    gRenderBuffer.m_indexCount = 0;
    gRenderBuffer.m_vertexCount = 0;
}

static void iRenderPushFlat(xPar* p, xParCmdTex* tex);
void iRenderPushQuadStreak(xPar* p, xParCmdTex* tex);

// Quads lying flat on the ground, each turned by its own yaw.
void iParMgrRenderParSys_Flat(void* data, xParGroup* ps)
{
    xPar* idx = ps->m_root;
    RwTexture* texture;
    RwRaster* raster;

    iRenderSetCameraViewMatrix(NULL);
    texture = (RwTexture*)xSTFindAsset(((zParSys*)data)->tasset->textureID, NULL);

    if (texture)
    {
        raster = texture->raster;

        if (raster)
        {
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
        }
    }

    for (; idx != NULL; idx = idx->m_next)
    {
        iRenderPushFlat(idx, ps->m_cmdTex);
    }

    iRenderFlush();
}

// Quads turned freely in the world by each particle's three rotation angles.
void iParMgrRenderParSys_Ground(void* data, xParGroup* ps)
{
    static RxObjSpace3DVertex v3d[4];
    static U16 i3d[6] = { 0, 1, 2, 3, 0, 1 };

    xPar* idx = ps->m_root;
    RwTexture* texture;
    RwRaster* raster;
    xParCmdTex* tex;
    void* vertices;
    F32 size;
    U8 r;
    U8 g;
    U8 b;
    U8 a;
    xMat3x3 groundmat;
    F32 angx;
    F32 angy;
    F32 angz;
    F32 px;
    F32 py;
    F32 pz;
    F32 rx;
    F32 ry;
    F32 rz;
    F32 ax;
    F32 ay;
    F32 az;
    F32 u1;
    F32 u2;
    F32 v1;
    F32 v2;

    iRenderSetCameraViewMatrix(NULL);
    texture = (RwTexture*)xSTFindAsset(((zParSys*)data)->tasset->textureID, NULL);

    if (texture)
    {
        raster = texture->raster;

        if (raster)
        {
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
        }
    }

    for (; idx != NULL; idx = idx->m_next)
    {
        tex = ps->m_cmdTex;

        if (gRenderBuffer.m_indexCount + 6 > 960 || gRenderBuffer.m_vertexCount + 4 > 480)
        {
            iRenderFlush();
        }

        angx = 0.0f;
        angy = 0.0f;
        angz = 0.0f;
        r = idx->m_c[0];
        size = 0.5f * idx->m_size;
        g = idx->m_c[1];
        b = idx->m_c[2];
        a = idx->m_c[3];

        // The rotation angles are bytes, 255 to a full turn.
        if (idx->m_rotdeg[0])
        {
            angx = 6.2831855f * (idx->m_rotdeg[0] / 255.0f);
        }

        if (idx->m_rotdeg[1])
        {
            angy = 6.2831855f * (idx->m_rotdeg[1] / 255.0f);
        }

        if (idx->m_rotdeg[2])
        {
            angz = 6.2831855f * (idx->m_rotdeg[2] / 255.0f);
        }

        xMat3x3Euler(&groundmat, angx, angy, angz);
        rx = groundmat.right.x * size;
        ry = groundmat.right.y * size;
        rz = groundmat.right.z * size;
        ax = groundmat.at.x * size;
        ay = groundmat.at.y * size;
        az = groundmat.at.z * size;
        px = idx->m_pos.x;
        py = idx->m_pos.y;
        pz = idx->m_pos.z;

        RwIm3DVertexSetRGBA(&v3d[0], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[1], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[2], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[3], r, g, b, a);

        RwIm3DVertexSetPos(&v3d[0], px - rx - ax, py - ry - ay, pz - rz - az);
        RwIm3DVertexSetPos(&v3d[1], px + rx + ax, py + ry + ay, pz + rz + az);
        RwIm3DVertexSetPos(&v3d[2], px + rx - ax, py + ry - ay, pz + rz - az);
        RwIm3DVertexSetPos(&v3d[3], px - rx + ax, py - ry + ay, pz - rz + az);

        // A texture command picks one cell of a texture sheet; without one the whole texture.
        if (tex)
        {
            u1 = tex->x1 + idx->m_texIdx[0] * tex->unit_width;
            v1 = tex->y1 + idx->m_texIdx[1] * tex->unit_height;
            u2 = tex->x1 + (idx->m_texIdx[0] + 1) * tex->unit_width;
            v2 = tex->y1 + (idx->m_texIdx[1] + 1) * tex->unit_height;
            RwIm3DVertexSetU(&v3d[0], u1);
            RwIm3DVertexSetV(&v3d[0], v1);
            RwIm3DVertexSetU(&v3d[1], u2);
            RwIm3DVertexSetV(&v3d[1], v2);
            RwIm3DVertexSetU(&v3d[2], u2);
            RwIm3DVertexSetV(&v3d[2], v1);
            RwIm3DVertexSetU(&v3d[3], u1);
            RwIm3DVertexSetV(&v3d[3], v2);
        }
        else
        {
            RwIm3DVertexSetU(&v3d[0], 0.0f);
            RwIm3DVertexSetV(&v3d[0], 0.0f);
            RwIm3DVertexSetU(&v3d[1], 1.0f);
            RwIm3DVertexSetV(&v3d[1], 1.0f);
            RwIm3DVertexSetU(&v3d[2], 1.0f);
            RwIm3DVertexSetV(&v3d[2], 0.0f);
            RwIm3DVertexSetU(&v3d[3], 0.0f);
            RwIm3DVertexSetV(&v3d[3], 1.0f);
        }

        {
            U16* dst = &gRenderBuffer.m_index[gRenderBuffer.m_indexCount];

            for (S32 i = 0; i < 6; i++)
            {
                *dst++ = gRenderBuffer.m_vertexCount + i3d[i];
            }
        }

        vertices = v3d;
        memcpy((U8*)gRenderBuffer.m_vertex + gRenderBuffer.m_vertexTypeSize * gRenderBuffer.m_vertexCount,
               vertices, gRenderBuffer.m_vertexTypeSize * 4);
        gRenderBuffer.m_indexCount += 6;
        gRenderBuffer.m_vertexCount += 4;
    }

    iRenderFlush();
}

void iParMgrRenderParSys_Static(void* data, xParGroup* ps)
{
}

// Camera-facing quads stretched from each particle back along its velocity.
void iParMgrRenderParSys_QuadStreak(void* data, xParGroup* ps)
{
    xPar* idx = ps->m_root;
    RwTexture* texture;
    RwRaster* raster;

    iRenderSetCameraViewMatrix(NULL);
    texture = (RwTexture*)xSTFindAsset(((zParSys*)data)->tasset->textureID, NULL);

    if (texture)
    {
        raster = texture->raster;

        if (raster)
        {
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
        }
    }
    else
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    }

    for (; idx != NULL; idx = idx->m_next)
    {
        iRenderPushQuadStreak(idx, ps->m_cmdTex);
    }

    iRenderFlush();
}

// One triangle per particle: a camera-facing base at the point five velocity steps back, tapering
// to the particle itself.
void iParMgrRenderParSys_InvStreak(void* data, xParGroup* ps)
{
    xPar* idx = ps->m_root;
    RwTexture* texture;
    RwRaster* raster;
    RxObjSpace3DVertex* v3d;

    iRenderSetCameraViewMatrix(NULL);
    texture = (RwTexture*)xSTFindAsset(((zParSys*)data)->tasset->textureID, NULL);

    if (texture)
    {
        raster = texture->raster;

        if (raster)
        {
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
        }
    }
    else
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    }

    v3d = gRenderBuffer.m_vertex;

    for (; idx != NULL; idx = idx->m_next)
    {
        F32 px = idx->m_pos.x;
        F32 py = idx->m_pos.y;
        F32 pz = idx->m_pos.z;
        F32 tx = px - 5.0f * idx->m_vel.x;
        F32 ty = py - 5.0f * idx->m_vel.y;
        F32 tz = pz - 5.0f * idx->m_vel.z;
        F32 rx = idx->m_size * gRenderBuffer.m_camViewR.x;
        F32 ry = idx->m_size * gRenderBuffer.m_camViewR.y;
        F32 rz = idx->m_size * gRenderBuffer.m_camViewR.z;

        RwIm3DVertexSetPos(&v3d[0], tx + rx, ty + ry, tz + rz);
        RwIm3DVertexSetPos(&v3d[1], tx - rx, ty - ry, tz - rz);
        RwIm3DVertexSetPos(&v3d[2], px, py, pz);
        RwIm3DVertexSetRGBA(&v3d[0], idx->m_c[0], idx->m_c[1], idx->m_c[2], idx->m_c[3]);
        RwIm3DVertexSetRGBA(&v3d[1], idx->m_c[0], idx->m_c[1], idx->m_c[2], idx->m_c[3]);
        RwIm3DVertexSetRGBA(&v3d[2], idx->m_c[0], idx->m_c[1], idx->m_c[2], idx->m_c[3]);
        RwIm3DVertexSetU(&v3d[2], 0.5f);
        RwIm3DVertexSetV(&v3d[2], 1.0f);
        RwIm3DVertexSetU(&v3d[1], 0.0f);
        RwIm3DVertexSetV(&v3d[1], 0.0f);
        RwIm3DVertexSetU(&v3d[0], 1.0f);
        RwIm3DVertexSetV(&v3d[0], 0.0f);
        v3d += 3;
        gRenderBuffer.m_vertexCount += 3;

        if (gRenderBuffer.m_vertexCount + 3 > 477)
        {
            iRenderFlush();
            v3d = gRenderBuffer.m_vertex;
        }
    }

    iRenderFlush();
}

// The same triangles as InvStreak.
void iParMgrRenderParSys_Streak(void* data, xParGroup* ps)
{
    xPar* idx = ps->m_root;
    RwTexture* texture;
    RwRaster* raster;
    RxObjSpace3DVertex* v3d;

    iRenderSetCameraViewMatrix(NULL);
    texture = (RwTexture*)xSTFindAsset(((zParSys*)data)->tasset->textureID, NULL);

    if (texture)
    {
        raster = texture->raster;

        if (raster)
        {
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
        }
    }
    else
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
    }

    v3d = gRenderBuffer.m_vertex;

    for (; idx != NULL; idx = idx->m_next)
    {
        F32 px = idx->m_pos.x;
        F32 py = idx->m_pos.y;
        F32 pz = idx->m_pos.z;
        F32 tx = px - 5.0f * idx->m_vel.x;
        F32 ty = py - 5.0f * idx->m_vel.y;
        F32 tz = pz - 5.0f * idx->m_vel.z;
        F32 rx = idx->m_size * gRenderBuffer.m_camViewR.x;
        F32 ry = idx->m_size * gRenderBuffer.m_camViewR.y;
        F32 rz = idx->m_size * gRenderBuffer.m_camViewR.z;

        RwIm3DVertexSetPos(&v3d[0], tx + rx, ty + ry, tz + rz);
        RwIm3DVertexSetPos(&v3d[1], tx - rx, ty - ry, tz - rz);
        RwIm3DVertexSetPos(&v3d[2], px, py, pz);
        RwIm3DVertexSetRGBA(&v3d[0], idx->m_c[0], idx->m_c[1], idx->m_c[2], idx->m_c[3]);
        RwIm3DVertexSetRGBA(&v3d[1], idx->m_c[0], idx->m_c[1], idx->m_c[2], idx->m_c[3]);
        RwIm3DVertexSetRGBA(&v3d[2], idx->m_c[0], idx->m_c[1], idx->m_c[2], idx->m_c[3]);
        RwIm3DVertexSetU(&v3d[2], 0.5f);
        RwIm3DVertexSetV(&v3d[2], 1.0f);
        RwIm3DVertexSetU(&v3d[1], 0.0f);
        RwIm3DVertexSetV(&v3d[1], 0.0f);
        RwIm3DVertexSetU(&v3d[0], 1.0f);
        RwIm3DVertexSetV(&v3d[0], 0.0f);
        v3d += 3;
        gRenderBuffer.m_vertexCount += 3;

        if (gRenderBuffer.m_vertexCount + 3 > 477)
        {
            iRenderFlush();
            v3d = gRenderBuffer.m_vertex;
        }
    }

    iRenderFlush();
}

// Queues one flat quad: the particle's yaw turns it about the vertical.
static void iRenderPushFlat(xPar* p, xParCmdTex* tex)
{
    static RxObjSpace3DVertex v3d[4];
    static U16 i3d[6] = { 0, 1, 2, 0, 2, 3 };

    void* vertices;
    U16* indices;
    U8 r;
    U8 g;
    U8 b;
    U8 a;
    F32 size;
    xMat3x3 groundmat;
    F32 yaw;
    F32 px;
    F32 py;
    F32 pz;
    F32 rx;
    F32 rz;
    F32 ax;
    F32 az;
    F32 u1;
    F32 u2;
    F32 v1;
    F32 v2;

    vertices = v3d;
    indices = i3d;

    if (gRenderBuffer.m_indexCount + 6 > 960)
    {
        iRenderFlush();
    }

    if (gRenderBuffer.m_vertexCount + 4 > 480)
    {
        iRenderFlush();
    }

    r = p->m_c[0];
    size = 0.5f * p->m_size;
    g = p->m_c[1];
    b = p->m_c[2];
    a = p->m_c[3];
    yaw = 0.0f;

    if (p->m_rotdeg[0])
    {
        yaw = 6.2831855f * (p->m_rotdeg[0] / 255.0f);
    }

    xMat3x3Euler(&groundmat, yaw, 0.0f, 0.0f);
    rx = groundmat.right.x * size;
    rz = groundmat.right.z * size;
    ax = groundmat.at.x * size;
    az = groundmat.at.z * size;
    px = p->m_pos.x;
    py = p->m_pos.y;
    pz = p->m_pos.z;

    RwIm3DVertexSetRGBA(&v3d[0], r, g, b, a);
    RwIm3DVertexSetRGBA(&v3d[1], r, g, b, a);
    RwIm3DVertexSetRGBA(&v3d[2], r, g, b, a);
    RwIm3DVertexSetRGBA(&v3d[3], r, g, b, a);

    RwIm3DVertexSetPos(&v3d[0], px - rx - ax, py, pz - rz - az);
    RwIm3DVertexSetPos(&v3d[1], px + rx - ax, py, pz + rz - az);
    RwIm3DVertexSetPos(&v3d[2], px + rx + ax, py, pz + rz + az);
    RwIm3DVertexSetPos(&v3d[3], px - rx + ax, py, pz - rz + az);

    if (tex)
    {
        u1 = tex->x1 + p->m_texIdx[0] * tex->unit_width;
        v1 = tex->y1 + p->m_texIdx[1] * tex->unit_height;
        u2 = tex->x1 + (p->m_texIdx[0] + 1) * tex->unit_width;
        v2 = tex->y1 + (p->m_texIdx[1] + 1) * tex->unit_height;
        RwIm3DVertexSetU(&v3d[0], u1);
        RwIm3DVertexSetV(&v3d[0], v1);
        RwIm3DVertexSetU(&v3d[1], u2);
        RwIm3DVertexSetV(&v3d[1], v1);
        RwIm3DVertexSetU(&v3d[2], u2);
        RwIm3DVertexSetV(&v3d[2], v2);
        RwIm3DVertexSetU(&v3d[3], u1);
        RwIm3DVertexSetV(&v3d[3], v2);
    }
    else
    {
        RwIm3DVertexSetU(&v3d[0], 0.0f);
        RwIm3DVertexSetV(&v3d[0], 0.0f);
        RwIm3DVertexSetU(&v3d[1], 1.0f);
        RwIm3DVertexSetV(&v3d[1], 0.0f);
        RwIm3DVertexSetU(&v3d[2], 1.0f);
        RwIm3DVertexSetV(&v3d[2], 1.0f);
        RwIm3DVertexSetU(&v3d[3], 0.0f);
        RwIm3DVertexSetV(&v3d[3], 1.0f);
    }

    {
        U16* dst = &gRenderBuffer.m_index[gRenderBuffer.m_indexCount];

        for (S32 i = 0; i < 6; i++)
        {
            *dst++ = gRenderBuffer.m_vertexCount + indices[i];
        }
    }

    memcpy((U8*)gRenderBuffer.m_vertex + gRenderBuffer.m_vertexTypeSize * gRenderBuffer.m_vertexCount, vertices,
           gRenderBuffer.m_vertexTypeSize * 4);
    gRenderBuffer.m_indexCount += 6;
    gRenderBuffer.m_vertexCount += 4;
}

// Queues one camera-facing quad from the particle back to the point five velocity steps behind
// it.
void iRenderPushQuadStreak(xPar* p, xParCmdTex* tex)
{
    static RxObjSpace3DVertex v3d[4];
    static U16 i3d[6] = { 0, 1, 2, 0, 2, 3 };

    void* vertices;
    U16* indices;
    F32 px;
    F32 py;
    F32 pz;
    F32 tx;
    F32 ty;
    F32 tz;
    F32 rx;
    F32 ry;
    F32 rz;
    F32 u1;
    F32 u2;
    F32 v1;
    F32 v2;

    vertices = v3d;
    indices = i3d;

    if (gRenderBuffer.m_indexCount + 6 > 960)
    {
        iRenderFlush();
    }

    if (gRenderBuffer.m_vertexCount + 4 > 480)
    {
        iRenderFlush();
    }

    px = p->m_pos.x;
    py = p->m_pos.y;
    pz = p->m_pos.z;
    tx = px - 5.0f * p->m_vel.x;
    ty = py - 5.0f * p->m_vel.y;
    tz = pz - 5.0f * p->m_vel.z;
    rx = p->m_size * gRenderBuffer.m_camViewR.x;
    ry = p->m_size * gRenderBuffer.m_camViewR.y;
    rz = p->m_size * gRenderBuffer.m_camViewR.z;

    RwIm3DVertexSetRGBA(&v3d[0], p->m_c[0], p->m_c[1], p->m_c[2], p->m_c[3]);
    RwIm3DVertexSetRGBA(&v3d[1], p->m_c[0], p->m_c[1], p->m_c[2], p->m_c[3]);
    RwIm3DVertexSetRGBA(&v3d[2], p->m_c[0], p->m_c[1], p->m_c[2], p->m_c[3]);
    RwIm3DVertexSetRGBA(&v3d[3], p->m_c[0], p->m_c[1], p->m_c[2], p->m_c[3]);

    RwIm3DVertexSetPos(&v3d[0], px - rx, py - ry, pz - rz);
    RwIm3DVertexSetPos(&v3d[1], tx - rx, ty - ry, tz - rz);
    RwIm3DVertexSetPos(&v3d[2], tx + rx, ty + ry, tz + rz);
    RwIm3DVertexSetPos(&v3d[3], px + rx, py + ry, pz + rz);

    if (tex)
    {
        u1 = tex->x1 + p->m_texIdx[0] * tex->unit_width;
        v1 = tex->y1 + p->m_texIdx[1] * tex->unit_height;
        u2 = tex->x1 + (p->m_texIdx[0] + 1) * tex->unit_width;
        v2 = tex->y1 + (p->m_texIdx[1] + 1) * tex->unit_height;
        RwIm3DVertexSetU(&v3d[0], u1);
        RwIm3DVertexSetV(&v3d[0], v2);
        RwIm3DVertexSetU(&v3d[1], u1);
        RwIm3DVertexSetV(&v3d[1], v1);
        RwIm3DVertexSetU(&v3d[2], u2);
        RwIm3DVertexSetV(&v3d[2], v1);
        RwIm3DVertexSetU(&v3d[3], u2);
        RwIm3DVertexSetV(&v3d[3], v2);
    }
    else
    {
        RwIm3DVertexSetU(&v3d[0], 0.0f);
        RwIm3DVertexSetV(&v3d[0], 1.0f);
        RwIm3DVertexSetU(&v3d[1], 0.0f);
        RwIm3DVertexSetV(&v3d[1], 0.0f);
        RwIm3DVertexSetU(&v3d[2], 1.0f);
        RwIm3DVertexSetV(&v3d[2], 0.0f);
        RwIm3DVertexSetU(&v3d[3], 1.0f);
        RwIm3DVertexSetV(&v3d[3], 1.0f);
    }

    {
        U16* dst = &gRenderBuffer.m_index[gRenderBuffer.m_indexCount];

        for (S32 i = 0; i < 6; i++)
        {
            *dst++ = gRenderBuffer.m_vertexCount + indices[i];
        }
    }

    memcpy((U8*)gRenderBuffer.m_vertex + gRenderBuffer.m_vertexTypeSize * gRenderBuffer.m_vertexCount, vertices,
           gRenderBuffer.m_vertexTypeSize * 4);
    gRenderBuffer.m_indexCount += 6;
    gRenderBuffer.m_vertexCount += 4;
}

// Camera-facing square sprites, written straight into the batch.
void iParMgrRenderParSys_Sprite(void* data, xParGroup* ps)
{
    xPar* idx = ps->m_root;
    zParSys* s = (zParSys*)data;
    RwTexture* texture;
    RwRaster* raster;
    S32 indexCount;
    S32 vertexCount;
    U16* i3d;
    RxObjSpace3DVertex* v3d;
    xParCmdTex* tex;
    xPar* p;
    F32 size;
    F32 px;
    F32 py;
    F32 pz;
    F32 rx;
    F32 ry;
    F32 rz;
    F32 ux;
    F32 uy;
    F32 uz;
    F32 u1;
    F32 u2;
    F32 v1;
    F32 v2;

    iRenderSetCameraViewMatrix(NULL);
    texture = (RwTexture*)xSTFindAsset(s->tasset->textureID, NULL);

    if (texture)
    {
        raster = texture->raster;

        if (raster)
        {
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
        }
    }

    tex = ps->m_cmdTex;
    i3d = gRenderBuffer.m_index;
    indexCount = 0;
    v3d = gRenderBuffer.m_vertex;
    vertexCount = 0;

    while (idx)
    {
        p = idx;
        idx = idx->m_next;
        size = 0.5f * p->m_size;
        rx = size * gRenderBuffer.m_camViewR.x;
        ry = size * gRenderBuffer.m_camViewR.y;
        rz = size * gRenderBuffer.m_camViewR.z;
        ux = size * gRenderBuffer.m_camViewU.x;
        uy = size * gRenderBuffer.m_camViewU.y;
        uz = size * gRenderBuffer.m_camViewU.z;
        px = p->m_pos.x;
        py = p->m_pos.y;
        pz = p->m_pos.z;

        RwIm3DVertexSetRGBA(&v3d[0], p->m_c[0], p->m_c[1], p->m_c[2], p->m_c[3]);
        RwIm3DVertexSetRGBA(&v3d[1], p->m_c[0], p->m_c[1], p->m_c[2], p->m_c[3]);
        RwIm3DVertexSetRGBA(&v3d[2], p->m_c[0], p->m_c[1], p->m_c[2], p->m_c[3]);
        RwIm3DVertexSetRGBA(&v3d[3], p->m_c[0], p->m_c[1], p->m_c[2], p->m_c[3]);

        RwIm3DVertexSetPos(&v3d[0], px - rx - ux, py - ry - uy, pz - rz - uz);
        RwIm3DVertexSetPos(&v3d[1], px + rx + ux, py + ry + uy, pz + rz + uz);
        RwIm3DVertexSetPos(&v3d[2], px + rx - ux, py + ry - uy, pz + rz - uz);
        RwIm3DVertexSetPos(&v3d[3], px - rx + ux, py - ry + uy, pz - rz + uz);

        if (tex)
        {
            u1 = tex->x1 + p->m_texIdx[0] * tex->unit_width;
            v1 = tex->y1 + p->m_texIdx[1] * tex->unit_height;
            u2 = tex->x1 + (p->m_texIdx[0] + 1) * tex->unit_width;
            v2 = tex->y1 + (p->m_texIdx[1] + 1) * tex->unit_height;
            RwIm3DVertexSetU(&v3d[0], u1);
            RwIm3DVertexSetV(&v3d[0], v1);
            RwIm3DVertexSetU(&v3d[1], u2);
            RwIm3DVertexSetV(&v3d[1], v2);
            RwIm3DVertexSetU(&v3d[2], u2);
            RwIm3DVertexSetV(&v3d[2], v1);
            RwIm3DVertexSetU(&v3d[3], u1);
            RwIm3DVertexSetV(&v3d[3], v2);
        }
        else
        {
            RwIm3DVertexSetU(&v3d[0], 0.0f);
            RwIm3DVertexSetV(&v3d[0], 0.0f);
            RwIm3DVertexSetU(&v3d[1], 1.0f);
            RwIm3DVertexSetV(&v3d[1], 1.0f);
            RwIm3DVertexSetU(&v3d[2], 1.0f);
            RwIm3DVertexSetV(&v3d[2], 0.0f);
            RwIm3DVertexSetU(&v3d[3], 0.0f);
            RwIm3DVertexSetV(&v3d[3], 1.0f);
        }

        i3d[0] = vertexCount;
        i3d[1] = vertexCount + 1;
        i3d[2] = vertexCount + 2;
        i3d[3] = vertexCount + 3;
        i3d[4] = vertexCount;
        i3d[5] = vertexCount + 1;
        indexCount += 6;
        v3d += 4;
        vertexCount += 4;
        i3d += 6;

        if (indexCount > 960 - 6 || vertexCount > 480 - 4)
        {
            gRenderBuffer.m_indexCount = indexCount;
            gRenderBuffer.m_vertexCount = vertexCount;
            iRenderFlush();
            i3d = gRenderBuffer.m_index;
            indexCount = 0;
            v3d = gRenderBuffer.m_vertex;
            vertexCount = 0;
        }
    }

    gRenderBuffer.m_indexCount = indexCount;
    gRenderBuffer.m_vertexCount = vertexCount;
    iRenderFlush();
}

void iParMgrRender()
{
}

void iParMgrUpdate(F32)
{
}

void iParMgrInit()
{
    iRenderInit();
}
