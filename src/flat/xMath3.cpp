
#include "xMath3.h"

#include "xMath.h"
#include "iMath3.h"

#include <math.h>

const xVec3 g_O3 = { 0.0f, 0.0f, 0.0f };
const xVec3 g_X3 = { 1.0f, 0.0f, 0.0f };
const xVec3 g_Y3 = { 0.0f, 1.0f, 0.0f };
const xVec3 g_Z3 = { 0.0f, 0.0f, 1.0f };
const xVec3 g_NX3 = { -1.0f, 0.0f, 0.0f };
const xVec3 g_NY3 = { 0.0f, -1.0f, 0.0f };
const xVec3 g_NZ3 = { 0.0f, 0.0f, -1.0f };
const xMat4x3 g_I3 = { { 1.0f, 0.0f, 0.0f }, 0, { 0.0f, 1.0f, 0.0f }, 0, { 0.0f, 0.0f, 1.0f }, 0,
                       { 0.0f, 0.0f, 0.0f }, 0 };
const xQuat g_IQ = { { 0.0f, 0.0f, 0.0f }, 1.0f };

static inline F32 isin(F32 x)
{
    return sin(x);
}

static inline F32 icos(F32 x)
{
    return cos(x);
}

static inline F32 xasin(F32 x)
{
    return asin(x);
}

static inline void xMat4x3Toworld(xVec3* o, const xMat4x3* m, const xVec3* v)
{
    xMat3x3RMulVec(o, (const xMat3x3*)m, v);
    xVec3AddTo(o, &m->pos);
}

void xMath3Init()
{
    iMath3Init();
}

void xMath3Exit()
{
    iMath3Exit();
}

F32 xVec3LengthFast(F32 x, F32 y, F32 z)
{
    F32 min;
    F32 max;
    F32 med;

    x = xabs(x);
    y = xabs(y);
    z = xabs(z);
    max = x;
    med = y;
    min = z;

    if (med > max)
    {
        F32 temp = med;
        med = max;
        max = temp;
    }

    if (min > med)
    {
        F32 temp = min;
        min = med;
        med = temp;
    }

    if (med > max)
    {
        F32 temp = med;
        med = max;
        max = temp;
    }

    return max + 0.34375f * med + 0.25f * min;
}

F32 xVec3Normalize(xVec3* o, const xVec3* v)
{
    F32 len = xVec3Length2(v);
    F32 one_len;

    if (len == 1.0f)
    {
        if (o != v)
        {
            xVec3Copy(o, v);
        }
        return 1.0f;
    }

    if (len == 0.0f)
    {
        if (o != v)
        {
            xVec3Copy(o, &g_Y3);
        }
        return 0.0f;
    }

    len = sqrtf(len);
    one_len = 1.0f / len;
    xVec3SMul(o, v, one_len);
    return len;
}

void xLine3VecDist2(const xVec3* p1, const xVec3* p2, const xVec3* v, xIsect* isx)
{
    xVec3 ldir;
    F32 ldirdotlv;
    F32 ldirlen2;

    xVec3Sub(&ldir, p2, p1);
    xVec3Sub(&isx->norm, v, p1);

    ldirdotlv = xVec3Dot(&ldir, &isx->norm);
    if (ldirdotlv <= 0.0f)
    {
        isx->dist = xVec3Length2(&isx->norm);
        return;
    }

    ldirlen2 = xVec3Length2(&ldir);
    if (ldirdotlv >= ldirlen2)
    {
        xVec3Sub(&isx->norm, v, p2);
        isx->dist = xVec3Length2(&isx->norm);
        return;
    }

    isx->dist = xVec3Length2(&isx->norm) - ldirdotlv * ldirdotlv / ldirlen2;
}

void xMat3x3Normalize(xMat3x3* o, const xMat3x3* m)
{
    xVec3Normalize(&o->right, &m->right);
    xVec3Normalize(&o->up, &m->up);
    xVec3Normalize(&o->at, &m->at);
}

void xMat3x3GetScale(const xMat3x3* m, xVec3* s)
{
    s->x = sqrtf(xVec3Length2(&m->right));
    s->y = sqrtf(xVec3Length2(&m->up));
    s->z = sqrtf(xVec3Length2(&m->at));
}

