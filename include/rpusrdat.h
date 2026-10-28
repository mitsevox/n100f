#ifndef RPUSRDAT_H
#define RPUSRDAT_H

#include <rpworld.h>

typedef enum RpUserDataFormat
{
    rpNAUSERDATAFORMAT = 0,
    rpINTUSERDATA,
    rpREALUSERDATA,
    rpSTRINGUSERDATA,
    rpUSERDATAFORCEENUMSIZEINT = 0x7FFFFFFF
} RpUserDataFormat;

typedef struct RpUserDataArray RpUserDataArray;
struct RpUserDataArray
{
    char* name;
    RpUserDataFormat format;
    S32 numElements;
    void* data;
};

#ifdef __cplusplus
extern "C" {
#endif

S32 RpGeometryAddUserDataArray(RpGeometry* geometry, char* name, RpUserDataFormat format,
                               S32 numElements);
S32 RpGeometryGetUserDataArrayCount(const RpGeometry* geometry);
RpUserDataArray* RpGeometryGetUserDataArray(const RpGeometry* geometry, S32 data);
S32 RpUserDataGetFormatSize(RpUserDataFormat format);
S32 RpUserDataPluginAttach(void);

#ifdef __cplusplus
}
#endif

#endif
