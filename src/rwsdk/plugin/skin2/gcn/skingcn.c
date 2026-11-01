#include <rpskin.h>
#include <dolphin/gx/GXGeometry.h>
#include <dolphin/gx/GXManage.h>
#include <dolphin/os.h>

// GameCube skinning: each frame the skinned positions and normals of an atomic are blended on the
// CPU from the geometry's original arrays into a resource entry of their own, which the
// all-in-one node then draws in place of the instanced arrays.

// The draw sync token to issue next.
extern U16 lbl_8025E800;

// The all-in-one node's reinstance callback before the skin pipeline replaced it.
extern GCNAllInOneCallBack lbl_8025F6A8;

// Per vertex component type (u8, s8, u16, s16, f32): the GQR load type, the size of one
// component, and the GQR scale for normals; and the size of a three-component vertex.
typedef struct SkinCompTable SkinCompTable;
struct SkinCompTable
{
    U8 value[5];
};

typedef struct SkinVertexSizes SkinVertexSizes;
struct SkinVertexSizes
{
    S32 value[5];
};

extern const SkinCompTable lbl_802628B0;
extern const SkinCompTable lbl_802628B8;
extern const SkinCompTable lbl_802628C0;
extern const SkinVertexSizes lbl_801AD280;

S32 _rwDlTokenQueryDone(U16 token);

// Draw sync tokens wrap at this value. Written for this project.
#define SKINGCN_TOKENWRAP 0xE000

// The skin plugin id, and the object type of an atomic. Written for this project.
#define SKINGCN_PLUGINID 0x116
#define SKINGCN_ATOMIC 1

// The skinned arrays' resource entry: the entry, then the draw sync token of the last frame that
// drew from it, then, 32-byte aligned, the positions and the normals. The word before the
// positions points back at the entry. Written for this project.
#define SKINGCN_TOKEN(_entry) (*(U16*)((_entry) + 1))
#define SKINGCN_ENTRY(_arrays) (((RwResEntry**)(_arrays)->pos)[-1])
#define SKINGCN_ALIGN32(_size) (((_size) + 0x1F) & ~0x1F)

