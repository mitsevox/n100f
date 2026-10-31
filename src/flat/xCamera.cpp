#include "xCamera.h"
#include "xMath.h"
#include "xMath2.h"
#include "xMath3.h"
#include "xMemMgr.h"
#include "xEnt.h"
#include "xEnv.h"
#include "xScene.h"
#include "xstransvc.h"
#include "iCamera.h"
#include "zGlobals.h"
#include "zScene.h"
#include "zGame.h"

#include <math.h>
#include <string.h>

struct p2MarkerAsset
{
    xVec3 pos;
};

F32 xFilterCurvedInterp1(F32 in_t);
F32 xFilterCurvedInterp2(F32 in_t);
F32 xFilterCurvedInterp3(F32 in_t);
F32 xFilterCurvedInterp4(F32 in_t);
F32 iurand();
void xScrFxUpdate(RwCamera* cam, F32 seconds);
void iCameraUpdateFog(RwCamera* cam, iTime t);

extern "C" void fn_80038D48(_xScene* sc, xRay3* r, void (*cb)(_xEnt*));
extern "C" U32 fn_80007BB4(const xRay3* r, const _xEnv* env, _xCollis* coll);
extern "C" void fn_8001F8C0(_tagxCamFrame* cam, _tagxCam* c, F32 seconds);

extern S32 gDeadScooby;
extern xVec3 gDeadScoobyPos;
extern S32 gPlayerHitWater;
extern xVec3 gPlayerWaterPos;

// sTargetPos trails the target position (see xCameraAdjustTargetPos); the camera states aim at it.
static xVec3 sTargetPos = { 0.0f, 0.0f, 0.0f };
static xVec3 sLastTargetPos = { 0.0f, 0.0f, 0.0f };
static xVec3 sLastCamPos = { 0.0f, 0.0f, 0.0f };
xVec3 sDummyTargetPos = { 0.0f, 0.0f, 0.0f };
xMat4x3 sDummyTargetFrame;
static U32 sCamDebugFixInterp = 1;
static S32 sOnGround;
static F32 sGroundHeight;
static F32 sCurXOffset;
static F32 sCurYOffset;
static S32 sControllerStill;
F32 gCameraLastFov;
S32 sDXLeft;
S32 sDXRight;
S32 sDYUp;
S32 sDYDown;
static F32 sShoulderTimer;

static void xCameraSeeThruCallback(_xEnt* ent);
static inline void GetPointOnLine(xVec3* C, U32 marker1id, U32 marker2id, xVec3* result);

static inline F32 xCameraApproach(F32 cur, F32 dst, F32 t)
{
    F32 d;

    if (cur == dst)
    {
        return cur;
    }
    d = (dst - cur) * t;
    return d + cur;
}

static inline _tagxCam* xCameraCur(_tagxCamFrame* cam)
{
    if (cam->t_cam)
    {
        return cam->t_cam;
    }
    return &cam->hi_cam;
}

// Only the first call creates the RenderWare camera.
void xCameraCreate(_tagxCamFrame* cam, U32 width, U32 height)
{
    static S32 first_time = 1;

    if (first_time)
    {
        first_time = 0;
        cam->lo_cam = iCameraCreate(width, height);
        xMat4x3Copy(&cam->hi_cam.mat, &g_I3);
        cam->t_cam = NULL;
    }
}

void xCameraDestroy(_tagxCamFrame*)
{
}

void xCameraBegin(_tagxCamFrame* cam, S32 clear)
{
    iCameraBegin(cam->lo_cam, clear);
    iCameraFrustumPlanes(cam->lo_cam, cam->frustplane);
    iCameraUpdateFog(cam->lo_cam, 0);
}

// xCameraFollowState with the sideways offset taken from the stick (sCurXOffset).
_tagxCam* xCameraFollowWithOffsetState(_tagxCamFrame* cam, F32 seconds)
{
    xVec3 new_pos;
    xVec3 preline_test_pos;
    F32 cpb;
    F32 cpa;
    S32 trans;
    xVec3 tmp;
    xMat3x3 mrot;
    xVec3 dirvec;
    xVec3 to_vec;
    _tagxCam* c;

    if (cam->t_cam)
    {
        c = cam->t_cam;
    }
    else
    {
        c = &cam->hi_cam;
    }

    tmp.x = 0.0f;
    tmp.y = c->u.f.height;
    tmp.z = c->u.f.distance;
    xMat3x3Euler(&mrot, DEG2RAD(c->u.f.rotation), 0.0f, 0.0f);
    xMat3x3LMulVec(&tmp, &mrot, &tmp);
    xVec3Add(&new_pos, &sTargetPos, &tmp);
    xVec3Sub(&to_vec, &new_pos, &sTargetPos);
    xMat3x3LookVec((xMat3x3*)&c->mat, &to_vec);

    dirvec = c->mat.right;
    xVec3Normalize(&dirvec, &dirvec);
    xVec3SMulBy(&dirvec, sCurXOffset);
    xVec3AddTo(&new_pos, &dirvec);
    dirvec = c->mat.up;
    xVec3Normalize(&dirvec, &dirvec);
    xVec3SMulBy(&dirvec, cam->hi_cam.view_offset.y);
    xVec3AddTo(&new_pos, &dirvec);
    dirvec = to_vec;
    xVec3Normalize(&dirvec, &dirvec);
    xVec3SMulBy(&dirvec, cam->hi_cam.view_offset.z);
    xVec3AddTo(&new_pos, &dirvec);

    trans = 0;
    if (cam->t_cam || seconds < 0.0f)
    {
        trans = 1;
    }
    cpa = trans ? 1.0f : c->u.f.rubber_band;
    cpb = 1.0f - cpa;

    preline_test_pos = c->mat.pos;
    if (!(c->flags & 0x1))
    {
        preline_test_pos.x = new_pos.x * cpa + c->mat.pos.x * cpb;
    }
    if (!(c->flags & 0x2))
    {
        preline_test_pos.y = new_pos.y * cpa + c->mat.pos.y * cpb;
    }
    if (!(c->flags & 0x4))
    {
        preline_test_pos.z = new_pos.z * cpa + c->mat.pos.z * cpb;
    }

    if (c->asset->markerid[0] && c->asset->markerid[1])
    {
        GetPointOnLine(&preline_test_pos, c->asset->markerid[0], c->asset->markerid[1],
                       &c->mat.pos);
    }
    else
    {
        c->mat.pos = preline_test_pos;
    }

    return c;
}

