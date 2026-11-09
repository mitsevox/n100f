#include <rpworld.h>
#include <string.h>

// GX component types (GXCompType) and color formats (the GX_RGB565 to GX_RGBA8 order).
#define COMP_U8  0
#define COMP_S8  1
#define COMP_U16 2
#define COMP_S16 3
#define COMP_F32 4

#define CLR_RGB565 0
#define CLR_RGB8   1
#define CLR_RGBX8  2
#define CLR_RGBA4  3
#define CLR_RGBA6  4
#define CLR_RGBA8  5

// Bytes per component, by component type, and bytes per color, by color format.
S32 lbl_801DDA48[5] = { 1, 1, 2, 2, 4 };
S32 lbl_801DDA5C[7] = { 2, 3, 4, 2, 3, 4, 0 };

#define ALIGN32(_size) (((_size) + 0x1F) & ~0x1F)

// The vertex data size for the attributes in flags (rpGEOMETRY* flags) of numVerts vertices:
// one 32-byte aligned stream per attribute, or one interleaved stream.
S32 fn_80159380(const RpGameCubeVtxFmt* vtxFmt, U32 flags, S32 numVerts, U8 numTexCoords)
{
    S32 size = 0;

    if (!vtxFmt)
    {
        vtxFmt = &_RpDlVtxFmtDefault;
    }

    if (vtxFmt->flags & VTXFMT_INTERLEAVED)
    {
        S32 stride = 0;

        if (flags & rpGEOMETRYPOSITIONS)
        {
            stride = lbl_801DDA48[vtxFmt->compType[0]] * 3;
        }

        if (flags & rpGEOMETRYNORMALS)
        {
            stride += lbl_801DDA48[vtxFmt->compType[1]] * (vtxFmt->frac[1] ? 9 : 3);
        }

        if (flags & rpGEOMETRYPRELIT)
        {
            stride += lbl_801DDA5C[vtxFmt->compType[10]];
        }

        if (flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2))
        {
            U32 i;

            for (i = 0; (S32)i < numTexCoords; i++)
            {
                stride += lbl_801DDA48[(vtxFmt->compType + 2)[i]] * 2;
            }
        }

        size = ALIGN32(stride * numVerts);
    }
    else
    {
        if (flags & rpGEOMETRYPOSITIONS)
        {
            size = ALIGN32(numVerts * (lbl_801DDA48[vtxFmt->compType[0]] * 3));
        }

        if (flags & rpGEOMETRYNORMALS)
        {
            size += ALIGN32(numVerts * (lbl_801DDA48[vtxFmt->compType[1]] * (vtxFmt->frac[1] ? 9 : 3)));
        }

        if (flags & rpGEOMETRYPRELIT)
        {
            size += ALIGN32(numVerts * lbl_801DDA5C[vtxFmt->compType[10]]);
        }

        if (flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2))
        {
            U32 i;

            for (i = 0; (S32)i < numTexCoords; i++)
            {
                size += ALIGN32(numVerts * (lbl_801DDA48[(vtxFmt->compType + 2)[i]] * 2));
            }
        }
    }

    return size;
}

// Writes numVerts positions in the format's position type, stride bytes apart, and returns
// the bytes one vertex takes (interleaved) or the 32-byte aligned stream size.
S32 _rwGCNVtxFmtInstPos3D(const RpGameCubeVtxFmt* vtxFmt, void* dst, const RwV3d* src,
                          S32 numVerts, U8 stride)
{
    U8 compType;

    if (!vtxFmt)
    {
        vtxFmt = &_RpDlVtxFmtDefault;
    }

    compType = vtxFmt->compType[0];
    switch (compType)
    {
    case COMP_U8:
    {
        F32 scale = (F32)(1 << vtxFmt->frac[0]);
        U8* out = (U8*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = src->x * scale;
            out[1] = src->y * scale;
            out[2] = src->z * scale;
            src++;
            out += stride;
        }
        break;
    }
    case COMP_S8:
    {
        F32 scale = (F32)(1 << vtxFmt->frac[0]);
        S8* out = (S8*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = src->x * scale;
            out[1] = src->y * scale;
            out[2] = src->z * scale;
            src++;
            out += stride;
        }
        break;
    }
    case COMP_U16:
    {
        F32 scale = (F32)(1 << vtxFmt->frac[0]);
        U16* out = (U16*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = src->x * scale;
            out[1] = src->y * scale;
            out[2] = src->z * scale;
            src++;
            out = (U16*)((U8*)out + stride);
        }
        break;
    }
    case COMP_S16:
    {
        F32 scale = (F32)(1 << vtxFmt->frac[0]);
        S16* out = (S16*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = src->x * scale;
            out[1] = src->y * scale;
            out[2] = src->z * scale;
            src++;
            out = (S16*)((U8*)out + stride);
        }
        break;
    }
    case COMP_F32:
        if (vtxFmt->flags & VTXFMT_INTERLEAVED)
        {
            RwV3d* out = (RwV3d*)dst;
            S32 i;

            for (i = 0; i < numVerts; i++)
            {
                *out = *src;
                src++;
                out = (RwV3d*)((U8*)out + stride);
            }
        }
        else
        {
            memcpy(dst, src, numVerts * lbl_801DDA48[vtxFmt->compType[0]] * 3);
        }
        break;
    }

    if (vtxFmt->flags & VTXFMT_INTERLEAVED)
    {
        return lbl_801DDA48[vtxFmt->compType[0]] * 3;
    }
    return ALIGN32(numVerts * lbl_801DDA48[vtxFmt->compType[0]] * 3);
}

