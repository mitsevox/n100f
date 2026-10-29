#include <rpworld.h>
#include <string.h>

// The lock-mode bit that invalidates the geometry's mesh, and all the lock-mode bits.
#define GEOMLOCK_POLYGONS 0x01
#define GEOMLOCK_ALL 0xFFF

// The geometry format flags RpGeometryCreate reads: triangle strips, prelight colors, normals,
// one or two texture coordinate sets, the texture coordinate set count in bits 16 to 23 and
// the native flags.
#define GEOMETRY_TRISTRIP 0x01
#define GEOMETRY_TEXTURED 0x04
#define GEOMETRY_PRELIT 0x08
#define GEOMETRY_NORMALS 0x10
#define GEOMETRY_TEXTURED2 0x80
#define GEOMETRY_FLAGSMASK 0xFF
#define GEOMETRY_TEXCOORDSETSMASK 0x00FF0000
#define GEOMETRY_NATIVEFLAGSMASK 0x0F000000

// The object type of a geometry.
#define GEOMETRY_OBJECTTYPE 8

// One past the most vertices a geometry can have: 16-bit vertex indices address 0x10000.
#define GEOMETRY_VERTEXLIMIT 0x10000

// The geometry chunk's struct, and the header ahead of each morph target's data.
typedef struct GeometryStreamData GeometryStreamData;
struct GeometryStreamData
{
    U32 format;
    S32 numTriangles;
    S32 numVertices;
    S32 numMorphTargets;
    RwSurfaceProperties surfaceProps;
};

typedef struct MorphTargetStreamData MorphTargetStreamData;
struct MorphTargetStreamData
{
    RwSphere boundingSphere;
    S32 pointsPresent;
    S32 normalsPresent;
};

static RwPluginRegistry geometryTKList = { sizeof(RpGeometry), sizeof(RpGeometry) };
static RwModuleInfo geometryModule;

void* _rpGeometryOpen(void* instance, S32 offset, S32 size)
{
    geometryModule.globalsOffset = offset;
    geometryModule.numInstances++;

    return instance;
}

void* _rpGeometryClose(void* instance, S32 offset, S32 size)
{
    geometryModule.numInstances--;

    return instance;
}

S32 RpGeometryAddMorphTargets(RpGeometry* geometry, S32 mtcount)
{
    U32 morphTargetSize;
    U32 size;
    S32 i;
    U8* data;
    RpMorphTarget* morphTargets;

    if (geometry->flags & rpGEOMETRYNATIVE)
    {
        morphTargetSize = sizeof(RpMorphTarget);
    }
    else
    {
        U32 vertexSize = geometry->numVertices * sizeof(RwV3d);

        morphTargetSize = sizeof(RpMorphTarget) + vertexSize;

        if (geometry->flags & GEOMETRY_NORMALS)
        {
            morphTargetSize += vertexSize;
        }
    }

    size = morphTargetSize * (geometry->numMorphTargets + mtcount);

    if (geometry->morphTarget)
    {
        U8* src;
        U8* dst;
        U32 count;

        morphTargets = (RpMorphTarget*)RwRealloc(geometry->morphTarget, size);

        if (!morphTargets)
        {
            RwError err;

            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000013, size);
            RwErrorSet(&err);

            return -1;
        }

        // Move the vertex data up past the new morph targets, from the end down.
        src = (U8*)morphTargets + morphTargetSize * geometry->numMorphTargets - 1;
        dst = src + mtcount * sizeof(RpMorphTarget);
        count = morphTargetSize * geometry->numMorphTargets -
                geometry->numMorphTargets * sizeof(RpMorphTarget);

        while (count--)
        {
            *dst-- = *src--;
        }
    }
    else
    {
        morphTargets = (RpMorphTarget*)RwMalloc(size);

        if (!morphTargets)
        {
            RwError err;

            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000013, size);
            RwErrorSet(&err);

            return -1;
        }
    }

    geometry->numMorphTargets += mtcount;
    geometry->morphTarget = morphTargets;

    data = (U8*)morphTargets + geometry->numMorphTargets * sizeof(RpMorphTarget);

    for (i = 0; i < geometry->numMorphTargets; i++)
    {
        RpMorphTarget* morphTarget = &geometry->morphTarget[i];

        morphTarget->verts = NULL;
        morphTarget->normals = NULL;

        if (!(geometry->flags & rpGEOMETRYNATIVE) && geometry->numVertices)
        {
            morphTarget->verts = (RwV3d*)data;
            data += geometry->numVertices * sizeof(RwV3d);

            if (geometry->flags & GEOMETRY_NORMALS)
            {
                morphTarget->normals = (RwV3d*)data;
                data += geometry->numVertices * sizeof(RwV3d);
            }
        }
    }

    for (i = geometry->numMorphTargets - mtcount; i < geometry->numMorphTargets; i++)
    {
        RpMorphTarget* morphTarget = &geometry->morphTarget[i];

        morphTarget->boundingSphere.center.x = 0.0f;
        morphTarget->boundingSphere.center.y = 0.0f;
        morphTarget->boundingSphere.center.z = 0.0f;
        morphTarget->boundingSphere.radius = 0.0f;
        morphTarget->parentGeom = geometry;
    }

    return geometry->numMorphTargets - mtcount;
}

