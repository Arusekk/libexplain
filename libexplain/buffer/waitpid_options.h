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

#ifndef LIBEXPLAIN_BUFFER_WAITPID_OPTIONS_H
#define LIBEXPLAIN_BUFFER_WAITPID_OPTIONS_H

struct libexplain_string_buffer_t; /* forward */

/**
  * The libexplain_buffer_waitpid_options function may be used to
  * decode and print waitpid(2) options.
  *
  * @param sb
  *     The string buffer to print into
  * @param options
  *     The options to be decoded
  */
void libexplain_buffer_waitpid_options(struct libexplain_string_buffer_t *sb,
    int options);

/**
  * The libexplain_parse_waitpid_options function is used to parse text
  * containing waitpid options into a numeric value.
  *
  * @param text
  *     The string to be parsed
  * @returns
  *     -1 on error, or the flags value if successful
  */
int libexplain_parse_waitpid_options(const char *text);

#endif /* LIBEXPLAIN_BUFFER_WAITPID_OPTIONS_H */