// The X counterpart of xCameraSetTargetDY, moving towards view_offset.x.
void xCameraSetTargetDX(_tagxCamFrame* cam, F32 dx, F32 seconds)
{
    _tagxCam* c = &cam->hi_cam;
    F32 desired;
    F32 start;
    F32 end;

    if (cam->t_cam)
    {
        c = cam->t_cam;
    }

    if (c->cam_type == eCamType_FollowWithOffset)
    {
        start = c->u.f.start_speed * seconds;
        end = c->u.f.end_speed * seconds;
    }
    else if (c->cam_type == eCamType_Shoulder)
    {
        start = 0.04f;
        end = 0.07f;
    }
    else
    {
        return;
    }

    if (dx < -32.0f || dx > 32.0f)
    {
        if (dx < 0.0f)
        {
            sDXRight = 0;
            sDXLeft++;
            if (sDXLeft >= c->asset->offset_start_frames)
            {
                desired = -c->view_offset.x;
            }
            else
            {
                return;
            }
        }
        else
        {
            sDXLeft = 0;
            sDXRight++;
            if (sDXRight >= c->asset->offset_start_frames)
            {
                desired = c->view_offset.x;
            }
            else
            {
                return;
            }
        }

        if (sDXRight > c->asset->offset_end_frames)
        {
            sDXRight = c->asset->offset_end_frames;
        }
        if (sDXLeft > c->asset->offset_end_frames)
        {
            sDXLeft = c->asset->offset_end_frames;
        }

        if (desired > sCurXOffset)
        {
            if (sDXRight >= c->asset->offset_end_frames || sDXLeft >= c->asset->offset_end_frames)
            {
                sCurXOffset += end;
            }
            else
            {
                sCurXOffset += start;
            }
            if (desired < sCurXOffset)
            {
                sCurXOffset = desired;
            }
        }
        else if (desired < sCurXOffset)
        {
            if (sDXRight >= c->asset->offset_end_frames || sDXLeft >= c->asset->offset_end_frames)
            {
                sCurXOffset -= end;
            }
            else
            {
                sCurXOffset -= start;
            }
            if (desired > sCurXOffset)
            {
                sCurXOffset = desired;
            }
        }
    }
}

// After the stick has been held up or down for offset_start_frames, move the Y offset towards
// view_offset.z, faster once it has been held for offset_end_frames.
void xCameraSetTargetDY(_tagxCamFrame* cam, F32 dy, F32 seconds)
{
    _tagxCam* c = &cam->hi_cam;
    F32 desired;
    F32 start;
    F32 end;

    if (cam->t_cam)
    {
        c = cam->t_cam;
    }

    if (c->cam_type == eCamType_FollowWithOffset)
    {
        start = c->u.f.start_speed * seconds;
        end = c->u.f.end_speed * seconds;
    }
    else if (c->cam_type == eCamType_Shoulder)
    {
        start = 0.04f;
        end = 0.07f;
    }
    else
    {
        return;
    }

    if (dy < -32.0f || dy > 32.0f)
    {
        if (dy < 0.0f)
        {
            sDYDown = 0;
            sDYUp++;
            if (sDYUp >= c->asset->offset_start_frames)
            {
                desired = -c->view_offset.z;
            }
            else
            {
                return;
            }
        }
        else
        {
            sDYUp = 0;
            sDYDown++;
            if (sDYDown >= c->asset->offset_start_frames)
            {
                desired = c->view_offset.z;
            }
            else
            {
                return;
            }
        }

        if (sDYDown > c->asset->offset_end_frames)
        {
            sDYDown = c->asset->offset_end_frames;
        }
        if (sDYUp > c->asset->offset_end_frames)
        {
            sDYUp = c->asset->offset_end_frames;
        }

        if (desired > sCurYOffset)
        {
            if (sDYDown >= c->asset->offset_end_frames || sDYUp >= c->asset->offset_end_frames)
            {
                sCurYOffset += end;
            }
            else
            {
                sCurYOffset += start;
            }
            if (desired < sCurYOffset)
            {
                sCurYOffset = desired;
            }
        }
        else if (desired < sCurYOffset)
        {
            if (sDYDown >= c->asset->offset_end_frames || sDYUp >= c->asset->offset_end_frames)
            {
                sCurYOffset -= end;
            }
            else
            {
                sCurYOffset -= start;
            }
            if (desired > sCurYOffset)
            {
                sCurYOffset = desired;
            }
        }
    }
}

// Feed the stick position: x and y move the view offsets, and any stick deflection past 32
// marks the controller as moving.
void xCameraSetController(_tagxCamFrame* cam, F32 x, F32 y, F32 seconds)
{
    xCameraSetTargetDX(cam, x, seconds);
    xCameraSetTargetDY(cam, y, seconds);
    if (x > 32.0f || x < -32.0f || y > 32.0f || y < -32.0f)
    {
        sControllerStill = 0;
    }
    else
    {
        sControllerStill++;
    }
}

