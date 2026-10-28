#include <errno.h>
#include "MSL_C.PPCEABI.bare.H/ansi_files.h"
#include "MSL_C.PPCEABI.bare.H/buffer_io.h"
#include "MSL_C.PPCEABI.bare.H/char_io.h"

int _fseek(FILE* file, long offset, int whence);

static inline long tell_file(FILE* file)
{
    int chars_in_undo_buffer = 0;
    long position;
    unsigned char file_kind = file->mode.file_kind;

    if (!((file_kind == __disk_file) || (file_kind == __console_file)) || file->state.error) {
        errno = EFPOS;
        return -1;
    }

    if (file->state.io_state == __neutral)
        return file->position;

    position = file->buffer_position + (file->buffer_ptr - file->buffer);

    if (file->state.io_state >= __rereading) {
        chars_in_undo_buffer = file->state.io_state - __rereading + 1;
        position -= chars_in_undo_buffer;
    }

    return position;
}

long ftell(FILE* file)
{
    return tell_file(file);
}

int _fseek(FILE* file, long offset, int whence)
{
    __pos_proc position_proc;
    unsigned char file_kind = file->mode.file_kind;

    if (file_kind != __disk_file || file->state.error != 0) {
        errno = EFPOS;
        return -1;
    }

    if (file->state.io_state == __writing) {
        if (__flush_buffer(file, 0) != 0) {
            file->state.error   = 1;
            file->buffer_length = 0;
            errno               = EFPOS;
            return -1;
        }
    }

    if (whence == SEEK_CUR) {
        whence = SEEK_SET;
        offset += tell_file(file);
    }

    if (whence != SEEK_END && file->mode.io_mode != __read_write
        && (file->state.io_state == __reading || file->state.io_state == __rereading)) {
        if (offset >= file->position || offset < file->buffer_position) {
            file->state.io_state = __neutral;
        } else {
            file->buffer_ptr     = file->buffer + (offset - file->buffer_position);
            file->buffer_length  = file->position - offset;
            file->state.io_state = __reading;
        }
    } else {
        file->state.io_state = __neutral;
    }

    if (file->state.io_state == __neutral) {
        position_proc = file->position_proc;
        if (position_proc != 0
            && position_proc(file->handle, (unsigned long*)&offset, whence, file->idle_proc) != 0) {
            file->state.error   = 1;
            file->buffer_length = 0;
            errno               = EFPOS;
            return -1;
        }

        file->state.eof     = 0;
        file->position      = offset;
        file->buffer_length = 0;
    }

    return 0;
}

int fseek(FILE* file, long offset, int whence)
{
    return _fseek(file, offset, whence);
}
