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

void iRenderInit();
void iRenderSetCameraViewMatrix(xMat4x3* m);
void iRenderTrianglesImmediate(S32 vertType, S32 vertTypeSize, void* data, S32 dataSize, U16* index,
                               S32 indexSize);
void iRenderFlush();
void iParMgrRenderParSys_Flat(void* data, xParGroup* ps);
void iParMgrRenderParSys_Ground(void* data, xParGroup* ps);
void iParMgrRenderParSys_Static(void* data, xParGroup* ps);
void iParMgrRenderParSys_QuadStreak(void* data, xParGroup* ps);
void iParMgrRenderParSys_InvStreak(void* data, xParGroup* ps);
void iParMgrRenderParSys_Streak(void* data, xParGroup* ps);
static void iRenderPushFlat(xPar* p, xParCmdTex* tex);
void iRenderPushQuadStreak(xPar* p, xParCmdTex* tex);
void iParMgrRenderParSys_Sprite(void* data, xParGroup* ps);
void iParMgrRender();
void iParMgrUpdate(F32);
void iParMgrInit();

static void iRenderPushFlat(xPar* p, xParCmdTex* tex);

void iRenderPushQuadStreak(xPar* p, xParCmdTex* tex);

void iParMgrInit()
{
    iRenderInit();
}

void iParMgrUpdate(F32)
{
}

void iParMgrRender()
{
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
    U8 r;
    U8 g;
    U8 b;
    U8 a;
    xVec3 right;
    xVec3 up;
    xVec3 position;
    xVec3 vertexpos0;
    xVec3 vertexpos1;
    xVec3 vertexpos2;
    xVec3 vertexpos3;
    F32 size;
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
        right.x = size * gRenderBuffer.m_camViewR.x;
        right.y = size * gRenderBuffer.m_camViewR.y;
        right.z = size * gRenderBuffer.m_camViewR.z;
        up.x = size * gRenderBuffer.m_camViewU.x;
        up.y = size * gRenderBuffer.m_camViewU.y;
        up.z = size * gRenderBuffer.m_camViewU.z;

        r = p->m_c[0];
        g = p->m_c[1];
        b = p->m_c[2];
        a = p->m_c[3];

        position.x = p->m_pos.x;
        position.y = p->m_pos.y;
        position.z = p->m_pos.z;
        RwIm3DVertexSetRGBA(&v3d[0], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[1], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[2], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[3], r, g, b, a);

        vertexpos0.x = position.x - right.x - up.x;
        vertexpos0.y = position.y - right.y - up.y;
        vertexpos0.z = position.z - right.z - up.z;
        RwIm3DVertexSetPos(&v3d[0], vertexpos0.x, vertexpos0.y, vertexpos0.z);
        vertexpos1.x = position.x + right.x + up.x;
        vertexpos1.y = position.y + right.y + up.y;
        vertexpos1.z = position.z + right.z + up.z;
        RwIm3DVertexSetPos(&v3d[1], vertexpos1.x, vertexpos1.y, vertexpos1.z);
        vertexpos2.x = position.x + right.x - up.x;
        vertexpos2.y = position.y + right.y - up.y;
        vertexpos2.z = position.z + right.z - up.z;
        RwIm3DVertexSetPos(&v3d[2], vertexpos2.x, vertexpos2.y, vertexpos2.z);
        vertexpos3.x = position.x - right.x + up.x;
        vertexpos3.y = position.y - right.y + up.y;
        vertexpos3.z = position.z - right.z + up.z;
        RwIm3DVertexSetPos(&v3d[3], vertexpos3.x, vertexpos3.y, vertexpos3.z);

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
            i3d = gRenderBuffer.m_index;
            v3d = gRenderBuffer.m_vertex;
            iRenderFlush();
            indexCount = 0;
            vertexCount = 0;
        }
    }

    gRenderBuffer.m_indexCount = indexCount;
    gRenderBuffer.m_vertexCount = vertexCount;
    iRenderFlush();
}

