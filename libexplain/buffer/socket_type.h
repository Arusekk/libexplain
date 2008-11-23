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

#ifndef LIBEXPLAIN_BUFFER_SOCKET_TYPE_H
#define LIBEXPLAIN_BUFFER_SOCKET_TYPE_H

struct libexplain_string_buffer_t; /* forward */

/**
  * The libexplain_buffer_socket_type function may be used to print a
  * symbolic representation of a socket type.
  *
  * @param sb
  *     The string buffer to print into.
  * @param type
  *     The socket type to decipher.
  */
void libexplain_buffer_socket_type(struct libexplain_string_buffer_t *sb,
    int type);

/**
  * The libexplain_parse_socket_type function may be used to parse a
  * string into a socket type value.
  *
  * @param text
  *     The string to parse.
  * @returns
  *     the socket type, or -1 on error
  */
int libexplain_parse_socket_type(const char *text);

#endif /* LIBEXPLAIN_BUFFER_SOCKET_TYPE_H */
