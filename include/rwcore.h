#ifndef RWCORE_H
#define RWCORE_H

#include "types.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

// RenderWare types get members once reconstructed code accesses them or a declared type
// embeds them; the others stay opaque.
typedef struct RwLLLink RwLLLink;
struct RwLLLink
{
    RwLLLink* next;
    RwLLLink* prev;
};

typedef struct RwLinkList RwLinkList;
struct RwLinkList
{
    RwLLLink link;
};

typedef struct RwObject RwObject;
struct RwObject
{
    U8 type;
    U8 subType;
    U8 flags;
    U8 privateFlags;
    void* parent;
};

typedef struct RwObjectHasFrame RwObjectHasFrame;
struct RwObjectHasFrame
{
    RwObject object;
    RwLLLink lFrame;
    RwObjectHasFrame* (*sync)(RwObjectHasFrame*);
};

typedef struct RwRGBAReal RwRGBAReal;
struct RwRGBAReal
{
    F32 red;
    F32 green;
    F32 blue;
    F32 alpha;
};

typedef struct RwV3d RwV3d;
struct RwV3d
{
    F32 x;
    F32 y;
    F32 z;
};

typedef struct RwBBox RwBBox;
struct RwBBox
{
    RwV3d sup;
    RwV3d inf;
};

typedef struct RwRGBA RwRGBA;
struct RwRGBA
{
    U8 red;
    U8 green;
    U8 blue;
    U8 alpha;
};

