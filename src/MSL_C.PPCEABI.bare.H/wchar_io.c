#include "MSL_C.PPCEABI.bare.H/ansi_files.h"

int fwide(FILE* file, int mode)
{
    if (!file || file->mode.file_kind == __closed_file)
        return 0;

    switch (file->mode.file_orientation) {
    case __unoriented:
        if (mode > 0) {
            file->mode.file_orientation = __wide_oriented;
        } else if (mode < 0) {
            file->mode.file_orientation = __char_oriented;
        }
        return mode;
    case __wide_oriented:
        return 1;
    case __char_oriented:
        return -1;
    }
}
