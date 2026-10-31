#include "zVolume.h"

#include <math.h>

#include "iMath.h"
#include "xEvent.h"
#include "xMath3.h"
#include "xMemMgr.h"
#include "xstransvc.h"

struct PreCalcOcclude
{
    xVec4 DepthVec;
    xVec4 FrustVec[4];
};

static _zVolume* vols;
static U16 nvols;

S32 gOccludeCount;
_zVolume* gOccludeList[10];
S32 gOccludeCalcCount;
PreCalcOcclude gOccludeCalc[10];

S32 zVolumeEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*);

// zVolume_OccludeTest is not in the target: nothing in the GC build calls it.

// Volumes save and restore only their base state.
void zVolumeSave(_zVolume* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

void zVolumeLoad(_zVolume* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

// Create one volume per 'VOLU' asset in the scene. The base's link pointer points just past the
// asset when the asset has links.
void zVolumeInit()
{
    zVolumeAsset* asset;
    U32 size;
    U16 i;

    nvols = xSTAssetCountByType('VOLU');

    if (nvols != 0)
    {
        vols = (_zVolume*)xMemAlloc(gActiveHeap, nvols * sizeof(_zVolume));

        for (i = 0; i < nvols; i++)
        {
            asset = (zVolumeAsset*)xSTFindAssetByType('VOLU', i, &size);

            _zVolume* vol = &vols[i];
            xBaseInit(vol, asset);
            vol->eventFunc = zVolumeEventCB;
            vol->asset = asset;

            if (vol->linkCount != 0)
            {
                vol->link = (p2LinkAsset*)(vol->asset + 1);
            }
            else
            {
                vol->link = NULL;
            }
        }
    }
    else
    {
        vols = NULL;
    }
}

// Mark every volume's bound as touching every quick-cull cell.
void zVolumeSetup()
{
    U32 i;

    for (i = 0; i < nvols; i++)
    {
        vols[i].asset->bound.qcd.zmask = 0xFFFFFFFF;
        vols[i].asset->bound.qcd.ymask = 0xFFFFFFFF;
        vols[i].asset->bound.qcd.xmask = 0xFFFFFFFF;
    }
}

// A volume's bound, held in its asset.
_xBound* zVolumeGetBound(_zVolume* vol)
{
    return &vol->asset->bound;
}

// The scene's nth volume.
_zVolume* zVolumeGetVolume(U16 n)
{
    return &vols[n];
}

// For each active occluder, build the plane of its (rotated) front face and the four planes from
// the camera through its edges, and store them for the per-model occlusion test. An occluder seen
// almost edge-on (camera within one unit of its plane) is skipped.
void zVolume_OccludePrecalc(xVec3* camPos)
{
    F32 camdot;
    F32 depthdot;
    xVec4 locFrustVec[4];
    PreCalcOcclude* calc;
    F32 s;
    F32 c;
    zVolumeAsset* a;
    xVec3 corner[5];
    S32 j;
    S32 i;

    gOccludeCalcCount = 0;

    for (i = 0; i < gOccludeCount; i++)
    {
        a = gOccludeList[i]->asset;

        c = cos(a->rot);
        s = sin(a->rot);

        calc = &gOccludeCalc[gOccludeCalcCount];

        corner[0].x = c * (a->bound.box.box.lower.x - a->xpivot) + a->xpivot;
        corner[0].y = a->bound.box.box.lower.y;
        corner[0].z = s * (a->bound.box.box.lower.x - a->xpivot) + a->bound.box.box.lower.z;

        corner[1].x = c * (a->bound.box.box.upper.x - a->xpivot) + a->xpivot;
        corner[1].y = a->bound.box.box.lower.y;
        corner[1].z = s * (a->bound.box.box.upper.x - a->xpivot) + a->bound.box.box.lower.z;

        corner[2].x = corner[1].x;
        corner[2].y = a->bound.box.box.upper.y;
        corner[2].z = corner[1].z;

        corner[3].x = corner[0].x;
        corner[3].y = a->bound.box.box.upper.y;
        corner[3].z = corner[0].z;

        corner[4] = corner[0];

        {
            xVec3 d1;
            xVec3 d2;

            xVec3Sub(&d1, &corner[1], &corner[0]);
            xVec3Sub(&d2, &corner[2], &corner[0]);
            xVec3Cross((xVec3*)&calc->DepthVec, &d1, &d2);
        }
        xVec3Normalize((xVec3*)&calc->DepthVec, (xVec3*)&calc->DepthVec);

        depthdot = xVec3Dot((xVec3*)&calc->DepthVec, &corner[0]);
        camdot = xVec3Dot((xVec3*)&calc->DepthVec, camPos);

        if (camdot > depthdot)
        {
            xVec3SMulBy((xVec3*)&calc->DepthVec, -1.0f);
            camdot = -camdot;
            depthdot = -depthdot;
        }

        if (iabs(camdot - depthdot) < 1.0f)
        {
            continue;
        }

        calc->DepthVec.w = depthdot;

        for (j = 0; j < 4; j++)
        {
            xVec3 d1;
            xVec3 d2;

            xVec3Sub(&d1, &corner[j], camPos);
            xVec3Sub(&d2, &corner[j + 1], camPos);
            xVec3Cross((xVec3*)&locFrustVec[j], &d1, &d2);
            xVec3Normalize((xVec3*)&locFrustVec[j], (xVec3*)&locFrustVec[j]);
        }

        if (xVec3Dot((xVec3*)&locFrustVec[0], &corner[0]) > xVec3Dot((xVec3*)&locFrustVec[0], &corner[2]))
        {
            for (j = 0; j < 4; j++)
            {
                xVec3SMulBy((xVec3*)&locFrustVec[j], -1.0f);
            }
        }

        for (j = 0; j < 4; j++)
        {
            locFrustVec[j].w = xVec3Dot((xVec3*)&locFrustVec[j], &corner[j]);
        }

        calc->FrustVec[0].x = locFrustVec[0].w;
        calc->FrustVec[0].y = locFrustVec[1].w;
        calc->FrustVec[0].z = locFrustVec[2].w;
        calc->FrustVec[0].w = locFrustVec[3].w;

        calc->FrustVec[1].x = locFrustVec[0].x;
        calc->FrustVec[1].y = locFrustVec[1].x;
        calc->FrustVec[1].z = locFrustVec[2].x;
        calc->FrustVec[1].w = locFrustVec[3].x;

        calc->FrustVec[2].x = locFrustVec[0].y;
        calc->FrustVec[2].y = locFrustVec[1].y;
        calc->FrustVec[2].z = locFrustVec[2].y;
        calc->FrustVec[2].w = locFrustVec[3].y;

        calc->FrustVec[3].x = locFrustVec[0].z;
        calc->FrustVec[3].y = locFrustVec[1].z;
        calc->FrustVec[3].z = locFrustVec[2].z;
        calc->FrustVec[3].w = locFrustVec[3].z;

        gOccludeCalcCount++;
    }
}

// Volume events: reset restores the base from its asset. Occlude-on adds the volume to the list
// of occluders (at most 10) that zVolume_OccludePrecalc reads. Occlude-off, when the volume is in
// the list, copies the last entry into the slot past it and shortens the list by one, so it drops
// the last entry whichever volume matched.
S32 zVolumeEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    _zVolume* vol = (_zVolume*)to;
    S32 i;

    switch (toEvent)
    {
    case eEventReset:
        xBaseReset(vol, vol->asset);
        break;
    case eEventOccludeOn:
        if (gOccludeCount == 10)
        {
            return 1;
        }

        for (i = 0; i < gOccludeCount; i++)
        {
            if (gOccludeList[i] == vol)
            {
                return 1;
            }
        }

        gOccludeList[gOccludeCount] = vol;
        gOccludeCount++;
        break;
    case eEventOccludeOff:
        for (i = 0; i < gOccludeCount; i++)
        {
            if (gOccludeList[i] == vol)
            {
                gOccludeList[gOccludeCount] = gOccludeList[gOccludeCount - 1];
                gOccludeCount--;
                return 1;
            }
        }
        break;
    }

    return 1;
}
