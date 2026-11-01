#include <rpworld.h>
#include <dolphin/gx/GXGeometry.h>
#include <dolphin/gx/GXManage.h>

// GameCube pipeline support: waiting for the GPU to finish with an instanced object before its
// memory is reused, and the callbacks the all-in-one nodes run to instance and draw objects.

// The draw sync token to issue next.
extern U16 lbl_8025E800;

S32 _rwDlTokenQueryDone(U16 token);

// Draw sync tokens wrap at this value. Written for this project.
#define GCPIPE_TOKENWRAP 0xE000

// The callbacks an all-in-one node keeps in its private data. Names written for this project;
// the field at 8 is not read or written here.
typedef struct GCNAllInOnePrivateData GCNAllInOnePrivateData;
struct GCNAllInOnePrivateData
{
    void* instanceCB;
    void* reinstanceCB;
    void* unk8;
    void* renderCB;
};

// The functions between the wait and the accessors (fn_801588E8, fn_80158B20, fn_80158DC4 and
// fn_80158FB0, one of them the default render callback) are still in assembly.

// Called when an instanced object's resource entry is freed: if the GPU may still be drawing it,
// issue a draw sync token for it now, then wait until the GPU has passed its token.
void _rxGCResEntryWaitDone(RwResEntry* resEntry)
{
    GCNVertexArrays* arrays = (GCNVertexArrays*)(resEntry + 1);

    if (arrays->syncToken == lbl_8025E800)
    {
        GXSetDrawSync(lbl_8025E800);
        lbl_8025E800 = (lbl_8025E800 + 1) % GCPIPE_TOKENWRAP;
    }

    while (!_rwDlTokenQueryDone(arrays->syncToken))
    {
    }

    GXInvalidateVtxCache();
}

void _rxGameCubeAllInOneSetInstanceCallBack(RxPipelineNode* node, void* callback)
{
    ((GCNAllInOnePrivateData*)node->privateData)->instanceCB = callback;
}

void* _rxGameCubeAllInOneGetInstanceCallBack(RxPipelineNode* node)
{
    return ((GCNAllInOnePrivateData*)node->privateData)->instanceCB;
}

void _rxGameCubeAllInOneSetReinstanceCallBack(RxPipelineNode* node, void* callback)
{
    ((GCNAllInOnePrivateData*)node->privateData)->reinstanceCB = callback;
}

void* _rxGameCubeAllInOneGetReinstanceCallBack(RxPipelineNode* node)
{
    return ((GCNAllInOnePrivateData*)node->privateData)->reinstanceCB;
}

void RxGameCubeAllInOneSetRenderCallBack(RxPipelineNode* node, void* callback)
{
    ((GCNAllInOnePrivateData*)node->privateData)->renderCB = callback;
}
