#ifndef RPMTEXGCN_H
#define RPMTEXGCN_H

#include <rpmtex.h>

// The GameCube multitexture effect (multiTexGcnData.c) and its pipeline (multiTexGcnPipe.c).
// Type and field names are written for this project. The fields are named after the GX call
// _rpGameCubeMTEffectSend passes each one to (GXSetTexCoordGen2, GXSetTevColorIn and so on);
// flags bits are named after what Send does with them.

// A texture coordinate generator (GXSetTexCoordGen2, GXSetTexCoordScaleManually).
typedef struct GCNMTTexCoordGen GCNMTTexCoordGen;
struct GCNMTTexCoordGen
{
    U32 flags;
    S32 func;
    S32 src;
    S32 mtx;
    S32 postMtx;
    U16 scaleS;
    U16 scaleT;
};

// GCNMTTexCoordGen flags.
#define GCNMTTEXGEN_DISABLED 0x01
#define GCNMTTEXGEN_NORMALIZE 0x02
#define GCNMTTEXGEN_MANUALSCALE 0x04

// A TEV stage: its inputs, operations, constant selections and order, and an indirect setup
// packed into one word (RpGameCubeTevIndUnpack).
typedef struct GCNMTTevStage GCNMTTevStage;
struct GCNMTTevStage
{
    S32 colorIn[4];
    S32 alphaIn[4];
    S32 colorOp;
    S32 colorBias;
    S32 colorScale;
    S32 colorClamp;
    S32 colorOut;
    S32 alphaOp;
    S32 alphaBias;
    S32 alphaScale;
    S32 alphaClamp;
    S32 alphaOut;
    S32 kColorSel;
    S32 kAlphaSel;
    S32 texCoord;
    S32 texMap;
    S32 channel;
    U32 flags;
    U32 indirect;
};

// GCNMTTevStage flags; the texture map value that disables texturing.
#define GCNMTTEV_DISABLED 0x01
#define GCNMTTEV_TEXMAPOFF 0x100

// A texture matrix (GXLoadTexMtxImm), optionally relative to a frame (GetTexFrameMatrix).
typedef struct GCNMTTexMatrix GCNMTTexMatrix;
struct GCNMTTexMatrix
{
    S32 id;
    S32 frame;
    U32 flags;
    F32 matrix[3][4];
};

// GCNMTTexMatrix flags: the matrix type passed to GX, and whether the frame's translation is kept.
#define GCNMTTEXMTX_TYPE 0x01
#define GCNMTTEXMTX_TRANSLATE 0x02

// An indirect texture matrix (GXSetIndTexMtx), optionally relative to a frame.
typedef struct GCNMTIndMatrix GCNMTIndMatrix;
struct GCNMTIndMatrix
{
    S32 id;
    S32 frame;
    S32 unk8;
    F32 offset[2][3];
    S32 scaleExp;
};

// An indirect texture stage (GXSetIndTexOrder, GXSetIndTexCoordScale).
typedef struct GCNMTIndStage GCNMTIndStage;
struct GCNMTIndStage
{
    U32 flags;
    S32 texCoord;
    S32 texMap;
    S32 scaleS;
    S32 scaleT;
};

// GCNMTConfig flags.
#define GCNMT_TEVCOLORS 0x01
#define GCNMT_KCOLORS 0x02
#define GCNMT_BLEND 0x04
#define GCNMT_ALPHACOMPARE 0x08
#define GCNMT_LIGHTING 0x10
#define GCNMT_ENVMATRIX 0x20

// The configuration RpGameCubeMTEffectGetConfig returns, right after the generic effect: what the
// effect sets (GCNMT_*), the capacity and the count of each element array, four TEV colors and
// four constant colors, the arrays, and the blend modes it sets.
typedef struct GCNMTConfig GCNMTConfig;
struct GCNMTConfig
{
    U32 flags;
    U8 maxTexCoordGens;
    U8 maxTevStages;
    U8 maxTexMatrices;
    U8 maxIndStages;
    U8 maxIndMatrices;
    U8 numTexCoordGens;
    U8 numTevStages;
    U8 numTexMatrices;
    U8 numIndStages;
    U8 numIndMatrices;
    S16 tevColors[16];
    U8 kColors[16];
    GCNMTTexMatrix* texMatrices;
    GCNMTTexCoordGen* texCoordGens;
    GCNMTIndMatrix* indMatrices;
    GCNMTIndStage* indStages;
    GCNMTTevStage* tevStages;
    S32 srcBlend;
    S32 destBlend;
};

typedef struct GCNMTEffect GCNMTEffect;
struct GCNMTEffect
{
    RpMTEffect effect;
    GCNMTConfig config;
};

// An indirect TEV stage, unpacked from its packed word (GXSetTevIndirect).
typedef struct GCNMTTevInd GCNMTTevInd;
struct GCNMTTevInd
{
    S32 indStage;
    S32 format;
    S32 bias;
    S32 matrix;
    S32 wrapS;
    S32 wrapT;
    U8 addPrev;
    U8 utcLod;
    S32 alpha;
};

#ifdef __cplusplus
extern "C" {
#endif

RpMTEffect* RpGameCubeMTEffectCreate(U32 numTevStages, U32 numTexCoordGens, U32 numTexMatrices,
                                     U32 numIndStages, U32 numIndMatrices);
GCNMTConfig* RpGameCubeMTEffectGetConfig(RpMTEffect* effect);
void RpGameCubeTevIndUnpack(GCNMTTevInd* tevInd, const U32* packed);

S32 _rpGameCubeMTPipeDataQueryNBTs(GCNObjectData* objectData);

#ifdef __cplusplus
}
#endif

#endif
