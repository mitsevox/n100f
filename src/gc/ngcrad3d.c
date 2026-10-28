#include <rad3d.h>

#include <dolphin/gx.h>
#include <dolphin/os.h>

struct RAD3DIMAGE
{
    U32 width;
    U32 height;
    S32 alpha_pixels;
    U32 bytes_per_pixel;
    U32 rad3d_surface_format;
    void* pixels;
    U32 pixels_size;
    GXTexObj texture;
};

extern U8* FMVWorkBuffer;

static S32 D3D_surface_type[5];
static U32 Pixel_info[5];

static S32 Built_tables;

static void Setup_surface_array()
{
    if (Built_tables)
    {
        return;
    }

    D3D_surface_type[0] = GX_TF_RGBA8;
    D3D_surface_type[1] = GX_TF_RGBA8;
    D3D_surface_type[2] = GX_TF_RGB565;
    D3D_surface_type[3] = GX_TF_RGB5A3;
    D3D_surface_type[4] = -1;

    Pixel_info[0] = 0x00000004;
    Pixel_info[1] = 0x80000004;
    Pixel_info[2] = 0x00000002;
    Pixel_info[3] = 0x80000002;
    Pixel_info[4] = 0x00000002;

    Built_tables = 1;
}

HRAD3DIMAGE Open_RAD_3D_image(HRAD3D rad_3d, U32 width, U32 height, U32 rad3d_surface_format)
{
    HRAD3DIMAGE rad_image;
    U32 bytes_per_pixel;

    Setup_surface_array();

    rad_image = (HRAD3DIMAGE)FMVWorkBuffer;
    bytes_per_pixel = Pixel_info[rad3d_surface_format] & 0xFF;
    FMVWorkBuffer += sizeof(struct RAD3DIMAGE);
    OSReport("### Using %d bytes from FMV work buffer\n", sizeof(struct RAD3DIMAGE));

    if (rad_image == NULL)
    {
        return NULL;
    }

    rad_image->width = width;
    rad_image->height = height;
    rad_image->alpha_pixels = (Pixel_info[rad3d_surface_format] & 0x80000000) ? 1 : 0;
    rad_image->bytes_per_pixel = bytes_per_pixel;
    rad_image->rad3d_surface_format = rad3d_surface_format;
    rad_image->pixels_size =
        GXGetTexBufferSize(width, height, D3D_surface_type[rad3d_surface_format], GX_FALSE, 0);

    FMVWorkBuffer = (U8*)(((U32)FMVWorkBuffer + 31) & ~31);
    rad_image->pixels = FMVWorkBuffer;
    FMVWorkBuffer += rad_image->pixels_size;
    OSReport("### Using %d bytes from FMV work buffer, %08X - %08X\n", rad_image->pixels_size,
             FMVWorkBuffer - rad_image->pixels_size, FMVWorkBuffer - 1);

    GXInitTexObj(&rad_image->texture, rad_image->pixels, width, height,
                 (GXTexFmt)D3D_surface_type[rad3d_surface_format], GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&rad_image->texture, GX_LINEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE,
                    GX_ANISO_1);

    return rad_image;
}

void Close_RAD_3D_image(HRAD3DIMAGE rad_image)
{
}

S32 Lock_RAD_3D_image(HRAD3DIMAGE rad_image, void* out_pixel_buffer, U32* out_buffer_pitch,
                      U32* out_surface_type)
{
    if (rad_image == NULL)
    {
        return 0;
    }

    if (out_pixel_buffer != NULL)
    {
        *(void**)out_pixel_buffer = rad_image->pixels;
    }

    if (out_buffer_pitch != NULL)
    {
        *out_buffer_pitch = rad_image->width * rad_image->bytes_per_pixel;
    }

    if (out_surface_type != NULL)
    {
        *out_surface_type = rad_image->rad3d_surface_format;
    }

    return 1;
}

void Unlock_RAD_3D_image(HRAD3DIMAGE rad_image)
{
    if (rad_image != NULL)
    {
        DCStoreRange(rad_image->pixels, rad_image->pixels_size);
    }
}

static void Submit_vertices(F32 dest_x, F32 dest_y, F32 scale_x, F32 scale_y, S32 width,
                            S32 height, F32 alpha);

void Blit_RAD_3D_image(HRAD3DIMAGE rad_image, F32 x_offset, F32 y_offset, F32 x_scale,
                       F32 y_scale, F32 alpha_level)
{
    if (rad_image == NULL)
    {
        return;
    }

    if (alpha_level >= 0.998f)
    {
        if (rad_image->alpha_pixels == 0)
        {
            GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        }
        else
        {
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_ZERO);
        }
    }
    else
    {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        if (rad_image->alpha_pixels == 0)
        {
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_KONST, GX_CA_ZERO);
        }
        else
        {
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        }
    }

    GXLoadTexObj(&rad_image->texture, GX_TEXMAP0);

    Submit_vertices(x_offset, y_offset, x_scale, y_scale, rad_image->width, rad_image->height,
                    255.0f * alpha_level);

    GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
}

static void Submit_vertices(F32 dest_x, F32 dest_y, F32 scale_x, F32 scale_y, S32 width,
                            S32 height, F32 alpha)
{
    S32 x1;
    S32 y1;

    x1 = width * scale_x + dest_x;
    y1 = height * scale_y + dest_y;

    GXSetCullMode(GX_CULL_NONE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition3s16((S32)dest_x, y1, 0);
    GXColor4u8(255, 255, 255, (S32)alpha);
    GXTexCoord2f32(0.0f, 1.0f);

    GXPosition3s16((S32)dest_x, (S32)dest_y, 0);
    GXColor4u8(255, 255, 255, (S32)alpha);
    GXTexCoord2f32(0.0f, 0.0f);

    GXPosition3s16(x1, (S32)dest_y, 0);
    GXColor4u8(255, 255, 255, (S32)alpha);
    GXTexCoord2f32(1.0f, 0.0f);

    GXPosition3s16(x1, y1, 0);
    GXColor4u8(255, 255, 255, (S32)alpha);
    GXTexCoord2f32(1.0f, 1.0f);

    GXEnd();
}