void xMat3x3GetEuler(const xMat3x3* m, xVec3* a)
{
    F32 roll;
    F32 yaw = -xasin(m->at.y);
    F32 pitch;

    if (yaw < (PI / 2))
    {
        if (yaw > -(PI / 2))
        {
            pitch = xAngleClamp(atan2(m->at.x, m->at.z));
            roll = xAngleClamp(atan2(m->right.y, m->up.y));
        }
        else
        {
            pitch = -xAngleClamp(atan2(-m->up.x, m->right.x));
            roll = 0.0f;
        }
    }
    else
    {
        pitch = xAngleClamp(atan2(-m->up.x, m->right.x));
        roll = 0.0f;
    }

    a->x = pitch;
    a->y = yaw;
    a->z = roll;
}

void xMat4x3MoveLocalRight(xMat4x3* m, F32 mag)
{
    m->pos.x += m->right.x * mag;
    m->pos.y += m->right.y * mag;
    m->pos.z += m->right.z * mag;
}

void xMat4x3MoveLocalUp(xMat4x3* m, F32 mag)
{
    m->pos.x += m->up.x * mag;
    m->pos.y += m->up.y * mag;
    m->pos.z += m->up.z * mag;
}

void xMat4x3MoveLocalAt(xMat4x3* m, F32 mag)
{
    m->pos.x += m->at.x * mag;
    m->pos.y += m->at.y * mag;
    m->pos.z += m->at.z * mag;
}

F32 xMat3x3LookVec(xMat3x3* m, const xVec3* at)
{
    F32 len = xVec3Normalize(&m->at, at);
    xVec3Inv(&m->at, &m->at);

    if (xabs(1.0f - m->at.y) < 0.00001f)
    {
        m->right.x = 1.0f;
        m->right.y = 0.0f;
        m->right.z = 0.0f;
        m->up.x = 0.0f;
        m->up.y = 0.0f;
        m->up.z = 1.0f;
        m->at.x = 0.0f;
        m->at.y = -1.0f;
        m->at.z = 0.0f;
        return len;
    }

    if (xabs(1.0f + m->at.y) < 0.00001f)
    {
        m->right.x = -1.0f;
        m->right.y = 0.0f;
        m->right.z = 0.0f;
        m->up.x = 0.0f;
        m->up.y = 0.0f;
        m->up.z = -1.0f;
        m->at.x = 0.0f;
        m->at.y = 1.0f;
        m->at.z = 0.0f;
        return len;
    }

    if (xabs(at->z) < 0.00001f && xabs(at->x) < 0.00001f)
    {
        m->right.x = 1.0f;
        m->right.y = 0.0f;
        m->right.z = 0.0f;
        m->up.x = 0.0f;
        m->up.y = 1.0f;
        m->up.z = 0.0f;
        m->at.x = 0.0f;
        m->at.y = 0.0f;
        m->at.z = 1.0f;
        return 0.0f;
    }

    m->right.x = m->at.z;
    m->right.y = 0.0f;
    m->right.z = -m->at.x;
    xVec3Normalize(&m->right, (xVec3*)m);
    xVec3Cross(&m->up, &m->at, &m->right);
    xVec3Cross(&m->right, &m->up, &m->at);
    m->flags = 0;
    return len;
}

void xMat3x3Euler(xMat3x3* m, xVec3* ypr)
{
    xMat3x3Euler(m, ypr->x, ypr->y, ypr->z);
}

void xMat3x3Euler(xMat3x3* m, F32 yaw, F32 pitch, F32 roll)
{
    F32 sy = isin(yaw);
    F32 cy = icos(yaw);
    F32 sp = isin(pitch);
    F32 cp = icos(pitch);
    F32 sr = isin(roll);
    F32 cr = icos(roll);

    m->right.x = cy * cr + sr * (sy * sp);
    m->right.y = cp * sr;
    m->right.z = -sy * cr + sr * (cy * sp);
    m->up.x = -cy * sr + cr * (sy * sp);
    m->up.y = cp * cr;
    m->up.z = sy * sr + cr * (cy * sp);
    m->at.x = sy * cp;
    m->at.y = -sp;
    m->at.z = cy * cp;
    m->flags = 0;
}

void xMat3x3RotC(xMat3x3* m, F32 _x, F32 _y, F32 _z, F32 t)
{
    F32 a;
    F32 b;
    F32 c;

    if (t == 0.0f)
    {
        xMat3x3Identity(m);
        return;
    }

    a = icos(t);
    b = isin(t);
    c = 1.0f - a;

    m->right.x = c * _x * _x + a;
    m->right.y = b * _z + c * _x * _y;
    m->right.z = -b * _y + c * _z * _x;
    m->up.x = -b * _z + c * _x * _y;
    m->up.y = c * _y * _y + a;
    m->up.z = b * _x + c * _y * _z;
    m->at.x = b * _y + c * _z * _x;
    m->at.y = -b * _x + c * _y * _z;
    m->at.z = c * _z * _z + a;
    m->flags = 0;
}

