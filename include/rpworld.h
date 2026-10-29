#ifndef RPWORLD_H
#define RPWORLD_H

#include <rwcore.h>

// RenderWare types get members once reconstructed code accesses them or a declared type
// embeds them; the others stay opaque.
typedef struct RpAtomic RpAtomic;
typedef struct RpSector RpSector;
typedef struct RpPolygon RpPolygon;
typedef struct RpVertexNormal RpVertexNormal;
typedef struct RwTexCoords RwTexCoords;
struct RwTexCoords
{
    F32 u;
    F32 v;
};
typedef struct RpCollSector RpCollSector;
typedef struct RpMeshHeader RpMeshHeader;

struct RpMeshHeader
{
    U32 flags;
    U16 numMeshes;
    U16 serialNum;
    U32 totalIndicesInMesh;
    U32 firstMeshOffset;
};

typedef struct RpMaterial RpMaterial;

typedef struct RpMesh RpMesh;
struct RpMesh
{
    U16* indices;
    U32 numIndices;
    RpMaterial* material;
};

typedef struct RpBuildMeshTriangle RpBuildMeshTriangle;
struct RpBuildMeshTriangle
{
    U16 vertIndex[3];
    RpMaterial* material;
};

typedef struct RpBuildMesh RpBuildMesh;
struct RpBuildMesh
{
    U32 triangleBufferSize;
    U32 numTriangles;
    RpBuildMeshTriangle* meshTriangles;
};

typedef RpMesh* (*RpMeshCallBack)(RpMesh* mesh, RpMeshHeader* meshHeader, void* pData);
typedef RpMaterial* (*RpMaterialCallBack)(RpMaterial* material, void* data);
struct RpMaterial
{
    RwTexture* texture;
    RwRGBA color;
    RxPipeline* pipeline;
    RwSurfaceProperties surfaceProps;
    S16 refCount;
    S16 pad;
};

struct RpSector
{
    S32 type;
};

struct RpPolygon
{
    U16 matIndex;
    U16 vertIndex[3];
};

#define rpWORLD 7

// The object type of a geometry.
#define GEOMETRY_OBJECTTYPE 8

typedef struct RpPlaneSector RpPlaneSector;
struct RpPlaneSector
{
    S32 type;
    F32 value;
    RpSector* leftSubTree;
    RpSector* rightSubTree;
    F32 leftValue;
    F32 rightValue;
};

typedef struct RpWorldSector RpWorldSector;
struct RpWorldSector
{
    S32 type;
    RpPolygon* polygons;
    RwV3d* vertices;
    RpVertexNormal* normals;
    RwTexCoords* texCoords[8];
    RwRGBA* preLitLum;
    RwResEntry* repEntry;
    RwLinkList collAtomicsInWorldSector;
    RwLinkList noCollAtomicsInWorldSector;
    RwLinkList lightsInWorldSector;
    RwBBox boundingBox;
    RwBBox tightBoundingBox;
    RpCollSector* colSectorRoot;
    RpMeshHeader* mesh;
    RxPipeline* pipeline;
    U16 matListWindowBase;
    U16 numVertices;
    U16 numPolygons;
    U16 pad;
};

typedef RpWorldSector* (*RpWorldSectorCallBackRender)(RpWorldSector* worldSector);
typedef RpWorldSector* (*RpWorldSectorCallBack)(RpWorldSector* worldSector, void* data);

typedef enum RpWorldRenderOrder
{
    rpWORLDRENDERNARENDERORDER = 0,
    rpWORLDRENDERFRONT2BACK = 1,
    rpWORLDRENDERBACK2FRONT = 2,
    rpWORLDRENDERORDERFORCEENUMSIZEINT = 0x7FFFFFFF
} RpWorldRenderOrder;

typedef struct RpMaterialList RpMaterialList;
struct RpMaterialList
{
    RpMaterial** materials;
    S32 numMaterials;
    S32 space;
};