RpGeometry* RpGeometryForAllMaterials(RpGeometry* geometry, RpMaterialCallBack fpCallBack,
                                      void* pData)
{
    S32 numMaterials = geometry->matList.numMaterials;
    S32 i;

    for (i = 0; i < numMaterials; i++)
    {
        if (!fpCallBack(geometry->matList.materials[i], pData))
        {
            return geometry;
        }
    }

    return geometry;
}

RpGeometry* RpGeometryLock(RpGeometry* geometry, S32 lockMode)
{
    geometry->lockedSinceLastInst |= lockMode;

    if (lockMode & GEOMLOCK_POLYGONS)
    {
        if (geometry->mesh)
        {
            _rpMeshDestroy(geometry->mesh);
            geometry->mesh = NULL;
        }
    }

    return geometry;
}

RpGeometry* RpGeometryUnlock(RpGeometry* geometry)
{
    if (!geometry->mesh)
    {
        RpBuildMesh* buildMesh;
        RpMeshHeader* mesh;
        S32 i;

        buildMesh = _rpBuildMeshCreate(geometry->numTriangles);

        if (buildMesh)
        {
            for (i = 0; i < geometry->numTriangles; i++)
            {
                RpTriangle* triangle = &geometry->triangles[i];
                RpMaterial* material;

                if (triangle->matIndex != -1)
                {
                    material = _rpMaterialListGetMaterial(&geometry->matList, triangle->matIndex);
                }
                else
                {
                    material = NULL;
                }

                _rpBuildMeshAddTriangle(buildMesh, material, triangle->vertIndex[0],
                                        triangle->vertIndex[1], triangle->vertIndex[2]);
            }

            if (geometry->flags & GEOMETRY_TRISTRIP)
            {
                mesh = _rpMeshOptimise(buildMesh, 1);
            }
            else
            {
                mesh = _rpMeshOptimise(buildMesh, 0);
            }

            if (mesh)
            {
                geometry->mesh = mesh;
                return geometry;
            }

            _rpBuildMeshDestroy(buildMesh);
            return NULL;
        }

        return NULL;
    }

    return geometry;
}

