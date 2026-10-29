#include "zPendulum.h"
#include "zGlobals.h"
#include "zScene.h"

void zEntEvent(xBase* to, U32 toEvent);
extern "C" void fn_80026A1C(_xEntMotion* motion, xVec3* dpos);

static void zPendulumTranslate(_xEnt* xent, xVec3* dpos);

static inline void zPendulum_Init(_zPendulum* pend, p2EntAsset* asset);

static inline void zPendulum_Init(_zPendulum* pend, p2EntAsset* asset)
{
    zEntInit(pend, asset, 'PEND');

    p2EntMotionAsset* motionAsset = (p2EntMotionAsset*)(asset + 1);

    if (pend->linkCount)
    {
        pend->link = (p2LinkAsset*)(motionAsset + 1);
    }
    else
    {
        pend->link = NULL;
    }

    pend->update = (void (*)(_xEnt*, _xScene*, F32))zPendulum_Update;
    pend->move = (void (*)(_xEnt*, _xScene*, F32, _xEntFrame*))zPendulum_Move;
    pend->eventFunc = zPendulumEventCB;
    pend->transl = zPendulumTranslate;

    zEntReset(pend);
    xEntMotionInit(&pend->motion, pend, motionAsset);
}

void zPendulum_Init(void* pend, void* asset)
{
    zPendulum_Init((_zPendulum*)pend, (p2EntAsset*)asset);
}

void zPendulum_Save(_zPendulum* pend, xSerial* s)
{
    zEntSave(pend, s);
}

void zPendulum_Load(_zPendulum* pend, xSerial* s)
{
    zEntLoad(pend, s);
}

void zPendulum_Setup(_zPendulum* pend, _xScene* sc)
{
    zEntSetup(pend);
    xEntMotionSetup(&pend->motion, sc);
}

void zPendulum_Reset(_zPendulum* pend, _xScene* sc)
{
    zEntReset(pend);
    xEntMotionReset(&pend->motion, sc);

    p2EntMotionPenData* aspen = &pend->motion.asset->pen;

    pend->lt = 0.0f;
    pend->q1t = 1.0f - aspen->phase / 6.2831855f;
    pend->q3t = 0.5f - aspen->phase / 6.2831855f;

    if (pend->q1t <= 0.0f)
    {
        pend->q1t += 1.0f;
    }
    if (pend->q1t > 1.0f)
    {
        pend->q1t -= 1.0f;
    }
    if (pend->q3t <= 0.0f)
    {
        pend->q3t += 1.0f;
    }
    if (pend->q3t > 1.0f)
    {
        pend->q3t -= 1.0f;
    }

    pend->q1t *= aspen->period;
    pend->q3t *= aspen->period;
}

void zPendulum_Update(_zPendulum* pend, _xScene* sc, F32 dt)
{
    xEntUpdate(pend, sc, dt);

    F32 t = pend->motion.t;
    p2EntMotionPenData* aspen = &pend->motion.asset->pen;

    if (t < pend->lt)
    {
        t += aspen->period;
    }

    if (pend->lt <= pend->q1t && t > pend->q1t)
    {
        zEntEvent(pend, 228);
    }
    else if (pend->lt <= pend->q3t && t > pend->q3t)
    {
        zEntEvent(pend, 228);
    }

    pend->lt = pend->motion.t;
}

void zPendulum_Move(_zPendulum* pend, _xScene* s, F32 dt, _xEntFrame* frame)
{
    xEntMotionMove(&pend->motion, s, dt, frame);
}

static void zPendulumTranslate(_xEnt* xent, xVec3* dpos)
{
    _zPendulum* pend = (_zPendulum*)xent;

    xEntDefaultTranslate(xent, dpos);
    fn_80026A1C(&pend->motion, dpos);
}

// q1t and q3t are the times in the swing where the pendulum passes the bottom; it swooshes there.
S32 zPendulumEventCB(xBase*, xBase* to, U32 toEvent, F32* toParam, xBase*)
{
    _zPendulum* pend = (_zPendulum*)to;

    switch (toEvent)
    {
    case 18:
        xEntMotionRun(&pend->motion);
        break;
    case 19:
        xEntMotionStop(&pend->motion);
        break;
    case 10:
        zPendulum_Reset(pend, globals.sceneCur);
        break;
    case 3:
        xEntShow(pend);
        break;
    case 4:
        xEntHide(pend);
        break;
    case 85:
        xEntShow(pend);
    case 83:
        pend->chkby = 0x18;
        break;
    case 86:
        xEntHide(pend);
    case 84:
        pend->chkby = 0;
        break;
    case 195:
    case 196:
    case 197:
    case 198:
    case 199:
    case 200:
    case 201:
    case 202:
        zEntAnimEvent(pend, toEvent, toParam);
        break;
    }

    return 1;
}
