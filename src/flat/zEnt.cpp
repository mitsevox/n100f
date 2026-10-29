#include "zEnt.h"

#include "iMath.h"
#include "iModel.h"
#include "xAnim.h"
#include "xString.h"
#include "xstransvc.h"
#include "zAnimList.h"
#include "zGlobals.h"
#include "zScene.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _ShadowParams
{
    U32 type;
    F32 at;
    F32 rad;
};

struct zAssetModelInfo
{
    U32 Magic;
    U32 NumModelInst;
    U32 AnimTableID;
};

struct zAssetModelInst
{
    U32 ModelID;
    U16 Flags;
    U8 Parent;
    U8 Bone;
    F32 RotPos[3][4];
};

// Shadow offset and radius per model type; 999 ends the list.
_ShadowParams gShadowParams[27] = {
    { 3, 0.2f, 1.25f },  { 4, -0.2f, 1.4f },  { 7, -0.4f, 1.4f },   { 9, 0.4f, 1.25f },
    { 15, 0.0f, 1.5f },  { 17, 0.0f, 1.25f }, { 18, 0.0f, 1.5f },   { 19, 0.25f, 1.5f },
    { 20, 1.0f, 1.75f }, { 21, 0.5f, 1.25f }, { 22, 0.0f, 1.25f },  { 24, 0.0f, 1.25f },
    { 26, 0.0f, 1.75f }, { 46, 0.25f, 1.5f }, { 12, 0.0f, 1.5f },   { 28, 0.0f, 1.5f },
    { 29, 0.0f, 1.5f },  { 30, 0.0f, 1.5f },  { 31, 0.0f, 1.5f },   { 32, 0.0f, 1.5f },
    { 33, 0.0f, 1.5f },  { 34, 0.0f, 1.5f },  { 35, 0.0f, 1.5f },   { 36, 0.0f, 1.5f },
    { 37, 0.0f, 1.5f },  { 999, 0.0f, 1.5f }, { 999, 0.0f, 1.5f },
};

