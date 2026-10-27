#include "zEntButton.h"
#include "zGlobals.h"
#include "xEvent.h"

void zEntButton_Update(_zEntButton* ent, _xScene* sc, F32 dt);
void zEntButton_Move(_zEntButton* ent, _xScene* s, F32 dt, _xEntFrame* frame);
S32 zEntButtonEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* b3);

void zEntButton_Init(void* ent, void* asset)
{
    zEntButton_Init((_zEntButton*)ent, (p2EntAsset*)asset);
}

void zEntButton_Init(_zEntButton* ent, p2EntAsset* asset)
{
    zEntInit(ent, asset, 'BUTN');

    zEntButtonAsset* passet = (zEntButtonAsset*)(asset + 1);
    p2EntMotionAsset* emasset = (p2EntMotionAsset*)(passet + 1);

    ent->basset = passet;

    ent->pflags |= 0x1;
    ent->penby |= 0x10;
    ent->chkby |= 0x10;

    ent->update = (void (*)(_xEnt*, _xScene*, F32))zEntButton_Update;
    ent->move = (void (*)(_xEnt*, _xScene*, F32, _xEntFrame*))zEntButton_Move;
    ent->eventFunc = zEntButtonEventCB;

    if (ent->linkCount)
    {
        ent->link = (p2LinkAsset*)(emasset + 1);
    }
    else
    {
        ent->link = NULL;
    }

    ent->eventFunc = zEntButtonEventCB;

    ent->state = 0;
    ent->oldState = ent->state;
    ent->counter = 1000;

    xEntReset(ent);
    xEntMotionInit(&ent->motion, ent, emasset);
    xEntMotionRun(&ent->motion);
    ent->oldMotState = ent->motion.mech.state;
}

void zEntButton_Move(_zEntButton* ent, _xScene* s, F32 dt, _xEntFrame* frame)
{
    xEntMotionMove(&ent->motion, s, dt, frame);
}

void zEntButton_Setup(_zEntButton* ent, _xScene* sc)
{
    xEntMotionSetup(&ent->motion, sc);
}

void zEntButton_Save(_zEntButton* ent, xSerial* s)
{
    zEntSave(ent, s);
}

void zEntButton_Load(_zEntButton* ent, xSerial* s)
{
    zEntLoad(ent, s);
}

void zEntButton_Reset(_zEntButton* ent, _xScene* sc)
{
    xEntReset(ent);
    xEntMotionReset(&ent->motion, sc);

    ent->pflags |= 0x1;
    ent->penby |= 0x10;
    if (ent->asset->flags & 0x1)
    {
        ent->flags |= 0x1;
    }
    ent->chkby |= 0x10;

    ent->state = 0;
    xEntMotionStop(&ent->motion);
    ent->oldState = 5;
    ent->oldMotState = 6;
}

void zEntButton_Update(_zEntButton* ent, _xScene* sc, F32 dt)
{
    xEntUpdate(ent, sc, dt);

    if (ent->motion.mech.state != ent->oldMotState)
    {
        switch (ent->motion.mech.state)
        {
        case 0:
            if (ent->state == 4)
            {
                ent->state = 0;
            }
            break;
        case 2:
            if (!xEntMotionIsStopped(&ent->motion))
            {
                ent->state = 3;
            }
            break;
        case 3:
            ent->state = 4;
            break;
        case 4:
        case 5:
        case 6:
            break;
        }
    }
    ent->oldMotState = ent->motion.mech.state;

    if (ent->state != ent->oldState)
    {
        switch (ent->state)
        {
        case 1:
            xEntMotionRun(&ent->motion);
            break;
        case 3:
            xEntMotionStop(&ent->motion);
            break;
        case 4:
            xEntMotionRun(&ent->motion);
            break;
        case 0:
            if (ent->motion.mech.state == 0)
            {
                xEntMotionStop(&ent->motion);
            }
            break;
        }
        ent->oldState = ent->state;
    }

    if (ent->basset->isReset)
    {
        ent->counter++;
    }

    if (ent->counter == 80)
    {
        zEntEvent(ent, ent, eEventUnpress);
    }
}

static void zEntButton_SetReady(_zEntButton* ent)
{
    if (ent->state != 0)
    {
        ent->state = 4;
    }
}

U32 zEntButton_Headbutt(_zEntButton* ent)
{
    if (ent->basset->actMethod == 2)
    {
        return 4;
    }
    return 0;
}

U32 zEntButton_Stand(_zEntButton* ent)
{
    if (ent->basset->actMethod == 0)
    {
        return 1;
    }
    return 0;
}

U32 zEntButton_ButtBounce(_zEntButton* ent)
{
    if (ent->basset->actMethod == 1)
    {
        return ent->basset->isSpringBB ? 3 : 2;
    }
    return 0;
}

U32 zEntButton_CheckState(_zEntButton* ent)
{
    return ent->state;
}

S32 zEntButtonEventCB(xBase*, xBase* to, U32 toEvent, F32* toParam, xBase*)
{
    _zEntButton* s = (_zEntButton*)to;

    switch (toEvent)
    {
    case eEventVisible:
    case eEventCollision_Visible_On:
        s->chkby |= 0x10;
        xEntShow(s);
        break;
    case eEventInvisible:
    case eEventCollision_Visible_Off:
        s->chkby &= (U8)~0x10;
        xEntHide(s);
        break;
    case eEventReset:
        zEntButton_Reset(s, (_xScene*)globals.sceneCur);
        break;
    case eEventPress:
        if (s->state == 0)
        {
            s->state = 1;
            s->counter = 0;
        }
        break;
    case eEventUnpress:
        if (s->state == 0)
        {
            break;
        }
        zEntButton_SetReady(s);
        break;
    case eEventToggle:
        if (s->state)
        {
            zEntEvent(s, s, eEventUnpress);
        }
        else
        {
            zEntEvent(s, s, eEventPress);
        }
        break;
    case eEventAnimPlay:
    case eEventAnimPlayLoop:
    case eEventAnimStop:
    case eEventAnimPause:
    case eEventAnimResume:
    case eEventAnimTogglePause:
    case eEventAnimPlayRandom:
    case eEventAnimPlayMaybe:
        zEntAnimEvent(s, toEvent, toParam);
        break;
    }

    return 1;
}