// Blends numVertices positions and normals by up to four bone matrices each, with paired-single
// quantized loads and stores (GQR6 for positions, GQR7 for normals).
asm void fn_80144D5C(const RwMatrixWeights* weights, const void* srcPos, const void* srcNrm,
                     void* dstPos, void* dstNrm, const RwMatrix* matrices, const U32* indices,
                     S32 numVertices, U32 posSize, U32 nrmSize, S32 nbtSize)
{
    nofralloc
    stwu r1, -0xb8(r1)
    stfd f14, 0x8(r1)
    stfd f15, 0x10(r1)
    stfd f16, 0x18(r1)
    stfd f17, 0x20(r1)
    stfd f18, 0x28(r1)
    stfd f19, 0x30(r1)
    stfd f20, 0x38(r1)
    stfd f21, 0x40(r1)
    stfd f22, 0x48(r1)
    stfd f23, 0x50(r1)
    stfd f24, 0x58(r1)
    stfd f25, 0x60(r1)
    stfd f26, 0x68(r1)
    stfd f27, 0x70(r1)
    stfd f28, 0x78(r1)
    stfd f29, 0x80(r1)
    stfd f30, 0x88(r1)
    stfd f31, 0x90(r1)
    stw r14, 0x98(r1)
    stw r15, 0x9c(r1)
    stw r16, 0xa0(r1)
    stw r17, 0xa4(r1)
    stw r18, 0xa8(r1)
    stw r19, 0xac(r1)
    stw r20, 0xb0(r1)
    lwz r18, 0xc0(r1)
    lwz r19, 0xc4(r1)
    lwz r20, 0xc8(r1)
    cmpwi r10, 0x0
    mtctr r10
    subi r3, r3, 0x4
    subi r9, r9, 0x4
    subf r4, r18, r4
    subf r5, r19, r5
    subf r6, r18, r6
    subf r7, r19, r7
    ble @8014503C
    cmpw r4, r5
@80144DF8:
    ps_sub f0, f0, f0
    lfsu f28, 0x4(r3)
    lfsu f29, 0x4(r3)
    lfsu f30, 0x4(r3)
    lfsu f31, 0x4(r3)
    lwzu r11, 0x4(r9)
    ps_cmpo0 cr5, f29, f0
    ps_cmpo0 cr6, f30, f0
    ps_cmpo0 cr7, f31, f0
    clrlslwi r14, r11, 24, 6
    rlwinm r15, r11, 30, 18, 25
    rlwinm r16, r11, 22, 18, 25
    rlwinm r17, r11, 14, 18, 25
    add r14, r14, r8
    add r15, r15, r8
    add r16, r16, r8
    add r17, r17, r8
    ble cr5, @80144E4C
    ble cr6, @80144E50
    ble cr7, @80144E54
    b @80144E58
@80144E4C:
    addi r15, r14, 0x0
@80144E50:
    addi r16, r14, 0x0
@80144E54:
    addi r17, r14, 0x0
@80144E58:
    psq_lux f16, r4, r18, 1, 6
    psq_lux f17, r4, r18, 1, 6
    psq_lux f18, r4, r18, 1, 6
    psq_lux f21, r5, r19, 1, 7
    psq_lux f22, r5, r19, 1, 7
    psq_lux f23, r5, r19, 1, 7
    add r5, r5, r20
    psq_l f0, 0x0(r14), 0, 0
    psq_l f1, 0x8(r14), 1, 0
    psq_l f2, 0x10(r14), 0, 0
    psq_l f3, 0x18(r14), 1, 0
    psq_l f4, 0x20(r14), 0, 0
    psq_l f5, 0x28(r14), 1, 0
    psq_l f6, 0x30(r14), 0, 0
    psq_l f7, 0x38(r14), 1, 0
    ps_madds0 f6, f0, f16, f6
    psq_l f8, 0x0(r15), 0, 0
    ps_madds0 f7, f1, f16, f7
    ps_muls0 f24, f0, f21
    psq_l f9, 0x8(r15), 1, 0
    ps_muls0 f25, f1, f21
    ps_madds0 f6, f2, f17, f6
    psq_l f10, 0x10(r15), 0, 0
    ps_madds0 f7, f3, f17, f7
    ps_madds0 f24, f2, f22, f24
    psq_l f11, 0x18(r15), 1, 0
    ps_madds0 f25, f3, f22, f25
    ps_madds0 f6, f4, f18, f6
    psq_l f12, 0x20(r15), 0, 0
    ps_madds0 f7, f5, f18, f7
    ps_madds0 f24, f4, f23, f24
    psq_l f13, 0x28(r15), 1, 0
    ps_madds0 f25, f5, f23, f25
    ps_muls0 f19, f6, f28
    psq_l f14, 0x30(r15), 0, 0
    ps_muls0 f20, f7, f28
    ps_muls0 f26, f24, f28
    psq_l f15, 0x38(r15), 1, 0
    ps_muls0 f27, f25, f28
    ble cr5, @8014500C
    nop
    ps_madds0 f14, f8, f16, f14
    psq_l f0, 0x0(r16), 0, 0
    ps_madds0 f15, f9, f16, f15
    ps_muls0 f24, f8, f21
    psq_l f1, 0x8(r16), 1, 0
    ps_muls0 f25, f9, f21
    ps_madds0 f14, f10, f17, f14
    psq_l f2, 0x10(r16), 0, 0
    ps_madds0 f15, f11, f17, f15
    ps_madds0 f24, f10, f22, f24
    psq_l f3, 0x18(r16), 1, 0
    ps_madds0 f25, f11, f22, f25
    ps_madds0 f14, f12, f18, f14
    psq_l f4, 0x20(r16), 0, 0
    ps_madds0 f15, f13, f18, f15
    ps_madds0 f24, f12, f23, f24
    psq_l f5, 0x28(r16), 1, 0
    ps_madds0 f25, f13, f23, f25
    ps_madds0 f19, f14, f29, f19
    psq_l f6, 0x30(r16), 0, 0
    ps_madds0 f20, f15, f29, f20
    ps_madds0 f26, f24, f29, f26
    psq_l f7, 0x38(r16), 1, 0
    ps_madds0 f27, f25, f29, f27
    ble cr6, @8014500C
    nop
    ps_madds0 f6, f0, f16, f6
    psq_l f8, 0x0(r17), 0, 0
    ps_madds0 f7, f1, f16, f7
    ps_muls0 f24, f0, f21
    psq_l f9, 0x8(r17), 1, 0
    ps_muls0 f25, f1, f21
    ps_madds0 f6, f2, f17, f6
    psq_l f10, 0x10(r17), 0, 0
    ps_madds0 f7, f3, f17, f7
    ps_madds0 f24, f2, f22, f24
    psq_l f11, 0x18(r17), 1, 0
    ps_madds0 f25, f3, f22, f25
    ps_madds0 f6, f4, f18, f6
    psq_l f12, 0x20(r17), 0, 0
    ps_madds0 f7, f5, f18, f7
    ps_madds0 f24, f4, f23, f24
    psq_l f13, 0x28(r17), 1, 0
    ps_madds0 f25, f5, f23, f25
    ps_madds0 f19, f6, f30, f19
    psq_l f14, 0x30(r17), 0, 0
    ps_madds0 f20, f7, f30, f20
    ps_madds0 f26, f24, f30, f26
    psq_l f15, 0x38(r17), 1, 0
    ps_madds0 f27, f25, f30, f27
    ble cr7, @8014500C
    nop
    ps_madds0 f14, f8, f16, f14
    ps_madds0 f15, f9, f16, f15
    ps_muls0 f24, f8, f21
    ps_muls0 f25, f9, f21
    ps_madds0 f14, f10, f17, f14
    ps_madds0 f15, f11, f17, f15
    ps_madds0 f24, f10, f22, f24
    ps_madds0 f25, f11, f22, f25
    ps_madds0 f14, f12, f18, f14
    ps_madds0 f15, f13, f18, f15
    ps_madds0 f24, f12, f23, f24
    ps_madds0 f25, f13, f23, f25
    ps_madds0 f19, f14, f31, f19
    ps_madds0 f20, f15, f31, f20
    ps_madds0 f26, f24, f31, f26
    ps_madds0 f27, f25, f31, f27
@8014500C:
    ps_merge11 f14, f19, f19
    psq_stux f19, r6, r18, 1, 6
    psq_stux f14, r6, r18, 1, 6
    psq_stux f20, r6, r18, 1, 6
    beq @80145034
    ps_merge11 f24, f26, f26
    psq_stux f26, r7, r19, 1, 7
    psq_stux f24, r7, r19, 1, 7
    psq_stux f27, r7, r19, 1, 7
    add r7, r7, r20
@80145034:
    bdnz @80144DF8
    nop
@8014503C:
    lfd f14, 0x8(r1)
    lfd f15, 0x10(r1)
    lfd f16, 0x18(r1)
    lfd f17, 0x20(r1)
    lfd f18, 0x28(r1)
    lfd f19, 0x30(r1)
    lfd f20, 0x38(r1)
    lfd f21, 0x40(r1)
    lfd f22, 0x48(r1)
    lfd f23, 0x50(r1)
    lfd f24, 0x58(r1)
    lfd f25, 0x60(r1)
    lfd f26, 0x68(r1)
    lfd f27, 0x70(r1)
    lfd f28, 0x78(r1)
    lfd f29, 0x80(r1)
    lfd f30, 0x88(r1)
    lfd f31, 0x90(r1)
    lwz r14, 0x98(r1)
    lwz r15, 0x9c(r1)
    lwz r16, 0xa0(r1)
    lwz r17, 0xa4(r1)
    lwz r18, 0xa8(r1)
    lwz r19, 0xac(r1)
    lwz r20, 0xb0(r1)
    addi r1, r1, 0xb8
    blr

}

