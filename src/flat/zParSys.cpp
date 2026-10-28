#include "zParSys.h"

#include "iParMgr.h"
#include "xEvent.h"
#include "xMemMgr.h"
#include "zRenderState.h"
#include "zScene.h"

struct zParSysInfo
{
    S32 type;
    void (*func)(void*, xParGroup*);
};

static zParSysInfo sParSysInfo[7] = {
    { 0, iParMgrRenderParSys_Sprite },     { 1, iParMgrRenderParSys_Streak },
    { 2, iParMgrRenderParSys_Flat },       { 3, iParMgrRenderParSys_Static },
    { 4, iParMgrRenderParSys_Ground },     { 5, iParMgrRenderParSys_QuadStreak },
    { 6, iParMgrRenderParSys_InvStreak },
};

static inline void xParCmdTexInit(xParCmdTex* tex)
{
    tex->unit_count = tex->rows * tex->cols;
    tex->unit_width = (tex->x2 - tex->x1) / tex->cols;
    tex->unit_height = (tex->y2 - tex->y1) / tex->rows;
}

void zParSysInit(void* b, void* tasset)
{
    zParSysInit((xBase*)b, (zParSysAsset*)tasset);
}

void zParSysInit(xBase* b, zParSysAsset* tasset)
{
    zParSys* t = (zParSys*)b;

    xBaseInit(b, tasset);
    t->eventFunc = zParSysEventCB;
    t->tasset = tasset;

    if (t->linkCount)
    {
        t->link = (p2LinkAsset*)((U8*)&t->tasset[1] + tasset->cmdSize);
    }
    else
    {
        t->link = NULL;
    }

    t->visible = tasset->parFlags & 0x1;
    t->cmdCount = tasset->cmdCount;
    if (t->cmdCount != 0U)
    {
        t->cmd = (zParCmd*)xMemAlloc(gActiveHeap, t->cmdCount * sizeof(zParCmd));
    }
    else
    {
        t->cmd = NULL;
    }

    U8* cmdPtr = (U8*)&tasset[1];
    for (S32 i = 0; i < t->cmdCount; i++)
    {
        t->cmd[i].flag = 1;
        t->cmd[i].tasset = (xParCmdAsset*)cmdPtr;
        cmdPtr += zParCmdGetSize(*(U32*)cmdPtr);
    }

    t->group = (_zParGroup*)xMemAlloc(gActiveHeap, sizeof(_zParGroup));
    xParGroupInit(t->group);
    xParGroupSetPriority(t->group, tasset->priority);
    xParGroupRegister(t->group);
    xParGroupSetAging(t->group, (((tasset->parFlags >> 1) & 0x1) ^ 0x1) & 0xFF);
    xParGroupSetVisibility(t->group, t->visible);
    xParGroupSetBack2Life(t->group, (((tasset->parFlags >> 2) & 0x1) ^ 0x1) & 0xFF);
    t->parent = NULL;

    for (S32 i = 0; i < t->cmdCount; i++)
    {
        if (t->cmd[i].tasset->type == XPARCMD_TYPE_TEX)
        {
            t->group->m_cmdTex = (xParCmdTex*)t->cmd[i].tasset;
            xParCmdTexInit(t->group->m_cmdTex);
            break;
        }
    }

    t->group->draw = sParSysInfo[t->tasset->renderFunc].func;
}

void zParSysSetup(zParSys* t)
{
    if (t != NULL && t->tasset != NULL && t->tasset->parentParSysID != 0)
    {
        t->parent = (zParSys*)zSceneFindObject(t->tasset->parentParSysID);
    }
}