typedef struct RpWorld RpWorld;
struct RpWorld
{
    RwObject object;
    U32 flags;
    RpWorldRenderOrder renderOrder;
    RpMaterialList matList;
    RpSector* rootSector;
    S32 numTexCoordSets;
    S32 numClumpsInWorld;
    RwLLLink* currentClumpLink;
    RwLinkList clumpList;
    RwLinkList lightList;
    RwLinkList directionalLightList;
    RwV3d worldOrigin;
    RwBBox boundingBox;
    RwSurfaceProperties surfaceProps;
    RpWorldSectorCallBackRender renderCallBack;
    RxPipeline* pipeline;
};

typedef struct RpGeometry RpGeometry;
typedef struct RpClump RpClump;
typedef struct RpTriangle RpTriangle;
struct RpTriangle
{
    U16 vertIndex[3];
    S16 matIndex;
};

typedef enum RpInterpolatorFlag
{
    rpINTERPOLATORDIRTYINSTANCE = 0x01,
    rpINTERPOLATORDIRTYSPHERE = 0x02,
    rpINTERPOLATORNOFRAMEDIRTY = 0x04,
    rpINTERPOLATORFLAGFORCEENUMSIZEINT = 0x7FFFFFFF
} RpInterpolatorFlag;

typedef struct RpInterpolator RpInterpolator;
struct RpInterpolator
{
    S32 flags;
    S16 startMorphTarget;
    S16 endMorphTarget;
    F32 time;
    F32 recipTime;
    F32 position;
};

typedef struct RpMorphTarget RpMorphTarget;
struct RpMorphTarget
{
    RpGeometry* parentGeom;
    RwSphere boundingSphere;
    RwV3d* verts;
    RwV3d* normals;
};

struct RpGeometry
{
    RwObject object;
    U32 flags;
    U16 lockedSinceLastInst;
    S16 refCount;
    S32 numTriangles;
    S32 numVertices;
    S32 numMorphTargets;
    S32 numTexCoordSets;
    RpMaterialList matList;
    RpTriangle* triangles;
    RwRGBA* preLitLum;
    RwTexCoords* texCoords[8];
    RwSurfaceProperties ignoredSurfaceProps;
    RpMeshHeader* mesh;
    RwResEntry* repEntry;
    RpMorphTarget* morphTarget;
};

typedef RpAtomic* (*RpAtomicCallBackRender)(RpAtomic* atomic);
typedef RpAtomic* (*RpAtomicCallBack)(RpAtomic* atomic, void* data);

#define rpATOMICCOLLISIONTEST 0x01
#define rpATOMICRENDER 0x04

#define rpGEOMETRYTRISTRIP 0x0001
#define rpGEOMETRYTEXTURED 0x0004
#define rpGEOMETRYPRELIT 0x0008
#define rpGEOMETRYNORMALS 0x0010
#define rpGEOMETRYTEXTURED2 0x0080
#define rpGEOMETRYNATIVE 0x01000000
#define rpGEOMETRYFLAGSMASK 0x00FF
#define rpGEOMETRYNATIVEFLAGSMASK 0x0F000000

#define rpWORLDTRISTRIP 0x01
#define rpWORLDNATIVE 0x01000000
#define rpWORLDSINGLEMALLOC 0x01

struct RpAtomic
{
    RwObjectHasFrame object;
    RwResEntry* repEntry;
    RpGeometry* geometry;
    RwSphere boundingSphere;
    RwSphere worldBoundingSphere;
    RpClump* clump;
    RwLLLink inClumpLink;
    RpAtomicCallBackRender renderCallBack;
    RpInterpolator interpolator;
    U16 renderFrame;
    U16 pad;
    RwLinkList llWorldSectorsInAtomic;
    RxPipeline* pipeline;
};

typedef RpClump* (*RpClumpCallBack)(RpClump* clump, void* data);

