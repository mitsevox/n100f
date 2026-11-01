#include <rpmtexgcn.h>
#include <string.h>

// GameCube multitexture effects: an effect is a TEV setup (stages, texture coordinate generators,
// texture matrices, indirect stages and indirect matrices), allocated in one block with the effect,
// and streamed as a fixed header followed by one small record per element.

void* RwMemLittleEndian16(void* mem, U32 size);
RpMTEffect* _rpMTEffectInit(RpMTEffect* effect, S32 platform);
S32 _rpMTEffectRegisterPlatform(S32 platform, void* readCB, void* writeCB, void* getSizeCB,
                                void* destroyCB);

// The multitexture plugin id and this platform. Written for this project.
#define MTGCN_PLUGINID 0x120
#define MTGCN_PLATFORM 6

// The streamed header. Streams older than 0x33002 stop before the last three counts.
typedef struct GCNMTStreamHeader GCNMTStreamHeader;
struct GCNMTStreamHeader
{
    U8 numTexCoordGens;
    U8 numTevStages;
    S8 flags;
    U8 blend;
    U16 tevColors[16];
    U8 kColors[16];
    U8 numTexMatrices;
    U8 numIndMatrices;
    U8 numIndStages;
    U8 pad;
};

typedef struct GCNMTStreamTexCoordGen GCNMTStreamTexCoordGen;
struct GCNMTStreamTexCoordGen
{
    U8 value[5];
    U8 pad[3];
    U16 scaleS;
    U16 scaleT;
};

typedef struct GCNMTStreamTevStage GCNMTStreamTevStage;
struct GCNMTStreamTevStage
{
    U8 value[24];
    U32 indirect;
};

typedef struct GCNMTStreamTexMatrix GCNMTStreamTexMatrix;
struct GCNMTStreamTexMatrix
{
    U8 value[3];
    U8 pad;
};

typedef struct GCNMTStreamIndMatrix GCNMTStreamIndMatrix;
struct GCNMTStreamIndMatrix
{
    U8 value[3];
    S8 scaleExp;
};

typedef struct GCNMTStreamIndStage GCNMTStreamIndStage;
struct GCNMTStreamIndStage
{
    U8 value[5];
    U8 pad[3];
};

// A TEV stage's fields, one byte each in the stream.
#define TEVSTAGE_PACK(_rec, _stage)                                                                \
    do                                                                                            \
    {                                                                                             \
        (_rec)->value[0] = (_stage)->colorIn[0];                                                  \
        (_rec)->value[1] = (_stage)->colorIn[1];                                                  \
        (_rec)->value[2] = (_stage)->colorIn[2];                                                  \
        (_rec)->value[3] = (_stage)->colorIn[3];                                                  \
        (_rec)->value[4] = (_stage)->alphaIn[0];                                                  \
        (_rec)->value[5] = (_stage)->alphaIn[1];                                                  \
        (_rec)->value[6] = (_stage)->alphaIn[2];                                                  \
        (_rec)->value[7] = (_stage)->alphaIn[3];                                                  \
        (_rec)->value[8] = (_stage)->colorOp;                                                     \
        (_rec)->value[9] = (_stage)->colorBias;                                                   \
        (_rec)->value[10] = (_stage)->colorScale;                                                 \
        (_rec)->value[11] = (_stage)->colorClamp;                                                 \
        (_rec)->value[12] = (_stage)->colorOut;                                                   \
        (_rec)->value[13] = (_stage)->alphaOp;                                                    \
        (_rec)->value[14] = (_stage)->alphaBias;                                                  \
        (_rec)->value[15] = (_stage)->alphaScale;                                                 \
        (_rec)->value[16] = (_stage)->alphaClamp;                                                 \
        (_rec)->value[17] = (_stage)->alphaOut;                                                   \
        (_rec)->value[18] = (_stage)->kColorSel;                                                  \
        (_rec)->value[19] = (_stage)->kAlphaSel;                                                  \
        (_rec)->value[20] = (_stage)->texCoord;                                                   \
        (_rec)->value[21] = (_stage)->texMap;                                                     \
        (_rec)->value[22] = (_stage)->channel;                                                    \
        (_rec)->value[23] = (_stage)->flags;                                                      \
    } while (0)

