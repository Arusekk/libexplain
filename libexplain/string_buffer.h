/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

void libexplain_string_buffer_printf(libexplain_string_buffer_t *sb,
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

#endif /* LIBEXPLAIN_STRING_BUFFER_H */