// The point nearest C on the segment between two markers.
static inline void GetPointOnLine(xVec3* C, U32 marker1id, U32 marker2id, xVec3* result)
{
    p2MarkerAsset* m1 = (p2MarkerAsset*)xSTFindAsset(marker1id, NULL);
    p2MarkerAsset* m2 = (p2MarkerAsset*)xSTFindAsset(marker2id, NULL);
    xVec3 AB;
    xVec3 normAB;
    F32 len;
    F32 r;

    xVec3Sub(&AB, &m2->pos, &m1->pos);
    r = (C->x - m1->pos.x) * AB.x + (C->y - m1->pos.y) * AB.y + (C->z - m1->pos.z) * AB.z;
    len = xVec3Normalize(&normAB, &AB);
    r /= len * len;
    if (isnan(r))
    {
        r = 0.0f;
    }

    if (r <= 0.0f)
    {
        *result = m1->pos;
    }
    else if (r >= 1.0f)
    {
        *result = m2->pos;
    }
    else
    {
        r *= len;
        result->x = r * normAB.x + m1->pos.x;
        result->y = r * normAB.y + m1->pos.y;
        result->z = r * normAB.z + m1->pos.z;
    }
}

// Orbit at rotation degrees around the target, distance back and height up, then shift by the
// view offset and ease into place by the rubber band; a marker pair confines it to a line.
_tagxCam* xCameraFollowState(_tagxCamFrame* cam, F32 seconds)
{
    xVec3 new_pos;
    xVec3 preline_test_pos;
    F32 cpb;
    F32 cpa;
    S32 trans;
    xVec3 tmp;
    xMat3x3 mrot;
    xVec3 dirvec;
    xVec3 to_vec;
    _tagxCam* c;

    if (cam->t_cam)
    {
        c = cam->t_cam;
    }
    else
    {
        c = &cam->hi_cam;
    }

    tmp.x = 0.0f;
    tmp.y = c->u.f.height;
    tmp.z = c->u.f.distance;
    xMat3x3Euler(&mrot, DEG2RAD(c->u.f.rotation), 0.0f, 0.0f);
    xMat3x3LMulVec(&tmp, &mrot, &tmp);
    xVec3Add(&new_pos, &sTargetPos, &tmp);
    xVec3Sub(&to_vec, &new_pos, &sTargetPos);
    xMat3x3LookVec((xMat3x3*)&c->mat, &to_vec);

    dirvec = c->mat.right;
    xVec3Normalize(&dirvec, &dirvec);
    xVec3SMulBy(&dirvec, cam->hi_cam.view_offset.x);
    xVec3AddTo(&new_pos, &dirvec);
    dirvec = c->mat.up;
    xVec3Normalize(&dirvec, &dirvec);
    xVec3SMulBy(&dirvec, cam->hi_cam.view_offset.y);
    xVec3AddTo(&new_pos, &dirvec);
    dirvec = to_vec;
    xVec3Normalize(&dirvec, &dirvec);
    xVec3SMulBy(&dirvec, cam->hi_cam.view_offset.z);
    xVec3AddTo(&new_pos, &dirvec);

    trans = 0;
    if (cam->t_cam || seconds < 0.0f)
    {
        trans = 1;
    }
    cpa = trans ? 1.0f : c->u.f.rubber_band;
    cpb = 1.0f - cpa;

    preline_test_pos = c->mat.pos;
    if (!(c->flags & 0x1))
    {
        preline_test_pos.x = new_pos.x * cpa + c->mat.pos.x * cpb;
    }
    if (!(c->flags & 0x2))
    {
        preline_test_pos.y = new_pos.y * cpa + c->mat.pos.y * cpb;
    }
    if (!(c->flags & 0x4))
    {
        preline_test_pos.z = new_pos.z * cpa + c->mat.pos.z * cpb;
    }

    if (c->asset->markerid[0] && c->asset->markerid[1])
    {
        GetPointOnLine(&preline_test_pos, c->asset->markerid[0], c->asset->markerid[1],
                       &c->mat.pos);
    }
    else
    {
        c->mat.pos = preline_test_pos;
    }

    return c;
}

// The shoulder position for the facing (x, z): distance behind the target and height above it.
static inline S32 ShoulderGetDesiredPos(_tagxCam* c, xVec3* pos, F32 x, F32 z)
{
    pos->x = sTargetPos.x - x * c->u.sh.distance;
    pos->y = sTargetPos.y + c->u.sh.height;
    pos->z = sTargetPos.z - z * c->u.sh.distance;
    return 1;
}

static inline void ShoulderStateSwitch(_tagxCamFrame*, _tagxCam* c, S32 new_state)
{
    if (new_state == 1)
    {
        sShoulderTimer = 0.0f;
    }
    c->u.sh.state = new_state;
}

// Shoulder state 0: keep behind the target along the camera's facing; once the stick has been
// released, start the wait.
extern "C" void fn_8001F8C0(_tagxCamFrame* cam, _tagxCam* c, F32 seconds)
{
    xVec3 desired_pos;
    xVec3 target_with_y;
    xVec3 vec;
    xVec2 dir;

    dir.x = cam->hi_cam.mat.at.x;
    dir.y = cam->hi_cam.mat.at.z;
    xVec2Normalize(&dir, &dir);
    ShoulderGetDesiredPos(c, &desired_pos, dir.x, dir.y);
    c->mat.pos.x = xCameraApproach(c->mat.pos.x, desired_pos.x, 1.0f);
    c->mat.pos.y = xCameraApproach(c->mat.pos.y, desired_pos.y, 1.0f);
    c->mat.pos.z = xCameraApproach(c->mat.pos.z, desired_pos.z, 1.0f);
    target_with_y = sTargetPos;
    target_with_y.y += cam->hi_cam.view_offset.y;
    xVec3Sub(&vec, &c->mat.pos, &target_with_y);
    xMat3x3LookVec((xMat3x3*)&c->mat, &vec);

    if (sControllerStill && seconds > 0.0f)
    {
        ShoulderStateSwitch(cam, c, 1);
    }
}

