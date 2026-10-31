#ifndef XCAMERA_H
#define XCAMERA_H

#include "types.h"
#include "xBase.h"
#include "xMath3.h"

#include <rwcore.h>

struct _xEnv;
struct _xEnt;

enum _tagTransType
{
    eTransType_None,
    eTransType_Interp1,
    eTransType_Interp2,
    eTransType_Interp3,
    eTransType_Interp4,
    eTransType_Linear,
    eTransType_Interp1Rev,
    eTransType_Interp2Rev,
    eTransType_Interp3Rev,
    eTransType_Interp4Rev,
    eTransType_Total
};

enum _tagCamType
{
    eCamType_Follow,
    eCamType_FollowWithOffset,
    eCamType_Static,
    eCamType_Path,
    eCamType_StaticFollow,
    eCamType_Shoulder,
    eCamType_Total
};

struct _tagxCamFollowAsset
{
    F32 rotation;
    F32 distance;
    F32 height;
    F32 rubber_band;
    F32 start_speed;
    F32 end_speed;
};

struct _tagxCamShoulderAsset
{
    F32 distance;
    F32 height;
    F32 realign_speed;
    F32 realign_delay;
};

struct _tagp2CamStaticAsset
{
    U32 unused;
};

struct _tagxCamPathAsset
{
    U32 assetID;
    F32 time_end;
    F32 time_delay;
};

struct _tagp2CamStaticFollowAsset
{
    F32 rubber_band;
};

struct xCamAsset : p2BaseAsset
{
    xVec3 pos;
    xVec3 at;
    xVec3 up;
    xVec3 right;
    xVec3 view_offset;
    S16 offset_start_frames;
    S16 offset_end_frames;
    F32 fov;
    F32 trans_time;
    _tagTransType trans_type;
    U32 flags;
    F32 fade_up;
    F32 fade_down;
    union
    {
        _tagxCamFollowAsset cam_follow;
        _tagxCamShoulderAsset cam_shoulder;
        _tagp2CamStaticAsset cam_static;
        _tagxCamPathAsset cam_path;
        _tagp2CamStaticFollowAsset cam_staticFollow;
    };
    U32 valid_flags;
    U32 markerid[2];
    U8 cam_type;
    U8 pad[3];
};

struct _tagxCamFollow
{
    F32 rotation;
    F32 distance;
    F32 height;
    F32 rubber_band;
    F32 start_speed;
    F32 end_speed;
};

struct _tagxCamShoulder
{
    F32 distance;
    F32 height;
    F32 realign_speed;
    F32 realign_delay;
    S32 state;
};

struct _tagxCamStatic
{
    U32 unused;
};

struct _tagxCamPath
{
    U32 assetID;
    F32 time_end;
};

struct _tagxCamStaticFollow
{
    F32 rubber_band;
};

union _tagCamInfo
{
    _tagxCamFollow f;
    _tagxCamStatic s;
    _tagxCamShoulder sh;
    _tagxCamPath p;
    _tagxCamStaticFollow sf;
};

// The GC object is 0xA0 bytes; the PS2 copy is 0xB0 because its matrices are 16-byte aligned.
struct _tagxCam : xBase
{
    U32 valid_flags;
    _tagCamType cam_type;
    U32 flags;
    xMat4x3 mat;
    xQuat rot;
    F32 fov;
    F32 trans_time;
    _tagTransType trans_type;
    xVec3 view_offset;
    _tagCamInfo u;
    xCamAsset* asset;
};

// orig_cam holds the camera a transition starts from, hi_cam the camera being shown and t_cam
// the camera being blended towards. The GC object is 0x1F0 bytes.
struct _tagxCamFrame
{
    RwCamera* lo_cam;
    _tagxCam orig_cam;
    _tagxCam hi_cam;
    xVec4 frustplane[8];
    _tagxCam* t_cam;
    F32 t_time;
    F32 g_time;
    _xEnv* env;
    xVec3* target_pos;
    xMat4x3* target_frame;
    xVec3 pre_align_dir;
    _tagxCam** cameras;
    U32 total_cameras;
};

extern F32 gCameraLastFov;

xMat4x3* xCameraGetCurMatrix(_tagxCamFrame* cam);
void xCameraTargetOnGround(S32 on_ground, F32 height);
void xCameraPickInitial(_tagxCamFrame* cam, _tagxCam* c);
void xCameraPickInitial(_tagxCamFrame* cam, U32 init_id);
void xCameraNext(_tagxCamFrame* cam, _tagxCam* trans_cam);
void xCameraInterpValid(_tagxCamFrame* cam, _tagTransType trans_type, F32 seconds);
_tagxCam* xCameraSetAsset(_tagxCamFrame* cam, xCamAsset* src, S32 idx);
_tagxCam* xCameraInit(_tagxCamFrame* cam, U32 total);
void xCameraRemoveEnv(_tagxCamFrame* camera);
void xCameraAssignEnv(_tagxCamFrame* camera, _xEnv* env);
void xCameraSetTarget(_tagxCamFrame* cam, xMat4x3* frame_target);
void xCameraSetTarget(_tagxCamFrame* cam, xVec3* new_target);
void xCameraShowRaster(_tagxCamFrame* cam);
void xCameraEnd(_tagxCamFrame* cam, F32 seconds, S32 update_scrn_fx);
void xCameraUpdate(_tagxCamFrame* cam, F32 seconds);
void xCameraRunState(_tagxCamFrame* cam, F32 seconds);
_tagxCam* xCameraShoulderState(_tagxCamFrame* cam, F32 seconds);
_tagxCam* xCameraFollowState(_tagxCamFrame* cam, F32 seconds);
void xCameraSetController(_tagxCamFrame* cam, F32 x, F32 y, F32 seconds);
void xCameraSetTargetDY(_tagxCamFrame* cam, F32 dy, F32 seconds);
void xCameraSetTargetDX(_tagxCamFrame* cam, F32 dx, F32 seconds);
_tagxCam* xCameraFollowWithOffsetState(_tagxCamFrame* cam, F32 seconds);
void xCameraBegin(_tagxCamFrame* cam, S32 clear);
void xCameraDestroy(_tagxCamFrame* cam);
void xCameraCreate(_tagxCamFrame* cam, U32 width, U32 height);

#endif
