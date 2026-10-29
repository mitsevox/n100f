#include "iAnimSKB.h"

#include "iAnim.h"
#include "xMath.h"
#include "xMemMgr.h"

#include <rtslerp.h>
#include <string.h>

void iAnimEvalSKB(iAnimSKBHeader* data, F32 time, U32 flags, xVec3* tran, xQuat* quat)
{
    F32 scalex, scaley, scalez;
    U32 i;
    U32 tidx;
    U32 bcount;
    U32 tcount;
    iAnimSKBKey* keys;
    F32* times;
    U16* offsets;
    F32 time1;
    F32 time2;
    F32 lerp;
    iAnimSKBKey* k;

    tcount = data->TimeCount;
    bcount = data->BoneCount;

    keys = (iAnimSKBKey*)(data + 1);
    times = (F32*)(keys + data->KeyCount);
    offsets = (U16*)(times + tcount);

    if (time < 0.0f)
    {
        time = 0.0f;
    }

    if (time > times[tcount - 1])
    {
        time = times[tcount - 1];
    }

    tidx = (tcount - 1) % 4;

    while (times[tidx] < time)
    {
        tidx += 4;
    }

    while (tidx && time <= times[tidx])
    {
        tidx--;
    }

    offsets += tidx * bcount;

    if (flags & 0x1)
    {
        bcount = 1;
    }

    if (flags & 0x2)
    {
        bcount--;
        offsets++;
    }

    scalex = data->Scale[0];
    scaley = data->Scale[1];
    scalez = data->Scale[2];

    for (i = 0; i < bcount; quat++, i++, tran++)
    {
        RtQuatSlerpCache qcache;
        RtQuat q1, q2;
        xVec3 t0, t1;

        k = &keys[*offsets];
        offsets++;

        time1 = time - times[k[0].TimeIndex];
        time2 = times[k[1].TimeIndex] - times[k[0].TimeIndex];
        lerp = time1 / time2;

        q1.imag.x = (1.0f / 32767.0f) * k[0].Quat[0];
        q1.imag.y = (1.0f / 32767.0f) * k[0].Quat[1];
        q1.imag.z = (1.0f / 32767.0f) * k[0].Quat[2];
        q1.real = (1.0f / 32767.0f) * k[0].Quat[3];

        q2.imag.x = (1.0f / 32767.0f) * k[1].Quat[0];
        q2.imag.y = (1.0f / 32767.0f) * k[1].Quat[1];
        q2.imag.z = (1.0f / 32767.0f) * k[1].Quat[2];
        q2.real = (1.0f / 32767.0f) * k[1].Quat[3];

        RtQuatSetupSlerpCache(&q1, &q2, &qcache);
        RtQuatSlerp((RtQuat*)quat, &q1, &q2, lerp, &qcache);

        t0.x = scalex * k[0].Tran[0];
        t0.y = scaley * k[0].Tran[1];
        t0.z = scalez * k[0].Tran[2];
        t1.x = scalex * k[1].Tran[0];
        t1.y = scaley * k[1].Tran[1];
        t1.z = scalez * k[1].Tran[2];

        tran->x = t0.x + lerp * (t1.x - t0.x);
        tran->y = t0.y + lerp * (t1.y - t0.y);
        tran->z = t0.z + lerp * (t1.z - t0.z);
    }
}

// Ramps the first maxbone bones' translations over the animation so its last frame meets the first frame of
// the following animation, removing the pop between them.
void _iAnimSKBFixTransPop(void* data, void* pose, U32 maxbone)
{
    U32 i;
    U32 boneIndex;
    F32 invDuration;
    xQuat* q_a;
    xVec3* t_a;
    xQuat* q_b;
    xVec3* t_b;
    iAnimSKBHeader* ha;
    iAnimSKBKey* ka;
    F32* ta;
    iAnimSKBHeader* hb;
    xVec3 scale;
    xVec3 pos;

    ha = (iAnimSKBHeader*)data;
    hb = (iAnimSKBHeader*)pose;

    q_a = (xQuat*)giAnimScratch;
    t_a = (xVec3*)(q_a + 64);
    q_b = (xQuat*)(giAnimScratch + 0x720);
    t_b = (xVec3*)(q_b + 64);

    ka = (iAnimSKBKey*)(ha + 1);
    ta = (F32*)(ka + ha->KeyCount);
    boneIndex = 0;

    if (maxbone == 0)
    {
        maxbone = ha->BoneCount;
    }

    scale.x = MAX(hb->Scale[0], ha->Scale[0]);
    scale.y = MAX(hb->Scale[1], ha->Scale[1]);
    scale.z = MAX(hb->Scale[2], ha->Scale[2]);

    iAnimEvalSKB(ha, iAnimDurationSKB(ha), 0, t_a, q_a);
    iAnimEvalSKB(hb, 0.0f, 0, t_b, q_b);

    for (i = 0; i < ha->BoneCount; i++)
    {
        xVec3Sub(&t_a[i], &t_b[i], &t_a[i]);
    }

    invDuration = 1.0f / iAnimDurationSKB(ha);

    for (i = 0; i < ha->KeyCount; i++)
    {
        if (i != 0 && ka[i].TimeIndex < ka[i - 1].TimeIndex)
        {
            boneIndex++;
        }

        pos.x = ha->Scale[0] * ka[i].Tran[0];
        pos.y = ha->Scale[1] * ka[i].Tran[1];
        pos.z = ha->Scale[2] * ka[i].Tran[2];

        if (boneIndex < maxbone)
        {
            pos.x += invDuration * (t_a[boneIndex].x * ta[ka[i].TimeIndex]);
            pos.y += invDuration * (t_a[boneIndex].y * ta[ka[i].TimeIndex]);
            pos.z += invDuration * (t_a[boneIndex].z * ta[ka[i].TimeIndex]);
        }

        ka[i].Tran[0] = pos.x / scale.x;
        ka[i].Tran[1] = pos.y / scale.y;
        ka[i].Tran[2] = pos.z / scale.z;
    }

    ha->Scale[0] = scale.x;
    ha->Scale[1] = scale.y;
    ha->Scale[2] = scale.z;
}