// Hold position for realign_delay seconds after the stick is released, then realign unless the
// target still faces within 60 degrees of where it faced when the wait began.
static void ShoulderStatePause(_tagxCamFrame* cam, _tagxCam* c, F32 seconds)
{
    xVec3 desired_pos;
    xVec2 dir;

    dir.x = cam->hi_cam.mat.at.x;
    dir.y = cam->hi_cam.mat.at.z;
    xVec2Normalize(&dir, &dir);
    ShoulderGetDesiredPos(c, &desired_pos, dir.x, dir.y);
    c->mat.pos.x = xCameraApproach(c->mat.pos.x, desired_pos.x, 1.0f);
    c->mat.pos.y = xCameraApproach(c->mat.pos.y, desired_pos.y, 1.0f);
    c->mat.pos.z = xCameraApproach(c->mat.pos.z, desired_pos.z, 1.0f);

    sShoulderTimer += seconds;
    if (!sControllerStill)
    {
        ShoulderStateSwitch(cam, c, 0);
    }

    if (sShoulderTimer >= c->u.sh.realign_delay)
    {
        xVec2 pre;
        xVec2 cur;

        sShoulderTimer = c->u.sh.realign_delay;
        if (cam->pre_align_dir.x == 0.0f && cam->pre_align_dir.y == 0.0f &&
            cam->pre_align_dir.z == 0.0f)
        {
            cam->pre_align_dir = cam->target_frame->at;
            ShoulderStateSwitch(cam, c, 2);
            return;
        }

        pre.x = cam->pre_align_dir.x;
        pre.y = cam->pre_align_dir.z;
        xVec2Normalize(&pre, &pre);
        cur.x = cam->target_frame->at.x;
        cur.y = cam->target_frame->at.z;
        xVec2Normalize(&cur, &cur);
        if (pre.x * cur.x + pre.y * cur.y <= 0.5f)
        {
            cam->pre_align_dir = cam->target_frame->at;
            ShoulderStateSwitch(cam, c, 2);
        }
    }
}

// Swing the camera behind the target by realign_speed degrees a step, stopping once it crosses
// the target's facing.
static void ShoulderStateRealign(_tagxCamFrame* cam, _tagxCam* c, F32)
{
    xMat3x3 rot_matrix;
    xVec3 vec;
    xVec3 desired_pos;
    xVec3 target_with_y;
    F32 rad_rot = DEG2RAD(c->u.sh.realign_speed);
    F32 dp1 = cam->hi_cam.mat.at.x * cam->target_frame->right.x +
              cam->hi_cam.mat.at.z * cam->target_frame->right.z;
    F32 dp2;

    if (dp1 < 0.0f)
    {
        xMat3x3Euler(&rot_matrix, -rad_rot, 0.0f, 0.0f);
    }
    else
    {
        xMat3x3Euler(&rot_matrix, rad_rot, 0.0f, 0.0f);
    }

    vec.x = cam->hi_cam.mat.at.x;
    vec.y = 0.0f;
    vec.z = cam->hi_cam.mat.at.z;
    xVec3Normalize(&vec, &vec);
    xMat3x3LMulVec(&vec, &rot_matrix, &vec);
    ShoulderGetDesiredPos(c, &desired_pos, vec.x, vec.z);
    c->mat.pos.x = xCameraApproach(c->mat.pos.x, desired_pos.x, 1.0f);
    c->mat.pos.y = xCameraApproach(c->mat.pos.y, desired_pos.y, 1.0f);
    c->mat.pos.z = xCameraApproach(c->mat.pos.z, desired_pos.z, 1.0f);
    target_with_y = sTargetPos;
    target_with_y.y += cam->hi_cam.view_offset.y;
    xVec3Sub(&vec, &c->mat.pos, &target_with_y);
    xMat3x3LookVec((xMat3x3*)&c->mat, &vec);

    dp2 = cam->hi_cam.mat.at.x * cam->target_frame->right.x +
          cam->hi_cam.mat.at.z * cam->target_frame->right.z;
    if ((dp1 <= 0.0f && dp2 >= 0.0f) || (dp1 >= 0.0f && dp2 <= 0.0f))
    {
        c->mat.pos = sTargetPos;
        c->mat.pos.y += c->u.sh.height;
        c->mat.pos.x -= cam->target_frame->at.x * c->u.sh.distance;
        c->mat.pos.z -= cam->target_frame->at.z * c->u.sh.distance;
        target_with_y = sTargetPos;
        target_with_y.y += cam->hi_cam.view_offset.y;
        xVec3Sub(&vec, &c->mat.pos, &target_with_y);
        xMat3x3LookVec((xMat3x3*)&c->mat, &vec);
        ShoulderStateSwitch(cam, c, 3);
    }

    if (!sControllerStill)
    {
        ShoulderStateSwitch(cam, c, 0);
    }
}

// Fade an entity the camera looks through, unless it is the player or already opaque.
static void xCameraSeeThruCallback(_xEnt* ent)
{
    U32 see_thru;

    if (ent->asset == NULL)
    {
        see_thru = 0;
    }
    else if (ent->asset->seeThru >= 1.0f)
    {
        see_thru = 0;
    }
    else
    {
        see_thru = 1;
    }

    if (see_thru && ent->baseType != 3)
    {
        ent->flags |= 0x4;
        ent->fade_tmr = 1.25f;
        if (zSceneIDFromName("o004") == (gTransitionSceneID ^ 0x20000000) &&
            (ent->id == 0xF933F89E || ent->id == 0xFFF2B7AA))
        {
            ent->fade_tmr = 0.25f;
        }
    }
}

