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

#define rwLLLinkGetNext(linkvar) ((linkvar)->next)
#define rwLinkListInitialize(list)                                                                 \
    ((list)->link.next = ((RwLLLink*)(list)), (list)->link.prev = ((RwLLLink*)(list)))
#define rwLinkListEmpty(list) (((list)->link.next) == (&(list)->link))
#define rwLinkListGetFirstLLLink(list) ((list)->link.next)
#define rwLinkListGetTerminator(list) (&((list)->link))
#define rwLinkListAddLLLink(list, linkvar)                                                         \
    ((linkvar)->next = (list)->link.next, (linkvar)->prev = (&(list)->link),                       \
     ((list)->link.next)->prev = (linkvar), (list)->link.next = (linkvar))
#define rwLinkListRemoveLLLink(linkvar)                                                            \
    (((linkvar)->prev)->next = (linkvar)->next, ((linkvar)->next)->prev = (linkvar)->prev)

typedef struct RwResEntry RwResEntry;
typedef void (*RwResEntryDestroyNotify)(RwResEntry* resEntry);
struct RwResEntry
{
    RwLLLink link;
    S32 size;
    void* owner;
    RwResEntry** ownerRef;
    RwResEntryDestroyNotify destroyNotify;
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

typedef RwObject* (*RwObjectCallBack)(RwObject* object, void* data);

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

typedef struct RwMatrixTolerance RwMatrixTolerance;
struct RwMatrixTolerance
{
    F32 Normal;
    F32 Orthogonal;
    F32 Identity;
};

typedef struct RwCamera RwCamera;
typedef struct RwFrame RwFrame;
typedef struct RwRaster RwRaster;
typedef struct RwStream RwStream;

#define rwRASTERGAMMACORRECTED 0x01

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

typedef union RwSplitBits RwSplitBits;
union RwSplitBits
{
    F32 nReal;
    volatile S32 nInt;
    volatile U32 nUInt;
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
    RwBBox frustumBoundBox;
    U8 unk128[0x60];
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

#define rwID_STRUCT 0x01
#define rwID_STRING 0x02
#define rwID_EXTENSION 0x03
#define rwID_CAMERA 0x05
#define rwID_TEXTURE 0x06
#define rwID_MATERIAL 0x07
#define rwID_MATLIST 0x08
#define rwID_ATOMICSECT 0x09
#define rwID_PLANESECT 0x0A
#define rwID_WORLD 0x0B
#define rwID_MATRIX 0x0D
#define rwID_FRAMELIST 0x0E
#define rwID_GEOMETRY 0x0F
#define rwID_CLUMP 0x10
#define rwID_LIGHT 0x12
#define rwID_UNICODESTRING 0x13
#define rwID_ATOMIC 0x14
#define rwID_GEOMETRYLIST 0x1A
#define rwID_RIGHTTORENDER 0x1f

#define rwLIBRARYBASEVERSION 0x31000
#define rwLIBRARYCURRENTVERSION 0x33002

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

typedef enum RwRasterFormat
{
    rwRASTERFORMATDEFAULT = 0x0000,
    rwRASTERFORMAT1555 = 0x0100,
    rwRASTERFORMAT565 = 0x0200,
    rwRASTERFORMAT4444 = 0x0300,
    rwRASTERFORMATLUM8 = 0x0400,
    rwRASTERFORMAT8888 = 0x0500,
    rwRASTERFORMAT888 = 0x0600,
    rwRASTERFORMAT16 = 0x0700,
    rwRASTERFORMAT24 = 0x0800,
    rwRASTERFORMAT32 = 0x0900,
    rwRASTERFORMAT555 = 0x0a00,
    rwRASTERFORMATAUTOMIPMAP = 0x1000,
    rwRASTERFORMATPAL8 = 0x2000,
    rwRASTERFORMATPAL4 = 0x4000,
    rwRASTERFORMATMIPMAP = 0x8000,
    rwRASTERFORMATPIXELFORMATMASK = 0x0f00,
    rwRASTERFORMATMASK = 0xff00,
    rwRASTERFORMATFORCEENUMSIZEINT = 0x7FFFFFFF
} RwRasterFormat;

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

typedef enum RwBlendFunction
{
    rwBLENDNABLEND = 0,
    rwBLENDZERO,
    rwBLENDONE,
    rwBLENDSRCCOLOR,
    rwBLENDINVSRCCOLOR,
    rwBLENDSRCALPHA,
    rwBLENDINVSRCALPHA,
    rwBLENDDESTALPHA,
    rwBLENDINVDESTALPHA,
    rwBLENDDESTCOLOR,
    rwBLENDINVDESTCOLOR,
    rwBLENDSRCALPHASAT,
    rwBLENDFUNCTIONFORCEENUMSIZEINT = 0x7FFFFFFF
} RwBlendFunction;

#define rwObjectGetSubType(o) (((const RwObject*)(o))->subType)
#define rwObjectGetFlags(o) (((const RwObject*)(o))->flags)
#define rwObjectTestFlags(o, f) ((((const RwObject*)(o))->flags) & (U8)(f))
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

typedef struct RwChunkHeaderInfo RwChunkHeaderInfo;
struct RwChunkHeaderInfo
{
    U32 type;
    U32 length;
    U32 version;
    U32 buildNum;
    S32 isComplex;
};

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

typedef char* (*vecStrcpyFunc)(char* dest, const char* srce);
typedef U32 (*vecStrlenFunc)(const char* string);

// Partial: this build's table is 0x40 bytes; only the two members the target reaches at fixed
// offsets are declared.
typedef struct RwStringFunctions RwStringFunctions;
struct RwStringFunctions
{
    U8 unk0[0x8];
    vecStrcpyFunc vecStrcpy;
    U8 unkC[0x20];
    vecStrlenFunc vecStrlen;
    U8 unk30[0x10];
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
typedef RwTexture* (*RwTextureCallBack)(RwTexture* texture, void* pData);

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

typedef struct RwPluginRegistry RwPluginRegistry;
struct RwPluginRegistry
{
    S32 sizeOfStruct;
    S32 origSizeOfStruct;
    S32 maxSizeOfStruct;
    S32 staticAlloc;
    RwPluginRegEntry* firstRegEntry;
    RwPluginRegEntry* lastRegEntry;
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

typedef void* (*RwPluginErrorStrCallBack)(void*);

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
    RwPluginObjectConstructor constructCB;
    RwPluginObjectDestructor destructCB;
    RwPluginObjectCopy copyCB;
    RwPluginErrorStrCallBack errStrCB;
    RwPluginRegEntry* nextRegEntry;
    RwPluginRegEntry* prevRegEntry;
    RwPluginRegistry* parentRegistry;
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

// Partial: this driver's 2D vertex is 0x1C bytes; the last word is not reached.
typedef struct RwIm2DVertex RwIm2DVertex;
struct RwIm2DVertex
{
    F32 x;
    F32 y;
    F32 z;
    RwRGBA emissiveColor;
    F32 u;
    F32 v;
    U8 unk18[4];
};
typedef U16 RwImVertexIndex;

#define RwIm2DVertexSetScreenX(vert, scrnx) ((vert)->x = (scrnx))
#define RwIm2DVertexSetScreenY(vert, scrny) ((vert)->y = (scrny))
#define RwIm2DVertexSetScreenZ(vert, scrnz) ((vert)->z = (scrnz))
#define RwIm2DVertexSetIntRGBA(vert, r, g, b, a)                                                   \
    do                                                                                             \
    {                                                                                              \
        (vert)->emissiveColor.red = r;                                                             \
        (vert)->emissiveColor.green = g;                                                           \
        (vert)->emissiveColor.blue = b;                                                            \
        (vert)->emissiveColor.alpha = a;                                                           \
    } while (0)

typedef struct RxObjSpace3DVertex RxObjSpace3DVertex;
struct RxObjSpace3DVertex
{
    F32 x;
    F32 y;
    F32 z;
    F32 nx;
    F32 ny;
    F32 nz;
    U8 r;
    U8 g;
    U8 b;
    U8 a;
    F32 u;
    F32 v;
};

typedef RxObjSpace3DVertex RxObjSpace3DLitVertex;
typedef RxObjSpace3DLitVertex RwIm3DVertex;

inline void RwIm3DVertexSetPos(RwIm3DVertex* vert, F32 x, F32 y, F32 z)
{
    vert->x = x;
    vert->y = y;
    vert->z = z;
}

inline void RwIm3DVertexSetU(RwIm3DVertex* vert, F32 u)
{
    vert->u = u;
}

inline void RwIm3DVertexSetV(RwIm3DVertex* vert, F32 v)
{
    vert->v = v;
}

inline void RwIm3DVertexSetRGBA(RwIm3DVertex* vert, U8 r, U8 g, U8 b, U8 a)
{
    vert->r = r;
    vert->g = g;
    vert->b = b;
    vert->a = a;
}

enum RwIm3DTransformFlags
{
    rwIM3D_VERTEXUV = 1,
    rwIM3D_ALLOPAQUE = 2,
    rwIM3D_NOCLIP = 4,
    rwIM3D_VERTEXXYZ = 8,
    rwIM3D_VERTEXRGBA = 16,
    rwIM3DTRANSFORMFLAGSFORCEENUMSIZEINT = 0x7FFFFFFF
};
typedef enum RwIm3DTransformFlags RwIm3DTransformFlags;

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
    RwLinkList dirtyFrameList;
    RwFileFunctions fileFuncs;
    RwStringFunctions stringFuncs;
    RwMemoryFunctions memoryFuncs;
    RwMemoryAllocFn memoryAlloc;
    RwMemoryFreeFn memoryFree;
    U8 unk148[0x8];
    U32 resArenaInitSize;
};

#define RWSRCGLOBAL(var) (((RwGlobals*)RwEngineInstance)->var)

#define rwstrcpy RWSRCGLOBAL(stringFuncs).vecStrcpy
#define rwstrlen RWSRCGLOBAL(stringFuncs).vecStrlen
#define RwMalloc(_s) ((RWSRCGLOBAL(memoryFuncs).rwmalloc)((_s)))
#define RwFree(_p) ((RWSRCGLOBAL(memoryFuncs).rwfree)((_p)))

// The resource arena's globals, at resourcesModule.globalsOffset in the engine instance. The
// entries live on the two embedded lists: usedEntryList points at the one holding the entries
// used since the last purge, unusedEntryList at the one holding the rest, and a purge swaps them.
typedef struct RwResourcesGlobals RwResourcesGlobals;
struct RwResourcesGlobals
{
    U32 arenaSize;
    S32 usedSize;
    S32 reclaimedSize;
    void* heap;
    RwLinkList entryLists[2];
    RwLinkList* unusedEntryList;
    RwLinkList* usedEntryList;
};

#define RWRESOURCESGLOBAL(var)                                                                     \
    (((RwResourcesGlobals*)((U8*)RwEngineInstance + resourcesModule.globalsOffset))->var)

// The release build's form: an entry still linked is moved to the head of the in-use list.
#define RwResourcesUseResEntry(entry)                                                              \
    ((((entry)->link.next) ? (rwLinkListRemoveLLLink(&((entry)->link)),                            \
                              rwLinkListAddLLLink(RWRESOURCESGLOBAL(usedEntryList),                \
                                                  &((entry)->link)))                               \
                           : NULL),                                                                \
     (entry))

#ifdef __cplusplus
extern "C" {
#endif

extern void* RwEngineInstance;
extern RwModuleInfo resourcesModule;

RwResEntry* RwResourcesAllocateResEntry(void* owner, RwResEntry** ownerRef, S32 size,
                                        RwResEntryDestroyNotify destroyNotify);
S32 RwResourcesFreeResEntry(RwResEntry* entry);
S32 RwResourcesGetArenaSize(void);
S32 RwResourcesSetArenaSize(U32 size);
S32 RwResourcesEmptyArena(void);
void _rwResourcesPurge(void);
S32 _rwResHeapInit(void* mem, U32 size);
S32 _rwResHeapClose(void* mem);
void _rwResHeapFree(void* mem);
void* _rwResHeapAlloc(void* mem, U32 size);
S32 RwIm2DRenderIndexedPrimitive(RwPrimitiveType primType, RwIm2DVertex* vertices,
                                 S32 numVertices, RwImVertexIndex* indices, S32 numIndices);

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
S32 RwImageDestroy(RwImage* image);
RwImage* RwImageFreePixels(RwImage* image);
const RwTexDictionary* RwTexDictionaryForAllTextures(const RwTexDictionary* dict,
                                                    RwTextureCallBack fpCallBack, void* pData);
S32 RwRenderStateSet(RwRenderState state, void* value);
void* RwIm3DTransform(RwIm3DVertex* pVerts, U32 numVerts, RwMatrix* ltm, U32 flags);
S32 RwIm3DEnd(void);
S32 RwIm3DRenderPrimitive(RwPrimitiveType primType);
S32 RwEngineRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor initCB,
                           RwPluginObjectDestructor termCB);
RwMatrix* RwMatrixUpdate(RwMatrix* matrix);
RwMatrix* RwMatrixOptimize(RwMatrix* matrix, const RwMatrixTolerance* tolerance);
RwFrame* RwFrameCreate();
S32 RwFrameDestroy(RwFrame* frame);
RwFrame* RwFrameOrthoNormalize(RwFrame* frame);
RwFrame* RwFrameForAllChildren(RwFrame* frame, RwFrameCallBack callBack, void* data);
RwFrame* RwFrameForAllObjects(RwFrame* frame, RwObjectCallBack callBack, void* data);
RwFrame* RwFrameGetRoot(const RwFrame* frame);
RwMatrix* RwFrameGetLTM(RwFrame* frame);
S32 RwFrameDestroyHierarchy(RwFrame* frame);
RwMatrix* RwMatrixMultiply(RwMatrix* matrixOut, const RwMatrix* MatrixIn1, const RwMatrix* matrixIn2);
RwV3d* RwV3dTransformPoints(RwV3d* pointsOut, const RwV3d* pointsIn, S32 numPoints,
                            const RwMatrix* matrix);
RwFrustumTestResult RwCameraFrustumTestSphere(const RwCamera* camera, const RwSphere* sphere);
RwStream* RwStreamOpen(RwStreamType type, RwStreamAccessType accessType, const void* pData);
S32 RwStreamClose(RwStream* stream, void* pData);
U32 RwStreamRead(RwStream* stream, void* buffer, U32 length);
RwStream* RwStreamWrite(RwStream* stream, const void* buffer, U32 length);
RwStream* RwStreamSkip(RwStream* stream, U32 offset);
RwStream* RwStreamReadChunkHeaderInfo(RwStream* stream, RwChunkHeaderInfo* chunkHeaderInfo);
RwStream* _rwStreamWriteVersionedChunkHeader(RwStream* stream, S32 type, S32 size, U32 version,
                                             U32 buildNum);
void* RwMemLittleEndian32(void* mem, U32 size);
void* RwMemNative32(void* mem, U32 size);
S32 RwStreamFindChunk(RwStream* stream, U32 type, U32* lengthOut, U32* versionOut);
S32 _rwFrameSyncDirty(void);

RwError* RwErrorSet(RwError* code);
S32 _rwerror(S32 code, ...);

RwFreeList* RwFreeListCreate(S32 entrySize, S32 entriesPerBlock, S32 alignment);
S32 RwFreeListDestroy(RwFreeList* freeList);
S32 _rxPipelineClose(void);
S32 _rxPipelineOpen(void);
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
RwTexture* RwTexDictionaryAddTexture(RwTexDictionary* dict, RwTexture* texture);
RwTexDictionary* RwTexDictionaryGetCurrent(void);

RwFrame* RwFrameUpdateObjects(RwFrame* frame);

F32 _rwSqrt(F32 num);
F32 _rwInvSqrt(F32 num);

RwStream* RwStreamReadInt32(RwStream* stream, S32* ints, U32 numBytes);
RwStream* RwStreamWriteInt32(RwStream* stream, const S32* ints, U32 numBytes);
RwStream* RwStreamReadReal(RwStream* stream, F32* reals, U32 numBytes);
RwStream* RwStreamWriteReal(RwStream* stream, const F32* reals, U32 numBytes);

S32 RwFrameRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RwFrameRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                RwPluginDataChunkWriteCallBack writeCB,
                                RwPluginDataChunkGetSizeCallBack getSizeCB);
S32 RwCameraRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                 RwPluginDataChunkWriteCallBack writeCB,
                                 RwPluginDataChunkGetSizeCallBack getSizeCB);
S32 RwTextureRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                            RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
S32 RwTextureRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                  RwPluginDataChunkWriteCallBack writeCB,
                                  RwPluginDataChunkGetSizeCallBack getSizeCB);

#ifdef __cplusplus
}
#endif

#endif
