#include <rtintsec.h>

// The sign of a real as an integer: 0 when it is positive, -1 when it is negative.
#define REALSIGN(r) (*(const S32*)&(r) >> 31)

// The bits of a real as an integer, for testing several signs at once.
#define REALBITS(r) (*(const S32*)&(r))

// Rejects the triangle when all three vertices lie beyond the sphere on one axis.
#define SPHERETRIANGLE_AXISREJECT(_c)                                                              \
    do                                                                                             \
    {                                                                                              \
        S32 signs = REALSIGN(vc[0]._c) + REALSIGN(vc[1]._c) + REALSIGN(vc[2]._c);                  \
                                                                                                   \
        if (signs == 0)                                                                            \
        {                                                                                          \
            F32 d0 = vc[0]._c - sphere->radius;                                                    \
            F32 d1 = vc[1]._c - sphere->radius;                                                    \
            F32 d2 = vc[2]._c - sphere->radius;                                                    \
                                                                                                   \
            if ((REALBITS(d0) | REALBITS(d1) | REALBITS(d2)) >= 0)                                 \
            {                                                                                      \
                return FALSE;                                                                      \
            }                                                                                      \
        }                                                                                          \
        else if (signs == -3)                                                                      \
        {                                                                                          \
            F32 d0 = vc[0]._c + sphere->radius;                                                    \
            F32 d1 = vc[1]._c + sphere->radius;                                                    \
            F32 d2 = vc[2]._c + sphere->radius;                                                    \
                                                                                                   \
            if ((REALBITS(d0) & REALBITS(d1) & REALBITS(d2)) < 0)                                  \
            {                                                                                      \
                return FALSE;                                                                      \
            }                                                                                      \
        }                                                                                          \
    } while (0)

// Rejects the triangle when the point of the edge from a to b nearest the sphere's centre is
// outside the sphere and the third vertex c lies beyond it.
#define SPHERETRIANGLE_EDGEREJECT(a, b, c)                                                         \
    do                                                                                             \
    {                                                                                              \
        RwV3d edge;                                                                                \
        RwV3d nearest;                                                                             \
        F32 t;                                                                                     \
        F32 length2;                                                                               \
                                                                                                   \
        RwV3dSubMacro(&edge, &vc[b], &vc[a]);                                                      \
        t = RwV3dDotProductMacro(&edge, &vc[a]) / RwV3dDotProductMacro(&edge, &edge);              \
        nearest.x = vc[a].x - edge.x * t;                                                          \
        nearest.y = vc[a].y - edge.y * t;                                                          \
        nearest.z = vc[a].z - edge.z * t;                                                          \
        length2 = RwV3dDotProductMacro(&nearest, &nearest);                                        \
                                                                                                   \
        if (length2 > sphereRadiusSquared && length2 < RwV3dDotProductMacro(&nearest, &vc[c]))     \
        {                                                                                          \
            return FALSE;                                                                          \
        }                                                                                          \
    } while (0)

// Rejects the triangle when vertex a is the nearest to the sphere's centre, it is outside the
// sphere, and both other vertices lie beyond it.
#define SPHERETRIANGLE_VERTEXREJECT(a, b, c, d)                                                    \
    do                                                                                             \
    {                                                                                              \
        if (RwV3dDotProductMacro(&vc[a], &vc[b]) > (d) &&                                         \
            RwV3dDotProductMacro(&vc[a], &vc[c]) > (d))                                            \
        {                                                                                          \
            return FALSE;                                                                          \
        }                                                                                          \
    } while (0)

// The outcode of a point against a box: which of the six faces it lies beyond.
#define BBOXTRIANGLE_OUTCODE(v)                                                                    \
    ((((v)->x > bbox->sup.x) ? 1 : (((v)->x < bbox->inf.x) ? 2 : 0)) |                            \
     (((v)->y > bbox->sup.y) ? 4 : (((v)->y < bbox->inf.y) ? 8 : 0)) |                            \
     (((v)->z > bbox->sup.z) ? 0x10 : (((v)->z < bbox->inf.z) ? 0x20 : 0)))