// Set up an entity of the given asset type: its collision class and bound, its model and, from
// its animation list, its animation table starting in the "idle" state.
void zEntInit(_zEnt* ent, p2EntAsset* asset, U32 type)
{
    xEntInit(ent, asset);

    if (type == 'PLYR')
    {
        ent->collType = 0x10;
        ent->collLev = 4;
        ent->bound.type = 1;
        zEntParseModelInfo(ent, asset->modelInfoID);
    }
    else if (type == 'NPC ')
    {
        ent->collType = 0x8;
        ent->collLev = 4;
        ent->bound.type = 1;
        zEntParseModelInfo(ent, asset->modelInfoID);
    }
    else if (type == 'ITEM')
    {
        ent->collType = 0x2;
        ent->collLev = 4;
        ent->bound.type = 1;
        ent->eventFunc = NULL;
        zEntParseModelInfo(ent, asset->modelInfoID);
    }
    else if (type == 'PKUP')
    {
        ent->collType = 0;
        ent->bound.type = 0;
    }
    else if (type == 'PLAT')
    {
        ent->collType = 0x4;
        if (asset->moreFlags & 2)
        {
            ent->collLev = 5;
        }
        else
        {
            ent->collLev = 4;
        }
        zEntParseModelInfo(ent, asset->modelInfoID);
        ent->bound.type = 4;
        ent->bound.mat = (xMat4x3*)ent->model->Mat;
    }
    else if (type == 'PEND')
    {
        ent->collType = 0x4;
        if (asset->moreFlags & 2)
        {
            ent->collLev = 5;
        }
        else
        {
            ent->collLev = 4;
        }
        zEntParseModelInfo(ent, asset->modelInfoID);
        ent->bound.type = 4;
        ent->bound.mat = (xMat4x3*)ent->model->Mat;
    }
    else if (type == 'TRIG')
    {
        ent->collType = 0x1;
        ent->bound.type = 0;
    }
    else if (type == 'HANG')
    {
        ent->collType = 0x4;
        ent->collLev = 4;
        ent->bound.type = 1;
        zEntParseModelInfo(ent, asset->modelInfoID);
    }
    else if (type == 'SIMP')
    {
        ent->collType = 0x2;
        if (asset->moreFlags & 2)
        {
            ent->collLev = 5;
        }
        else
        {
            ent->collLev = 4;
        }
        ent->bound.type = 4;
        zEntParseModelInfo(ent, asset->modelInfoID);
        ent->bound.mat = (xMat4x3*)ent->model->Mat;
    }
    else if (type == 'UI  ')
    {
        ent->collType = 0;
        ent->bound.type = 0;
        if (ent->baseType != 0x21 && asset->modelInfoID != 0)
        {
            U32 size;
            void* buf = xSTFindAsset(asset->modelInfoID, &size);
            xEntLoadModel(ent, (RpAtomic*)buf);
            if (ent->model != NULL)
            {
                ent->bound.mat = (xMat4x3*)ent->model->Mat;
            }
        }
    }
    else if (type == 'BUTN')
    {
        ent->collType = 0x4;
        if (asset->moreFlags & 2)
        {
            ent->collLev = 5;
        }
        else
        {
            ent->collLev = 4;
        }
        ent->bound.type = 4;
        zEntParseModelInfo(ent, asset->modelInfoID);
        ent->bound.mat = (xMat4x3*)ent->model->Mat;
    }
    else if (type == 'DSTR')
    {
        ent->collType = 0x4;
        if (asset->moreFlags & 2)
        {
            ent->collLev = 5;
        }
        else
        {
            ent->collLev = 4;
        }
        ent->bound.type = 4;
        zEntParseModelInfo(ent, asset->modelInfoID);
        ent->bound.mat = (xMat4x3*)ent->model->Mat;
    }
    else if (type == 'EGEN')
    {
        ent->collType = 0x2;
        if (asset->moreFlags & 2)
        {
            ent->collLev = 5;
        }
        else
        {
            ent->collLev = 4;
        }
        ent->bound.type = 4;
        zEntParseModelInfo(ent, asset->modelInfoID);
        ent->bound.mat = (xMat4x3*)ent->model->Mat;
    }

    if (asset->animListID != 0)
    {
        S32 num_used = zAnimListGetNumUsed(asset->animListID);
        if (num_used > 0)
        {
            ent->atbl = zAnimListGetTable(asset->animListID);
            xAnimPoolAlloc(&globals.sceneCur->mempool, ent, ent->atbl, ent->model);
            xAnimState* ast = xAnimTableGetState(ent->atbl, "idle");
            if (ast != NULL)
            {
                xAnimSingle* single = ent->model->Anim->Single;
                single->State = ast;
                single->Time = 0.0f;
                single->CurrentSpeed = 1.0f;
            }
        }
        else
        {
            ent->atbl = NULL;
        }
    }
    else
    {
        ent->atbl = NULL;
    }

    xEntInitForType(ent);
}

// Set up the entity part of a game object.
void zEntSetup(_zEnt* ent)
{
    xEntSetup(ent);
}

// Save the entity state.
void zEntSave(_zEnt* ent, xSerial* s)
{
    xEntSave(ent, s);
}

// Restore the entity state.
void zEntLoad(_zEnt* ent, xSerial* s)
{
    xEntLoad(ent, s);
}

// Reset the entity part of a game object.
void zEntReset(_zEnt* ent)
{
    xEntReset(ent);
}

// Send an event to every object in the scene.
void zEntEventAll(xBase* from, U32 fromEvent, U32 toEvent, F32* toParam)
{
    zScene* s = globals.sceneCur;

    for (U16 i = 0; i < s->num_base; i++)
    {
        zEntEvent(from, fromEvent, s->base[i], toEvent, toParam, NULL, 0);
    }
}

// Send an event to every scene object of one base type.
void zEntEventAllOfType(xBase* from, U32 fromEvent, U32 toEvent, F32* toParam, U32 type)
{
    zScene* s = globals.sceneCur;
    if (s == NULL)
    {
        return;
    }

    for (U16 i = 0; i < s->num_base; i++)
    {
        xBase* base = s->base[i];
        if (type == base->baseType)
        {
            zEntEvent(from, fromEvent, base, toEvent, toParam, NULL, 0);
        }
    }
}