F32 iAnimDurationSKB(iAnimSKBHeader* data)
{
    return ((F32*)((iAnimSKBKey*)(data + 1) + data->KeyCount))[data->TimeCount - 1];
}

// Swaps the complete key runs (rotation and translation) of two bones, moving the runs between them, and
// rebuilds the key offset table.
void _iAnimSKBSwapTranslate(iAnimSKBHeader* data, U32 bonelo, U32 bonehi)
{
    U32 i;
    U32 j;
    iAnimSKBKey* buflo;
    iAnimSKBKey* bufhi;
    U32 bcount;
    U32 tcount;
    U16* offsets;
    U16 keyIndex;
    U32 lofirst;
    iAnimSKBKey* bufmid;
    iAnimSKBKey* keys;
    U32 lolast;
    U32 locount;
    U32 hifirst;
    U32 hicount;
    S32 out;
    U32 midfirst;
    U32 midlast;
    S32 midcount;

    keys = (iAnimSKBKey*)(data + 1);
    tcount = data->TimeCount;
    bcount = data->BoneCount;
    offsets = (U16*)((F32*)(keys + data->KeyCount) + tcount);

    buflo = (iAnimSKBKey*)xMemPushTemp(tcount * 3 * sizeof(iAnimSKBKey));
    bufhi = buflo + tcount;
    bufmid = bufhi + tcount;

    lofirst = offsets[bonelo];
    lolast = offsets[(tcount - 2) * bcount + bonelo] + 1;
    locount = lolast - lofirst + 1;
    memcpy(buflo, &keys[lofirst], locount * sizeof(iAnimSKBKey));

    hifirst = offsets[bonehi];
    hicount = offsets[(tcount - 2) * bcount + bonehi] + 1 - hifirst + 1;
    memcpy(bufhi, &keys[hifirst], hicount * sizeof(iAnimSKBKey));

    midfirst = lolast + 1;
    midlast = hifirst - 1;
    midcount = midlast - midfirst + 1;

    if (midcount > 0)
    {
        memcpy(bufmid, &keys[midfirst], midcount * sizeof(iAnimSKBKey));
    }

    memcpy(&keys[lofirst], bufhi, hicount * sizeof(iAnimSKBKey));
    out = lofirst + hicount;

    if (midcount > 0)
    {
        memcpy(&keys[out], bufmid, midcount * sizeof(iAnimSKBKey));
        out += midcount;
    }

    memcpy(&keys[out], buflo, locount * sizeof(iAnimSKBKey));

    xMemPopTemp(buflo);

    keyIndex = 0;

    for (i = 0; i < bcount; i++)
    {
        for (j = 0; j < tcount - 1; j++)
        {
            while (keys[keyIndex + 1].TimeIndex <= j)
            {
                keyIndex++;
            }

            offsets[j * bcount + i] = keyIndex;
        }

        while (keys[keyIndex].TimeIndex != tcount - 1)
        {
            keyIndex++;
        }

        keyIndex++;
    }
}