// The destroy notify of a skinned atomic's instanced entry: wait for the GPU, free the skinned
// arrays' entry and hand the original arrays back to the instanced entry.
void fn_801450A8(RwResEntry* resEntry)
{
    GCNVertexArrays* arrays = (GCNVertexArrays*)(resEntry + 1);
    RpSkin* skin;

    if (arrays->syncToken == lbl_8025E800)
    {
        GXSetDrawSync(lbl_8025E800);
        lbl_8025E800 = (lbl_8025E800 + 1) % SKINGCN_TOKENWRAP;
    }

    while (!_rwDlTokenQueryDone(arrays->syncToken))
    {
    }

    if (arrays->pos)
    {
        RwResourcesFreeResEntry(SKINGCN_ENTRY(arrays));
    }

    if (((RwObject*)resEntry->owner)->type == SKINGCN_ATOMIC)
    {
        skin = SKINGEOMETRYGETDATA(((RpAtomic*)resEntry->owner)->geometry);
    }
    else
    {
        skin = SKINGEOMETRYGETDATA((RpGeometry*)resEntry->owner);
    }

    arrays->pos = skin->savedPos;
    arrays->nrm = skin->savedNrm;
    skin->savedPos = NULL;
    skin->savedNrm = NULL;

    GXInvalidateVtxCache();
}