void xMat3x3RotY(xMat3x3* m, F32 t)
{
    F32 a = icos(t);
    F32 b = isin(t);

    xVec3Init(&m->right, a, 0.0f, -b);
    xVec3Copy(&m->up, &g_Y3);
    xVec3Init(&m->at, b, 0.0f, a);
    m->flags = 0;
}

void xMat3x3LMulRotY(xMat3x3* o, const xMat3x3* m, F32 t)
{
    F32 a = icos(t);
    F32 b = isin(t);
    F32 temp;

    if (o == m)
    {
        temp = a * o->right.x - b * o->at.x;
        o->at.x = a * o->at.x + b * o->right.x;
        o->right.x = temp;

        temp = a * o->right.y - b * o->at.y;
        o->at.y = a * o->at.y + b * o->right.y;
        o->right.y = temp;

        temp = a * o->right.z - b * o->at.z;
        o->at.z = a * o->at.z + b * o->right.z;
        o->right.z = temp;
    }
    else
    {
        xVec3Copy(&o->up, &m->up);
        o->right.x = a * m->right.x - b * m->at.x;
        o->at.x = a * m->at.x + b * m->right.x;
        o->right.y = a * m->right.y - b * m->at.y;
        o->at.y = a * m->at.y + b * m->right.y;
        o->right.z = a * m->right.z - b * m->at.z;
        o->at.z = a * m->at.z + b * m->right.z;
        o->flags = 0;
    }
}

void xMat3x3RMulRotY(xMat3x3* o, const xMat3x3* m, F32 t)
{
    F32 a = icos(t);
    F32 b = isin(t);
    F32 temp;

    if (o == m)
    {
        temp = a * o->right.x + b * o->right.z;
        o->right.z = a * o->right.z - b * o->right.x;
        o->right.x = temp;

        temp = a * o->up.x + b * o->up.z;
        o->up.z = a * o->up.z - b * o->up.x;
        o->up.x = temp;

        temp = a * o->at.x + b * o->at.z;
        o->at.z = a * o->at.z - b * o->at.x;
        o->at.x = temp;
    }
    else
    {
        o->right.x = a * m->right.x + b * m->right.z;
        o->right.y = m->right.y;
        o->right.z = a * m->right.z - b * m->right.x;
        o->up.x = a * m->up.x + b * m->up.z;
        o->up.y = m->up.y;
        o->up.z = a * m->up.z - b * m->up.x;
        o->at.x = a * m->at.x + b * m->at.z;
        o->at.y = m->at.y;
        o->at.z = a * m->at.z - b * m->at.x;
        o->flags = 0;
    }
}

void xMat3x3Transpose(xMat3x3* o, const xMat3x3* m)
{
    if (o == m)
    {
        F32 temp;

        temp = o->right.y;
        o->right.y = o->up.x;
        o->up.x = temp;

        temp = o->right.z;
        o->right.z = o->at.x;
        o->at.x = temp;

        temp = o->up.z;
        o->up.z = o->at.y;
        o->at.y = temp;
    }
    else
    {
        o->right.x = m->right.x;
        o->right.y = m->up.x;
        o->right.z = m->at.x;
        o->up.x = m->right.y;
        o->up.y = m->up.y;
        o->up.z = m->at.y;
        o->at.x = m->right.z;
        o->at.y = m->up.z;
        o->at.z = m->at.z;
        o->flags = 0;
    }
}

void xMat3x3Mul(xMat3x3* o, const xMat3x3* a, const xMat3x3* b)
{
    xMat3x3 temp;
    xMat3x3* tp;
    U32 usetemp = (o == a || o == b);

    if (usetemp)
    {
        tp = &temp;
    }
    else
    {
        tp = o;
    }

    tp->right.x = a->right.x * b->right.x + a->right.y * b->up.x + a->right.z * b->at.x;
    tp->right.y = a->right.x * b->right.y + a->right.y * b->up.y + a->right.z * b->at.y;
    tp->right.z = a->right.x * b->right.z + a->right.y * b->up.z + a->right.z * b->at.z;
    tp->up.x = a->up.x * b->right.x + a->up.y * b->up.x + a->up.z * b->at.x;
    tp->up.y = a->up.x * b->right.y + a->up.y * b->up.y + a->up.z * b->at.y;
    tp->up.z = a->up.x * b->right.z + a->up.y * b->up.z + a->up.z * b->at.z;
    tp->at.x = a->at.x * b->right.x + a->at.y * b->up.x + a->at.z * b->at.x;
    tp->at.y = a->at.x * b->right.y + a->at.y * b->up.y + a->at.z * b->at.y;
    tp->at.z = a->at.x * b->right.z + a->at.y * b->up.z + a->at.z * b->at.z;

    if (usetemp)
    {
        xMat3x3Copy(o, tp);
    }
    else
    {
        o->flags = 0;
    }
}

