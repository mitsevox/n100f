typedef float RwReal;
typedef int RwInt32;
typedef struct RwV3d { RwReal x, y, z; } RwV3d;
typedef struct RwError { RwInt32 pluginID; RwInt32 errorCode; } RwError;

extern RwReal _rwSqrt(RwReal x);
extern RwReal _rwInvSqrt(RwReal x);
extern RwInt32 _rwerror(RwInt32 code, ...);
extern RwError *RwErrorSet(RwError *code);

void V3dNormalizeNoCheck(RwV3d *out, const RwV3d *in)
{
    RwReal recip = _rwInvSqrt(in->x * in->x + in->y * in->y + in->z * in->z);
    out->x = in->x * recip;
    out->y = in->y * recip;
    out->z = in->z * recip;
}

RwReal RwV3dNormalize(RwV3d *out, const RwV3d *in)
{
    RwReal length2 = in->x * in->x + in->y * in->y + in->z * in->z;
    RwReal length = _rwSqrt(length2);
    RwReal recip = _rwInvSqrt(length2);
    out->x = in->x * recip;
    out->y = in->y * recip;
    out->z = in->z * recip;
    if (length <= 0.0f)
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x19);
        RwErrorSet(&err);
    }
    return length;
}

RwReal RwV3dLength(const RwV3d *in)
{
    return _rwSqrt(in->x * in->x + in->y * in->y + in->z * in->z);
}