// The over-the-shoulder camera: state 0 follows, 1 waits after the stick is released, 2 swings
// back behind the target and 3 holds there until the stick moves again.
_tagxCam* xCameraShoulderState(_tagxCamFrame* cam, F32 seconds)
{
    static xMat4x3 old_matrix = { 0 };
    _tagxCam* c;

    if (cam->t_cam)
    {
        c = cam->t_cam;
    }
    else
    {
        c = &cam->hi_cam;
    }

    if (old_matrix.at.x + old_matrix.at.y + old_matrix.at.z != 0.0f)
    {
        memcpy(&c->mat, &old_matrix, sizeof(xMat4x3));
    }

    switch (c->u.sh.state)
    {
    case 0:
        fn_8001F8C0(cam, c, seconds);
        break;
    case 1:
        ShoulderStatePause(cam, c, seconds);
        break;
    case 2:
        ShoulderStateRealign(cam, c, seconds);
        break;
    case 3:
    {
        xVec3 desired_pos;
        xVec3 target_with_y;
        xVec3 vec;

        vec.x = cam->hi_cam.mat.at.x;
        vec.y = 0.0f;
        vec.z = cam->hi_cam.mat.at.z;
        xVec3Normalize(&vec, &vec);
        desired_pos.x = sTargetPos.x - vec.x * c->u.sh.distance;
        desired_pos.y = sTargetPos.y + c->u.sh.height;
        desired_pos.z = sTargetPos.z - vec.z * c->u.sh.distance;
        c->mat.pos.x = xCameraApproach(c->mat.pos.x, desired_pos.x, 0.2f);
        c->mat.pos.y = xCameraApproach(c->mat.pos.y, desired_pos.y, 0.2f);
        c->mat.pos.z = xCameraApproach(c->mat.pos.z, desired_pos.z, 0.2f);
        target_with_y = sTargetPos;
        target_with_y.y += cam->hi_cam.view_offset.y;
        xVec3Sub(&vec, &c->mat.pos, &target_with_y);
        xMat3x3LookVec((xMat3x3*)&c->mat, &vec);
        if (!sControllerStill)
        {
            c->u.sh.state = 0;
        }
        break;
    }
    }

    memcpy(&old_matrix, &c->mat, sizeof(xMat4x3));

    if (c->flags & 0x80)
    {
        _xCollis collis_env;
        xMat4x3 target_pos_matrix;
        xVec3 target_with_y;
        xMat4x3 desired_pos_matrix;
        xRay3 ray;

        memcpy(&target_pos_matrix, &c->mat, sizeof(xMat4x3));
        target_with_y = sTargetPos;
        target_with_y.y += cam->hi_cam.view_offset.y;
        memcpy(&desired_pos_matrix, &c->mat, sizeof(xMat4x3));
        xMat4x3MoveLocalRight(&desired_pos_matrix, sCurXOffset);
        xMat4x3MoveLocalAt(&desired_pos_matrix, sCurYOffset);
        xMat4x3MoveLocalUp(&desired_pos_matrix, 0.25f * sCurYOffset);

        ray.origin = target_with_y;
        xVec3Sub(&ray.dir, &desired_pos_matrix.pos, &target_with_y);
        ray.max_t = xVec3Normalize(&ray.dir, &ray.dir);
        ray.min_t = 0.1f;
        ray.flags = 0xC00;
        memset(&collis_env, 0, sizeof(_xCollis));
        fn_80007BB4(&ray, cam->env, &collis_env);

        if ((collis_env.flags & 0x1) && collis_env.dist < 1e19f)
        {
            collis_env.dist -= 0.1f;
            if (collis_env.dist < 0.0f)
            {
                collis_env.dist = 0.0f;
            }
            memcpy(&c->mat, &target_pos_matrix, sizeof(xMat4x3));
            c->mat.pos.x += ray.dir.x * collis_env.dist;
            c->mat.pos.y += ray.dir.y * collis_env.dist;
            c->mat.pos.z += ray.dir.z * collis_env.dist;
            c->mat.pos.y += 1.5f * (1.0f - collis_env.dist / ray.max_t);
        }
        else
        {
            memcpy(&c->mat, &desired_pos_matrix, sizeof(xMat4x3));
        }
    }
    else
    {
        xMat4x3MoveLocalRight(&c->mat, sCurXOffset);
        xMat4x3MoveLocalAt(&c->mat, sCurYOffset);
        xMat4x3MoveLocalUp(&c->mat, 0.25f * sCurYOffset);
    }

    return c;
}

// Ease sTargetPos halfway towards the target each update. While the target is in the air the
// height is held at the last ground height, so jumps do not bob the camera.
static inline void xCameraAdjustTargetPos(_tagxCamFrame* cam, F32)
{
    _tagxCam* c = xCameraCur(cam);

    if (c->flags & 0x40)
    {
        xVec3 desired_target_pos = *cam->target_pos;
        xVec3 cur_pos;

        if (!sOnGround)
        {
            if (desired_target_pos.y > sGroundHeight)
            {
                desired_target_pos.y = sGroundHeight;
            }
            else
            {
                sGroundHeight = desired_target_pos.y;
            }
        }
        else
        {
            sGroundHeight = desired_target_pos.y;
        }

        cur_pos = sTargetPos;
        sTargetPos.x = xCameraApproach(cur_pos.x, desired_target_pos.x, 0.5f);
        sTargetPos.y = xCameraApproach(cur_pos.y, desired_target_pos.y, 0.5f);
        sTargetPos.z = xCameraApproach(cur_pos.z, desired_target_pos.z, 0.5f);
    }
    else
    {
        sTargetPos = *cam->target_pos;
    }

    if (gDeadScooby)
    {
        sTargetPos = gDeadScoobyPos;
    }
    if (gPlayerHitWater)
    {
        sTargetPos = gPlayerWaterPos;
    }
}