RpGeometry* RpGeometryCreate(S32 numVerts, S32 numTriangles, U32 format)
{
    U32 numTexCoordSets;
    U32 native;
    U32 flags;
    U32 texFlags;
    U32 size;
    RpGeometry* geometry;

    if (numVerts < 0 || numVerts >= GEOMETRY_VERTEXLIMIT || numTriangles < 0)
    {
        if (numVerts >= 0 && numVerts >= GEOMETRY_VERTEXLIMIT)
        {
            RwError err;

            err.pluginID = 2;
            err.errorCode = _rwerror(6);
            RwErrorSet(&err);
        }

        return NULL;
    }

    size = geometryTKList.sizeOfStruct;

    if (format & GEOMETRY_TEXCOORDSETSMASK)
    {
        numTexCoordSets = (format & GEOMETRY_TEXCOORDSETSMASK) >> 16;
    }
    else if (format & GEOMETRY_TEXTURED2)
    {
        numTexCoordSets = 2;
    }
    else
    {
        numTexCoordSets = (format & GEOMETRY_TEXTURED) >> 2;
    }

    if (numTexCoordSets == 1)
    {
        texFlags = GEOMETRY_TEXTURED;
    }
    else
    {
        texFlags = (numTexCoordSets > 1) ? GEOMETRY_TEXTURED2 : 0;
    }

    native = format & rpGEOMETRYNATIVE;
    flags = (format & GEOMETRY_FLAGSMASK) & ~(GEOMETRY_TEXTURED | GEOMETRY_TEXTURED2);
    flags |= texFlags;

    if (!native)
    {
        if (flags & GEOMETRY_PRELIT)
        {
            size += numVerts * sizeof(RwRGBA);
        }

        if (numTexCoordSets)
        {
            size += numTexCoordSets * (numVerts * sizeof(RwTexCoords));
        }

        size += numTriangles * sizeof(RpTriangle);
    }

    geometry = (RpGeometry*)RwMalloc(size);

    if (!geometry)
    {
        return NULL;
    }

    if (!_rpMaterialListInitialize(&geometry->matList))
    {
        return NULL;
    }

    geometry->morphTarget = NULL;
    geometry->numMorphTargets = 0;
    rwObjectInitialize(geometry, GEOMETRY_OBJECTTYPE, 0);
    geometry->repEntry = NULL;
    geometry->lockedSinceLastInst = 0;
    geometry->refCount = 1;
    geometry->mesh = NULL;
    geometry->numTexCoordSets = numTexCoordSets;
    memset(geometry->texCoords, 0, sizeof(geometry->texCoords));
    geometry->preLitLum = NULL;
    geometry->triangles = NULL;
    geometry->numTriangles = numTriangles;
    geometry->flags = flags | (format & GEOMETRY_NATIVEFLAGSMASK);
    geometry->numVertices = numVerts;

    if (!native)
    {
        U8* data = (U8*)geometry + geometryTKList.sizeOfStruct;
        U32 i;

        if ((flags & GEOMETRY_PRELIT) && numVerts)
        {
            geometry->preLitLum = (RwRGBA*)data;
            data += numVerts * sizeof(RwRGBA);
        }

        if (numTexCoordSets && numVerts)
        {
            for (i = 0; i < numTexCoordSets; i++)
            {
                geometry->texCoords[i] = (RwTexCoords*)data;
                data += numVerts * sizeof(RwTexCoords);
            }
        }

        if (numTriangles)
        {
            S32 j;

            geometry->triangles = (RpTriangle*)data;

            for (j = 0; j < numTriangles; j++)
            {
                geometry->triangles[j].matIndex = -1;
            }
        }
    }

    if (RpGeometryAddMorphTargets(geometry, 1) < 0)
    {
        _rpMaterialListDeinitialize(&geometry->matList);
        RwFree(geometry);

        return NULL;
    }

    geometry->ignoredSurfaceProps.ambient = 1.0f;
    geometry->ignoredSurfaceProps.diffuse = 1.0f;
    geometry->ignoredSurfaceProps.specular = 1.0f;

    _rwPluginRegistryInitObject(&geometryTKList, geometry);

    return geometry;
}

void _rpGeometryAddRef(RpGeometry* geometry)
{
    geometry->refCount++;
}

S32 RpGeometryDestroy(RpGeometry* geometry)
{
    geometry->refCount--;
    if (geometry->refCount <= 0)
    {
        if (geometry->repEntry)
        {
            RwResourcesFreeResEntry(geometry->repEntry);
        }

        geometry->refCount++;

        RpGeometryLock(geometry, GEOMLOCK_ALL);

        _rwPluginRegistryDeInitObject(&geometryTKList, geometry);

        if (geometry->morphTarget)
        {
            RwFree(geometry->morphTarget);
            geometry->morphTarget = NULL;
        }

        _rpMaterialListDeinitialize(&geometry->matList);

        geometry->refCount--;

        RwFree(geometry);
    }

    return 1;
}

S32 RpGeometryRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                             RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&geometryTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

S32 RpGeometryRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                   RwPluginDataChunkWriteCallBack writeCB,
                                   RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&geometryTKList, pluginID, readCB, writeCB,
                                            getSizeCB);
}