// Partial: the GameCube layout has the light and camera lists the PS2 DWARF lacks; the member
// after the callback is not declared.
struct RpClump
{
    RwObject object;
    RwLinkList atomicList;
    RwLinkList lightList;
    RwLinkList cameraList;
    RwLLLink inWorldLink;
    RpClumpCallBack callback;
    U8 unk2C[0x4];
};

#define RpAtomicGetFrame(_atomic) ((RwFrame*)rwObjectGetParent(_atomic))
#define RpClumpGetFrame(_clump) ((RwFrame*)rwObjectGetParent(_clump))
#define RpClumpSetFrame(_clump, _frame) (rwObjectSetParent(_clump, _frame), (_clump))

typedef struct RpLight RpLight;
struct RpLight
{
    RwObjectHasFrame object;
    F32 radius;
    RwRGBAReal color;
    F32 minusCosAngle;
    RwLinkList WorldSectorsInLight;
    RwLLLink inWorld;
    U16 lightFrame;
    U16 pad;
};

// Links an atomic into a sector it touches: one link in the sector's atomic list, one in the
// atomic's sector list.
typedef struct WorldTie WorldTie;
struct WorldTie
{
    RwLLLink sectorLink;
    RpAtomic* atomic;
    RwLLLink atomicLink;
    RpWorldSector* sector;
};

// Links a light into a sector it reaches, the same way.
typedef struct LightTie LightTie;
struct LightTie
{
    RwLLLink sectorLink;
    RpLight* light;
    RwLLLink lightLink;
    RpWorldSector* sector;
};

#define rpLIGHTPOSITIONINGSTART 0x80

typedef enum RpLightType
{
    rpNALIGHTTYPE = 0,
    rpLIGHTDIRECTIONAL,
    rpLIGHTAMBIENT,
    rpLIGHTPOINT = rpLIGHTPOSITIONINGSTART,
    rpLIGHTSPOT,
    rpLIGHTSPOTSOFT,
    rpLIGHTTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RpLightType;

#define RpLightGetType(_light) ((RpLightType)rwObjectGetSubType((_light)))
#define RpLightGetFrame(_light) ((RwFrame*)rwObjectGetParent((_light)))
#define RpLightSetFrame(_light, _frame) (_rwObjectHasFrameSetFrame((_light), (_frame)), (_light))
#define RpLightSetFlags(_light, _flags) ((((RwObject*)(_light))->flags = (U8)(_flags)), (_light))

enum RpLightFlag
{
    rpLIGHTLIGHTATOMICS = 0x01,
    rpLIGHTLIGHTWORLD = 0x02,
    rpLIGHTFLAGFORCEENUMSIZEINT = 0x7FFFFFFF
};

typedef RwCamera* (*RwCameraCallBack)(RwCamera* camera, void* data);
typedef RpLight* (*RpLightCallBack)(RpLight* light, void* data);

#ifdef __cplusplus
extern "C" {
#endif

RpLight* RpLightCreate(S32 type);
S32 RpLightDestroy(RpLight* light);
RpLight* RpLightSetRadius(RpLight* light, F32 radius);
RpLight* RpLightSetColor(RpLight* light, const RwRGBAReal* color);
RpLight* RpLightSetConeAngle(RpLight* light, F32 angle);
RpLight* RpLightStreamRead(RwStream* stream);
S32 RpLightRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpLightRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                RwPluginDataChunkWriteCallBack writeCB,
                                RwPluginDataChunkGetSizeCallBack getSizeCB);
S32 RpMaterialRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                             RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpMaterialRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                   RwPluginDataChunkWriteCallBack writeCB,
                                   RwPluginDataChunkGetSizeCallBack getSizeCB);

