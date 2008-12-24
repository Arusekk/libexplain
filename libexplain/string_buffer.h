/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_STRING_BUFFER_H
#define LIBEXPLAIN_STRING_BUFFER_H

#include <libexplain/ac/stdarg.h>
#include <libexplain/ac/stddef.h>

#include <libexplain/gcc_attributes.h>

#ifndef i18n
#define i18n(x) x
#endif

/**
  * The libexplain_string_buffer_t struct is used to represent a piece
  * of memory which is building a C string.
  * This does not require the use of dynamic memory.
  */
typedef struct libexplain_string_buffer_t libexplain_string_buffer_t;
struct libexplain_string_buffer_t
{
    char *message;
    size_t position;
    size_t maximum;
};

/**
  * The libexplain_string_buffer_init function may be used to prespare a
  * string buffer for use in building a C string.
  *
  * @param sb
  *    The string buffer to be initialised.
  * @param message
  *    The array which is to contain the string being constructed by the
  *    string buffer.
  * @param message_size
  *    The size (in bytes) of the array which is to contain the string
  *    being constructed by the string buffer.
  */
void libexplain_string_buffer_init(libexplain_string_buffer_t *sb,
    char *message, int message_size);

void libexplain_string_buffer_putc(libexplain_string_buffer_t *sb, int c);

void libexplain_string_buffer_puts(libexplain_string_buffer_t *sb,
    const char *s);

void libexplain_string_buffer_puts_quoted(libexplain_string_buffer_t *sb,
    const char *s);

/**
  * The libexplain_string_buffer_printf function is used to print a
  * formatted string into a string buffer.
  *
  * @param sb
  *     The string buffer to print into
  * @param fmt
  *     The format string controling the types of the remaining arguments.
  *     See printf(3) for more information.
  */
void libexplain_string_buffer_printf(libexplain_string_buffer_t *sb,
    const char *fmt, ...)
                                                 LIBEXPLAIN_FORMAT_PRINTF(2, 3);

/**
  * The libexplain_string_buffer_printf function is used to print a
  * formatted string into a string buffer, translating the formatting text.
  *
  * @param sb
  *     The string buffer to print into
  * @param fmt
  *     The format string controling the types of the remaining
  *     arguments.  See printf(3) for more information on the content of
  *     this string.  It will be passed through libexplain_gettext for
  *     translation before it is used.
  */
void libexplain_string_buffer_printf_gettext(libexplain_string_buffer_t *sb,
    const char *fmt, ...)
                                                 LIBEXPLAIN_FORMAT_PRINTF(2, 3);

void libexplain_string_buffer_vprintf(libexplain_string_buffer_t *sb,
    const char *fmt, va_list ap);

void libexplain_string_buffer_copy(libexplain_string_buffer_t *dst,
    const libexplain_string_buffer_t *src);

void libexplain_string_buffer_path_join(libexplain_string_buffer_t *sb,
    const char *s);

/**
  * The libexplain_string_buffer_full functionmay be used to determine
  * whether or not a string buffer is full (has no more room for
  * additional characters).
  *
  * @param sb
  *     The string buffer to test.
  */
int libexplain_string_buffer_full(const libexplain_string_buffer_t *sb);

/**
  * The libexplain_string_buffer_write function may be used to append
  * a run of bytes to the end of a string buffer.  They do not need to
  * be NUL terminated; indeed, they shall <b>not</b> contain any NUL
  * characters.
  *
  * @param sb
  *     The string buffer to print into
  * @param data
  *     Pointer to the base of the array of characters to be written
  *     into the string buffer.
  * @param data_size
  *     The size in bytes (printable characters could be multi-byte
  *     sequences) the size in bytes of the array of bytes to write into
  *     the string buffer.
  */
void libexplain_string_buffer_write(libexplain_string_buffer_t *sb,
    const char *data, size_t data_size);

/**
  * The libexplain_string_buffer_truncate function is used to trim a
  * string buffer back to the given size.
  *
  * @param sb
  *     The string buffer to shorten
  * @param new_position
  *     The newer shorter position (ignored of longer)
  */
void libexplain_string_buffer_truncate(libexplain_string_buffer_t *sb,
    long new_position);

#endif /* LIBEXPLAIN_STRING_BUFFER_H */
