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

#ifndef LIBEXPLAIN_PERMISSION_MODE_H
#define LIBEXPLAIN_PERMISSION_MODE_H

#include <libexplain/ac/stddef.h>

struct libexplain_string_buffer_t; /* forward */

/**
  * The libexplain_buffer_permission_mode function may be used to decode
  * a set of permission mode bits into human readable text, something
  * like the programmer would have written.
  *
  * @param sb
  *    The string buffer to write the mode into.
  *    This makes the function thread safe if the buffer is suitable.
  * @param mode
  *    the permission mode bits to be decoded
  */
void libexplain_buffer_permission_mode(struct libexplain_string_buffer_t *sb,
    int mode);

/**
  * The libexplain_message_permission_mode function may be used to
  * decode a set of permission mode bits into human readable text,
  * something like the programmer would have written.
  *
  * @param message
  *    The character array to write the mode into.
  *    This makes the function thread safe if the array is suitable.
  * @param message_size
  *    The size in bytes of the character array to write the mode into.
  * @param mode
  *    the permission mode bits to be decoded
  */
void libexplain_message_permission_mode(char *message, size_t message_size,
    int mode);

/**
  * The libexplain_permission_mode function may be used to decode a set
  * of permission mode bits into human readable text, something like the
  * programmer would have written.
  *
  * @param mode
  *    the permission mode bits to be decoded
  * @return
  *     pointer to string in shared buffer, it will be overwritten by
  *     subsequent calls to this function.
  * @note
  *    this function is not thread safe.
  */
const char *libexplain_permission_mode(int mode);

/**
  * The libexplain_permission_mode_parse function may be used to parse a
  * strings containing a symbolic representation of a permission mode,
  * turning it into a permission mode value.
  *
  * @param text
  *     The text to be parsed to extract a permission mode value.
  * @returns
  *     permission mode value
  * @note
  *     If there is a parse error, a fatal error message is printed, and
  *     exit(1) is called.  If there is an error, this function will not
  *     return.
  */
int libexplain_permission_mode_parse(const char *text);

#endif /* LIBEXPLAIN_PERMISSION_MODE_H */