RpGeometry* RpGeometryLock(RpGeometry* geometry, S32 lockMode);
RpGeometry* RpGeometryUnlock(RpGeometry* geometry);
S32 RpGeometryDestroy(RpGeometry* geometry);
void _rpGeometryAddRef(RpGeometry* geometry);
RpGeometry* RpGeometryStreamRead(RwStream* stream);

RpAtomic* RpAtomicCreate(void);
RpAtomic* RpAtomicSetFrame(RpAtomic* atomic, RwFrame* frame);
RpAtomic* RpAtomicSetGeometry(RpAtomic* atomic, RpGeometry* geometry, U32 flags);
S32 RpAtomicDestroy(RpAtomic* atomic);
RpWorld* RpAtomicGetWorld(const RpAtomic* atomic);
const RwSphere* RpAtomicGetWorldBoundingSphere(RpAtomic* atomic);
S32 RpAtomicRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                           RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpAtomicRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                 RwPluginDataChunkWriteCallBack writeCB,
                                 RwPluginDataChunkGetSizeCallBack getSizeCB);
S32 RpAtomicSetStreamAlwaysCallBack(U32 pluginID, RwPluginDataChunkAlwaysCallBack alwaysCB);
S32 RpAtomicSetStreamRightsCallBack(U32 pluginID, RwPluginDataChunkRightsCallBack rightsCB);
S32 RpAtomicGetPluginOffset(U32 pluginID);
S32 RpClumpRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpGeometryRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                             RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpGeometryRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                   RwPluginDataChunkWriteCallBack writeCB,
                                   RwPluginDataChunkGetSizeCallBack getSizeCB);
S32 RpWorldSectorRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                                RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpWorldSectorRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                      RwPluginDataChunkWriteCallBack writeCB,
                                      RwPluginDataChunkGetSizeCallBack getSizeCB);
S32 RpWorldRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RpWorldRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                RwPluginDataChunkWriteCallBack writeCB,
                                RwPluginDataChunkGetSizeCallBack getSizeCB);

S32 RpClumpDestroy(RpClump* clump);
RpClump* RpClumpAddAtomic(RpClump* clump, RpAtomic* atomic);
RpClump* RpClumpRemoveAtomic(RpClump* clump, RpAtomic* atomic);
RpClump* RpClumpForAllAtomics(RpClump* clump, RpAtomicCallBack callback, void* pData);
RpClump* RpClumpForAllCameras(RpClump* clump, RwCameraCallBack callback, void* pData);
RpClump* RpClumpForAllLights(RpClump* clump, RpLightCallBack callback, void* pData);
RpClump* RpClumpRemoveLight(RpClump* clump, RpLight* light);
RpClump* RpClumpRemoveCamera(RpClump* clump, RwCamera* camera);
RpClump* RpClumpStreamRead(RwStream* stream);

RpWorld* RpWorldAddCamera(RpWorld* world, RwCamera* camera);
RpWorld* RpWorldRemoveCamera(RpWorld* world, RwCamera* camera);
RpWorld* RpWorldAddLight(RpWorld* world, RpLight* light);
RpWorld* RpWorldRemoveLight(RpWorld* world, RpLight* light);
S32 RpWorldDestroy(RpWorld* world);

extern RwPluginRegistry sectorTKList;