typedef enum RwFogType
{
    rwFOGTYPENAFOGTYPE = 0,
    rwFOGTYPELINEAR = 1,
    rwFOGTYPEEXPONENTIAL = 2,
    rwFOGTYPEEXPONENTIAL2 = 3,
    rwFOGTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwFogType;

typedef struct RwMatrixTag RwMatrixTag;
struct RwMatrixTag
{
    RwV3d right;
    U32 flags;
    RwV3d up;
    U32 pad1;
    RwV3d at;
    U32 pad2;
    RwV3d pos;
    U32 pad3;
};

typedef struct RwMatrixTag RwMatrix;
typedef struct RwCamera RwCamera;
typedef struct RwFrame RwFrame;
typedef struct RwRaster RwRaster;
typedef struct RwStream RwStream;

struct RwRaster
{
    RwRaster* parent;
    U8* cpPixels;
    U8* palette;
    S32 width;
    S32 height;
    S32 depth;
    S32 stride;
    S16 nOffsetX;
    S16 nOffsetY;
    U8 cType;
    U8 cFlags;
    U8 privateFlags;
    U8 cFormat;
    U8* originalPixels;
    S32 originalWidth;
    S32 originalHeight;
    S32 originalStride;
};

typedef struct RwV2d RwV2d;
struct RwV2d
{
    F32 x;
    F32 y;
};

typedef struct RwPlane RwPlane;
struct RwPlane
{
    RwV3d normal;
    F32 distance;
};

typedef struct RwFrustumPlane RwFrustumPlane;
struct RwFrustumPlane
{
    RwPlane plane;
    U8 closestX;
    U8 closestY;
    U8 closestZ;
    U8 pad;
};

typedef enum RwCameraProjection
{
    rwNACAMERAPROJECTION = 0,
    rwPERSPECTIVE = 1,
    rwPARALLEL = 2,
    rwCAMERAPROJECTIONFORCEENUMSIZEINT = 0x7FFFFFFF
} RwCameraProjection;

// Partial: the frustum bounding box and corners after frustumPlanes are not declared.
struct RwCamera
{
    RwObjectHasFrame object;
    RwCameraProjection projectionType;
    RwCamera* (*beginUpdate)(RwCamera*);
    RwCamera* (*endUpdate)(RwCamera*);
    RwMatrix viewMatrix;
    RwRaster* frameBuffer;
    RwRaster* zBuffer;
    RwV2d viewWindow;
    RwV2d recipViewWindow;
    RwV2d viewOffset;
    F32 nearPlane;
    F32 farPlane;
    F32 fogPlane;
    F32 zScale;
    F32 zShift;
    U16 renderFrame;
    U16 pad;
    RwFrustumPlane frustumPlanes[6];
    U8 unk110[0x78];
};

struct RwFrame
{
    RwObject object;
    RwLLLink inDirtyListLink;
    RwMatrix modelling;
    RwMatrix ltm;
    RwLinkList objectList;
    RwFrame* child;
    RwFrame* next;
    RwFrame* root;
};

typedef RwFrame* (*RwFrameCallBack)(RwFrame* frame, void* data);

typedef struct RwSphere RwSphere;
struct RwSphere
{
    RwV3d center;
    F32 radius;
};

typedef struct RwMemory RwMemory;
struct RwMemory
{
    U8* start;
    U32 length;
};

typedef enum RwStreamType
{
    rwNASTREAM = 0,
    rwSTREAMFILE,
    rwSTREAMFILENAME,
    rwSTREAMMEMORY,
    rwSTREAMCUSTOM,
    rwSTREAMTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwStreamType;

typedef enum RwStreamAccessType
{
    rwNASTREAMACCESS = 0,
    rwSTREAMREAD,
    rwSTREAMWRITE,
    rwSTREAMAPPEND,
    rwSTREAMACCESSTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwStreamAccessType;

typedef enum RwFrustumTestResult
{
    rwSPHEREOUTSIDE = 0,
    rwSPHEREBOUNDARY = 1,
    rwSPHEREINSIDE = 2,
    rwFRUSTUMTESTRESULTFORCEENUMSIZEINT = 0x7FFFFFFF
} RwFrustumTestResult;

#define rwID_CLUMP 0x10

#define rwMATRIXTYPEORTHONORMAL 0x00000003
#define rwMATRIXINTERNALIDENTITY 0x00020000

#define RwMatrixSetIdentityMacro(m)                                                                \
    do                                                                                             \
    {                                                                                              \
        (m)->right.x = (m)->up.y = (m)->at.z = (F32)((1.0));                                       \
        (m)->right.y = (m)->right.z = (m)->up.x = (F32)((0.0));                                    \
        (m)->up.z = (m)->at.x = (m)->at.y = (F32)((0.0));                                          \
        (m)->pos.x = (m)->pos.y = (m)->pos.z = (F32)((0.0));                                       \
        (m)->flags |= (rwMATRIXINTERNALIDENTITY | rwMATRIXTYPEORTHONORMAL);                        \
    } while (0)

#define _RW_S1 ((float)-1.6666667163e-01)
#define _RW_S2 ((float)8.3333337680e-03)
#define _RW_S3 ((float)-1.9841270114e-04)
#define _RW_S4 ((float)2.7557314297e-06)
#define _RW_S5 ((float)-2.5050759689e-08)
#define _RW_S6 ((float)1.5896910177e-10)

#define RwSinMinusPiToPiMacro(result, x)                                                           \
    do                                                                                             \
    {                                                                                              \
        const float z = x * x;                                                                     \
        const float v = z * x;                                                                     \
        const float r = (_RW_S2 + z * (_RW_S3 + z * (_RW_S4 + z * (_RW_S5 + z * _RW_S6))));        \
        result = x + v * (_RW_S1 + z * r);                                                         \
    } while (0)

#define RwV3dScaleMacro(o, a, s)                                                                   \
    do                                                                                             \
    {                                                                                              \
        (o)->x = (((a)->x) * ((s)));                                                               \
        (o)->y = (((a)->y) * ((s)));                                                               \
        (o)->z = (((a)->z) * ((s)));                                                               \
    } while (0)

#define RwV3dIncrementScaledMacro(o, a, s)                                                         \
    do                                                                                             \
    {                                                                                              \
        (o)->x += (((a)->x) * ((s)));                                                              \
        (o)->y += (((a)->y) * ((s)));                                                              \
        (o)->z += (((a)->z) * ((s)));                                                              \
    } while (0)

#define RwCameraGetCurrentCamera() ((RwCamera*)RWSRCGLOBAL(curCamera))

typedef enum RwCameraClearMode
{
    rwCAMERACLEARIMAGE = 0x1,
    rwCAMERACLEARZ = 0x2,
    rwCAMERACLEARSTENCIL = 0x4,
    rwCAMERACLEARMODEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwCameraClearMode;

typedef enum RwRasterType
{
    rwRASTERTYPENORMAL = 0x00,
    rwRASTERTYPEZBUFFER = 0x01,
    rwRASTERTYPECAMERA = 0x02,
    rwRASTERTYPETEXTURE = 0x04,
    rwRASTERTYPECAMERATEXTURE = 0x05,
    rwRASTERTYPEMASK = 0x07,
    rwRASTERDONTALLOCATE = 0x80,
    rwRASTERTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwRasterType;

typedef enum RwRenderState
{
    rwRENDERSTATENARENDERSTATE = 0,
    rwRENDERSTATETEXTURERASTER,
    rwRENDERSTATETEXTUREADDRESS,
    rwRENDERSTATETEXTUREADDRESSU,
    rwRENDERSTATETEXTUREADDRESSV,
    rwRENDERSTATETEXTUREPERSPECTIVE,
    rwRENDERSTATEZTESTENABLE,
    rwRENDERSTATESHADEMODE,
    rwRENDERSTATEZWRITEENABLE,
    rwRENDERSTATETEXTUREFILTER,
    rwRENDERSTATESRCBLEND,
    rwRENDERSTATEDESTBLEND,
    rwRENDERSTATEVERTEXALPHAENABLE,
    rwRENDERSTATEBORDERCOLOR,
    rwRENDERSTATEFOGENABLE,
    rwRENDERSTATEFOGCOLOR,
    rwRENDERSTATEFOGTYPE,
    rwRENDERSTATEFOGDENSITY,
    rwRENDERSTATECULLMODE = 20,
    rwRENDERSTATESTENCILENABLE,
    rwRENDERSTATESTENCILFAIL,
    rwRENDERSTATESTENCILZFAIL,
    rwRENDERSTATESTENCILPASS,
    rwRENDERSTATESTENCILFUNCTION,
    rwRENDERSTATESTENCILFUNCTIONREF,
    rwRENDERSTATESTENCILFUNCTIONMASK,
    rwRENDERSTATESTENCILFUNCTIONWRITEMASK,
    rwRENDERSTATEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwRenderState;

#define rwObjectGetParent(object) (((const RwObject*)(object))->parent)
#define rwObjectSetParent(c, p) (((RwObject*)(c))->parent) = (void*)(p)
#define RwFrameGetMatrix(_f) (&(_f)->modelling)
#define RwCameraGetFrame(_camera) ((RwFrame*)rwObjectGetParent((_camera)))
#define RwCameraSetFrame(_camera, _frame) \
    (_rwObjectHasFrameSetFrame((_camera), (_frame)), (_camera))
#define RwCameraGetRaster(_camera) ((_camera)->frameBuffer)
#define RwCameraSetRaster(_camera, _raster) (((_camera)->frameBuffer = (_raster)), (_camera))
#define RwCameraGetZRaster(_camera) ((_camera)->zBuffer)
#define RwCameraSetZRaster(_camera, _raster) (((_camera)->zBuffer = (_raster)), (_camera))
#define RwCameraSetFogDistance(_camera, _distance) \
    (((_camera)->fogPlane = (_distance)), (_camera))

typedef struct RwError RwError;
struct RwError
{
    S32 pluginID;
    S32 errorCode;
};

typedef struct RwModuleInfo RwModuleInfo;
struct RwModuleInfo
{
    S32 globalsOffset;
    S32 numInstances;
};

typedef struct RwMemoryFunctions RwMemoryFunctions;
struct RwMemoryFunctions
{
    void* (*rwmalloc)(U32 size);
    void (*rwfree)(void* mem);
    void* (*rwrealloc)(void* mem, U32 newSize);
    void* (*rwcalloc)(U32 numObj, U32 sizeObj);
};

typedef struct RwFileFunctions RwFileFunctions;
struct RwFileFunctions
{
    S32 (*rwfexist)(const char* name);
    void* (*rwfopen)(const char* name, const char* access);
    S32 (*rwfclose)(void* fptr);
    U32 (*rwfread)(void* addr, U32 size, U32 count, void* fptr);
    U32 (*rwfwrite)(const void* addr, U32 size, U32 count, void* fptr);
    char* (*rwfgets)(char* buffer, S32 maxLen, void* fptr);
    S32 (*rwfputs)(const char* buffer, void* fptr);
    S32 (*rwfeof)(void* fptr);
    S32 (*rwfseek)(void* fptr, S32 offset, S32 origin);
    S32 (*rwfflush)(void* fptr);
    S32 (*rwftell)(void* fptr);
};

typedef struct RwSurfaceProperties RwSurfaceProperties;
struct RwSurfaceProperties
{
    F32 ambient;
    F32 specular;
    F32 diffuse;
};

typedef struct RwTexDictionary RwTexDictionary;
struct RwTexDictionary
{
    RwObject object;
    RwLinkList texturesInDict;
    RwLLLink lInInstance;
};

// Partial: the name, mask and filtering members are not declared; the GameCube layout has
// refCount at +0x54.
typedef struct RwTexture RwTexture;
struct RwTexture
{
    RwRaster* raster;
    RwTexDictionary* dict;
    RwLLLink lInDictionary;
    U8 unk10[0x44];
    S32 refCount;
};

typedef struct RwImage RwImage;
struct RwImage
{
    S32 flags;
    S32 width;
    S32 height;
    S32 depth;
    S32 stride;
    U8* cpPixels;
    RwRGBA* palette;
};

typedef RwTexture* (*RwTextureCallBackRead)(const char* name, const char* maskName);

// Partial: only entrySize is declared.
typedef struct RwFreeList RwFreeList;
struct RwFreeList
{
    U8 unk0[0x10];
    S32 entrySize;
};

typedef void* (*RwMemoryAllocFn)(RwFreeList* fl);
typedef RwFreeList* (*RwMemoryFreeFn)(RwFreeList* fl, void* pData);

typedef struct RwPluginRegEntry RwPluginRegEntry;

// Partial: only the object sizes and the first entry are declared.
typedef struct RwPluginRegistry RwPluginRegistry;
struct RwPluginRegistry
{
    S32 sizeOfStruct;
    S32 origSizeOfStruct;
    U8 unk8[0x8];
    RwPluginRegEntry* firstRegEntry;
    U8 unk14[0x4];
};

typedef void* (*RwPluginObjectConstructor)(void* object, S32 offsetInObject, S32 sizeInObject);
typedef void* (*RwPluginObjectDestructor)(void* object, S32 offsetInObject, S32 sizeInObject);
typedef void* (*RwPluginObjectCopy)(void* dstObject, const void* srcObject, S32 offsetInObject,
                                    S32 sizeInObject);
typedef RwStream* (*RwPluginDataChunkReadCallBack)(RwStream* stream, S32 binaryLength,
                                                   void* object, S32 offsetInObject,
                                                   S32 sizeInObject);
typedef RwStream* (*RwPluginDataChunkWriteCallBack)(RwStream* stream, S32 binaryLength,
                                                    const void* object, S32 offsetInObject,
                                                    S32 sizeInObject);
typedef S32 (*RwPluginDataChunkGetSizeCallBack)(const void* object, S32 offsetInObject,
                                                 S32 sizeInObject);
typedef S32 (*RwPluginDataChunkAlwaysCallBack)(void* object, S32 offsetInObject,
                                               S32 sizeInObject);
typedef S32 (*RwPluginDataChunkRightsCallBack)(void* object, S32 offsetInObject,
                                               S32 sizeInObject, U32 extraData);

// Partial: the object callbacks between rightsCB and nextRegEntry are not declared.
struct RwPluginRegEntry
{
    S32 offset;
    S32 size;
    U32 pluginID;
    RwPluginDataChunkReadCallBack readCB;
    RwPluginDataChunkWriteCallBack writeCB;
    RwPluginDataChunkGetSizeCallBack getSizeCB;
    RwPluginDataChunkAlwaysCallBack alwaysCB;
    RwPluginDataChunkRightsCallBack rightsCB;
    U8 unk20[0x10];
    RwPluginRegEntry* nextRegEntry;
};

typedef enum RwPrimitiveType
{
    rwPRIMTYPENAPRIMTYPE = 0,
    rwPRIMTYPELINELIST = 1,
    rwPRIMTYPEPOLYLINE = 2,
    rwPRIMTYPETRILIST = 3,
    rwPRIMTYPETRISTRIP = 4,
    rwPRIMTYPETRIFAN = 5,
    rwPRIMTYPEPOINTLIST = 6,
    rwPRIMITIVETYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RwPrimitiveType;

typedef struct RwIm2DVertex RwIm2DVertex;
typedef U16 RwImVertexIndex;

typedef S32 (*RwStandardFunc)(void* pOut, void* pInOut, S32 nI);
typedef S32 (*RwSystemFunc)(S32 nOption, void* pOut, void* pInOut, S32 nIn);
typedef S32 (*RwRenderStateSetFunction)(RwRenderState nState, void* pParam);
typedef S32 (*RwRenderStateGetFunction)(RwRenderState nState, void* pParam);
typedef S32 (*RwIm2DRenderLineFunction)(RwIm2DVertex* vertices, S32 numVertices, S32 vert1,
                                        S32 vert2);
typedef S32 (*RwIm2DRenderTriangleFunction)(RwIm2DVertex* vertices, S32 numVertices, S32 vert1,
                                            S32 vert2, S32 vert3);
typedef S32 (*RwIm2DRenderPrimitiveFunction)(RwPrimitiveType primType, RwIm2DVertex* vertices,
                                             S32 numVertices);
typedef S32 (*RwIm2DRenderIndexedPrimitiveFunction)(RwPrimitiveType primType,
                                                    RwIm2DVertex* vertices, S32 numVertices,
                                                    RwImVertexIndex* indices, S32 numIndices);

// Partial: the immediate-mode 3D entries after the 2D ones are not declared.
typedef struct RwDevice RwDevice;
struct RwDevice
{
    F32 gammaCorrection;
    RwSystemFunc fpSystem;
    F32 zBufferNear;
    F32 zBufferFar;
    RwRenderStateSetFunction fpRenderStateSet;
    RwRenderStateGetFunction fpRenderStateGet;
    RwIm2DRenderLineFunction fpIm2DRenderLine;
    RwIm2DRenderTriangleFunction fpIm2DRenderTriangle;
    RwIm2DRenderPrimitiveFunction fpIm2DRenderPrimitive;
    RwIm2DRenderIndexedPrimitiveFunction fpIm2DRenderIndexedPrimitive;
    U8 unk28[0x10];
};

// Partial: only the members reconstructed code reaches are declared.
typedef struct RwGlobals RwGlobals;
struct RwGlobals
{
    void* curCamera;
    void* curWorld;
    U16 renderFrame;
    U16 lightFrame;
    U16 pad[2];
    RwDevice dOpenDevice;
    RwStandardFunc stdFunc[29];
    U8 unkBC[0x8];
    RwFileFunctions fileFuncs;
    U8 unkF0[0x40];
    RwMemoryFunctions memoryFuncs;
    RwMemoryAllocFn memoryAlloc;
    RwMemoryFreeFn memoryFree;
    U8 unk148[0xC];
};

#define RWSRCGLOBAL(var) (((RwGlobals*)RwEngineInstance)->var)

#ifdef __cplusplus
extern "C" {
#endif

extern void* RwEngineInstance;

RwCamera* RwCameraCreate();
S32 RwCameraDestroy(RwCamera* camera);
RwCamera* RwCameraBeginUpdate(RwCamera* camera);
RwCamera* RwCameraEndUpdate(RwCamera* camera);
RwCamera* RwCameraClear(RwCamera* camera, RwRGBA* colour, S32 clearMode);
RwCamera* RwCameraShowRaster(RwCamera* camera, void* pDev, U32 flags);
RwCamera* RwCameraSetNearClipPlane(RwCamera* camera, F32 nearClip);
RwCamera* RwCameraSetFarClipPlane(RwCamera* camera, F32 farClip);
RwCamera* RwCameraSetViewWindow(RwCamera* camera, const RwV2d* viewWindow);
S32 RwCameraRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                           RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
void _rwObjectHasFrameSetFrame(void* object, RwFrame* frame);
RwRaster* RwRasterCreate(S32 width, S32 height, S32 depth, S32 flags);
S32 RwRasterDestroy(RwRaster* raster);
S32 RwRenderStateSet(RwRenderState state, void* value);
RwMatrix* RwMatrixUpdate(RwMatrix* matrix);
RwFrame* RwFrameCreate();
S32 RwFrameDestroy(RwFrame* frame);
RwFrame* RwFrameOrthoNormalize(RwFrame* frame);
RwFrame* RwFrameForAllChildren(RwFrame* frame, RwFrameCallBack callBack, void* data);
RwFrame* RwFrameGetRoot(const RwFrame* frame);
S32 RwFrameDestroyHierarchy(RwFrame* frame);
RwMatrix* RwMatrixMultiply(RwMatrix* matrixOut, const RwMatrix* MatrixIn1, const RwMatrix* matrixIn2);
RwV3d* RwV3dTransformPoints(RwV3d* pointsOut, const RwV3d* pointsIn, S32 numPoints,
                            const RwMatrix* matrix);
RwFrustumTestResult RwCameraFrustumTestSphere(const RwCamera* camera, const RwSphere* sphere);
RwStream* RwStreamOpen(RwStreamType type, RwStreamAccessType accessType, const void* pData);
S32 RwStreamClose(RwStream* stream, void* pData);
S32 RwStreamFindChunk(RwStream* stream, U32 type, U32* lengthOut, U32* versionOut);
void _rwFrameSyncDirty();

RwError* RwErrorSet(RwError* code);
S32 _rwerror(S32 code, ...);

RwFreeList* RwFreeListCreate(S32 entrySize, S32 entriesPerBlock, S32 alignment);
S32 RwFreeListDestroy(RwFreeList* freeList);
RwFreeList* RwFreeListForAllUsed(RwFreeList* freeList, void (*callBack)(void* mem, void* data),
                                 void* data);

S32 _rwPluginRegistryAddPlugin(RwPluginRegistry* reg, S32 size, U32 pluginID,
                               RwPluginObjectConstructor constructCB,
                               RwPluginObjectDestructor destructCB,
                               RwPluginObjectCopy copyCB);
S32 _rwPluginRegistryAddPluginStream(RwPluginRegistry* reg, U32 pluginID,
                                     RwPluginDataChunkReadCallBack readCB,
                                     RwPluginDataChunkWriteCallBack writeCB,
                                     RwPluginDataChunkGetSizeCallBack getSizeCB);
S32 _rwPluginRegistryAddPlgnStrmlwysCB(RwPluginRegistry* reg, U32 pluginID,
                                       RwPluginDataChunkAlwaysCallBack alwaysCB);
S32 _rwPluginRegistryAddPlgnStrmRightsCB(RwPluginRegistry* reg, U32 pluginID,
                                         RwPluginDataChunkRightsCallBack rightsCB);
S32 _rwPluginRegistryGetPluginOffset(const RwPluginRegistry* reg, U32 pluginID);
const RwPluginRegistry* _rwPluginRegistryInitObject(const RwPluginRegistry* reg, void* object);
const RwPluginRegistry* _rwPluginRegistryDeInitObject(const RwPluginRegistry* reg, void* object);

void _rwObjectHasFrameReleaseFrame(void* object);

S32 RwTextureDestroy(RwTexture* texture);

RwFrame* RwFrameUpdateObjects(RwFrame* frame);

F32 _rwSqrt(F32 num);
F32 _rwInvSqrt(F32 num);

#ifdef __cplusplus
}
#endif

#endif