// Adds an offset to one bone's translation keys. An axis scale grows when the largest offset value exceeds
// the largest original value on that axis, and keys are clamped to +/-32767.
void _iAnimSKBAdjustTranslate(iAnimSKBHeader* data, U32 bone, F32* tran)
{
    S32 ipos;
    U32 i;
    U32 idx;
    U32 keyfirst;
    U32 keylast;
    U32 kcount;
    U32 tcount;
    F32 outScale[3];
    F32 pos;
    F32 factor[3];
    F32 oldmax[3] = { 0.0f, 0.0f, 0.0f };
    F32 newmax[3] = { 0.0f, 0.0f, 0.0f };
    iAnimSKBKey* keys;
    U16* offsets;

    tcount = data->TimeCount;
    kcount = data->KeyCount;
    keys = (iAnimSKBKey*)(data + 1);
    offsets = (U16*)((F32*)(keys + kcount) + tcount);
    keyfirst = offsets[bone];
    keylast = offsets[(tcount - 2) * data->BoneCount + bone] + 1;

    for (i = 0; i < kcount; i++)
    {
        for (idx = 0; idx < 3; idx++)
        {
            if (tran[idx])
            {
                pos = data->Scale[idx] * keys[i].Tran[idx];

                if (xabs(pos) > oldmax[idx])
                {
                    oldmax[idx] = xabs(pos);
                }

                if (i >= keyfirst && i <= keylast)
                {
                    pos += tran[idx];

                    if (xabs(pos) > newmax[idx])
                    {
                        newmax[idx] = xabs(pos);
                    }
                }
            }
        }
    }

    for (idx = 0; idx < 3; idx++)
    {
        if (tran[idx])
        {
            if (newmax[idx] > oldmax[idx])
            {
                outScale[idx] = data->Scale[idx] * newmax[idx] / oldmax[idx];
            }
            else
            {
                outScale[idx] = data->Scale[idx];
            }

            factor[idx] = 1.0f / outScale[idx];
        }
    }

    for (i = 0; i < kcount; i++)
    {
        for (idx = 0; idx < 3; idx++)
        {
            if (tran[idx])
            {
                if (i >= keyfirst && i <= keylast)
                {
                    ipos = factor[idx] * (data->Scale[idx] * keys[i].Tran[idx] + tran[idx]);

                    if (ipos < -32767)
                    {
                        ipos = -32767;
                    }
                    else if (ipos > 32767)
                    {
                        ipos = 32767;
                    }

                    keys[i].Tran[idx] = ipos;
                }
                else if (data->Scale[idx] != outScale[idx])
                {
                    ipos = data->Scale[idx] * keys[i].Tran[idx] * factor[idx];

                    if (ipos < -32767)
                    {
                        ipos = -32767;
                    }
                    else if (ipos > 32767)
                    {
                        ipos = 32767;
                    }

                    keys[i].Tran[idx] = ipos;
                }
            }
        }
    }

    for (idx = 0; idx < 3; idx++)
    {
        if (tran[idx])
        {
            data->Scale[idx] = outScale[idx];
        }
    }
}

// Turns one bone's keys half a turn about the vertical axis.
void _iAnimSKBFlip180(iAnimSKBHeader* data, U32 bone)
{
    U32 i;
    U32 keyfirst;
    U32 keylast;
    U32 kcount;
    U32 tcount;
    iAnimSKBKey* keys;
    U16* offsets;
    F32 quat[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
    F32 _s, _x, _y, _z;

    tcount = data->TimeCount;
    kcount = data->KeyCount;
    keys = (iAnimSKBKey*)(data + 1);
    offsets = (U16*)((F32*)(keys + kcount) + tcount);
    keyfirst = offsets[bone];
    keylast = offsets[(tcount - 2) * data->BoneCount + bone] + 1;

    for (i = 0; i < kcount; i++)
    {
        if (i >= keyfirst && i <= keylast)
        {
            _s = quat[3] * keys[i].Quat[3] - quat[0] * keys[i].Quat[0] - quat[1] * keys[i].Quat[1] -
                 quat[2] * keys[i].Quat[2];
            _x = quat[0] * keys[i].Quat[3] + quat[3] * keys[i].Quat[0] + quat[1] * keys[i].Quat[2] -
                 quat[2] * keys[i].Quat[1];
            _y = quat[1] * keys[i].Quat[3] + quat[3] * keys[i].Quat[1] + quat[2] * keys[i].Quat[0] -
                 quat[0] * keys[i].Quat[2];
            _z = quat[2] * keys[i].Quat[3] + quat[3] * keys[i].Quat[2] + quat[0] * keys[i].Quat[1] -
                 quat[1] * keys[i].Quat[0];

            keys[i].Quat[0] = CLAMP(_x, -32767.0f, 32767.0f);
            keys[i].Quat[1] = CLAMP(_y, -32767.0f, 32767.0f);
            keys[i].Quat[2] = CLAMP(_z, -32767.0f, 32767.0f);
            keys[i].Quat[3] = CLAMP(_s, -32767.0f, 32767.0f);

            keys[i].Tran[0] = -keys[i].Tran[0];
            keys[i].Tran[2] = -keys[i].Tran[2];
        }
    }
}