#define TEVSTAGE_UNPACK(_stage, _rec)                                                              \
    do                                                                                            \
    {                                                                                             \
        (_stage)->colorIn[0] = (_rec)->value[0];                                                  \
        (_stage)->colorIn[1] = (_rec)->value[1];                                                  \
        (_stage)->colorIn[2] = (_rec)->value[2];                                                  \
        (_stage)->colorIn[3] = (_rec)->value[3];                                                  \
        (_stage)->alphaIn[0] = (_rec)->value[4];                                                  \
        (_stage)->alphaIn[1] = (_rec)->value[5];                                                  \
        (_stage)->alphaIn[2] = (_rec)->value[6];                                                  \
        (_stage)->alphaIn[3] = (_rec)->value[7];                                                  \
        (_stage)->colorOp = (_rec)->value[8];                                                     \
        (_stage)->colorBias = (_rec)->value[9];                                                   \
        (_stage)->colorScale = (_rec)->value[10];                                                 \
        (_stage)->colorClamp = (_rec)->value[11];                                                 \
        (_stage)->colorOut = (_rec)->value[12];                                                   \
        (_stage)->alphaOp = (_rec)->value[13];                                                    \
        (_stage)->alphaBias = (_rec)->value[14];                                                  \
        (_stage)->alphaScale = (_rec)->value[15];                                                 \
        (_stage)->alphaClamp = (_rec)->value[16];                                                 \
        (_stage)->alphaOut = (_rec)->value[17];                                                   \
        (_stage)->kColorSel = (_rec)->value[18];                                                  \
        (_stage)->kAlphaSel = (_rec)->value[19];                                                  \
        (_stage)->texCoord = (_rec)->value[20];                                                   \
        (_stage)->texMap = (_rec)->value[21];                                                     \
        (_stage)->channel = (_rec)->value[22];                                                    \
        (_stage)->flags = (_rec)->value[23];                                                      \
    } while (0)

// The size of an effect's stream data.
#define MTGCN_STREAMSIZE(_config)                                                                  \
    ((_config)->numTexMatrices * (sizeof(GCNMTStreamTexMatrix) + sizeof(F32) * 12) +             \
     sizeof(GCNMTStreamHeader) +                                                                   \
     (_config)->numTexCoordGens * sizeof(GCNMTStreamTexCoordGen) +                                 \
     (_config)->numIndMatrices * (sizeof(GCNMTStreamIndMatrix) + sizeof(F32) * 6) +              \
     (_config)->numIndStages * sizeof(GCNMTStreamIndStage) +                                       \
     (_config)->numTevStages * sizeof(GCNMTStreamTevStage))


U32 GameCubeMTEffectStreamGetSize(RpMTEffect* effect)
{
    GCNMTConfig* config = &((GCNMTEffect*)effect)->config;

    return MTGCN_STREAMSIZE(config);
}

