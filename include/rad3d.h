#ifndef RAD3D_H
#define RAD3D_H

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int HRAD3D;

typedef struct RAD3DIMAGE* HRAD3DIMAGE;

HRAD3DIMAGE Open_RAD_3D_image(HRAD3D rad_3d, U32 width, U32 height, U32 rad3d_surface_format);
void Close_RAD_3D_image(HRAD3DIMAGE rad_image);
S32 Lock_RAD_3D_image(HRAD3DIMAGE rad_image, void* out_pixel_buffer, U32* out_buffer_pitch,
                      U32* out_surface_type);
void Unlock_RAD_3D_image(HRAD3DIMAGE rad_image);
void Blit_RAD_3D_image(HRAD3DIMAGE rad_image, F32 x_offset, F32 y_offset, F32 x_scale,
                       F32 y_scale, F32 alpha_level);

#ifdef __cplusplus
}
#endif

#endif
