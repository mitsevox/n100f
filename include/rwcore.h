#ifndef RWCORE_H
#define RWCORE_H

#include "types.h"

// RenderWare types are opaque until code that accesses their members is reconstructed.
struct RwLLLink
{
    RwLLLink* next;
    RwLLLink* prev;
};

struct RwLinkList
{
    RwLLLink link;
};

struct RwObject
{
    U8 type;
    U8 subType;
    U8 flags;
    U8 privateFlags;
    void* parent;
};

struct RwObjectHasFrame
{
    RwObject object;
    RwLLLink lFrame;
    RwObjectHasFrame* (*sync)(RwObjectHasFrame*);
};

struct RwRGBAReal
{
    F32 red;
    F32 green;
    F32 blue;
    F32 alpha;
};

struct RwRGBA
{
    U8 red;
    U8 green;
    U8 blue;
    U8 alpha;
};

enum RwFogType
{
    rwFOGTYPENAFOGTYPE = 0,
    rwFOGTYPELINEAR = 1,
    rwFOGTYPEEXPONENTIAL = 2,
    rwFOGTYPEEXPONENTIAL2 = 3,
    rwFOGTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
};

struct RwMatrixTag;
typedef struct RwMatrixTag RwMatrix;
struct RwCamera;
struct RwFrame;

#ifdef __cplusplus
extern "C" {
#endif

RwCamera* RwCameraShowRaster(RwCamera* camera, void* pDev, U32 flags);
void _rwFrameSyncDirty();

#ifdef __cplusplus
}
#endif

#endif