// Accepts the triangle when an edge (start offset p from the face, direction d along the face's
// axis a) crosses the face inside the box on the two other axes b and c.
#define BBOXTRIANGLE_FACETEST(p, da, db, dc, bsup, binf, csup, cinf)                               \
    do                                                                                             \
    {                                                                                              \
        F32 tmp = (p) * (db);                                                                      \
                                                                                                   \
        if ((da) < 0.0f)                                                                           \
        {                                                                                          \
            if (tmp < (bsup) * (da) && tmp > (binf) * (da))                                        \
            {                                                                                      \
                tmp = (p) * (dc);                                                                  \
                                                                                                   \
                if (tmp < (csup) * (da) && tmp > (cinf) * (da))                                    \
                {                                                                                  \
                    return TRUE;                                                                   \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
        else if (tmp > (bsup) * (da) && tmp < (binf) * (da))                                       \
        {                                                                                          \
            tmp = (p) * (dc);                                                                      \
                                                                                                   \
            if (tmp > (csup) * (da) && tmp < (cinf) * (da))                                        \
            {                                                                                      \
                return TRUE;                                                                       \
            }                                                                                      \
        }                                                                                          \
    } while (0)

// Accepts the triangle when the edge from va to vb crosses one of the faces that separate their
// outcodes inside the box.
#define BBOXTRIANGLE_EDGETEST(va, vb, codeA, codeB)                                                \
    do                                                                                             \
    {                                                                                              \
        if (!((codeA) & (codeB)))                                                                  \
        {                                                                                          \
            S32 faces = (codeA) ^ (codeB);                                                         \
            RwV3d d;                                                                               \
            RwV3d toInf;                                                                           \
            RwV3d toSup;                                                                           \
                                                                                                   \
            RwV3dSubMacro(&d, vb, va);                                                             \
            RwV3dSubMacro(&toInf, va, &bbox->inf);                                                 \
            RwV3dSubMacro(&toSup, va, &bbox->sup);                                                 \
                                                                                                   \
            if (faces & 1)                                                                         \
            {                                                                                      \
                BBOXTRIANGLE_FACETEST(toSup.x, d.x, d.y, d.z, toSup.y, toInf.y, toSup.z, toInf.z); \
            }                                                                                      \
            if (faces & 2)                                                                         \
            {                                                                                      \
                BBOXTRIANGLE_FACETEST(toInf.x, d.x, d.y, d.z, toSup.y, toInf.y, toSup.z, toInf.z); \
            }                                                                                      \
            if (faces & 4)                                                                         \
            {                                                                                      \
                BBOXTRIANGLE_FACETEST(toSup.y, d.y, d.z, d.x, toSup.z, toInf.z, toSup.x, toInf.x); \
            }                                                                                      \
            if (faces & 8)                                                                         \
            {                                                                                      \
                BBOXTRIANGLE_FACETEST(toInf.y, d.y, d.z, d.x, toSup.z, toInf.z, toSup.x, toInf.x); \
            }                                                                                      \
            if (faces & 0x10)                                                                      \
            {                                                                                      \
                BBOXTRIANGLE_FACETEST(toSup.z, d.z, d.x, d.y, toSup.x, toInf.x, toSup.y, toInf.y); \
            }                                                                                      \
            if (faces & 0x20)                                                                      \
            {                                                                                      \
                BBOXTRIANGLE_FACETEST(toInf.z, d.z, d.x, d.y, toSup.x, toInf.x, toSup.y, toInf.y); \
            }                                                                                      \
        }                                                                                          \
    } while (0)

S32 RtIntersectionSphereTriangle(const RwSphere* sphere, const RwV3d* v0, const RwV3d* v1,
                                 const RwV3d* v2, RwV3d* normal, F32* distance)
{
    RwV3d vc[3];
    RwV3d e1;
    RwV3d e2;
    F32 length2;
    F32 factor;
    F32 distToPlane;
    F32 sphereRadiusSquared;
    F32 d0;
    F32 d1;
    F32 d2;

    RwV3dSubMacro(&vc[0], v0, &sphere->center);
    RwV3dSubMacro(&vc[1], v1, &sphere->center);
    RwV3dSubMacro(&vc[2], v2, &sphere->center);

    SPHERETRIANGLE_AXISREJECT(x);
    SPHERETRIANGLE_AXISREJECT(y);
    SPHERETRIANGLE_AXISREJECT(z);

    RwV3dSubMacro(&e1, v1, v0);
    RwV3dSubMacro(&e2, v2, v0);
    RwV3dCrossProductMacro(normal, &e1, &e2);

    length2 = RwV3dDotProductMacro(normal, normal);

    if (length2 <= 0.0f)
    {
        return FALSE;
    }

    factor = _rwInvSqrt(length2);
    RwV3dScaleMacro(normal, normal, factor);

    distToPlane = RwV3dDotProductMacro(&vc[0], normal);

    if (distToPlane < -sphere->radius || distToPlane > sphere->radius)
    {
        return FALSE;
    }

    *distance = -distToPlane;

    sphereRadiusSquared = sphere->radius * sphere->radius;

    d0 = RwV3dDotProductMacro(&vc[0], &vc[0]);

    if (d0 <= sphereRadiusSquared)
    {
        return TRUE;
    }

    d1 = RwV3dDotProductMacro(&vc[1], &vc[1]);

    if (d1 <= sphereRadiusSquared)
    {
        return TRUE;
    }

    d2 = RwV3dDotProductMacro(&vc[2], &vc[2]);

    if (d2 <= sphereRadiusSquared)
    {
        return TRUE;
    }

    if (d0 < d1)
    {
        if (d2 < d0)
        {
            SPHERETRIANGLE_VERTEXREJECT(2, 0, 1, d2);
        }
        else
        {
            SPHERETRIANGLE_VERTEXREJECT(0, 1, 2, d0);
        }
    }
    else if (d2 < d1)
    {
        SPHERETRIANGLE_VERTEXREJECT(2, 0, 1, d2);
    }
    else
    {
        SPHERETRIANGLE_VERTEXREJECT(1, 0, 2, d1);
    }

    SPHERETRIANGLE_EDGEREJECT(0, 1, 2);
    SPHERETRIANGLE_EDGEREJECT(1, 2, 0);
    SPHERETRIANGLE_EDGEREJECT(2, 0, 1);

    return TRUE;
}

S32 RtIntersectionBBoxTriangle(const RwBBox* bbox, const RwV3d* v0, const RwV3d* v1,
                               const RwV3d* v2)
{
    S32 v0Test;
    S32 v1Test;
    S32 v2Test;
    RwV3d e1;
    RwV3d e2;
    RwV3d norm;
    RwV3d diag;
    RwV3d start;
    F32 det;
    F32 dist;

    v0Test = BBOXTRIANGLE_OUTCODE(v0);

    if (!v0Test)
    {
        return TRUE;
    }

    v1Test = BBOXTRIANGLE_OUTCODE(v1);

    if (!v1Test)
    {
        return TRUE;
    }

    v2Test = BBOXTRIANGLE_OUTCODE(v2);

    if (!v2Test)
    {
        return TRUE;
    }

    if (v0Test & v1Test & v2Test)
    {
        return FALSE;
    }

    BBOXTRIANGLE_EDGETEST(v0, v1, v0Test, v1Test);
    BBOXTRIANGLE_EDGETEST(v0, v2, v0Test, v2Test);
    BBOXTRIANGLE_EDGETEST(v1, v2, v1Test, v2Test);

    // No edge crosses the box: test the box diagonal that the triangle's normal points along
    // against the triangle.
    RwV3dSubMacro(&e1, v1, v0);
    RwV3dSubMacro(&e2, v2, v0);
    RwV3dCrossProductMacro(&norm, &e1, &e2);

    diag.x = bbox->sup.x - bbox->inf.x;
    start.x = bbox->inf.x - v0->x;

    if ((REALBITS(norm.x) ^ REALBITS(norm.y)) < 0)
    {
        diag.y = bbox->inf.y - bbox->sup.y;
        start.y = bbox->sup.y - v0->y;
    }
    else
    {
        diag.y = bbox->sup.y - bbox->inf.y;
        start.y = bbox->inf.y - v0->y;
    }

    if ((REALBITS(norm.x) ^ REALBITS(norm.z)) < 0)
    {
        diag.z = bbox->inf.z - bbox->sup.z;
        start.z = bbox->sup.z - v0->z;
    }
    else
    {
        diag.z = bbox->sup.z - bbox->inf.z;
        start.z = bbox->inf.z - v0->z;
    }

    det = -RwV3dDotProductMacro(&diag, &norm);
    dist = RwV3dDotProductMacro(&start, &norm);

    if (det < 0.0f)
    {
        if (0.0f > dist && dist > det)
        {
            RwV3d cross;
            F32 u;
            F32 v;

            RwV3dCrossProductMacro(&cross, &start, &diag);
            u = RwV3dDotProductMacro(&e2, &cross);

            if (0.0f > u && u > det)
            {
                v = -RwV3dDotProductMacro(&e1, &cross);

                if (0.0f > v && u + v > det)
                {
                    return TRUE;
                }
            }
        }
    }
    else if (0.0f < dist && dist < det)
    {
        RwV3d cross;
        F32 u;
        F32 v;

        RwV3dCrossProductMacro(&cross, &start, &diag);
        u = RwV3dDotProductMacro(&e2, &cross);

        if (0.0f < u && u < det)
        {
            v = -RwV3dDotProductMacro(&e1, &cross);

            if (0.0f < v && u + v < det)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}