static inline void zParSysReset(zParSys* t)
{
    xBaseReset(t, t->tasset);

    if (t->group != NULL)
    {
        xParGroupKillAllParticles(t->group);

        // parFlags: bit 0 visible, bit 1 stops aging, bit 2 stops respawning.
        t->visible = t->tasset->parFlags & 0x1;

        xParGroupSetAging(t->group, (((t->tasset->parFlags >> 1) & 0x1) ^ 0x1) & 0xFF);
        xParGroupSetVisibility(t->group, t->visible);
        xParGroupSetBack2Life(t->group, (((t->tasset->parFlags >> 2) & 0x1) ^ 0x1) & 0xFF);
    }
}

void zParSysExit(zParSys* t)
{
    if (t->group != NULL)
    {
        xParGroupKillAllParticles(t->group);
        xParGroupUnregister(t->group);
    }
}

S32 zParSysEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    zParSys* t = (zParSys*)to;

    switch (toEvent)
    {
    case eEventReset:
        zParSysReset(t);
        break;
    case eEventVisible:
        t->visible = 1;
        if (t->group != NULL)
        {
            xParGroupSetVisibility(t->group, t->visible);
        }
        break;
    case eEventInvisible:
        t->visible = 0;
        if (t->group != NULL)
        {
            xParGroupSetVisibility(t->group, t->visible);
        }
        break;
    case eEventOn:
        if (t->group != NULL)
        {
            xParGroupSetActive(t->group, 1);
        }
        break;
    case eEventOff:
        if (t->group != NULL)
        {
            xParGroupSetActive(t->group, 0);
        }
        break;
    }

    return 1;
}

static void zParGroupUpdateR(zParSys* s, _zParGroup* g, F32 dt);
static inline void zParGroupUpdate(zParSys* s, _zParGroup* g, F32 dt);

// A system first runs its parent chain's commands on each of its groups, root first, then its own.
void zParSysUpdate(xBase* to, _xScene*, F32 dt)
{
    zParSys* s = (zParSys*)to;
    zParSys* parent = s->parent;

    _zParGroup* g = s->group;
    while (g != NULL)
    {
        if (g->m_active)
        {
            if (parent)
            {
                zParGroupUpdateR(parent, g, dt);
            }

            if (g->m_alive)
            {
                zParGroupUpdate(s, g, dt);
            }
        }

        xParGroupAnimate(g, dt);
        g = (_zParGroup*)g->m_next;
    }
}

static void zParGroupUpdateR(zParSys* s, _zParGroup* g, F32 dt)
{
    if (s->parent != NULL)
    {
        zParGroupUpdateR(s->parent, g, dt);
    }

    if (!s->group->m_active)
    {
        return;
    }

    for (S32 i = 0; i < s->cmdCount; i++)
    {
        zParCmd* cmd = &s->cmd[i];
        if (cmd && cmd->tasset)
        {
            void (*func)(zParCmd*, xParGroup*, F32) = zParCmdGetUpdateFunc(cmd->tasset->type);
            if (func)
            {
                func(cmd, g, dt);
            }
        }
    }
}

static inline void zParGroupUpdate(zParSys* s, _zParGroup* g, F32 dt)
{
    for (S32 i = 0; i < s->cmdCount; i++)
    {
        zParCmd* cmd = &s->cmd[i];
        if (cmd && cmd->tasset)
        {
            void (*func)(zParCmd*, xParGroup*, F32) = zParCmdGetUpdateFunc(cmd->tasset->type);
            if (func)
            {
                func(cmd, g, dt);
            }
        }
    }
}

void zParSysRender(xBase* b)
{
    _zParGroup* g;
    zParSys* s = (zParSys*)b;

    zRenderState(SDRS_Particles);

    g = s->group;
    while (g != NULL)
    {
        if (g->m_active && g->m_visible && g->m_alive && !g->m_culled && g->draw != NULL)
        {
            zRenderStateSetSrcBlendMode(s->tasset->renderSrcBlendMode);
            zRenderStateSetDstBlendMode(s->tasset->renderDstBlendMode);
            zRenderStateSetTexture(s->tasset->textureID);
            g->draw(s, g);
        }

        g = (_zParGroup*)g->m_next;
    }
}
