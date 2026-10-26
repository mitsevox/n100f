#ifndef RPWORLD_H
#define RPWORLD_H

#include <rwcore.h>

// RenderWare types get members once reconstructed code accesses them or a declared type
// embeds them; the others stay opaque.
struct RpAtomic;
struct RpMaterial;
struct RpSector;

enum RpWorldRenderOrder
{
    rpWORLDRENDERNARENDERORDER = 0,
    rpWORLDRENDERFRONT2BACK = 1,
    rpWORLDRENDERBACK2FRONT = 2,
    rpWORLDRENDERORDERFORCEENUMSIZEINT = 0x7FFFFFFF
};

struct RpMaterialList
{
    RpMaterial** materials;
    S32 numMaterials;
    S32 space;
};

// Partial: members from +0x68 on are not declared, so sizeof(RpWorld) is not its size.
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
};

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

#ifdef __cplusplus
extern "C" {
#endif

RpWorld* RpWorldAddCamera(RpWorld* world, RwCamera* camera);
RpWorld* RpWorldRemoveCamera(RpWorld* world, RwCamera* camera);
S32 RpWorldDestroy(RpWorld* world);

#ifdef __cplusplus
}
#endif

#endif
