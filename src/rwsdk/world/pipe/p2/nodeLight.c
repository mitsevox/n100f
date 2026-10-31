#include <rpworld.h>

// The light node: adds each light's contribution to the camera-space vertex colors of a mesh, in
// object space. The colors are in the 0-255 range.

// The node's cluster slots.
#define LIGHTCL_MESHSTATE 0
#define LIGHTCL_OBJVERTS 1
#define LIGHTCL_CAMVERTS 2
#define LIGHTCL_LIGHTS 3
#define LIGHTCL_VSTEPS 4

#define LIGHT_COLORSCALE 255.0f

extern RxClusterDefinition RxClLights;
extern RxClusterDefinition RxClVSteps;

static void _rwApplyAmbientLight(RxPipelineNode* self, RxPacket* packet, RpLight* light,
                                 RwMatrix* inverseMat, RwSurfaceProperties* surfaceProps,
                                 F32 invScale);
static void _rwApplyDirectionalLight(RxPipelineNode* self, RxPacket* packet, RpLight* light,
                                     RwMatrix* inverseMat, RwSurfaceProperties* surfaceProps,
                                     F32 invScale);
static void _rwApplyPointLight(RxPipelineNode* self, RxPacket* packet, RpLight* light,
                               RwMatrix* inverseMat, RwSurfaceProperties* surfaceProps,
                               F32 invScale);
static void _rwApplySpotLight(RxPipelineNode* self, RxPacket* packet, RpLight* light,
                              RwMatrix* inverseMat, RwSurfaceProperties* surfaceProps,
                              F32 invScale);
static void _rwApplySpotSoftLight(RxPipelineNode* self, RxPacket* packet, RpLight* light,
                                  RwMatrix* inverseMat, RwSurfaceProperties* surfaceProps,
                                  F32 invScale);

