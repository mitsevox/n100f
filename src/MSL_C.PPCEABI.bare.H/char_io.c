#include "MSL_C.PPCEABI.bare.H/ansi_files.h"
#include "MSL_C.PPCEABI.bare.H/buffer_io.h"
#include "MSL_C.PPCEABI.bare.H/char_io.h"

int fgetc(FILE* file)
{
    int io_state;
    int result;

    file->buffer_length = 0;
    if (file->state.error || file->mode.file_kind == __closed_file)
        return -1;

    io_state = file->state.io_state;
    if (io_state == __writing || !(file->mode.io_mode & __read)) {
        file->state.error   = 1;
        file->buffer_length = 0;
        return -1;
    }

    if (io_state >= __rereading) {
        file->state.io_state = io_state - 1;
        if (io_state == __rereading)
            file->buffer_length = file->saved_buffer_length;
        return (unsigned char)file->ungetc_buffer[io_state - __rereading];
    }

    file->state.io_state = __reading;
    result               = __load_buffer(file, 0, 0);
    if (result != __no_io_error || file->buffer_length == 0) {
        if (result == __io_error) {
            file->state.error   = 1;
            file->buffer_length = 0;
        } else {
            file->state.io_state = __neutral;
            file->state.eof      = 1;
            file->buffer_length  = 0;
        }
        return -1;
    }

    file->buffer_length--;
    return *file->buffer_ptr++;
}

char* fgets(char* string, int count, FILE* file)
{
    char* current;
    int character;

    current = string;
    count -= 1;

    if (count < 0)
        return 0;

    if (count != 0) {
        do {
            if (fwide(file, -1) >= 0)
                character = -1;
            else if (file->buffer_length-- != 0) {
                character = *file->buffer_ptr++;
            } else {
                character = fgetc(file);
            }

            if (character == -1) {
                if (!file->state.eof || current == string)
                    return 0;
                break;
            }

            *current++ = character;
            if (character == '\n')
                break;
        } while (--count != 0);
    }

    *current = 0;
    return string;
}

int fputc(int character, FILE* file)
{
    int file_kind = file->mode.file_kind;

    file->buffer_length = 0;
    if (file->state.error || file_kind == __closed_file)
        return -1;

    if (file_kind == __console_file)
        __stdio_atexit();

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
        return -1;
    }

    if (file->mode.buffer_mode == _IOFBF
        || file->buffer_size == (size_t)(file->buffer_ptr - file->buffer)) {
        if (__flush_buffer(file, 0) != 0) {
            file->state.error   = 1;
            file->buffer_length = 0;
            return -1;
        }
    }

    file->buffer_length--;
    *file->buffer_ptr++ = character;

    if (file->mode.buffer_mode != _IOFBF) {
        if (file->mode.buffer_mode == _IONBF || character == '\n') {
            if (__flush_buffer(file, 0) != 0) {
                file->state.error   = 1;
                file->buffer_length = 0;
                return -1;
            }
        }

        file->buffer_length = 0;
    }

    return (unsigned char)character;
}

int fputs(const char* string, FILE* file)
{
    signed char character;
    int result = 0;

    while ((character = *string++) != 0) {
        if ((fwide(file, -1) >= 0                ? -1
                    : file->buffer_length-- != 0 ? * file->buffer_ptr++ = character
                                                 : fputc(character, file))
            == -1) {
            result = -1;
            break;
        }
    }

    return result;
}