// Queues one camera-facing quad from the particle back to the point five velocity steps behind
// it.
void iRenderPushQuadStreak(xPar* p, xParCmdTex* tex)
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
    RwV3d right;
    F32 tx;
    F32 ty;
    F32 tz;
    F32 tx2;
    F32 ty2;
    F32 tz2;
    F32 px;
    F32 py;
    F32 pz;
    RwV3d head;
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
    size = p->m_size;
    right.x = size * gRenderBuffer.m_camViewR.x;
    right.y = size * gRenderBuffer.m_camViewR.y;
    right.z = size * gRenderBuffer.m_camViewR.z;

    tx2 = tx;
    ty2 = ty;
    tz2 = tz;
    head.x = px;
    head.y = py;
    head.z = pz;
    head.x -= right.x;
    head.y -= right.y;
    head.z -= right.z;

    tx -= right.x;
    ty -= right.y;
    tz -= right.z;
    tx2 += right.x;
    ty2 += right.y;
    tz2 += right.z;

    r = p->m_c[0];
    g = p->m_c[1];
    b = p->m_c[2];
    a = p->m_c[3];
    RwIm3DVertexSetRGBA(&v3d[0], r, g, b, a);
    RwIm3DVertexSetRGBA(&v3d[1], r, g, b, a);
    RwIm3DVertexSetRGBA(&v3d[2], r, g, b, a);
    RwIm3DVertexSetRGBA(&v3d[3], r, g, b, a);

    RwIm3DVertexSetPos(&v3d[0], head.x, head.y, head.z);
    RwIm3DVertexSetPos(&v3d[1], tx, ty, tz);
    RwIm3DVertexSetPos(&v3d[2], tx2, ty2, tz2);
    RwIm3DVertexSetPos(&v3d[3], px + right.x, py + right.y, pz + right.z);

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
        // The textureless path leaves vertex 3's V unchanged.
        RwIm3DVertexSetU(&v3d[3], 0.0f);
        RwIm3DVertexSetV(&v3d[2], 1.0f);
    }

    {
        U16* dst = &gRenderBuffer.m_index[gRenderBuffer.m_indexCount];

        U16* cursor = indices;
        for (S32 i = 0; i < 6; i++)
        {
            *dst++ = gRenderBuffer.m_vertexCount + *cursor++;
        }
    }

    memcpy((U8*)gRenderBuffer.m_vertex + gRenderBuffer.m_vertexTypeSize * gRenderBuffer.m_vertexCount, vertices,
           gRenderBuffer.m_vertexTypeSize * 4);
    gRenderBuffer.m_indexCount += 6;
    gRenderBuffer.m_vertexCount += 4;
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
    RwV2d at;
    RwV2d basis;
    xVec3 position;
    RwV2d left;
    RwV2d right;
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
    // Work in the X/Z plane; position.y supplies the height.
    at.x = groundmat.at.x * size;
    at.y = groundmat.at.z * size;
    basis.x = groundmat.right.x * size;
    basis.y = groundmat.right.z * size;
    position.x = p->m_pos.x;
    position.y = p->m_pos.y;
    position.z = p->m_pos.z;

    left.x = position.x - basis.x;
    left.y = position.z - basis.y;
    right.x = position.x + basis.x;
    right.y = position.z + basis.y;

    left.x -= at.x;
    left.y -= at.y;
    right.x -= at.x;
    right.y -= at.y;

    RwIm3DVertexSetRGBA(&v3d[0], r, g, b, a);
    RwIm3DVertexSetRGBA(&v3d[1], r, g, b, a);
    RwIm3DVertexSetRGBA(&v3d[2], r, g, b, a);
    RwIm3DVertexSetRGBA(&v3d[3], r, g, b, a);

    RwIm3DVertexSetPos(&v3d[0], left.x, position.y, left.y);
    RwIm3DVertexSetPos(&v3d[1], right.x, position.y, right.y);
    RwIm3DVertexSetPos(&v3d[2], at.x + (position.x + basis.x), position.y,
                       at.y + (position.z + basis.y));
    RwIm3DVertexSetPos(&v3d[3], at.x + (position.x - basis.x), position.y,
                       at.y + (position.z - basis.y));

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

        U16* cursor = indices;
        for (S32 i = 0; i < 6; i++)
        {
            *dst++ = gRenderBuffer.m_vertexCount + *cursor++;
        }
    }

    memcpy((U8*)gRenderBuffer.m_vertex + gRenderBuffer.m_vertexTypeSize * gRenderBuffer.m_vertexCount, vertices,
           gRenderBuffer.m_vertexTypeSize * 4);
    gRenderBuffer.m_indexCount += 6;
    gRenderBuffer.m_vertexCount += 4;
}

