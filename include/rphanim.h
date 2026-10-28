#ifndef RPHANIM_H
#define RPHANIM_H

#include <rpworld.h>

typedef struct RpHAnimHierarchy RpHAnimHierarchy;

typedef struct RpHAnimNodeInfo RpHAnimNodeInfo;
struct RpHAnimNodeInfo
{
    S32 nodeID;
    S32 nodeIndex;
    S32 flags;
    RwFrame* pFrame;
};

struct RpHAnimHierarchy
{
    S32 flags;
    S32 numNodes;
    U8 opaque_8[0x28 - 0x8];
    RwMatrix* pMatrixArray;
    U8 opaque_2C[0x30 - 0x2C];
    RpHAnimNodeInfo* pNodeInfo;
};

typedef enum RpHAnimHierarchyFlag
{
    rpHANIMHIERARCHYSUBHIERARCHY = 0x01,
    rpHANIMHIERARCHYNOMATRICES = 0x02,
    rpHANIMHIERARCHYUPDATEMODELLINGMATRICES = 0x1000,
    rpHANIMHIERARCHYUPDATELTMS = 0x2000,
    rpHANIMHIERARCHYLOCALSPACEMATRICES = 0x4000,
    rpHANIMHIERARCHYFLAGFORCEENUMSIZEINT = 0x7FFFFFFF
} RpHAnimHierarchyFlag;

#ifdef __cplusplus
extern "C" {
#endif

RpHAnimHierarchy* RpHAnimHierarchyCreate(S32 numNodes, U32* nodeFlags, S32* nodeIDs,
                                         RpHAnimHierarchyFlag flags, S32 maxKeyFrameSize);
S32 RpHAnimFrameSetHierarchy(RwFrame* frame, RpHAnimHierarchy* hierarchy);
RpHAnimHierarchy* RpHAnimFrameGetHierarchy(RwFrame* frame);

#ifdef __cplusplus
}
#endif

#endif