// Send an event to every scene object of one base type.
void zEntEventAllOfType(U32 toEvent, U32 type)
{
    zEntEventAllOfType(NULL, 0, toEvent, NULL, type);
}

// Send an event to the scene object with the given name.
void zEntEvent(char* to, U32 toEvent)
{
    U32 id = xStrHash(to);
    xBase* sendTo = zSceneFindObject(id);

    if (sendTo != NULL)
    {
        zEntEvent(sendTo, toEvent);
    }
}

// Send an event with no source, parameters or widget.
void zEntEvent(xBase* to, U32 toEvent)
{
    zEntEvent(NULL, 0, to, toEvent, NULL, NULL, 0);
}

// Send an event with parameters and no source.
void zEntEvent(xBase* to, U32 toEvent, F32* toParam)
{
    zEntEvent(NULL, 0, to, toEvent, toParam, NULL, 0);
}

// Send an event with parameters and a widget, no source.
void zEntEvent(xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    zEntEvent(NULL, 0, to, toEvent, toParam, toParamWidget, 0);
}

// Send an event from one object to another without parameters.
void zEntEvent(xBase* from, xBase* to, U32 toEvent)
{
    zEntEvent(from, 0, to, toEvent, NULL, NULL, 0);
}

// Send an event to an object: the disable and enable events switch it first, then its handler
// runs and its links fire on to their targets.
void zEntEvent(xBase* from, U32 fromEvent, xBase* to, U32 toEvent, F32* toParam,
               xBase* toParamWidget, S32 forceEvent)
{
    if (toEvent == 2)
    {
        xBaseDisable(to);
    }
    else if (toEvent == 1)
    {
        xBaseEnable(to);
    }

    if (to->eventFunc != NULL && (xBaseIsEnabled(to) || forceEvent))
    {
        to->eventFunc(from, to, toEvent, toParam, toParamWidget);
    }

    if (xBaseIsEnabled(to) && to->linkCount != 0)
    {
        p2LinkAsset* idx = to->link;

        for (S32 i = 0; i < to->linkCount; i++, idx++)
        {
            if (toEvent == idx->srcEvent)
            {
                if (idx->chkAssetID == 0 || (from != NULL && idx->chkAssetID == from->id))
                {
                    xBase* sendTo = zSceneFindObject(idx->dstAssetID);

                    if (sendTo != NULL)
                    {
                        xBase* b;
                        if (idx->paramWidgetAssetID != 0)
                        {
                            b = zSceneFindObject(idx->paramWidgetAssetID);
                        }
                        else
                        {
                            b = NULL;
                        }

                        zEntEvent(to, toEvent, sendTo, idx->dstEvent, idx->param, b, 0);
                    }
                }
            }
        }
    }
}

// Build the model hierarchy a 'MINF' asset describes; nested 'MINF' entries recurse. Extra
// atomics in a model file hang off its instance.
xModelInstance* zEntRecurseModelInfo(void* info, _xEnt* ent)
{
    zAssetModelInfo* zinfo = (zAssetModelInfo*)info;
    zAssetModelInst* zinst = (zAssetModelInst*)(zinfo + 1);
    xModelInstance* tempInst[64];
    U32 bufsize;

    for (U32 i = 0; i < zinfo->NumModelInst; i++)
    {
        RpAtomic* imodel = (RpAtomic*)xSTFindAsset(zinst[i].ModelID, &bufsize);
        if (*(U32*)imodel == 0x464E494D)
        {
            tempInst[i] = zEntRecurseModelInfo(imodel, ent);
            if (i != 0)
            {
                tempInst[i]->Flags |= zinst[i].Flags;
                tempInst[i]->BoneIndex = zinst[i].Bone;
                xModelInstanceAttach(tempInst[i], tempInst[zinst[i].Parent]);
            }
        }
        else if (i == 0)
        {
            tempInst[i] = xModelInstanceAlloc(imodel, ent, 0, 0, NULL);
            if (ent->baseType != 3)
            {
                while ((imodel = iModelFile_RWMultiAtomic(imodel)) != NULL)
                {
                    xModelInstanceAttach(xModelInstanceAlloc(imodel, ent, 0x8, 0, NULL),
                                         tempInst[i]);
                }
            }
        }
        else
        {
            tempInst[i] = xModelInstanceAlloc(imodel, ent, zinst[i].Flags, zinst[i].Bone, NULL);
            xModelInstanceAttach(tempInst[i], tempInst[zinst[i].Parent]);
        }
    }

    if (zinfo->AnimTableID != 0)
    {
        xAnimTable* table = (xAnimTable*)xSTFindAsset(zinfo->AnimTableID, &bufsize);
        tempInst[0]->Anim = xAnimPoolAlloc(&globals.sceneCur->mempool, ent, table, tempInst[0]);
    }
    return tempInst[0];
}