// The destroy notify of the skinned arrays' entry: wait until the GPU has passed its token.
void _rpSkinResEntryWaitDone(RwResEntry* resEntry)
{
    if (SKINGCN_TOKEN(resEntry) == lbl_8025E800)
    {
        GXSetDrawSync(lbl_8025E800);
        lbl_8025E800 = (lbl_8025E800 + 1) % SKINGCN_TOKENWRAP;
    }

    while (!_rwDlTokenQueryDone(SKINGCN_TOKEN(resEntry)))
    {
    }

    GXInvalidateVtxCache();
}

// Makes sure the skinned arrays' entry exists and is not in use by the GPU, allocating a new one
// sized for the geometry's vertex format when needed.
S32 fn_80145228(RpSkin* skin, RpAtomic* atomic, GCNVertexArrays* arrays, RwResEntry** entryRef)
{
    RpGeometry* geometry = atomic->geometry;
    RpGameCubeVtxFmt* vtxFmt;
    RwResEntry* entry;
    RwResEntry* repEntry;
    S32 size;
    S32 nrmSize;
    S32 numNrm;

    RwResourcesUseResEntry(*entryRef);
    (*entryRef)->destroyNotify = fn_801450A8;

    if (!skin->savedPos)
    {
        skin->savedPos = arrays->pos;
        skin->savedNrm = arrays->nrm;
        arrays->pos = NULL;
        arrays->nrm = NULL;
    }

    if (arrays->pos)
    {
        if (!_rwDlTokenQueryDone(SKINGCN_TOKEN(SKINGCN_ENTRY(arrays))))
        {
            SKINGCN_ENTRY(arrays)->ownerRef = NULL;
            arrays->pos = NULL;
            arrays->nrm = NULL;
        }
        else
        {
            GXInvalidateVtxCache();
        }
    }

    if (!arrays->pos)
    {
        vtxFmt = fn_8015D818(geometry);

        if (vtxFmt)
        {
            SkinVertexSizes sizes = lbl_801AD280;

            size = SKINGCN_ALIGN32(geometry->numVertices * sizes.value[vtxFmt->compType[0]]) + 6;

            if (geometry->flags & rpGEOMETRYNORMALS)
            {
                numNrm = 1;

                if (vtxFmt->frac[1])
                {
                    numNrm = 3;
                }

                size += SKINGCN_ALIGN32(geometry->numVertices *
                                        (sizes.value[vtxFmt->compType[1]] * numNrm));
            }
        }
        else
        {
            nrmSize = SKINGCN_ALIGN32(geometry->numVertices * (S32)sizeof(RwV3d));
            size = nrmSize + 6;

            if (geometry->flags & rpGEOMETRYNORMALS)
            {
                size += nrmSize;
            }
        }

        entry = RwResourcesAllocateResEntry(atomic, (RwResEntry**)&arrays->pos, size,
                                            _rpSkinResEntryWaitDone);

        if (geometry->numMorphTargets == 1)
        {
            repEntry = geometry->repEntry;
        }
        else
        {
            repEntry = atomic->repEntry;
        }

        if (!entry || !repEntry)
        {
            if (entry)
            {
                RwResourcesFreeResEntry(entry);
            }

            return FALSE;
        }

        arrays->pos = (void*)(((U32)arrays->pos + sizeof(RwResEntry) + 6 + 0x1F) & ~0x1F);

        if (geometry->flags & rpGEOMETRYNORMALS)
        {
            arrays->nrm =
                (U8*)arrays->pos + SKINGCN_ALIGN32(geometry->numVertices * arrays->posStride);
        }

        SKINGCN_ENTRY(arrays) = entry;
    }
    else
    {
        RwResourcesUseResEntry(SKINGCN_ENTRY(arrays));
    }

    SKINGCN_TOKEN(SKINGCN_ENTRY(arrays)) = lbl_8025E800;

    return TRUE;
}