RwStream* GameCubeMTEffectStreamWrite(RpMTEffect* effect, RwStream* stream)
{
    GCNMTConfig* config = &((GCNMTEffect*)effect)->config;
    GCNMTStreamHeader header;
    U32 i;

    if (!_rwStreamWriteVersionedChunkHeader(stream, rwID_STRUCT, MTGCN_STREAMSIZE(config), 0x33002,
                                            0xFFFF))
    {
        return NULL;
    }

    header.numTexCoordGens = config->numTexCoordGens;
    header.numTevStages = config->numTevStages;
    header.flags = config->flags;
    header.numTexMatrices = config->numTexMatrices;
    header.numIndMatrices = config->numIndMatrices;
    header.numIndStages = config->numIndStages;
    header.blend = (config->destBlend & 0xF) | ((config->srcBlend << 4) & 0xF0);
    memcpy(header.kColors, config->kColors, sizeof(header.kColors));
    memcpy(header.tevColors, config->tevColors, sizeof(header.tevColors));
    RwMemLittleEndian16(header.tevColors, sizeof(header.tevColors));

    if (!RwStreamWrite(stream, &header, sizeof(header)))
    {
        return NULL;
    }

    for (i = 0; i < config->numTexCoordGens; i++)
    {
        GCNMTTexCoordGen* src = &config->texCoordGens[i];
        GCNMTStreamTexCoordGen rec;

        rec.value[0] = src->flags;
        rec.value[1] = src->func;
        rec.value[2] = src->src;
        rec.value[3] = src->mtx;
        rec.value[4] = src->postMtx;
        rec.pad[0] = 0;
        rec.pad[1] = 0;
        rec.pad[2] = 0;
        rec.scaleS = src->scaleS;
        rec.scaleT = src->scaleT;
        RwMemLittleEndian16(&rec.scaleS, sizeof(U16) * 2);

        if (!RwStreamWrite(stream, &rec, sizeof(rec)))
        {
            return NULL;
        }
    }

    for (i = 0; i < config->numTevStages; i++)
    {
        GCNMTTevStage* src = &config->tevStages[i];
        GCNMTStreamTevStage rec;

        TEVSTAGE_PACK(&rec, src);
        rec.indirect = src->indirect;
        RwMemLittleEndian32(&rec.indirect, sizeof(S32));

        if (src->texMap & GCNMTTEV_TEXMAPOFF)
        {
            rec.value[23] |= 0x80;
        }

        if (!RwStreamWrite(stream, &rec, sizeof(rec)))
        {
            return NULL;
        }
    }

    for (i = 0; i < config->numTexMatrices; i++)
    {
        GCNMTTexMatrix* src = &config->texMatrices[i];
        GCNMTStreamTexMatrix rec;

        rec.value[0] = src->id;
        rec.value[1] = src->frame;
        rec.value[2] = src->flags;
        rec.pad = 0;

        if (!RwStreamWrite(stream, &rec, sizeof(rec)) ||
            !RwStreamWriteReal(stream, src->matrix[0], sizeof(src->matrix)))
        {
            return NULL;
        }
    }

    for (i = 0; i < config->numIndMatrices; i++)
    {
        GCNMTIndMatrix* src = &config->indMatrices[i];
        GCNMTStreamIndMatrix rec;

        rec.value[0] = src->id;
        rec.value[1] = src->frame;
        rec.value[2] = src->unk8;
        rec.scaleExp = src->scaleExp;

        if (!RwStreamWrite(stream, &rec, sizeof(rec)) ||
            !RwStreamWriteReal(stream, src->offset[0], sizeof(src->offset)))
        {
            return NULL;
        }
    }

    for (i = 0; i < config->numIndStages; i++)
    {
        GCNMTIndStage* src = &config->indStages[i];
        GCNMTStreamIndStage rec;

        rec.value[0] = src->flags;
        rec.value[1] = src->texCoord;
        rec.value[2] = src->texMap;
        rec.value[3] = src->scaleS;
        rec.value[4] = src->scaleT;
        rec.pad[0] = 0;
        rec.pad[1] = 0;
        rec.pad[2] = 0;

        if (!RwStreamWrite(stream, &rec, sizeof(rec)))
        {
            return NULL;
        }
    }

    return (RwStream*)effect;
}