// An entity's model asset is either a plain model or a 'MINF' hierarchy.
void zEntParseModelInfo(_xEnt* ent, U32 assetID)
{
    U32 bufsize;
    void* info = xSTFindAsset(assetID, &bufsize);

    if (*(U32*)info == 0x464E494D)
    {
        ent->model = zEntRecurseModelInfo(info, ent);
    }
    else
    {
        xEntLoadModel(ent, (RpAtomic*)info);
    }
}

// Script animation events: play a numbered loop or stop state, freeze or resume the clip, or
// pick a stop state at random.
void zEntAnimEvent(_zEnt* ent, U32 animEvent, F32* animParam)
{
    xAnimSingle* single = ent->model->Anim->Single;

    switch (animEvent)
    {
    case 0xC3:
    case 0xC4:
    {
        if (animParam == NULL)
        {
            break;
        }

        S32 anum = (S32)animParam[0] - 1;
        if (anum < 0 || anum >= 10 || ent->atbl == NULL)
        {
            break;
        }

        char name[12];
        if (animEvent == 0xC4)
        {
            sprintf(name, "loop%d", anum);
        }
        else
        {
            sprintf(name, "stop%d", anum);
        }

        xAnimState* ast = xAnimTableGetState(ent->atbl, name);
        if (ast != NULL)
        {
            single->State = ast;
            single->Time = 0.0f;
            single->CurrentSpeed = 1.0f;
        }
        break;
    }
    case 0xC5:
    {
        char name[12];
        strcpy(name, single->State->Name);
        name[0] = 's';
        name[1] = 't';
        single->State = xAnimTableGetState(ent->atbl, name);
        break;
    }
    case 0xC6:
        single->CurrentSpeed = 0.0f;
        break;
    case 0xC7:
        single->CurrentSpeed = 1.0f;
        break;
    case 0xC8:
        if (single->CurrentSpeed)
        {
            single->CurrentSpeed = 0.0f;
        }
        else
        {
            single->CurrentSpeed = 1.0f;
        }
        break;
    case 0xC9:
    {
        if (animParam == NULL)
        {
            break;
        }

        S32 anum1 = (S32)animParam[0] - 1;
        S32 anum2 = (S32)animParam[1] - 1;
        if (anum1 < 0 || anum1 > anum2 || anum2 >= 10 || ent->atbl == NULL)
        {
            break;
        }

        S32 anum = irand();
        anum2 = anum2 - anum1 + 1;
        anum %= anum2;

        char name[12];
        sprintf(name, "stop%d", anum1 + anum);

        xAnimState* ast = xAnimTableGetState(ent->atbl, name);
        if (ast != NULL)
        {
            single->State = ast;
            single->Time = 0.0f;
            single->CurrentSpeed = 1.0f;
        }
        break;
    }
    case 0xCA:
    {
        if (animParam == NULL)
        {
            break;
        }

        F32 prob = 0.01f * animParam[1];
        S32 anum = (S32)animParam[0] - 1;
        if (anum < 0 || anum >= 10)
        {
            break;
        }

        if (iurand() < prob && ent->atbl != NULL)
        {
            char name[12];
            sprintf(name, "stop%d", anum);

            xAnimState* ast = xAnimTableGetState(ent->atbl, name);
            if (ast != NULL)
            {
                single->State = ast;
                single->Time = 0.0f;
                single->CurrentSpeed = 1.0f;
            }
        }
        break;
    }
    }
}