// The skin pipeline's reinstance callback: reinstance the original arrays when needed, build the
// bone matrices for this frame and blend the skinned arrays from them.
RpAtomic* fn_80145598(RpAtomic* atomic, GCNObjectData* objectData)
{
    RpGeometry* geometry = atomic->geometry;
    RpSkin* skin = SKINGEOMETRYGETDATA(geometry);
    GCNVertexArrays* arrays = (GCNVertexArrays*)(objectData->repEntry + 1);
    RpHAnimHierarchy* hierarchy;
    RwMatrix* ltm;
    const RwMatrix* skinToBone;
    RwMatrix* matrix;
    RpGameCubeVtxFmt* vtxFmt;
    void* dstPos;
    void* srcNrm;
    S32 numVertices;
    S32 i;
    U32 posSize;
    U32 nrmSize;
    S32 nbtSize;
    register U32 gqr6;
    register U32 gqr7;

    if (!skin->savedPos)
    {
        if (!(geometry->flags & rpGEOMETRYNATIVE))
        {
            lbl_8025F6A8(atomic, objectData);
        }

        skin->savedPos = arrays->pos;
        skin->savedNrm = arrays->nrm;
        arrays->pos = NULL;
        arrays->nrm = NULL;
    }
    else if (!(geometry->flags & rpGEOMETRYNATIVE))
    {
        void* pos = arrays->pos;
        void* nrm = arrays->nrm;

        arrays->pos = skin->savedPos;
        arrays->nrm = skin->savedNrm;
        lbl_8025F6A8(atomic, objectData);
        arrays->pos = pos;
        arrays->nrm = nrm;
    }

    if (!fn_80145228(skin, atomic, arrays, &objectData->repEntry))
    {
        return NULL;
    }

    hierarchy = SKINATOMICGETDATA(atomic);
    ltm = RwFrameGetLTM(RpAtomicGetFrame(atomic));
    skinToBone = RpSkinGetSkinToBoneMatrices(skin);
    matrix = _rpSkinGlobals.matrixCache;

    if (hierarchy)
    {
        if (hierarchy->flags & rpHANIMHIERARCHYNOMATRICES)
        {
            RwMatrix inverse;
            RwMatrix tmp;

            RwMatrixInvert(&inverse, ltm);

            for (i = 0; i < hierarchy->numNodes; i++)
            {
                RwMatrixMultiply(&tmp, skinToBone, RwFrameGetLTM(hierarchy->pNodeInfo[i].pFrame));
                RwMatrixMultiply(matrix, &tmp, &inverse);
                skinToBone++;
                matrix++;
            }
        }
        else if (hierarchy->flags & rpHANIMHIERARCHYLOCALSPACEMATRICES)
        {
            for (i = 0; i < hierarchy->numNodes; i++)
            {
                RwMatrixMultiply(matrix, skinToBone, &hierarchy->pMatrixArray[i]);
                skinToBone++;
                matrix++;
            }
        }
        else
        {
            RwMatrix inverse;
            RwMatrix tmp;

            RwMatrixInvert(&inverse, ltm);

            for (i = 0; i < hierarchy->numNodes; i++)
            {
                RwMatrixMultiply(&tmp, skinToBone, &hierarchy->pMatrixArray[i]);
                RwMatrixMultiply(matrix, &tmp, &inverse);
                skinToBone++;
                matrix++;
            }
        }
    }

    numVertices = geometry->numVertices;
    vtxFmt = fn_8015D818(geometry);
    posSize = sizeof(F32);
    nrmSize = sizeof(F32);
    srcNrm = skin->savedNrm;
    nbtSize = 0;
    dstPos = arrays->pos;
    gqr6 = 0;
    gqr7 = 0;

    if (vtxFmt)
    {
        SkinCompTable types = lbl_802628B0;
        SkinCompTable sizes = lbl_802628B8;
        SkinCompTable scales = lbl_802628C0;

        posSize = sizes.value[vtxFmt->compType[0]];
        nrmSize = sizes.value[vtxFmt->compType[1]];
        nbtSize = vtxFmt->frac[1] ? nrmSize * 6 : 0;
        gqr6 = types.value[vtxFmt->compType[0]] | (vtxFmt->frac[0] << 8);
        gqr7 = types.value[vtxFmt->compType[1]] | (scales.value[vtxFmt->compType[1]] << 8);
        gqr6 |= gqr6 << 16;
        gqr7 |= gqr7 << 16;
    }

    asm
    {
        mtspr 918, gqr6
        mtspr 919, gqr7
    }

    if (!arrays->nrm)
    {
        srcNrm = skin->savedPos;
    }

    fn_80144D5C(skin->vertexBoneWeights, skin->savedPos, srcNrm, dstPos, arrays->nrm,
                _rpSkinGlobals.matrixCache, skin->vertexBoneIndices, numVertices, posSize, nrmSize,
                nbtSize);

    DCFlushRange(SKINGCN_ENTRY(arrays) + 1, SKINGCN_ENTRY(arrays)->size);

    return atomic;
}