void xMat3x3LMulVec(xVec3* o, const xMat3x3* m, const xVec3* v)
{
    xVec3Init(o, m->right.x * v->x + m->right.y * v->y + m->right.z * v->z,
              m->up.x * v->x + m->up.y * v->y + m->up.z * v->z,
              m->at.x * v->x + m->at.y * v->y + m->at.z * v->z);
}

void xMat3x3RMulVec(xVec3* o, const xMat3x3* m, const xVec3* v)
{
    F32 y = m->right.y * v->x + m->up.y * v->y + m->at.y * v->z;
    F32 z = m->right.z * v->x + m->up.z * v->y + m->at.z * v->z;

    o->x = m->right.x * v->x + m->up.x * v->y + m->at.x * v->z;
    o->y = y;
    o->z = z;
}

void xMat3x3Tolocal(xVec3* o, const xMat3x3* m, const xVec3* v)
{
    F32 rlen2 = xVec3Length2(&m->right);
    F32 ulen2 = xVec3Length2(&m->up);
    F32 alen2 = xVec3Length2(&m->at);

    xMat3x3LMulVec(o, m, v);
    o->x *= 1.0f / rlen2;
    o->y *= 1.0f / ulen2;
    o->z *= 1.0f / alen2;
}

void xMat4x3Orthonormalize(xMat4x3* o, const xMat4x3* m)
{
    xVec3Cross(&o->right, &o->up, &o->at);
    xVec3Normalize(&o->right, (xVec3*)o);
    xVec3Cross(&o->up, &o->at, &o->right);
    xVec3Normalize(&o->up, &o->up);
    xVec3Cross(&o->at, &o->right, &o->up);
    xVec3Normalize(&o->at, &o->at);
    xVec3Copy(&o->pos, &m->pos);
}

void xMat4x3Rot(xMat4x3* m, const xVec3* a, F32 t, const xVec3* p)
{
    xMat4x3 temp;

    xMat3x3RotC((xMat3x3*)m, a->x, a->y, a->z, t);
    xVec3Copy(&m->pos, p);
    xMat3x3Identity((xMat3x3*)&temp);
    xVec3Inv(&temp.pos, p);
    xMat4x3Mul(m, &temp, m);
}

void xMat4x3Mul(xMat4x3* o, const xMat4x3* a, const xMat4x3* b)
{
    xVec3 pos;

    xMat4x3Toworld(&pos, b, &a->pos);
    xMat3x3Mul((xMat3x3*)o, (const xMat3x3*)a, (const xMat3x3*)b);
    xVec3Copy(&o->pos, &pos);
}

void xMat4x3LMul3x3(xMat4x3* o, const xMat4x3* m, const xMat3x3* m3x3)
{
    xVec3 pos;

    xMat3x3RMulVec(&pos, m3x3, &m->pos);
    xMat3x3Mul((xMat3x3*)o, (const xMat3x3*)m, m3x3);
    xVec3Copy(&o->pos, &pos);
}

void xMat4x3RMul3x3(xMat4x3* o, const xMat3x3* m3x3, const xMat4x3* m)
{
    xMat3x3Mul((xMat3x3*)o, m3x3, (const xMat3x3*)m);
    if (o != m)
    {
        xVec3Copy(&o->pos, &m->pos);
    }
}