RpWorldSector* RpWorldSectorRender(RpWorldSector* worldSector);
RpWorld* RpWorldForAllWorldSectors(RpWorld* world, RpWorldSectorCallBack fpCallBack, void* pData);
void* _rpMaterialOpen(void* instance, S32 offset, S32 size);
void* _rpMaterialClose(void* instance, S32 offset, S32 size);
void* _rpMeshOpen(void* instance, S32 offset, S32 size);
void* _rpMeshClose(void* instance, S32 offset, S32 size);
void* _rpGeometryOpen(void* instance, S32 offset, S32 size);
void* _rpGeometryClose(void* instance, S32 offset, S32 size);
void* _rpClumpOpen(void* instance, S32 offset, S32 size);
void* _rpClumpClose(void* instance, S32 offset, S32 size);
void* _rpLightOpen(void* instance, S32 offset, S32 size);
void* _rpLightClose(void* instance, S32 offset, S32 size);
void* _rpSectorOpen(void* instance, S32 offset, S32 size);
void* _rpSectorClose(void* instance, S32 offset, S32 size);
void* _rpBinaryWorldOpen(void* instance, S32 offset, S32 size);
void* _rpBinaryWorldClose(void* instance, S32 offset, S32 size);
S32 _rpWorldObjRegisterExtensions(void);
S32 _rpClumpRegisterExtensions(void);
S32 _rpTieDestroy(WorldTie* tie);
S32 _rpLightTieDestroy(LightTie* tie);
RpMaterialList* _rpMaterialListInitialize(RpMaterialList* matList);
RpMaterialList* _rpMaterialListDeinitialize(RpMaterialList* matList);
RpBuildMesh* _rpBuildMeshCreate(U32 bufferSize);
S32 _rpBuildMeshDestroy(RpBuildMesh* mesh);
RpBuildMesh* _rpBuildMeshAddTriangle(RpBuildMesh* mesh, RpMaterial* material, S32 vert1,
                                     S32 vert2, S32 vert3);
RpMeshHeader* _rpMeshOptimise(RpBuildMesh* buildmesh, U32 flags);
RpMaterial* _rpMaterialListGetMaterial(const RpMaterialList* matList, S32 matIndex);
RpMaterialList* _rpMaterialListStreamRead(RwStream* stream, RpMaterialList* matList);
void _rpMaterialSetDefaultSurfaceProperties(const RwSurfaceProperties* surfaceProps);
S32 _rpMaterialListFindMaterialIndex(const RpMaterialList* matList, const RpMaterial* material);
RpMeshHeader* _rpMeshHeaderCreate(U32 size);
S32 _rpMeshDestroy(RpMeshHeader* mesh);
RwStream* _rpMeshWrite(const RpMeshHeader* meshHeader, const void* object, RwStream* stream,
                       const RpMaterialList* matList);
RpMeshHeader* _rpMeshRead(RwStream* stream, const void* object, const RpMaterialList* matList);
S32 _rpMeshSize(const RpMeshHeader* meshHeader, const void* object);

RxPipeline* RpWorldSetDefaultSectorPipeline(RxPipeline* pipeline);
RxPipeline* RpAtomicSetDefaultPipeline(RxPipeline* pipeline);
void _rpWorldPipelineClose(void);
S32 _rpWorldPipelineOpen(void);
S32 _rpWorldPipeAttach(void);
S32 _rxWorldDevicePluginAttach(void);
S32 _rpCreatePlatformMaterialPipelines(void);
void _rpDestroyPlatformMaterialPipelines(void);
S32 _rpCreatePlatformWorldSectorPipelines(void);
void _rpDestroyPlatformWorldSectorPipelines(void);
S32 _rpCreatePlatformAtomicPipelines(void);
void _rpDestroyPlatformAtomicPipelines(void);

RxNodeDefinition* RxNodeDefinitionGetLight(void);
RxNodeDefinition* RxNodeDefinitionGetPostLight(void);
RxNodeDefinition* RxNodeDefinitionGetAtomicInstance(void);
RxNodeDefinition* RxNodeDefinitionGetAtomicEnumerateLights(void);
RxNodeDefinition* RxNodeDefinitionGetMaterialScatter(void);
RxNodeDefinition* RxNodeDefinitionGetWorldSectorInstance(void);
RxNodeDefinition* RxNodeDefinitionGetWorldSectorEnumerateLights(void);
RxNodeDefinition* RxNodeDefinitionGetGameCubeAtomicAllInOne(void);
RxNodeDefinition* RxNodeDefinitionGetGameCubeWorldSectorAllInOne(void);

#ifdef __cplusplus
}
#endif

#endif