RpGeometry* _rpSkinInitialize(RpGeometry* geometry)
{
    RpSkin* skin = SKINGEOMETRYGETDATA(geometry);

    if (skin)
    {
        if (geometry->flags & rpGEOMETRYNATIVE)
        {
            if (geometry->flags & rpGEOMETRYNORMALS)
            {
                geometry->lockedSinceLastInst |= 6;
            }
            else
            {
                geometry->lockedSinceLastInst |= 2;
            }
        }

        skin->savedPos = NULL;
        skin->savedNrm = NULL;
    }

    return geometry;
}

RpGeometry* _rpSkinDeinitialize(RpGeometry* geometry)
{
    RpSkin* skin = SKINGEOMETRYGETDATA(geometry);

    if (skin)
    {
        skin->savedPos = NULL;
        skin->savedNrm = NULL;
    }

    return geometry;
}

// Creates a skin pipeline of the given type: one GameCube atomic all-in-one node with the given
// instance, reinstance and render callbacks (the default reinstance callback is kept for the
// skin's own one to call).
RxPipeline* _rpSkinPipelineCreate(RpSkinType type, void* instanceCB, void* reinstanceCB,
                                  void* renderCB)
{
    RxPipeline* pipeline = RxPipelineCreate();
    RxLockedPipe* lpipe;
    RxNodeDefinition* nodeDef;
    RxPipelineNode* node;

    pipeline->pluginId = SKINGCN_PLUGINID;
    pipeline->pluginData = type;

    lpipe = RxPipelineLock(pipeline);
    nodeDef = RxNodeDefinitionGetGameCubeAtomicAllInOne();
    RxLockedPipeUnlock(RxLockedPipeAddFragment(lpipe, NULL, nodeDef, NULL));

    node = RxPipelineFindNodeByName(pipeline, nodeDef->name, NULL, NULL);

    if (instanceCB)
    {
        _rxGameCubeAllInOneSetInstanceCallBack(node, instanceCB);
    }

    if (reinstanceCB)
    {
        lbl_8025F6A8 = (GCNAllInOneCallBack)_rxGameCubeAllInOneGetReinstanceCallBack(node);
        _rxGameCubeAllInOneSetReinstanceCallBack(node, reinstanceCB);
    }

    if (renderCB)
    {
        RxGameCubeAllInOneSetRenderCallBack(node, renderCB);
    }

    return pipeline;
}