// Run the current camera's state, then fade out the scenery between the camera and the target.
void xCameraRunState(_tagxCamFrame* cam, F32 seconds)
{
    _tagxCam* c;

    if (seconds > 0.0f)
    {
        cam->g_time += seconds;
    }

    xCameraAdjustTargetPos(cam, seconds);

    switch (cam->hi_cam.cam_type)
    {
    case eCamType_Follow:
        c = xCameraFollowState(cam, seconds);
        break;
    case eCamType_FollowWithOffset:
        c = xCameraFollowWithOffsetState(cam, seconds);
        break;
    case eCamType_Static:
        c = xCameraCur(cam);
        break;
    case eCamType_StaticFollow:
    {
        xVec3 to_vec;

        c = xCameraCur(cam);
        xVec3Sub(&to_vec, &c->mat.pos, &sTargetPos);
        xMat3x3LookVec((xMat3x3*)&c->mat, &to_vec);
        c->mat.pos = c->asset->pos;
        break;
    }
    case eCamType_Path:
        c = NULL;
        break;
    case eCamType_Shoulder:
        c = xCameraShoulderState(cam, seconds);
        break;
    }

    {
        xVec3 offset;
        xRay3 ray;
        xMat3x3 rot;
        _xCollis collis;

        memset(&collis, 0, sizeof(_xCollis));
        ray.origin = c->mat.pos;
        xVec3Sub(&ray.dir, &sTargetPos, &ray.origin);
        ray.max_t = xVec3Normalize(&ray.dir, &ray.dir);
        ray.min_t = 0.0f;
        ray.flags = 0xC00;
        ray.dir = cam->hi_cam.mat.at;
        fn_80038D48(g_xSceneCur, &ray, xCameraSeeThruCallback);

        xVec3SMul(&offset, &cam->hi_cam.mat.right, 0.5f);
        xMat3x3RotC(&rot, ray.dir.x, ray.dir.y, ray.dir.z, 6.2831855f * iurand());
        xMat3x3RMulVec(&offset, &rot, &offset);
        xVec3AddTo(&ray.dir, &offset);
        ray.max_t *= xVec3Normalize(&ray.dir, &ray.dir);
        fn_80038D48(g_xSceneCur, &ray, xCameraSeeThruCallback);
    }
}

void xCameraUpdate(_tagxCamFrame* cam, F32 seconds)
{
    if (!zGameIsPaused())
    {
        if (cam->t_cam && cam->t_cam->trans_type != eTransType_None)
        {
            xCameraInterpValid(cam, cam->t_cam->trans_type, seconds);
        }
        xCameraRunState(cam, seconds);
        iCameraUpdatePos(cam->lo_cam, &cam->hi_cam.mat);
        gCameraLastFov = cam->hi_cam.fov;
        iCameraSetFOV(cam->lo_cam, cam->hi_cam.fov);
        sLastCamPos = cam->hi_cam.mat.pos;
    }
}

void xCameraEnd(_tagxCamFrame* cam, F32 seconds, S32 update_scrn_fx)
{
    if (update_scrn_fx)
    {
        xScrFxUpdate(cam->lo_cam, seconds);
    }
    iCameraEnd(cam->lo_cam);
}

void xCameraShowRaster(_tagxCamFrame* cam)
{
    iCameraShowRaster(cam->lo_cam);
}

void xCameraSetTarget(_tagxCamFrame* cam, xVec3* new_target)
{
    sLastTargetPos = sTargetPos;
    cam->target_pos = new_target;
    sTargetPos = *new_target;
}

void xCameraSetTarget(_tagxCamFrame* cam, xMat4x3* frame_target)
{
    cam->target_frame = frame_target;
}

void xCameraAssignEnv(_tagxCamFrame* camera, _xEnv* env)
{
    camera->env = env;
    iCameraAssignEnv(camera->lo_cam, camera->env->geom);
}

void xCameraRemoveEnv(_tagxCamFrame* camera)
{
    iCameraRemoveEnv(camera->lo_cam, camera->env->geom);
}

// Allocate total camera slots and aim at a dummy target until a real one is set.
_tagxCam* xCameraInit(_tagxCamFrame* cam, U32 total)
{
    S32 i;
    _tagxCam* array;

    xCameraSetTarget(cam, &sDummyTargetPos);
    xCameraSetTarget(cam, &sDummyTargetFrame);
    cam->total_cameras = total;
    cam->cameras = (_tagxCam**)xMemAlloc(gActiveHeap, cam->total_cameras * sizeof(_tagxCam*));
    array = (_tagxCam*)xMemAlloc(gActiveHeap, cam->total_cameras * sizeof(_tagxCam));
    for (i = 0; i < (S32)total; i++)
    {
        cam->cameras[i] = &array[i];
    }
    return array;
}

