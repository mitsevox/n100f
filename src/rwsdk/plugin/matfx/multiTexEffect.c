#include <rpmtex.h>
#include <string.h>

#define MATFX_PLUGIN_ID 0x120
#define MTEFFECT_NUMPLATFORMS 9
#define MTEFFECT_PATHSIZE 0x100
#define MTEFFECT_BUFFERSIZE 0x220
#define rwID_MTEFFECTNATIVE 0x20

struct RpMTEffectDict
{
    RwLinkList effectList;
    RwLLLink inGlobalsLink;
};

typedef RpMTEffect* (*MTEffectStreamReadCallBack)(RwStream* stream, S32 platform, U32 version,
                                                  U32 length);
typedef RwStream* (*MTEffectStreamWriteCallBack)(RwStream* stream, RpMTEffect* effect);
typedef U32 (*MTEffectStreamGetSizeCallBack)(RpMTEffect* effect);
typedef void (*MTEffectDestroyCallBack)(RpMTEffect* effect);

// A platform's effect callbacks.
typedef struct MTEffectRegEntry MTEffectRegEntry;
struct MTEffectRegEntry
{
    S32 platform;
    MTEffectDestroyCallBack destroy;
    MTEffectStreamReadCallBack read;
    MTEffectStreamWriteCallBack write;
    MTEffectStreamGetSizeCallBack getSize;
};

static MTEffectRegEntry EffectRegEntries[MTEFFECT_NUMPLATFORMS];
static RpMTEffectDict* DummyDict;

#define MTEFFECTGLOBALS()                                                                          \
    ((MTEffectGlobals*)((U8*)RwEngineInstance + _rpMultiTextureModule.globalsOffset))
#define MTEFFECTGLOBAL(var) (MTEFFECTGLOBALS()->var)

S32 _rpMTEffectSystemInit(void)
{
    memset(EffectRegEntries, 0, sizeof(EffectRegEntries));

    return TRUE;
}

S32 _rpMTEffectRegisterPlatform(S32 platform, MTEffectStreamReadCallBack readCB,
                                MTEffectStreamWriteCallBack writeCB,
                                MTEffectStreamGetSizeCallBack getSizeCB,
                                MTEffectDestroyCallBack destroyCB)
{
    MTEffectRegEntry* regEntry = &EffectRegEntries[platform];

    regEntry->platform = platform;
    regEntry->read = readCB;
    regEntry->write = writeCB;
    regEntry->getSize = getSizeCB;
    regEntry->destroy = destroyCB;

    return TRUE;
}

S32 _rpMTEffectOpen(void)
{
    char* buffer;

    rwLinkListInitialize(&MTEFFECTGLOBAL(dictList));

    DummyDict = RpMTEffectDictCreate();

    if (!DummyDict)
    {
        return FALSE;
    }

    MTEFFECTGLOBAL(currentDict) = DummyDict;

    buffer = (char*)RwMalloc(MTEFFECT_BUFFERSIZE);

    if (!buffer)
    {
        RwError err;

        RpMTEffectDictDestroy(MTEFFECTGLOBAL(currentDict));

        err.pluginID = MATFX_PLUGIN_ID;
        err.errorCode = _rwerror(0x80000013, MTEFFECT_BUFFERSIZE);
        RwErrorSet(&err);
        return FALSE;
    }

    memset(buffer, 0, MTEFFECT_BUFFERSIZE);

    MTEFFECTGLOBAL(path) = buffer;
    MTEFFECTGLOBAL(fullPath) = buffer + MTEFFECT_PATHSIZE;
    MTEFFECTGLOBAL(pathSize) = MTEFFECT_PATHSIZE;

    return TRUE;
}

S32 _rpMTEffectClose(void)
{
    RwLLLink* cur;
    RwLinkList* dictList;

    if (MTEFFECTGLOBAL(path))
    {
        RwFree(MTEFFECTGLOBAL(path));
        MTEFFECTGLOBAL(path) = NULL;
        MTEFFECTGLOBAL(fullPath) = NULL;
        MTEFFECTGLOBAL(pathSize) = 0;
    }

    dictList = &MTEFFECTGLOBAL(dictList);
    cur = rwLinkListGetFirstLLLink(dictList);

    while (cur != rwLinkListGetTerminator(dictList))
    {
        RpMTEffectDict* dict = rwLLLinkGetData(cur, RpMTEffectDict, inGlobalsLink);

        if (dict == DummyDict)
        {
            RpMTEffectDictDestroy(DummyDict);
            DummyDict = NULL;
            break;
        }

        cur = rwLLLinkGetNext(cur);
    }

    return TRUE;
}

RpMTEffect* _rpMTEffectInit(RpMTEffect* effect, S32 platform)
{
    memset(effect, 0, sizeof(RpMTEffect));

    effect->platform = platform;
    effect->refCount = 1;
    rwLLLinkInitialize(&effect->inDictLink);

    if (platform && MTEFFECTGLOBAL(currentDict))
    {
        RpMTEffectDictAddEffect(MTEFFECTGLOBAL(currentDict), effect);
    }

    return effect;
}

RpMTEffectDict* RpMTEffectDictCreate(void)
{
    RpMTEffectDict* dict = (RpMTEffectDict*)RwMalloc(sizeof(RpMTEffectDict));

    if (!dict)
    {
        RwError err;

        err.pluginID = MATFX_PLUGIN_ID;
        err.errorCode = _rwerror(0x80000013, sizeof(RpMTEffectDict));
        RwErrorSet(&err);
        return NULL;
    }

    rwLinkListInitialize(&dict->effectList);
    rwLinkListAddLLLink(&MTEFFECTGLOBAL(dictList), &dict->inGlobalsLink);

    return dict;
}

