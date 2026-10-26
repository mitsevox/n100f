#ifndef RPWORLD_H
#define RPWORLD_H

#include <rwcore.h>

// RenderWare types are opaque until code that accesses their members is reconstructed.
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

// Members past directionalLightList are not declared: the PS2 DWARF has
// surfaceProps at +0x68, which bfbb's GameCube RenderWare headers lack.
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