// Fill camera slot idx from its asset and mark which of its fields transitions may blend.
_tagxCam* xCameraSetAsset(_tagxCamFrame* cam, xCamAsset* src, S32 idx)
{
    _tagxCam* dst = cam->cameras[idx];

    dst->asset = src;
    dst->id = src->id;
    dst->link = (p2LinkAsset*)((U32*)src + 1);
    dst->linkCount = src->linkCount;
    dst->baseType = src->baseType;
    dst->cam_type = (_tagCamType)src->cam_type;
    dst->mat.right = src->right;
    dst->mat.up = src->up;
    dst->mat.at = src->at;
    dst->mat.pos = src->pos;
    dst->fov = src->fov;
    xMat4x3Orthonormalize(&dst->mat, &dst->mat);
    dst->trans_time = src->trans_time;
    dst->trans_type = src->trans_type;
    dst->flags = src->flags;
    dst->view_offset.x = -1.0f * src->view_offset.x;
    dst->view_offset.y = src->view_offset.y;
    dst->view_offset.z = -1.0f * src->view_offset.z;
    dst->valid_flags = 0x100C0;

    switch (src->cam_type)
    {
    case eCamType_FollowWithOffset:
        dst->valid_flags |= 0x400000;
        dst->u.f.start_speed = src->cam_follow.start_speed;
        dst->u.f.end_speed = src->cam_follow.end_speed;
    case eCamType_Follow:
        dst->u.f.rotation = src->cam_follow.rotation;
        dst->u.f.distance = src->cam_follow.distance;
        dst->u.f.height = src->cam_follow.height;
        dst->u.f.rubber_band =
            src->cam_follow.rubber_band < 0.5f ? src->cam_follow.rubber_band : 0.5f;
        dst->valid_flags |= 0x30F;
        dst->view_offset.z = 0.0f;
        break;
    case eCamType_Shoulder:
        dst->u.sh.distance = src->cam_shoulder.distance;
        dst->u.sh.height = src->cam_shoulder.height;
        dst->u.sh.realign_speed = src->cam_shoulder.realign_speed;
        dst->u.sh.realign_delay = src->cam_shoulder.realign_delay;
        dst->valid_flags |= 0x1E0000 | 0x300;
        break;
    case eCamType_StaticFollow:
        dst->u.sf.rubber_band = src->cam_staticFollow.rubber_band < 0.5f ?
                                    src->cam_staticFollow.rubber_band :
                                    0.5f;
        dst->valid_flags |= 0x30F;
        dst->view_offset.z = 0.0f;
        break;
    case eCamType_Static:
        dst->u.s.unused = src->cam_static.unused;
        dst->valid_flags |= 0x300;
        dst->view_offset.z = 0.0f;
        break;
    case eCamType_Path:
        dst->u.p.assetID = src->cam_path.assetID;
        dst->valid_flags |= 0x300;
        dst->view_offset.z = 0.0f;
        break;
    }

    return cam->cameras[idx];
}

// Blend each field t_cam marks valid from orig_cam towards t_cam, along the transition curve.
void xCameraInterpValid(_tagxCamFrame* cam, _tagTransType trans_type, F32 seconds)
{
    _tagxCam* dest = cam->t_cam;
    _tagxCam* src = &cam->hi_cam;
    _tagxCam* orig = &cam->orig_cam;
    F32 t;

    if (seconds < 0.0f)
    {
        t = 1.0f;
    }
    else
    {
        t = cam->t_time / dest->trans_time;
    }

    if (t >= 1.0f)
    {
        t = 1.0f;
        cam->t_time = 0.0f;
        cam->t_cam = NULL;
    }
    else
    {
        cam->t_time += seconds;
    }

    switch (trans_type)
    {
    case eTransType_Interp1:
        t = xFilterCurvedInterp1(t);
        break;
    case eTransType_Interp2:
        t = xFilterCurvedInterp2(t);
        break;
    case eTransType_Interp3:
        t = xFilterCurvedInterp3(t);
        break;
    case eTransType_Interp4:
        t = xFilterCurvedInterp4(t);
        break;
    case eTransType_Interp1Rev:
        t = 1.0f - xFilterCurvedInterp1(1.0f - t);
        break;
    case eTransType_Interp2Rev:
        t = 1.0f - xFilterCurvedInterp2(1.0f - t);
        break;
    case eTransType_Interp3Rev:
        t = 1.0f - xFilterCurvedInterp3(1.0f - t);
        break;
    case eTransType_Interp4Rev:
        t = 1.0f - xFilterCurvedInterp4(1.0f - t);
        break;
    }

    if (dest->cam_type == orig->cam_type)
    {
        if (dest->valid_flags & 0x1)
        {
            src->u.f.rotation = xCameraApproach(orig->u.f.rotation, dest->u.f.rotation, t);
        }
        if (dest->valid_flags & 0x2)
        {
            src->u.f.distance = xCameraApproach(orig->u.f.distance, dest->u.f.distance, t);
        }
        if (dest->valid_flags & 0x4)
        {
            src->u.f.height = xCameraApproach(orig->u.f.height, dest->u.f.height, t);
        }
        if (dest->valid_flags & 0x8)
        {
            src->u.f.rubber_band = xCameraApproach(orig->u.f.rubber_band, dest->u.f.rubber_band, t);
        }
    }

    if (dest->valid_flags & 0x20000)
    {
        src->u.sh.distance = xCameraApproach(orig->u.sh.distance, dest->u.sh.distance, t);
    }
    if (dest->valid_flags & 0x40000)
    {
        src->u.sh.height = xCameraApproach(orig->u.sh.height, dest->u.sh.height, t);
    }
    if (dest->valid_flags & 0x80000)
    {
        src->u.sh.realign_speed =
            xCameraApproach(orig->u.sh.realign_speed, dest->u.sh.realign_speed, t);
    }
    if (dest->valid_flags & 0x100000)
    {
        src->u.sh.realign_delay =
            xCameraApproach(orig->u.sh.realign_delay, dest->u.sh.realign_delay, t);
    }

    if (dest->valid_flags & 0x40)
    {
        src->mat.pos.x = xCameraApproach(orig->mat.pos.x, dest->mat.pos.x, t);
        src->mat.pos.y = xCameraApproach(orig->mat.pos.y, dest->mat.pos.y, t);
        src->mat.pos.z = xCameraApproach(orig->mat.pos.z, dest->mat.pos.z, t);
    }

    if (dest->valid_flags & 0x80)
    {
        if (t != 1.0f)
        {
            if (sCamDebugFixInterp)
            {
                xQuatFromMat(&dest->rot, (xMat3x3*)&dest->mat);
                xQuatNormalize(&dest->rot, &dest->rot);
                if (xQuatDot(&dest->rot, &src->rot) < 0.0f)
                {
                    dest->rot.s = -dest->rot.s;
                    xVec3SMulBy(&dest->rot.v, -1.0f);
                }
                xQuatSlerp(&src->rot, &orig->rot, &dest->rot, t);
                xQuatToMat(&src->rot, (xMat3x3*)&src->mat);
            }
            else
            {
                xQuat a;
                xQuat b;
                xQuat o;
                xVec3 pos = src->mat.pos;

                xQuatFromMat(&a, (xMat3x3*)&orig->mat);
                xQuatFromMat(&b, (xMat3x3*)&dest->mat);
                xQuatSlerp(&o, &a, &b, t);
                xQuatToMat(&o, (xMat3x3*)&src->mat);
                src->mat.pos = pos;
            }
        }
        else
        {
            xQuat cur_fm;
            xQuat dst_fm;

            acos(xQuatDot(&src->rot, &dest->rot));
            xQuatFromMat(&cur_fm, (xMat3x3*)&src->mat);
            xQuatFromMat(&dst_fm, (xMat3x3*)&dest->mat);
            acos(xQuatDot(&cur_fm, &dst_fm));
            acos(xQuatDot(&cur_fm, &src->rot));
            acos(xQuatDot(&dst_fm, &dest->rot));
            src->mat = dest->mat;
        }
    }

    if (dest->valid_flags & 0x100)
    {
        src->fov = xCameraApproach(orig->fov, dest->fov, t);
    }

    src->view_offset.x = xCameraApproach(orig->view_offset.x, dest->view_offset.x, t);
    src->view_offset.y = xCameraApproach(orig->view_offset.y, dest->view_offset.y, t);
    src->view_offset.z = xCameraApproach(orig->view_offset.z, dest->view_offset.z, t);
}