// The same triangles as InvStreak.
void iParMgrRenderParSys_Streak(void* data, xParGroup* ps)
{
    xPar* idx = ps->m_root;
    RwTexture* texture;
    RwRaster* raster;
    RxObjSpace3DVertex* v3d;
    F32 px;
    F32 py;
    F32 pz;
    F32 size;
    RwV3d opposite;
    RwV3d right;
    RwV3d tail;
    U8 r;
    U8 g;
    U8 b;
    U8 a;

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
        px = idx->m_pos.x;
        py = idx->m_pos.y;
        pz = idx->m_pos.z;
        size = idx->m_size;
        right.x = size * gRenderBuffer.m_camViewR.x;
        right.y = size * gRenderBuffer.m_camViewR.y;
        right.z = size * gRenderBuffer.m_camViewR.z;
        tail.x = px - 5.0f * idx->m_vel.x;
        tail.y = py - 5.0f * idx->m_vel.y;
        tail.z = pz - 5.0f * idx->m_vel.z;
        opposite.x = tail.x;
        opposite.y = tail.y;
        opposite.z = tail.z;

        tail.x -= right.x;
        tail.y -= right.y;
        tail.z -= right.z;
        opposite.x += right.x;
        opposite.y += right.y;
        opposite.z += right.z;
        RwIm3DVertexSetPos(&v3d[0], opposite.x, opposite.y, opposite.z);
        RwIm3DVertexSetPos(&v3d[1], tail.x, tail.y, tail.z);
        RwIm3DVertexSetPos(&v3d[2], px, py, pz);
        r = idx->m_c[0];
        g = idx->m_c[1];
        b = idx->m_c[2];
        a = idx->m_c[3];
        RwIm3DVertexSetRGBA(&v3d[0], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[1], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[2], r, g, b, a);
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

// One triangle per particle: a camera-facing base at the point five velocity steps back, tapering
// to the particle itself.
void iParMgrRenderParSys_InvStreak(void* data, xParGroup* ps)
{
    xPar* idx = ps->m_root;
    RwTexture* texture;
    RwRaster* raster;
    RxObjSpace3DVertex* v3d;
    F32 px;
    F32 py;
    F32 pz;
    F32 size;
    RwV3d opposite;
    RwV3d right;
    RwV3d tail;
    U8 r;
    U8 g;
    U8 b;
    U8 a;

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
        px = idx->m_pos.x;
        py = idx->m_pos.y;
        pz = idx->m_pos.z;
        size = idx->m_size;
        right.x = size * gRenderBuffer.m_camViewR.x;
        right.y = size * gRenderBuffer.m_camViewR.y;
        right.z = size * gRenderBuffer.m_camViewR.z;
        tail.x = px - 5.0f * idx->m_vel.x;
        opposite.x = tail.x;
        tail.y = py - 5.0f * idx->m_vel.y;
        opposite.y = tail.y;
        tail.z = pz - 5.0f * idx->m_vel.z;
        opposite.z = tail.z;

        tail.x -= right.x;
        tail.y -= right.y;
        tail.z -= right.z;
        opposite.x += right.x;
        opposite.y += right.y;
        opposite.z += right.z;
        RwIm3DVertexSetPos(&v3d[0], opposite.x, opposite.y, opposite.z);
        RwIm3DVertexSetPos(&v3d[1], tail.x, tail.y, tail.z);
        RwIm3DVertexSetPos(&v3d[2], px, py, pz);
        r = idx->m_c[0];
        g = idx->m_c[1];
        b = idx->m_c[2];
        a = idx->m_c[3];
        RwIm3DVertexSetRGBA(&v3d[0], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[1], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[2], r, g, b, a);
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

void iParMgrRenderParSys_Static(void* data, xParGroup* ps)
{
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
    U16* indices;
    F32 size;
    U8 r;
    U8 g;
    U8 b;
    U8 a;
    xMat3x3 groundmat;
    F32 angx;
    F32 angy;
    F32 angz;
    F32 oldX;
    F32 oldY;
    F32 oldZ;
    xVec3 at;
    xVec3 basis;
    xVec3 center;
    xVec3 left;
    xVec3 right;
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
        vertices = v3d;
        indices = i3d;
        tex = ps->m_cmdTex;

        if (gRenderBuffer.m_indexCount + 6 > 960 || gRenderBuffer.m_vertexCount + 4 > 480)
        {
            iRenderFlush();
        }

        size = 0.5f * idx->m_size;
        angx = angy = angz = 0.0f;
        r = idx->m_c[0];
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
        at.x = groundmat.at.x * size;
        at.y = groundmat.at.y * size;
        at.z = groundmat.at.z * size;
        basis.x = groundmat.right.x * size;
        basis.y = groundmat.right.y * size;
        basis.z = groundmat.right.z * size;
        center.x = idx->m_pos.x;
        center.y = idx->m_pos.y;
        center.z = idx->m_pos.z;

        oldX = center.x - basis.x;
        left.x = oldX;
        oldY = center.y - basis.y;
        left.y = oldY;
        oldZ = center.z - basis.z;
        left.z = oldZ;
        right.x = center.x + basis.x;
        right.y = center.y + basis.y;
        right.z = center.z + basis.z;

        left.x -= at.x;
        left.y -= at.y;
        left.z -= at.z;
        right.x = at.x + right.x;
        right.y = at.y + right.y;
        right.z = at.z + right.z;

        RwIm3DVertexSetRGBA(&v3d[0], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[1], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[2], r, g, b, a);
        RwIm3DVertexSetRGBA(&v3d[3], r, g, b, a);

        RwIm3DVertexSetPos(&v3d[0], left.x, left.y, left.z);
        RwIm3DVertexSetPos(&v3d[1], right.x, right.y, right.z);
        RwIm3DVertexSetPos(&v3d[2], (center.x + basis.x) - at.x, (center.y + basis.y) - at.y, (center.z + basis.z) - at.z);
        RwIm3DVertexSetPos(&v3d[3], at.x + oldX, at.y + oldY, at.z + oldZ);

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

            U16* cursor = indices;
            for (S32 i = 0; i < 6; i++)
            {
                *dst++ = gRenderBuffer.m_vertexCount + *cursor++;
            }
        }

        memcpy((U8*)gRenderBuffer.m_vertex + gRenderBuffer.m_vertexTypeSize * gRenderBuffer.m_vertexCount,
               vertices, gRenderBuffer.m_vertexTypeSize * 4);
        gRenderBuffer.m_indexCount += 6;
        gRenderBuffer.m_vertexCount += 4;
    }

    iRenderFlush();
}

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

// Draws whatever the batch holds and empties it.
void iRenderFlush()
{
    if (gRenderBuffer.m_vertexCount > 0)
    {
        iRenderTrianglesImmediate(gRenderBuffer.m_vertexType, gRenderBuffer.m_vertexTypeSize,
                                  gRenderBuffer.m_vertex, gRenderBuffer.m_vertexCount,
                                  gRenderBuffer.m_index, gRenderBuffer.m_indexCount);
    }

    gRenderBuffer.m_vertexCount = 0;
    gRenderBuffer.m_indexCount = 0;
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
