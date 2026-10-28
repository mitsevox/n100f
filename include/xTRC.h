#ifndef XTRC_H
#define XTRC_H

#include "types.h"

// The GC values, which add the disk states to the PS2 enum.
enum _tagTRCState
{
    TRC_Unknown,
    TRC_PadMissing,
    TRC_PadInserted,
    TRC_PadInvalidNoAnalog,
    TRC_PadInvalidType,
    TRC_DiskNotIdentified,
    TRC_DiskIdentified,
    TRC_DiskTrayOpen,
    TRC_DiskTrayClosed,
    TRC_DiskNoDisk,
    TRC_DiskInvalid,
    TRC_DiskRetry,
    TRC_DiskFatal,
    TRC_Total
};

extern _tagTRCState gTrcDisk[2];

#endif