RpGeometry* RpGeometryStreamRead(RwStream* stream)
{
    U32 version;
    GeometryStreamData data;
    MorphTargetStreamData mtData;
    RpGeometry* geometry;
    S32 i;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, NULL, &version))
    {
        return NULL;
    }

    if (version < rwLIBRARYBASEVERSION || version > rwLIBRARYCURRENTVERSION)
    {
        RwError err;

        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);

        return NULL;
    }

    if (RwStreamRead(stream, &data, sizeof(data)) != sizeof(data))
    {
        return NULL;
    }

    RwMemNative32(&data, sizeof(data));
    RwMemFloat32ToReal(&data.surfaceProps, sizeof(data.surfaceProps));

    geometry = RpGeometryCreate(data.numVertices, data.numTriangles, data.format);

    if (!geometry)
    {
        return NULL;
    }

    geometry->ignoredSurfaceProps = data.surfaceProps;

    if (data.numMorphTargets > 1)
    {
        if (RpGeometryAddMorphTargets(geometry, data.numMorphTargets - 1) < 0)
        {
            RpGeometryDestroy(geometry);
            return NULL;
        }
    }

    if (!(geometry->flags & rpGEOMETRYNATIVE))
    {
        if (geometry->numVertices)
        {
            if (data.format & GEOMETRY_PRELIT)
            {
                U32 size = geometry->numVertices * sizeof(RwRGBA);

                if (RwStreamRead(stream, geometry->preLitLum, size) != size)
                {
                    RpGeometryDestroy(geometry);
                    return NULL;
                }
            }

            if (geometry->numTexCoordSets > 0)
            {
                U32 size = geometry->numVertices * sizeof(RwTexCoords);

                for (i = 0; i < geometry->numTexCoordSets; i++)
                {
                    if (!RwStreamReadReal(stream, (F32*)geometry->texCoords[i], size))
                    {
                        RpGeometryDestroy(geometry);
                        return NULL;
                    }
                }
            }

            if (geometry->numTriangles)
            {
                RpTriangle* triangle = geometry->triangles;
                S32 numTriangles = geometry->numTriangles;
                U32 size = numTriangles * sizeof(RpTriangle);

                if (RwStreamRead(stream, triangle, size) != size)
                {
                    RpGeometryDestroy(geometry);
                    return NULL;
                }

                RwMemNative32(triangle, size);

                while (numTriangles--)
                {
                    U32 word;

                    word = *(U32*)&triangle->vertIndex[0];
                    triangle->vertIndex[0] = (U16)(word >> 16);
                    triangle->vertIndex[1] = (U16)word;

                    word = *(U32*)&triangle->vertIndex[2];
                    triangle->vertIndex[2] = (U16)(word >> 16);
                    triangle->matIndex = (S16)word;

                    triangle++;
                }
            }
        }
    }

    for (i = 0; i < geometry->numMorphTargets; i++)
    {
        RpMorphTarget* morphTarget = &geometry->morphTarget[i];

        if (RwStreamRead(stream, &mtData, sizeof(mtData)) != sizeof(mtData))
        {
            RpGeometryDestroy(geometry);
            return NULL;
        }

        RwMemFloat32ToReal(&mtData.boundingSphere.center, sizeof(mtData.boundingSphere.center));
        RwMemFloat32ToReal(&mtData.boundingSphere.radius, sizeof(mtData.boundingSphere.radius));
        RwMemNative32(&mtData, sizeof(mtData));

        morphTarget->boundingSphere = mtData.boundingSphere;

        if (mtData.pointsPresent && mtData.normalsPresent)
        {
            if (!RwStreamReadReal(stream, (F32*)morphTarget->verts,
                                  geometry->numVertices * sizeof(RwV3d) * 2))
            {
                RpGeometryDestroy(geometry);
                return NULL;
            }
        }
        else
        {
            if (mtData.pointsPresent)
            {
                if (!RwStreamReadReal(stream, (F32*)morphTarget->verts,
                                      geometry->numVertices * sizeof(RwV3d)))
                {
                    RpGeometryDestroy(geometry);
                    return NULL;
                }
            }

            if (mtData.normalsPresent)
            {
                if (!RwStreamReadReal(stream, (F32*)morphTarget->normals,
                                      geometry->numVertices * sizeof(RwV3d)))
                {
                    RpGeometryDestroy(geometry);
                    return NULL;
                }
            }
        }
    }

    if (!RwStreamFindChunk(stream, rwID_MATLIST, NULL, &version))
    {
        return NULL;
    }

    if (version < rwLIBRARYBASEVERSION || version > rwLIBRARYCURRENTVERSION)
    {
        RpGeometryDestroy(geometry);
        {
            RwError err;

            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000004);
            RwErrorSet(&err);
        }

        return NULL;
    }

    _rpMaterialSetDefaultSurfaceProperties(&data.surfaceProps);

    if (!_rpMaterialListStreamRead(stream, &geometry->matList))
    {
        RpGeometryDestroy(geometry);
        return NULL;
    }

    _rpMaterialSetDefaultSurfaceProperties(NULL);

    if (!_rwPluginRegistryReadDataChunks(&geometryTKList, stream, geometry))
    {
        RpGeometryDestroy(geometry);
        return NULL;
    }

    if (!RpGeometryUnlock(geometry))
    {
        RpGeometryDestroy(geometry);
        return NULL;
    }

    return geometry;
}