void RpMTEffectDictDestroy(RpMTEffectDict* dict)
{
    RwLLLink* cur;

    if (dict == MTEFFECTGLOBAL(currentDict))
    {
        MTEFFECTGLOBAL(currentDict) = NULL;
    }

    cur = rwLinkListGetFirstLLLink(&dict->effectList);

    while (cur != rwLinkListGetTerminator(&dict->effectList))
    {
        RpMTEffect* effect = rwLLLinkGetData(cur, RpMTEffect, inDictLink);

        cur = rwLLLinkGetNext(cur);
        RpMTEffectDictRemoveEffect(effect);
    }

    rwLinkListRemoveLLLink(&dict->inGlobalsLink);

    RwFree(dict);
}

RpMTEffectDict* RpMTEffectDictAddEffect(RpMTEffectDict* dict, RpMTEffect* effect)
{
    if (rwLLLinkAttached(&effect->inDictLink))
    {
        rwLinkListRemoveLLLink(&effect->inDictLink);
        RpMTEffectDestroy(effect);
    }

    rwLinkListAddLLLink(&dict->effectList, &effect->inDictLink);
    RpMTEffectAddRef(effect);

    return dict;
}

RpMTEffect* RpMTEffectDictRemoveEffect(RpMTEffect* effect)
{
    if (rwLLLinkAttached(&effect->inDictLink))
    {
        rwLinkListRemoveLLLink(&effect->inDictLink);
        RpMTEffectDestroy(effect);
    }

    return effect;
}

RpMTEffect* RpMTEffectCreateDummy(void)
{
    RpMTEffect* effect = (RpMTEffect*)RwMalloc(sizeof(RpMTEffect));

    if (!effect)
    {
        RwError err;

        err.pluginID = MATFX_PLUGIN_ID;
        err.errorCode = _rwerror(0x80000013, sizeof(RpMTEffect));
        RwErrorSet(&err);
        return NULL;
    }

    return _rpMTEffectInit(effect, 0);
}

void RpMTEffectDestroy(RpMTEffect* effect)
{
    effect->refCount--;

    if (effect->refCount == 0)
    {
        RpMTEffectDictRemoveEffect(effect);

        if (effect->platform && EffectRegEntries[effect->platform].destroy)
        {
            EffectRegEntries[effect->platform].destroy(effect);
        }
        else
        {
            RwFree(effect);
        }
    }
}

static RpMTEffect* MTEffectDictFindNamedEffect(RpMTEffectDict* dict, const char* name)
{
    RwLLLink* cur = rwLinkListGetFirstLLLink(&dict->effectList);

    while (cur != rwLinkListGetTerminator(&dict->effectList))
    {
        RpMTEffect* effect = rwLLLinkGetData(cur, RpMTEffect, inDictLink);

        if (!rwstrcmp(effect->name, name))
        {
            return effect;
        }

        cur = rwLLLinkGetNext(cur);
    }

    return NULL;
}

static RpMTEffect* MTEffectStreamRead(RwStream* stream)
{
    S32 platform;
    U32 version;
    U32 length;
    char name[32];
    MTEffectRegEntry* regEntry;
    RpMTEffect* effect;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, NULL, NULL) ||
        !RwStreamRead(stream, &platform, sizeof(S32)))
    {
        return NULL;
    }

    RwMemNative32(&platform, sizeof(S32));

    regEntry = &EffectRegEntries[platform];

    if (!regEntry->read)
    {
        return NULL;
    }

    if (!_rwStringStreamFindAndRead(name, stream))
    {
        return NULL;
    }

    if (!RwStreamFindChunk(stream, rwID_EXTENSION, &length, &version))
    {
        return NULL;
    }

    effect = regEntry->read(stream, platform, version, length);

    if (!effect)
    {
        return NULL;
    }

    RpMTEffectSetName(effect, name);

    return effect;
}

RpMTEffect* RpMTEffectFind(const char* name)
{
    MTEffectGlobals* globals = MTEFFECTGLOBALS();
    RpMTEffect* effect = NULL;
    RwStream* stream;
    char* fullPath;

    if (globals->currentDict)
    {
        effect = MTEffectDictFindNamedEffect(globals->currentDict, name);
    }
    else
    {
        RwLLLink* cur = rwLinkListGetFirstLLLink(&globals->dictList);

        while (cur != rwLinkListGetTerminator(&globals->dictList))
        {
            RpMTEffectDict* dict = rwLLLinkGetData(cur, RpMTEffectDict, inGlobalsLink);

            effect = MTEffectDictFindNamedEffect(dict, name);

            if (effect)
            {
                break;
            }

            cur = rwLLLinkGetNext(cur);
        }
    }

    if (effect)
    {
        RpMTEffectAddRef(effect);
        return effect;
    }

    fullPath = MTEFFECTGLOBAL(fullPath);
    rwstrcpy(fullPath, MTEFFECTGLOBAL(path));
    rwstrncat(fullPath, name, 31);

    stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, fullPath);

    if (!stream)
    {
        return NULL;
    }

    if (!RwStreamFindChunk(stream, rwID_MTEFFECTNATIVE, NULL, NULL))
    {
        RwStreamClose(stream, NULL);
        return NULL;
    }

    effect = MTEffectStreamRead(stream);

    RwStreamClose(stream, NULL);

    return effect;
}

RpMTEffect* RpMTEffectSetName(RpMTEffect* effect, const char* name)
{
    rwstrncpy(effect->name, name, 31);

    return effect;
}

RpMTEffect* RpMTEffectAddRef(RpMTEffect* effect)
{
    effect->refCount++;

    return effect;
}
