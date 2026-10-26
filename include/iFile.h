#ifndef IFILE_H
#define IFILE_H

#include "types.h"

struct tag_xFile;

#define IFILE_SEEK_SET 0

S32 iFileSeek(tag_xFile* file, S32 offset, S32 whence);
U32 iFileRead(tag_xFile* file, void* buf, U32 size);
U32 iFileClose(tag_xFile* file);

#endif