void xQuatFromMat(xQuat* q, const xMat3x3* m)
{
    static S32 nxt[3] = { 1, 2, 0 };

    F32* mp = (F32*)m;
    F32* qvp = (F32*)q;
    F32 tr = m->right.x + m->up.y + m->at.z;
    F32 root;
    S32 i;
    S32 j;
    S32 k;

    if (tr > 0.0f)
    {
        root = sqrtf(1.0f + tr);
        q->s = 0.5f * root;
        root = 0.5f / root;
        q->v.x = root * (m->at.y - m->up.z);
        q->v.y = root * (m->right.z - m->at.x);
        q->v.z = root * (m->up.x - m->right.y);
    }
    else
    {
        i = 0;
        if (mp[5] > mp[0])
        {
            i = 1;
        }
        if (mp[10] > mp[i * 5])
        {
            i = 2;
        }

        j = nxt[i];
        k = nxt[j];

        root = sqrtf(1.0f + (mp[i * 5] - mp[j * 5] - mp[k * 5]));
        if (xabs(root) < 0.00001f)
        {
            xQuatCopy(q, &g_IQ);
            return;
        }

        qvp[i] = 0.5f * root;
        root = 0.5f / root;
        q->s = root * (mp[j + k * 4] - mp[k + j * 4]);
        qvp[j] = root * (mp[i + j * 4] + mp[j + i * 4]);
        qvp[k] = root * (mp[i + k * 4] + mp[k + i * 4]);
    }
}

void xQuatToMat(const xQuat* q, xMat3x3* m)
{
    F32 tx = 2.0f * q->v.x;
    F32 ty = 2.0f * q->v.y;
    F32 tz = 2.0f * q->v.z;
    F32 tsx = tx * q->s;
    F32 tsy = ty * q->s;
    F32 tsz = tz * q->s;
    F32 txx = tx * q->v.x;
    F32 txy = ty * q->v.x;
    F32 txz = tz * q->v.x;
    F32 tyy = ty * q->v.y;
    F32 tyz = tz * q->v.y;
    F32 tzz = tz * q->v.z;

    m->right.x = (1.0f - tyy) - tzz;
    m->right.y = txy - tsz;
    m->right.z = txz + tsy;

    m->up.x = txy + tsz;
    m->up.y = (1.0f - tzz) - txx;
    m->up.z = tyz - tsx;

    m->at.x = txz - tsy;
    m->at.y = tyz + tsx;
    m->at.z = (1.0f - txx) - tyy;

    m->flags = 0;
}

void xQuatConj(xQuat* o, const xQuat* q)
{
    o->s = q->s;
    xVec3Inv(&o->v, &q->v);
}

F32 xQuatNormalize(xQuat* o, const xQuat* q)
{
    F32 len = xQuatLength2(q);
    F32 one_len;

    if (len == 1.0f)
    {
        if (o != q)
        {
            xQuatCopy(o, q);
        }
        return 1.0f;
    }

    if (len == 0.0f)
    {
        if (o != q)
        {
            xQuatCopy(o, &g_IQ);
        }
        return 0.0f;
    }

    len = sqrtf(len);
    one_len = 1.0f / len;
    xQuatSMul(o, q, one_len);
    return len;
}

void xQuatSlerp(xQuat* o, const xQuat* a, const xQuat* b, F32 t)
{
    F32 theta;
    F32 asph;
    F32 bsph;
    F32 abdot;
    F32 one_sintheta;
    xQuat b2;
    xQuat qa;
    xQuat qb;

    abdot = xQuatDot(a, b);
    if (abdot < 0.0f)
    {
        abdot = -abdot;
        b2.v.x = -b->v.x;
        b2.v.y = -b->v.y;
        b2.v.z = -b->v.z;
        b2.s = -b->s;
        b = &b2;
    }

    if (abdot >= 0.999f)
    {
        asph = 1.0f - t;
        bsph = t;
    }
    else
    {
        theta = acos(abdot);
        one_sintheta = 1.0f / isin(theta);
        asph = one_sintheta * isin((1.0f - t) * theta);
        bsph = one_sintheta * isin(t * theta);
    }

    xQuatSMul(&qa, a, asph);
    xQuatSMul(&qb, b, bsph);
    xQuatAdd(o, &qa, &qb);
}

void xQuatMul(xQuat* o, const xQuat* a, const xQuat* b)
{
    F32 _s = a->s * b->s - a->v.x * b->v.x - a->v.y * b->v.y - a->v.z * b->v.z;
    xVec3 v;
    v.x = a->s * b->v.x + a->v.x * b->s + a->v.y * b->v.z - a->v.z * b->v.y;
    v.y = a->s * b->v.y + a->v.y * b->s + a->v.z * b->v.x - a->v.x * b->v.z;
    v.z = a->s * b->v.z + a->v.z * b->s + a->v.x * b->v.y - a->v.y * b->v.x;
    o->v = v;
    o->s = _s;
}