// Writes numVerts normals: signed bytes scaled by 64, signed shorts scaled by 16384, or floats
// (copied per vertex when interleaved or fractional, else in one block); unsigned formats write nothing.
S32 _rwGCNVtxFmtInstNrm(const RpGameCubeVtxFmt* vtxFmt, void* dst, const RwV3d* src,
                        S32 numVerts, U8 stride)
{
    U8 compType;
    S32 count;

    if (!vtxFmt)
    {
        vtxFmt = &_RpDlVtxFmtDefault;
    }

    compType = vtxFmt->compType[1];
    switch (compType)
    {
    case COMP_U8:
    case COMP_U16:
        break;
    case COMP_S8:
    {
        F32 scale = 64.0f;
        S8* out = (S8*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = src->x * scale;
            out[1] = src->y * scale;
            out[2] = src->z * scale;
            src++;
            out += stride;
        }
        break;
    }
    case COMP_S16:
    {
        F32 scale = 16384.0f;
        S16* out = (S16*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = src->x * scale;
            out[1] = src->y * scale;
            out[2] = src->z * scale;
            src++;
            out = (S16*)((U8*)out + stride);
        }
        break;
    }
    case COMP_F32:
        if ((vtxFmt->flags & VTXFMT_INTERLEAVED) || vtxFmt->frac[1])
        {
            RwV3d* out = (RwV3d*)dst;
            S32 i;

            for (i = 0; i < numVerts; i++)
            {
                *out = *src;
                src++;
                out = (RwV3d*)((U8*)out + stride);
            }
        }
        else
        {
            memcpy(dst, src, numVerts * lbl_801DDA48[vtxFmt->compType[1]] * 3);
        }
        break;
    }

    if (vtxFmt->flags & VTXFMT_INTERLEAVED)
    {
        return lbl_801DDA48[vtxFmt->compType[1]] * (vtxFmt->frac[1] ? 9 : 3);
    }

    count = 3;
    if (vtxFmt->frac[1])
    {
        count = 9;
    }
    return ALIGN32(count * (numVerts * lbl_801DDA48[vtxFmt->compType[1]]));
}

// The same from compressed normals (bytes scaled by 128).
S32 _rwGCNVtxFmtInstNrmCmp(const RpGameCubeVtxFmt* vtxFmt, void* dst, const RpVertexNormal* src,
                           S32 numVerts, U8 stride)
{
    S32 count;

    if (!vtxFmt)
    {
        vtxFmt = &_RpDlVtxFmtDefault;
    }

    switch (vtxFmt->compType[1])
    {
    case COMP_U8:
    case COMP_U16:
        break;
    case COMP_S8:
    {
        F32 scale = 64.0f;
        S8* out = (S8*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            RpVertexNormal n;

            n.x = (1.0f / 128.0f) * (F32)src->x;
            n.y = (1.0f / 128.0f) * (F32)src->y;
            n.z = (1.0f / 128.0f) * (F32)src->z;
            out[0] = n.x * scale;
            out[1] = n.y * scale;
            out[2] = n.z * scale;
            src++;
            out += stride;
        }
        break;
    }
    case COMP_S16:
    {
        F32 scale = 16384.0f;
        S16* out = (S16*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            RpVertexNormal n;

            n.x = (1.0f / 128.0f) * (F32)src->x;
            n.y = (1.0f / 128.0f) * (F32)src->y;
            n.z = (1.0f / 128.0f) * (F32)src->z;
            out[0] = n.x * scale;
            out[1] = n.y * scale;
            out[2] = n.z * scale;
            src++;
            out = (S16*)((U8*)out + stride);
        }
        break;
    }
    case COMP_F32:
    {
        S32 i;
        F32* out = (F32*)dst;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = (1.0f / 128.0f) * (F32)src->x;
            out[1] = (1.0f / 128.0f) * (F32)src->y;
            out[2] = (1.0f / 128.0f) * (F32)src->z;
            src++;
            out = (F32*)((U8*)out + stride);
        }
        break;
    }
    }

    if (vtxFmt->flags & VTXFMT_INTERLEAVED)
    {
        return lbl_801DDA48[vtxFmt->compType[1]] * (vtxFmt->frac[1] ? 9 : 3);
    }

    count = 3;
    if (vtxFmt->frac[1])
    {
        count = 9;
    }
    return ALIGN32(count * (numVerts * lbl_801DDA48[vtxFmt->compType[1]]));
}

