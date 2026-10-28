#include <stdlib.h>
#include "MSL_C.PPCEABI.bare.H/ansi_files.h"
#include "MSL_C.PPCEABI.bare.H/buffer_io.h"

enum { __align_buffer, __dont_align_buffer };

static inline void prep_buffer(FILE* file)
{
    file->buffer_ptr    = file->buffer;
    file->buffer_length = file->buffer_size;
    file->buffer_length -= file->position & file->buffer_alignment;
    file->buffer_position = file->position;
}

void __prep_buffer(FILE* file)
{
    prep_buffer(file);
}

int __load_buffer(FILE* file, size_t* bytes_loaded, int alignment)
{
    int result;

    prep_buffer(file);
    if (alignment == __dont_align_buffer)
        file->buffer_length = file->buffer_size;

    result = file->read_proc(file->handle, file->buffer, &file->buffer_length, file->idle_proc);
    if (result == __io_EOF)
        file->buffer_length = 0;

    if (bytes_loaded != 0)
        *bytes_loaded = file->buffer_length;

    if (result != 0)
        return result;

    file->position += file->buffer_length;
    return __no_io_error;
}

int __flush_buffer(FILE* file, size_t* bytes_flushed)
{
    size_t buffer_length;
    int result;

    buffer_length = file->buffer_ptr - file->buffer;
    if (buffer_length != 0) {
        file->buffer_length = buffer_length;
        result
            = file->write_proc(file->handle, file->buffer, &file->buffer_length, file->idle_proc);

        if (bytes_flushed != 0)
            *bytes_flushed = file->buffer_length;

        if (result != 0)
            return result;

        file->position += file->buffer_length;
    }

    prep_buffer(file);
    return __no_io_error;
}

int setvbuf(FILE* file, char* buffer, int mode, size_t size)
{
    int file_kind = file->mode.file_kind;

    if (mode == _IONBF)
        fflush(file);

    if (file->state.io_state != __neutral || file_kind == 0)
        return -1;

    if (mode != _IONBF && mode != _IOLBF && mode != _IOFBF)
        return -1;

    if (file->buffer != 0 && file->state.free_buffer)
        free(file->buffer);

    file->mode.buffer_mode  = mode;
    file->state.free_buffer = 0;
    file->buffer            = (unsigned char*)&file->char_buffer;
    file->buffer_ptr        = (unsigned char*)&file->char_buffer;
    file->buffer_size       = 1;
    file->buffer_length     = 0;
    file->buffer_alignment  = 0;

    if (mode == _IONBF || size < 1) {
        *file->buffer_ptr = 0;
        return 0;
    }

    if (buffer == 0) {
        buffer = malloc(size);
        if (buffer == 0)
            return -1;
        file->state.free_buffer = 1;
    }

    file->buffer           = (unsigned char*)buffer;
    file->buffer_ptr       = file->buffer;
    file->buffer_size      = size;
    file->buffer_alignment = 0;
    return 0;
}
