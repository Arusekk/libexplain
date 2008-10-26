/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_BUFFER_ACCESS_MODE_H
#define LIBEXPLAIN_BUFFER_ACCESS_MODE_H

struct libexplain_string_buffer_t; /* forward */

/**
  * The libexplain_buffer_access_mode function may be used to
  * emit a human readable representation of an access mode.
  *
  * @param sb
  *     The string buffer to print into.
  * @param mode
  *     The access(2) mode to encode.
  */
void libexplain_buffer_access_mode(struct libexplain_string_buffer_t *sb,
    int mode);

/**
  * The libexplain_access_mode_parse function may be used to parse a C-like
  * expression string, into an access(2) mode argument.
  *
  * @param text
  *     The text to be parsed.
  * @returns
  *     int; open mode on success, -1 on failure
  */
int libexplain_access_mode_parse(const char *text);

#endif /* LIBEXPLAIN_BUFFER_ACCESS_MODE_H */