RpMTEffect* GameCubeMTEffectStreamRead(RwStream* stream, S32 platform, U32 version, U32 length)
{
    S32 old = version < 0x33002;
    GCNMTStreamHeader header;
    RpMTEffect* effect;
    GCNMTConfig* config;
    U32 size;
    U32 i;

    if (old)
    {
        size = sizeof(header) - 4;
        header.numTexMatrices = 0;
        header.numIndMatrices = 0;
        header.numIndStages = 0;
    }
    else
    {
        size = sizeof(header);
    }

    if (size != RwStreamRead(stream, &header, size))
    {
        return NULL;
    }

    effect = RpGameCubeMTEffectCreate(header.numTevStages, header.numTexCoordGens,
                                      header.numTexMatrices, header.numIndStages,
                                      header.numIndMatrices);

    if (!effect)
    {
        return NULL;
    }

    config = RpGameCubeMTEffectGetConfig(effect);
    config->flags = header.flags;
    config->srcBlend = (header.blend >> 4) & 0xF;
    config->destBlend = header.blend & 0xF;
    memcpy(config->kColors, header.kColors, sizeof(header.kColors));
    RwMemNative32(header.tevColors, sizeof(header.tevColors));
    memcpy(config->tevColors, header.tevColors, sizeof(header.tevColors));

    for (i = 0; i < config->numTexCoordGens; i++)
    {
        GCNMTTexCoordGen* dst = &config->texCoordGens[i];
        GCNMTStreamTexCoordGen rec;

        if (old)
        {
            size = 5;
            rec.scaleS = 0;
            rec.scaleT = 0;
        }
        else
        {
            size = sizeof(rec);
        }

        if (size != RwStreamRead(stream, &rec, size))
        {
            RwFree(effect);
            return NULL;
        }

        dst->flags = rec.value[0];
        dst->func = rec.value[1];
        dst->src = rec.value[2];
        dst->mtx = rec.value[3];
        dst->postMtx = rec.value[4];
        RwMemNative32(&rec.scaleS, sizeof(U16) * 2);
        dst->scaleS = rec.scaleS;
        dst->scaleT = rec.scaleT;
    }

    for (i = 0; i < config->numTevStages; i++)
    {
        GCNMTTevStage* dst = &config->tevStages[i];
        GCNMTStreamTevStage rec;

        if (old)
        {
            size = sizeof(rec) - sizeof(S32);
            rec.indirect = 0;
        }
        else
        {
            size = sizeof(rec);
        }

        if (size != RwStreamRead(stream, &rec, size))
        {
            RwFree(effect);
            return NULL;
        }

        TEVSTAGE_UNPACK(dst, &rec);
        RwMemNative32(&rec.indirect, sizeof(S32));
        dst->indirect = rec.indirect;

        if (dst->flags & 0x80)
        {
            dst->texMap |= GCNMTTEV_TEXMAPOFF;
            dst->flags &= ~0x80;
        }
    }

    for (i = 0; i < config->numTexMatrices; i++)
    {
        GCNMTTexMatrix* dst = &config->texMatrices[i];
        GCNMTStreamTexMatrix rec;

        if (RwStreamRead(stream, &rec, sizeof(rec)) != sizeof(rec) ||
            !RwStreamReadReal(stream, dst->matrix[0], sizeof(dst->matrix)))
        {
            RwFree(effect);
            return NULL;
        }

        dst->id = rec.value[0];
        dst->frame = rec.value[1];
        dst->flags = rec.value[2];
    }

    for (i = 0; i < config->numIndMatrices; i++)
    {
        GCNMTIndMatrix* dst = &config->indMatrices[i];
        GCNMTStreamIndMatrix rec;

        if (RwStreamRead(stream, &rec, sizeof(rec)) != sizeof(rec) ||
            !RwStreamReadReal(stream, dst->offset[0], sizeof(dst->offset)))
        {
            RwFree(effect);
            return NULL;
        }

        dst->id = rec.value[0];
        dst->frame = rec.value[1];
        dst->unk8 = rec.value[2];
        dst->scaleExp = rec.scaleExp;
    }

    for (i = 0; i < config->numIndStages; i++)
    {
        GCNMTIndStage* dst = &config->indStages[i];
        GCNMTStreamIndStage rec;

        if (RwStreamRead(stream, &rec, sizeof(rec)) != sizeof(rec))
        {
            RwFree(effect);
            return NULL;
        }

        dst->flags = rec.value[0];
        dst->texCoord = rec.value[1];
        dst->texMap = rec.value[2];
        dst->scaleS = rec.value[3];
        dst->scaleT = rec.value[4];
    }

    return effect;
}

