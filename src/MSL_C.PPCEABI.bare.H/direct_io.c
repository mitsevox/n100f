#include <string.h>
#include "MSL_C.PPCEABI.bare.H/ansi_files.h"
#include "MSL_C.PPCEABI.bare.H/buffer_io.h"
#include "MSL_C.PPCEABI.bare.H/char_io.h"
#include "MSL_C.PPCEABI.bare.H/direct_io.h"

size_t fread(void* buffer, size_t memb_size, size_t num_memb, FILE* file)
{
    return __fread(buffer, memb_size, num_memb, file);
}

size_t __fread(void* buffer, size_t memb_size, size_t num_memb, FILE* file)
{
    unsigned char* cur_ptr;
    size_t num_bytes;
    size_t rem_bytes;
    size_t bytes_read;
    int result;
    int buffered;

    if (fwide(file, 0) == 0)
        fwide(file, -1);

    rem_bytes = memb_size * num_memb;
    if (rem_bytes == 0 || file->state.error || file->mode.file_kind == __closed_file)
        return 0;

    buffered = !file->mode.binary_io || file->mode.buffer_mode == _IOFBF;

    if (file->state.io_state == __neutral && (file->mode.io_mode & __read)) {
        file->state.io_state = __reading;
        file->buffer_length  = 0;
    }

    if (file->state.io_state < __reading) {
        file->state.error   = 1;
        file->buffer_length = 0;
        return 0;
    }

    if ((file->mode.buffer_mode & _IOLBF) && __flush_line_buffered_output_files() != 0) {
        file->state.error   = 1;
        file->buffer_length = 0;
        return 0;
    }

    cur_ptr    = buffer;
    bytes_read = 0;

    if (rem_bytes != 0 && file->state.io_state >= __rereading) {
        do {
            if (fwide(file, 0) == 1) {
                bytes_read += 2;
                rem_bytes -= 2;
                *(wchar_t*)cur_ptr = file->ungetc_wide_buffer[file->state.io_state - __rereading];
                cur_ptr += 2;
            } else {
                bytes_read++;
                rem_bytes--;
                *cur_ptr++ = file->ungetc_buffer[file->state.io_state - __rereading];
            }

            file->state.io_state--;
        } while (rem_bytes != 0 && file->state.io_state >= __rereading);

        if (file->state.io_state == __reading)
            file->buffer_length = file->saved_buffer_length;
    }

    if (rem_bytes != 0 && (file->buffer_length != 0 || buffered)) {
        do {
            if (file->buffer_length == 0) {
                result = __load_buffer(file, 0, 0);
                if (result != 0) {
                    if (result == __io_error) {
                        file->state.error   = 1;
                        file->buffer_length = 0;
                    } else {
                        file->state.io_state = __neutral;
                        file->state.eof      = 1;
                        file->buffer_length  = 0;
                    }
                    rem_bytes = 0;
                    break;
                }
            }

            num_bytes = file->buffer_length;
            if (num_bytes > rem_bytes)
                num_bytes = rem_bytes;

            memcpy(cur_ptr, file->buffer_ptr, num_bytes);
            rem_bytes -= num_bytes;
            cur_ptr += num_bytes;
            bytes_read += num_bytes;
            file->buffer_ptr += num_bytes;
            file->buffer_length -= num_bytes;
        } while (rem_bytes != 0 && buffered);
    }

    if (rem_bytes != 0 && !buffered) {
        unsigned char* saved_buffer = file->buffer;
        size_t saved_size = file->buffer_size;

        file->buffer      = cur_ptr;
        file->buffer_size = rem_bytes;

        result = __load_buffer(file, &num_bytes, 1);
        if (result != 0) {
            if (result == __io_error) {
                file->state.error   = 1;
                file->buffer_length = 0;
            } else {
                file->state.io_state = __neutral;
                file->state.eof      = 1;
                file->buffer_length  = 0;
            }
        }

        bytes_read += num_bytes;
        file->buffer      = saved_buffer;
        file->buffer_size = saved_size;
        __prep_buffer(file);
        file->buffer_length = 0;
    }

    return bytes_read / memb_size;
}

size_t fwrite(const void* buffer, size_t memb_size, size_t num_memb, FILE* file)
{
    return __fwrite(buffer, memb_size, num_memb, file);
}

size_t __fwrite(const void* buffer, size_t memb_size, size_t num_memb, FILE* file)
{
    unsigned char* cur_ptr;
    size_t num_bytes;
    size_t rem_bytes;
    size_t bytes_written;
    int result;
    int buffered;

    if (fwide(file, 0) == 0)
        fwide(file, -1);

    rem_bytes = memb_size * num_memb;

    if (rem_bytes == 0 || file->state.error || file->mode.file_kind == __closed_file)
        return 0;

    if (file->mode.file_kind == __console_file)
        __stdio_atexit();

    buffered = !file->mode.binary_io || file->mode.buffer_mode == _IOFBF || file->mode.buffer_mode == _IOLBF;

    if (file->state.io_state == __neutral && (file->mode.io_mode & __write)) {
        if (file->mode.io_mode & __append) {
            if (fseek(file, 0, SEEK_END) != 0)
                return 0;
        }
        file->state.io_state = __writing;
        __prep_buffer(file);
    }

    if (file->state.io_state != __writing) {
        file->state.error   = 1;
        file->buffer_length = 0;
        return 0;
    }

    cur_ptr       = (unsigned char*)buffer;
    bytes_written = 0;

    if (rem_bytes != 0 && (file->buffer_ptr != file->buffer || buffered)) {
        file->buffer_length = file->buffer_size - (file->buffer_ptr - file->buffer);

        do {
            unsigned char* newline = 0;

            num_bytes = file->buffer_length;
            if (num_bytes > rem_bytes)
                num_bytes = rem_bytes;

            if (file->mode.buffer_mode == _IOLBF && num_bytes != 0) {
                newline = __memrchr(cur_ptr, '\n', num_bytes);
                if (newline != 0)
                    num_bytes = newline + 1 - cur_ptr;
            }

            if (num_bytes != 0) {
                memcpy(file->buffer_ptr, cur_ptr, num_bytes);
                cur_ptr += num_bytes;
                bytes_written += num_bytes;
                rem_bytes -= num_bytes;
                file->buffer_ptr += num_bytes;
                file->buffer_length -= num_bytes;
            }

            if (file->buffer_length == 0 || newline != 0 || file->mode.buffer_mode == _IONBF) {
                result = __flush_buffer(file, 0);
                if (result != 0) {
                    file->state.error   = 1;
                    file->buffer_length = 0;
                    rem_bytes           = 0;
                    break;
                }
            }
        } while (rem_bytes != 0 && buffered);
    }

    if (rem_bytes != 0 && !buffered) {
        unsigned char* saved_buffer = file->buffer;
        size_t saved_size = file->buffer_size;

        file->buffer      = cur_ptr;
        file->buffer_size = rem_bytes;
        file->buffer_ptr  = cur_ptr + rem_bytes;

        if (__flush_buffer(file, &num_bytes) != 0) {
            file->state.error   = 1;
            file->buffer_length = 0;
        }

        bytes_written += num_bytes;
        file->buffer      = saved_buffer;
        file->buffer_size = saved_size;
        __prep_buffer(file);
        file->buffer_length = 0;
    }

    if (file->mode.buffer_mode != _IOFBF)
        file->buffer_length = 0;

    return (bytes_written + memb_size - 1) / memb_size;
}
