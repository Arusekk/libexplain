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

#ifndef LIBEXPLAIN_BUFFER_LSEEK_WHENCE_H
#define LIBEXPLAIN_BUFFER_LSEEK_WHENCE_H

struct libexplain_string_buffer_t; /* forward */

/**
  * The libexplain_buffer_lseek_whence function may be used to form a
  * human readable representation of an lseek when value.
  *
  * @param sb
  *    the string buffer in which to write the whens symbol
  * @param whence
  *    the value to be decoded
  */
void libexplain_buffer_lseek_whence(struct libexplain_string_buffer_t *sb,
    int whence);

/**
  * The libexplain_lseek_whence_parse function may be used to parse
  * a text string into an lseek whence value.  It may be symbolic or
  * numeric.
  *
  * @param text
  *     The text string to be parsed.
  * @returns
  *     int, an lseek whence value, or -1 on error
  */
int libexplain_lseek_whence_parse(const char *text);

#endif /* LIBEXPLAIN_BUFFER_LSEEK_WHENCE_H */