// Lights the mesh with every light in the lights cluster, unless the mesh is wholly clipped. A
// mesh that is not at the world origin gets the lights moved into its object space.
static S32 _LightFn(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    RxPacket* packet = RxPacketFetch(self);
    RxCluster* meshState = RxClusterLockRead(packet, LIGHTCL_MESHSTATE);
    RxMeshStateVector* meshData = (RxMeshStateVector*)meshState->currentData;

    if (!meshData->ClipFlagsAnd)
    {
        RxCluster* lights = RxClusterLockRead(packet, LIGHTCL_LIGHTS);
        U32 numLights = lights->numUsed;

        if (numLights)
        {
            RwMatrix* inverseMat;
            F32 invScale;

            if (meshData->Obj2World.flags & rwMATRIXINTERNALIDENTITY)
            {
                invScale = 1.0f;
                inverseMat = NULL;
            }
            else
            {
                inverseMat = RwMatrixCreate();
                RwMatrixInvert(inverseMat, &meshData->Obj2World);
                invScale = _rwSqrt(RwV3dDotProductMacro(&inverseMat->at, &inverseMat->at));
            }

            do
            {
                RpLight* light = *(RpLight**)lights->currentData;

                switch (RpLightGetType(light))
                {
                case rpLIGHTAMBIENT:
                    _rwApplyAmbientLight(self, packet, light, inverseMat,
                                         &meshData->SurfaceProperties, invScale);
                    break;
                case rpLIGHTDIRECTIONAL:
                    _rwApplyDirectionalLight(self, packet, light, inverseMat,
                                             &meshData->SurfaceProperties, invScale);
                    break;
                case rpLIGHTPOINT:
                    _rwApplyPointLight(self, packet, light, inverseMat,
                                       &meshData->SurfaceProperties, invScale);
                    break;
                case rpLIGHTSPOT:
                    _rwApplySpotLight(self, packet, light, inverseMat,
                                      &meshData->SurfaceProperties, invScale);
                    break;
                case rpLIGHTSPOTSOFT:
                    _rwApplySpotSoftLight(self, packet, light, inverseMat,
                                          &meshData->SurfaceProperties, invScale);
                    break;
                }

                lights->currentData = (U8*)lights->currentData + lights->stride;
            } while (--numLights);

            if (inverseMat)
            {
                RwMatrixDestroy(inverseMat);
            }
        }
    }

    RxPacketDispatch(packet, 0, self);

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetLight(void)
{
    static RxClusterRef LightCLI[] = {
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClObjSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClCamSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClLights, rxCLALLOWABSENT, 0 },
        { &RxClVSteps, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq LightInputVR[] = {
        rxCLREQ_REQUIRED, rxCLREQ_REQUIRED, rxCLREQ_REQUIRED, rxCLREQ_OPTIONAL, rxCLREQ_OPTIONAL,
    };

    static RxClusterValid LightOutputV[] = {
        rxCLVALID_NOCHANGE, rxCLVALID_NOCHANGE, rxCLVALID_VALID,
        rxCLVALID_NOCHANGE, rxCLVALID_NOCHANGE,
    };

    static char outputName[] = "Output";

    static RxOutputSpec LightOutput[] = {
        { outputName, LightOutputV, rxCLVALID_NOCHANGE },
    };

    static char _Light_csl[] = "Light.csl";

    static RxNodeDefinition nodeLightCSL = {
        _Light_csl,
        { _LightFn, NULL, NULL, NULL, NULL, NULL, NULL },
        { 5, LightCLI, LightInputVR, 1, LightOutput },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeLightCSL;
}

// The vertex-steps cluster, when the packet has a valid one, lists how many vertices to step over
// before each vertex to light, starting with the first; otherwise every vertex is lit.
#define LIGHT_BEGIN(_objStride)                                                                    \
    if (vSteps && (vSteps->flags & CLUSTER_VALID))                                                 \
    {                                                                                              \
        U8 _first;                                                                                 \
                                                                                                   \
        step = (U8*)vSteps->data;                                                                  \
        numVerts = vSteps->numUsed;                                                                \
        _first = *step++;                                                                          \
        objVert = (RxObjSpace3DVertex*)((U8*)objVert + _first * (_objStride));                     \
        camVert += _first;                                                                         \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
        step = NULL;                                                                               \
        numVerts = camVerts->numUsed;                                                              \
    }

// Moves both vertex cursors on to the next vertex to light.
#define LIGHT_NEXT(_objStride)                                                                     \
    {                                                                                              \
        S32 _count = 1;                                                                            \
                                                                                                   \
        if (step)                                                                                  \
        {                                                                                          \
            _count = *step++;                                                                      \
        }                                                                                          \
                                                                                                   \
        do                                                                                         \
        {                                                                                          \
            objVert = (RxObjSpace3DVertex*)((U8*)objVert + (_objStride));                          \
            camVert++;                                                                             \
        } while (--_count);                                                                        \
    }

// Adds one grey product to all three channels, or the scaled light color.
#define LIGHT_ADDGREY(_vert, _amount)                                                              \
    {                                                                                              \
        (_vert)->col.red += (_amount);                                                             \
        (_vert)->col.green += (_amount);                                                           \
        (_vert)->col.blue += (_amount);                                                            \
    }

#define LIGHT_ADDCOLOR(_vert, _amount, _light)                                                     \
    {                                                                                              \
        (_vert)->col.red += (_amount) * (_light)->color.red;                                       \
        (_vert)->col.green += (_amount) * (_light)->color.green;                                   \
        (_vert)->col.blue += (_amount) * (_light)->color.blue;                                     \
    }

static void _rwApplyAmbientLight(RxPipelineNode* self, RxPacket* packet, RpLight* light,
                                 RwMatrix* inverseMat, RwSurfaceProperties* surfaceProps,
                                 F32 invScale)
{
    RxCluster* camVerts = RxClusterLockWrite(packet, LIGHTCL_CAMVERTS, self);
    RxCluster* vSteps = RxClusterLockRead(packet, LIGHTCL_VSTEPS);
    RxCamSpace3DVertex* camVert = (RxCamSpace3DVertex*)camVerts->currentData;
    U8* step;
    S32 numVerts;

    if (vSteps && (vSteps->flags & CLUSTER_VALID))
    {
        step = (U8*)vSteps->data;
        numVerts = vSteps->numUsed;
        camVert += *step++;
    }
    else
    {
        step = NULL;
        numVerts = camVerts->numUsed;
    }

    if (light->object.object.privateFlags & LIGHT_PRIVATENOCHROMA)
    {
        F32 amount = LIGHT_COLORSCALE * light->color.red * surfaceProps->ambient;
        S32 i;

        for (i = numVerts; i; i--)
        {
            LIGHT_ADDGREY(camVert, amount);

            if (step)
            {
                camVert += *step++;
            }
            else
            {
                camVert++;
            }
        }
    }
    else
    {
        F32 amount = LIGHT_COLORSCALE * surfaceProps->ambient;
        S32 i;

        for (i = numVerts; i; i--)
        {
            LIGHT_ADDCOLOR(camVert, amount, light);

            if (step)
            {
                camVert += *step++;
            }
            else
            {
                camVert++;
            }
        }
    }
}

static void _rwApplyDirectionalLight(RxPipelineNode* self, RxPacket* packet, RpLight* light,
                                     RwMatrix* inverseMat, RwSurfaceProperties* surfaceProps,
                                     F32 invScale)
{
    RxCluster* objVerts = RxClusterLockRead(packet, LIGHTCL_OBJVERTS);
    RxCluster* camVerts = RxClusterLockWrite(packet, LIGHTCL_CAMVERTS, self);
    RxCluster* vSteps = RxClusterLockRead(packet, LIGHTCL_VSTEPS);
    U16 objStride = objVerts->stride;
    RxObjSpace3DVertex* objVert = (RxObjSpace3DVertex*)objVerts->currentData;
    RxCamSpace3DVertex* camVert = (RxCamSpace3DVertex*)camVerts->currentData;
    U8* step;
    S32 numVerts;

    LIGHT_BEGIN(objStride);

    if (RpLightGetFrame(light))
    {
        RwV3d dir = RwFrameGetLTM(RpLightGetFrame(light))->at;

        if (inverseMat)
        {
            RwV3dTransformVectors(&dir, &dir, 1, inverseMat);
            _rwV3dNormalize(&dir, &dir);
        }

        dir.x = -1.0f * dir.x;
        dir.y = -1.0f * dir.y;
        dir.z = -1.0f * dir.z;

        if (light->object.object.privateFlags & LIGHT_PRIVATENOCHROMA)
        {
            F32 scale = LIGHT_COLORSCALE * light->color.red * surfaceProps->diffuse;
            S32 i;

            for (i = numVerts; i; i--)
            {
                F32 lambert = dir.x * objVert->nx + dir.y * objVert->ny + dir.z * objVert->nz;

                if (lambert > 0.0f)
                {
                    F32 amount = lambert * scale;

                    LIGHT_ADDGREY(camVert, amount);
                }

                LIGHT_NEXT(objStride);
            }
        }
        else
        {
            F32 scale = LIGHT_COLORSCALE * surfaceProps->diffuse;
            S32 i;

            for (i = numVerts; i; i--)
            {
                F32 lambert = dir.x * objVert->nx + dir.y * objVert->ny + dir.z * objVert->nz;

                if (lambert > 0.0f)
                {
                    F32 amount = lambert * scale;

                    LIGHT_ADDCOLOR(camVert, amount, light);
                }

                LIGHT_NEXT(objStride);
            }
        }
    }
}

// The light's position in object space, with its radius scaled to match.
#define LIGHT_POSITION(_pos)                                                                       \
    if (inverseMat)                                                                                \
    {                                                                                              \
        F32 _radius = light->radius * invScale;                                                    \
                                                                                                   \
        invRadius = 1.0f / _radius;                                                                \
        radiusSquared = _radius * _radius;                                                         \
        RwV3dTransformPoints((_pos), (_pos), 1, inverseMat);                                       \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
        radiusSquared = light->radius * light->radius;                                             \
        invRadius = 1.0f / light->radius;                                                          \
    }

static void _rwApplyPointLight(RxPipelineNode* self, RxPacket* packet, RpLight* light,
                               RwMatrix* inverseMat, RwSurfaceProperties* surfaceProps,
                               F32 invScale)
{
    RxCluster* objVerts = RxClusterLockRead(packet, LIGHTCL_OBJVERTS);
    RxCluster* camVerts = RxClusterLockWrite(packet, LIGHTCL_CAMVERTS, self);
    RxCluster* vSteps = RxClusterLockRead(packet, LIGHTCL_VSTEPS);
    U16 objStride = objVerts->stride;
    RxObjSpace3DVertex* objVert = (RxObjSpace3DVertex*)objVerts->currentData;
    RxCamSpace3DVertex* camVert = (RxCamSpace3DVertex*)camVerts->currentData;
    U8* step;
    S32 numVerts;
    RwV3d pos;
    F32 invRadius;
    F32 radiusSquared;

    LIGHT_BEGIN(objStride);

    pos = RwFrameGetLTM(RpLightGetFrame(light))->pos;

    LIGHT_POSITION(&pos);

    if (light->object.object.privateFlags & LIGHT_PRIVATENOCHROMA)
    {
        F32 scale = LIGHT_COLORSCALE * light->color.red * surfaceProps->diffuse;

        while (numVerts--)
        {
            RwV3d vec;
            F32 lambert;

            vec.x = pos.x - objVert->x;
            vec.y = pos.y - objVert->y;
            vec.z = pos.z - objVert->z;
            lambert = vec.x * objVert->nx + vec.y * objVert->ny + vec.z * objVert->nz;

            if (lambert > 0.0f && RwV3dDotProductMacro(&vec, &vec) < radiusSquared)
            {
                F32 dist = _rwSqrt(RwV3dDotProductMacro(&vec, &vec));
                F32 recipDist = 0.0f;
                F32 amount;

                if (dist > recipDist)
                {
                    recipDist = 1.0f / dist;
                }

                amount = scale * lambert * (recipDist - invRadius);
                LIGHT_ADDGREY(camVert, amount);
            }

            LIGHT_NEXT(objStride);
        }
    }
    else
    {
        F32 scale = LIGHT_COLORSCALE * surfaceProps->diffuse;

        while (numVerts--)
        {
            RwV3d vec;
            F32 lambert;

            vec.x = pos.x - objVert->x;
            vec.y = pos.y - objVert->y;
            vec.z = pos.z - objVert->z;
            lambert = vec.x * objVert->nx + vec.y * objVert->ny + vec.z * objVert->nz;

            if (lambert > 0.0f && RwV3dDotProductMacro(&vec, &vec) < radiusSquared)
            {
                F32 dist = _rwSqrt(RwV3dDotProductMacro(&vec, &vec));
                F32 recipDist = 0.0f;
                F32 amount;

                if (dist > recipDist)
                {
                    recipDist = 1.0f / dist;
                }

                amount = scale * lambert * (recipDist - invRadius);
                LIGHT_ADDCOLOR(camVert, amount, light);
            }

            LIGHT_NEXT(objStride);
        }
    }
}

static void _rwApplySpotLight(RxPipelineNode* self, RxPacket* packet, RpLight* light,
                              RwMatrix* inverseMat, RwSurfaceProperties* surfaceProps,
                              F32 invScale)
{
    RxCluster* objVerts = RxClusterLockRead(packet, LIGHTCL_OBJVERTS);
    RxCluster* camVerts = RxClusterLockWrite(packet, LIGHTCL_CAMVERTS, self);
    RxCluster* vSteps = RxClusterLockRead(packet, LIGHTCL_VSTEPS);
    U16 objStride = objVerts->stride;
    RxObjSpace3DVertex* objVert = (RxObjSpace3DVertex*)objVerts->currentData;
    RxCamSpace3DVertex* camVert = (RxCamSpace3DVertex*)camVerts->currentData;
    U8* step;
    S32 numVerts;
    RwV3d pos;
    RwV3d at;
    F32 invRadius;
    F32 radiusSquared;

    LIGHT_BEGIN(objStride);

    pos = RwFrameGetLTM(RpLightGetFrame(light))->pos;
    at = RwFrameGetLTM(RpLightGetFrame(light))->at;

    if (inverseMat)
    {
        F32 radius = light->radius * invScale;

        invRadius = 1.0f / radius;
        radiusSquared = radius * radius;
        RwV3dTransformPoints(&pos, &pos, 1, inverseMat);
        RwV3dTransformVectors(&at, &at, 1, inverseMat);
        _rwV3dNormalize(&at, &at);
    }
    else
    {
        radiusSquared = light->radius * light->radius;
        invRadius = 1.0f / light->radius;
    }

    if (light->object.object.privateFlags & LIGHT_PRIVATENOCHROMA)
    {
        F32 scale = LIGHT_COLORSCALE * light->color.red * surfaceProps->diffuse;

        while (numVerts--)
        {
            RwV3d vec;
            F32 lambert;

            vec.x = pos.x - objVert->x;
            vec.y = pos.y - objVert->y;
            vec.z = pos.z - objVert->z;
            lambert = vec.x * objVert->nx + vec.y * objVert->ny + vec.z * objVert->nz;

            if (lambert > 0.0f && RwV3dDotProductMacro(&vec, &vec) < radiusSquared)
            {
                F32 dist = _rwSqrt(RwV3dDotProductMacro(&vec, &vec));

                if (RwV3dDotProductMacro(&vec, &at) < dist * light->minusCosAngle)
                {
                    F32 recipDist = 0.0f;
                    F32 amount;

                    if (dist > recipDist)
                    {
                        recipDist = 1.0f / dist;
                    }

                    amount = scale * lambert * (recipDist - invRadius);
                    LIGHT_ADDGREY(camVert, amount);
                }
            }

            LIGHT_NEXT(objStride);
        }
    }
    else
    {
        F32 scale = LIGHT_COLORSCALE * surfaceProps->diffuse;

        while (numVerts--)
        {
            RwV3d vec;
            F32 lambert;

            vec.x = pos.x - objVert->x;
            vec.y = pos.y - objVert->y;
            vec.z = pos.z - objVert->z;
            lambert = vec.x * objVert->nx + vec.y * objVert->ny + vec.z * objVert->nz;

            if (lambert > 0.0f && RwV3dDotProductMacro(&vec, &vec) < radiusSquared)
            {
                F32 dist = _rwSqrt(RwV3dDotProductMacro(&vec, &vec));

                if (RwV3dDotProductMacro(&vec, &at) < dist * light->minusCosAngle)
                {
                    F32 recipDist = 0.0f;
                    F32 amount;

                    if (dist > recipDist)
                    {
                        recipDist = 1.0f / dist;
                    }

                    amount = scale * lambert * (recipDist - invRadius);
                    LIGHT_ADDCOLOR(camVert, amount, light);
                }
            }

            LIGHT_NEXT(objStride);
        }
    }
}

static void _rwApplySpotSoftLight(RxPipelineNode* self, RxPacket* packet, RpLight* light,
                                  RwMatrix* inverseMat, RwSurfaceProperties* surfaceProps,
                                  F32 invScale)
{
    RxCluster* objVerts = RxClusterLockRead(packet, LIGHTCL_OBJVERTS);
    RxCluster* camVerts = RxClusterLockWrite(packet, LIGHTCL_CAMVERTS, self);
    RxCluster* vSteps = RxClusterLockRead(packet, LIGHTCL_VSTEPS);
    U16 objStride = objVerts->stride;
    RxObjSpace3DVertex* objVert = (RxObjSpace3DVertex*)objVerts->currentData;
    RxCamSpace3DVertex* camVert = (RxCamSpace3DVertex*)camVerts->currentData;
    U8* step;
    S32 numVerts;
    RwV3d pos;
    RwV3d at;
    F32 invRadius;
    F32 radiusSquared;

    LIGHT_BEGIN(objStride);

    pos = RwFrameGetLTM(RpLightGetFrame(light))->pos;
    at = RwFrameGetLTM(RpLightGetFrame(light))->at;

    if (inverseMat)
    {
        F32 radius = light->radius * invScale;

        invRadius = 1.0f / radius;
        radiusSquared = radius * radius;
        RwV3dTransformPoints(&pos, &pos, 1, inverseMat);
        RwV3dTransformVectors(&at, &at, 1, inverseMat);
        _rwV3dNormalize(&at, &at);
    }
    else
    {
        radiusSquared = light->radius * light->radius;
        invRadius = 1.0f / light->radius;
    }

    if (light->object.object.privateFlags & LIGHT_PRIVATENOCHROMA)
    {
        F32 scale = LIGHT_COLORSCALE * light->color.red * surfaceProps->diffuse;

        while (numVerts--)
        {
            RwV3d vec;
            F32 lambert;

            vec.x = pos.x - objVert->x;
            vec.y = pos.y - objVert->y;
            vec.z = pos.z - objVert->z;
            lambert = vec.x * objVert->nx + vec.y * objVert->ny + vec.z * objVert->nz;

            if (lambert > 0.0f && RwV3dDotProductMacro(&vec, &vec) < radiusSquared)
            {
                F32 dist = _rwSqrt(RwV3dDotProductMacro(&vec, &vec));
                F32 cone = dist * light->minusCosAngle;
                F32 alongAt = RwV3dDotProductMacro(&vec, &at);

                if (alongAt < cone)
                {
                    F32 recipDist = 0.0f;
                    F32 amount;
                    F32 edge;

                    if (dist > recipDist)
                    {
                        recipDist = 1.0f / dist;
                    }

                    edge = dist + cone;
                    amount = scale * lambert * (recipDist - invRadius);

                    if (edge > 0.0f)
                    {
                        F32 falloff = (dist + alongAt) / edge;

                        amount *= 1.0f - falloff * falloff;
                    }

                    LIGHT_ADDGREY(camVert, amount);
                }
            }

            LIGHT_NEXT(objStride);
        }
    }
    else
    {
        F32 scale = LIGHT_COLORSCALE * surfaceProps->diffuse;

        while (numVerts--)
        {
            RwV3d vec;
            F32 lambert;

            vec.x = pos.x - objVert->x;
            vec.y = pos.y - objVert->y;
            vec.z = pos.z - objVert->z;
            lambert = vec.x * objVert->nx + vec.y * objVert->ny + vec.z * objVert->nz;

            if (lambert > 0.0f && RwV3dDotProductMacro(&vec, &vec) < radiusSquared)
            {
                F32 dist = _rwSqrt(RwV3dDotProductMacro(&vec, &vec));
                F32 cone = dist * light->minusCosAngle;
                F32 alongAt = RwV3dDotProductMacro(&vec, &at);

                if (alongAt < cone)
                {
                    F32 recipDist = 0.0f;
                    F32 amount;
                    F32 edge;

                    if (dist > recipDist)
                    {
                        recipDist = 1.0f / dist;
                    }

                    edge = dist + cone;
                    amount = scale * lambert * (recipDist - invRadius);

                    if (edge > 0.0f)
                    {
                        F32 falloff = (dist + alongAt) / edge;

                        amount *= 1.0f - falloff * falloff;
                    }

                    LIGHT_ADDCOLOR(camVert, amount, light);
                }
            }

            LIGHT_NEXT(objStride);
        }
    }
}