// Writes numVerts prelight colors in the format's color format.
S32 _rwGCNVtxFmtInstClr(const RpGameCubeVtxFmt* vtxFmt, void* dst, const RwRGBA* src,
                        S32 numVerts, U8 stride)
{
    U8 compType;

    if (!vtxFmt)
    {
        vtxFmt = &_RpDlVtxFmtDefault;
    }

    compType = vtxFmt->compType[10];
    switch (compType)
    {
    case CLR_RGB565:
    {
        U16* out = (U16*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            U8 red = src->red;
            U8 green = src->green;
            U8 blue = src->blue;

            *out = ((red << 8) & 0xF800) | ((green << 3) & 0x7E0) | ((blue >> 3) & 0x1F);
            src++;
            out = (U16*)((U8*)out + stride);
        }
        break;
    }
    case CLR_RGB8:
    {
        U8* out = (U8*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = src->red;
            out[1] = src->green;
            out[2] = src->blue;
            src++;
            out += stride;
        }
        break;
    }
    case CLR_RGBA4:
    {
        U16* out = (U16*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            U8 red = src->red;
            U8 green = src->green;
            U8 blue = src->blue;

            *out = ((red << 8) & 0xF000) | ((green << 4) & 0xF00) | (blue & 0xF0) |
                   ((src->alpha >> 4) & 0xF);
            src++;
            out = (U16*)((U8*)out + stride);
        }
        break;
    }
    case CLR_RGBA6:
    {
        U8* out = (U8*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = (src->red & 0xFC) | ((src->green >> 6) & 0x3);
            out[1] = ((src->green << 2) & 0xF0) | ((src->blue >> 4) & 0xF);
            out[2] = ((src->blue << 4) & 0xC0) | ((src->alpha >> 2) & 0x3F);
            src++;
            out += stride;
        }
        break;
    }
    case CLR_RGBA8:
    case CLR_RGBX8:
        if (vtxFmt->flags & VTXFMT_INTERLEAVED)
        {
            RwRGBA* out = (RwRGBA*)dst;
            S32 i;

            for (i = 0; i < numVerts; i++)
            {
                *out = *src;
                src++;
                out = (RwRGBA*)((U8*)out + stride);
            }
        }
        else
        {
            memcpy(dst, src, numVerts * lbl_801DDA5C[vtxFmt->compType[10]]);
        }
        break;
    }

    if (vtxFmt->flags & VTXFMT_INTERLEAVED)
    {
        return lbl_801DDA5C[vtxFmt->compType[10]];
    }
    return ALIGN32(numVerts * lbl_801DDA5C[vtxFmt->compType[10]]);
}

// Writes numVerts coordinates of texture coordinate set index.
S32 _rwGCNVtxFmtInstTex(const RpGameCubeVtxFmt* vtxFmt, S32 index, void* dst,
                        const RwTexCoords* src, S32 numVerts, U8 stride)
{
    register U8 compType;
    register const U8* type;

    if (!vtxFmt)
    {
        vtxFmt = &_RpDlVtxFmtDefault;
    }

    type = vtxFmt->compType + index;
    // FAKEMATCH: inline asm loads the set's component type with the update form (lbzu), leaving type
    // at compType[index + 2]; real form believed to be type = vtxFmt->compType + index + 2;
    // compType = *type;, whose +2 GC/1.3.2's add propagation folds into the later loads
    asm { lbzu compType, 2(type) }
    switch (compType)
    {
    case COMP_U8:
    {
        F32 scale = (F32)(1 << vtxFmt->frac[index + 2]);
        U8* out = (U8*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = src->u * scale;
            out[1] = src->v * scale;
            src++;
            out += stride;
        }
        break;
    }
    case COMP_S8:
    {
        F32 scale = (F32)(1 << vtxFmt->frac[index + 2]);
        S8* out = (S8*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = src->u * scale;
            out[1] = src->v * scale;
            src++;
            out += stride;
        }
        break;
    }
    case COMP_U16:
    {
        F32 scale = (F32)(1 << vtxFmt->frac[index + 2]);
        U16* out = (U16*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = src->u * scale;
            out[1] = src->v * scale;
            src++;
            out = (U16*)((U8*)out + stride);
        }
        break;
    }
    case COMP_S16:
    {
        F32 scale = (F32)(1 << vtxFmt->frac[index + 2]);
        S16* out = (S16*)dst;
        S32 i;

        for (i = 0; i < numVerts; i++)
        {
            out[0] = src->u * scale;
            out[1] = src->v * scale;
            src++;
            out = (S16*)((U8*)out + stride);
        }
        break;
    }
    case COMP_F32:
        if (vtxFmt->flags & VTXFMT_INTERLEAVED)
        {
            RwTexCoords* out = (RwTexCoords*)dst;
            S32 i;

            for (i = 0; i < numVerts; i++)
            {
                *out = *src;
                src++;
                out = (RwTexCoords*)((U8*)out + stride);
            }
        }
        else
        {
            memcpy(dst, src, numVerts * lbl_801DDA48[compType] * 2);
        }
        break;
    }

    if (vtxFmt->flags & VTXFMT_INTERLEAVED)
    {
        return lbl_801DDA48[*type] * 2;
    }
    return ALIGN32(numVerts * lbl_801DDA48[*type] * 2);
}
