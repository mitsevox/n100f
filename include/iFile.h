#ifndef IFILE_H
#define IFILE_H

#include "types.h"

#include <dolphin/dvd.h>

// Partial: the members after the DVD file info are left opaque. The GC object is 0xF0 bytes.
struct tag_iFile
{
    U32 flags;
    char path[128];
    S32 entrynum;
    DVDFileInfo fileInfo;
    U8 opaque_C4[0xF0 - 0xC4];
};

struct tag_xFile
{
    char relname[16];
    tag_iFile ps;
};

#define IFILE_SEEK_SET 0

U32 iFileOpen(const char* name, S32 flags, tag_xFile* file);
S32 iFileSeek(tag_xFile* file, S32 offset, S32 whence);
U32 iFileRead(tag_xFile* file, void* buf, U32 size);
U32 iFileClose(tag_xFile* file);

#endif