S32 _rpGameCubeMTDataPluginAttach(void)
{
    return _rpMTEffectRegisterPlatform(MTGCN_PLATFORM, GameCubeMTEffectStreamRead,
                                       (void*)GameCubeMTEffectStreamWrite,
                                       GameCubeMTEffectStreamGetSize, NULL);
}

// The element arrays follow the effect in the same allocation, each 4-byte aligned.
#define MTGCN_ALIGN4(_size) (((_size) + 3) & ~3)

RpMTEffect* RpGameCubeMTEffectCreate(U32 numTevStages, U32 numTexCoordGens, U32 numTexMatrices,
                                     U32 numIndStages, U32 numIndMatrices)
{
    S32 texCoordGenSize = numTexCoordGens * sizeof(GCNMTTexCoordGen);
    S32 indStageSize = numIndStages * sizeof(GCNMTIndStage);
    S32 texMatrixSize = numTexMatrices * sizeof(GCNMTTexMatrix);
    S32 indMatrixSize = numIndMatrices * sizeof(GCNMTIndMatrix);
    S32 size;
    GCNMTEffect* effect;
    U8* mem;

    size = MTGCN_ALIGN4(sizeof(GCNMTEffect) + texCoordGenSize);
    size = MTGCN_ALIGN4(size + numTevStages * (S32)sizeof(GCNMTTevStage));
    size = MTGCN_ALIGN4(size + indStageSize);
    size = MTGCN_ALIGN4(size + texMatrixSize);
    size = MTGCN_ALIGN4(size + indMatrixSize);

    effect = (GCNMTEffect*)RwMalloc(size);

    if (!effect)
    {
        RwError err;
        err.pluginID = MTGCN_PLUGINID;
        err.errorCode = _rwerror(0x80000013, size);
        RwErrorSet(&err);
        return NULL;
    }

    memset(effect, 0, size);
    _rpMTEffectInit(&effect->effect, MTGCN_PLATFORM);

    effect->config.maxTevStages = numTevStages;
    mem = (U8*)(effect + 1);
    effect->config.maxTexCoordGens = numTexCoordGens;
    effect->config.maxIndStages = numIndStages;
    effect->config.maxTexMatrices = numTexMatrices;
    effect->config.maxIndMatrices = numIndMatrices;
    effect->config.numTevStages = effect->config.maxTevStages;
    effect->config.numTexCoordGens = effect->config.maxTexCoordGens;
    effect->config.numIndStages = effect->config.maxIndStages;
    effect->config.numTexMatrices = effect->config.maxTexMatrices;
    effect->config.numIndMatrices = effect->config.maxIndMatrices;

    if (numTexMatrices)
    {
        effect->config.texMatrices = (GCNMTTexMatrix*)mem;
        mem += texMatrixSize;
    }

    if (numTexCoordGens)
    {
        effect->config.texCoordGens = (GCNMTTexCoordGen*)mem;
        mem += texCoordGenSize;
    }

    if (numIndMatrices)
    {
        effect->config.indMatrices = (GCNMTIndMatrix*)mem;
        mem += indMatrixSize;
    }

    if (numIndStages)
    {
        effect->config.indStages = (GCNMTIndStage*)mem;
        mem += indStageSize;
    }

    if (numTevStages)
    {
        effect->config.tevStages = (GCNMTTevStage*)mem;
    }

    return &effect->effect;
}

GCNMTConfig* RpGameCubeMTEffectGetConfig(RpMTEffect* effect)
{
    return &((GCNMTEffect*)effect)->config;
}

void RpGameCubeTevIndUnpack(GCNMTTevInd* tevInd, const U32* packed)
{
    U32 value = *packed;

    tevInd->indStage = value & 3;
    tevInd->format = (value >> 2) & 3;
    tevInd->bias = (value >> 4) & 7;
    tevInd->matrix = (value >> 7) & 0xF;
    tevInd->wrapS = (value >> 11) & 7;
    tevInd->wrapT = (value >> 14) & 7;
    tevInd->addPrev = (value >> 17) & 1;
    tevInd->utcLod = (value >> 18) & 1;
    tevInd->alpha = (value >> 19) & 3;
}