// The parameters follow the instance list of a multi-model ('MINF') asset.
zAssetModelParam* zEntGetModelParams(U32 assetID, U32* size)
{
    U32 bufsize;
    void* info = xSTFindAsset(assetID, &bufsize);
    zAssetModelInfo* minf = (zAssetModelInfo*)info;

    if (minf->Magic == 0x464E494D)
    {
        *size = bufsize - (minf->NumModelInst * sizeof(zAssetModelInst) + sizeof(zAssetModelInfo));
        if (*size != 0)
        {
            return (zAssetModelParam*)((char*)minf + minf->NumModelInst * sizeof(zAssetModelInst) +
                                       sizeof(zAssetModelInfo));
        }
    }
    else
    {
        *size = 0;
    }

    return NULL;
}

// Each parameter is an 8-byte header followed by WordLength words of string.
char* zParamGetString(zAssetModelParam* param, U32 size, char* tok, char* def)
{
    U32 testhash = xStrHash(tok);

    while (param != NULL && size != 0)
    {
        if (param->HashID == testhash)
        {
            return (char*)param->String;
        }

        size -= param->WordLength * 4 + 8;
        param = (zAssetModelParam*)((char*)param + (param->WordLength * 4 + 8));
    }
    return def;
}

// Read an integer model parameter, or def when it is missing.
S32 zParamGetInt(zAssetModelParam* param, U32 size, const char* tok, S32 def)
{
    char* str = zParamGetString(param, size, (char*)tok, NULL);
    if (str != NULL)
    {
        return atoi(str);
    }
    return def;
}

// Read a float model parameter, or def when it is missing.
F32 zParamGetFloat(zAssetModelParam* param, U32 size, const char* tok, F32 def)
{
    char* str = zParamGetString(param, size, (char*)tok, NULL);
    if (str != NULL)
    {
        return atof(str);
    }
    return def;
}

S32 zParamGetVector(zAssetModelParam* param, U32 size, char* tok, xVec3 def,
                    xVec3* result)
{
    char* str = zParamGetString(param, size, tok, NULL);
    S32 list = 0;
    xVec3 vec = { 0.0f, 0.0f, 0.0f };

    *result = def;
    if (str != NULL)
    {
        list = xStrParseFloatList((F32*)&vec, str, 3);
        if (list > 0)
        {
            result->x = vec.x;
        }
        if (list > 1)
        {
            result->y = vec.y;
        }
        if (list > 2)
        {
            result->z = vec.z;
        }
    }
    return list;
}

// Read up to three components of a vector parameter over the default.
S32 zParamGetVector(zAssetModelParam* param, U32 size, const char* tok, xVec3 def,
                    xVec3* result)
{
    return zParamGetVector(param, size, (char*)tok, def, result);
}

// The shadow of an entity sits on its bounding sphere and is a little wider than it. Villains of
// a listed subtype move it along their facing and scale it.
void zEntGetShadowParams(_xEnt* ent, xVec3* center, F32* radius)
{
    _ShadowParams* sp;
    xVec3* at;
    U32 i;
    F32 rad;
    xVec3 c;

    at = (xVec3*)&ent->model->Mat->at;
    rad = (ent->bound.sph.r < 0.01f) ? 2.0f : 2.4f * ent->bound.sph.r;
    c = ent->bound.sph.center;

    if (ent->baseType == 2)
    {
        sp = gShadowParams;
        for (i = 0; i < 27; i++)
        {
            if (ent->subType == sp->type)
            {
                rad *= sp->rad;
                c.x += sp->at * at->x;
                c.y += sp->at * at->y;
                c.z += sp->at * at->z;
                break;
            }
            sp++;
        }
    }

    *radius = rad;
    *center = c;
}