// Switch to trans_cam, blending from the current view when it has a transition type.
void xCameraNext(_tagxCamFrame* cam, _tagxCam* trans_cam)
{
    if (cam->hi_cam.asset->id == trans_cam->asset->id)
    {
        xSTAssetName(trans_cam->id);
        return;
    }

    switch (trans_cam->cam_type)
    {
    case eCamType_Follow:
    case eCamType_FollowWithOffset:
    case eCamType_Static:
    case eCamType_Path:
    case eCamType_StaticFollow:
        break;
    case eCamType_Shoulder:
        trans_cam->u.sh.state = 0;
        cam->pre_align_dir.x = 0.0f;
        cam->pre_align_dir.y = 0.0f;
        cam->pre_align_dir.z = 0.0f;
        break;
    }

    gCameraLastFov = 0.0f;
    sGroundHeight = sTargetPos.y;

    switch (trans_cam->trans_type)
    {
    case eTransType_None:
        memcpy(&cam->hi_cam, trans_cam, sizeof(_tagxCam));
        cam->t_cam = NULL;
        cam->t_time = -1.0f;
        cam->g_time = 0.0f;
        xCameraRunState(cam, -1.0f);
        break;
    case eTransType_Interp1:
    case eTransType_Interp2:
    case eTransType_Interp3:
    case eTransType_Interp4:
    case eTransType_Linear:
    case eTransType_Interp1Rev:
    case eTransType_Interp2Rev:
    case eTransType_Interp3Rev:
    case eTransType_Interp4Rev:
        memcpy(&cam->orig_cam, &cam->hi_cam, sizeof(_tagxCam));
        memcpy(&cam->hi_cam, trans_cam, sizeof(_tagxCam));
        if (sCamDebugFixInterp)
        {
            xMat3x3 from_new_src;
            xQuat* dst_rot = &cam->hi_cam.rot;
            xQuat* src_rot = &trans_cam->rot;

            cam->hi_cam.mat = cam->orig_cam.mat;
            xQuatFromMat(dst_rot, (xMat3x3*)&cam->hi_cam.mat);
            xQuatFromMat(src_rot, (xMat3x3*)&trans_cam->mat);
            xQuatNormalize(dst_rot, dst_rot);
            xQuatNormalize(src_rot, src_rot);
            if (xQuatDot(dst_rot, src_rot) < 0.0f)
            {
                dst_rot->s = -dst_rot->s;
                xVec3SMulBy(&dst_rot->v, -1.0f);
                xQuatToMat(dst_rot, &from_new_src);
            }
            xQuatCopy(&cam->orig_cam.rot, dst_rot);
        }
        cam->t_cam = trans_cam;
        cam->t_time = 0.0f;
        break;
    }

    cam->g_time = 0.0f;
}

void xCameraPickInitial(_tagxCamFrame* cam, U32 init_id)
{
    if (init_id)
    {
        _tagxCam* c;
        S32 i;

        for (i = 0; i < (S32)cam->total_cameras; i++)
        {
            c = cam->cameras[i];
            if (c->id == init_id)
            {
                goto found;
            }
        }
        c = NULL;
    found:
        xCameraPickInitial(cam, c);
    }
}

// Make c the shown camera at once, with no transition.
void xCameraPickInitial(_tagxCamFrame* cam, _tagxCam* c)
{
    xCameraSetTarget(cam, (xVec3*)&globals.player.ent.frame->mat.pos);
    xCameraSetTarget(cam, (xMat4x3*)globals.player.ent.model->Mat);
    memcpy(&cam->hi_cam, c, sizeof(_tagxCam));
    cam->t_cam = NULL;
    cam->t_time = -1.0f;
    cam->g_time = 0.0f;

    switch (cam->hi_cam.cam_type)
    {
    case eCamType_Follow:
    case eCamType_FollowWithOffset:
    case eCamType_Static:
    case eCamType_Path:
    case eCamType_StaticFollow:
        break;
    case eCamType_Shoulder:
        cam->hi_cam.u.sh.state = 0;
        cam->pre_align_dir.x = 0.0f;
        cam->pre_align_dir.y = 0.0f;
        cam->pre_align_dir.z = 0.0f;
        break;
    }

    gCameraLastFov = 0.0f;
    sGroundHeight = sTargetPos.y;
    xCameraRunState(cam, -1.0f);
}

void xCameraTargetOnGround(S32 on_ground, F32)
{
    sOnGround = on_ground;
}

xMat4x3* xCameraGetCurMatrix(_tagxCamFrame* cam)
{
    return &cam->hi_cam.mat;
}
