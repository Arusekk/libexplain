/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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
  * The libexplain_buffer_socket_type_from_fildes function may be used
  * to supplement an error explanation with the type of a socet, taken
  * from the file descriptor.  Nothing is printed if thr socket type
  * cannot be determined.
  *
  * @param sb
  *     The string buffer to print into.
  * @param fildes
  *     the file descriptor to extract the socket type from and then print it
  */
void libexplain_buffer_socket_type_from_fildes(
    struct libexplain_string_buffer_t *sb, int fildes);

/**
  * The libexplain_parse_socket_type_or_die function may be used to
  * parse a string into a socket type value.  On error, prints a
  * diagnostic and exits EXIT_FAILURE.
  *
  * @param text
  *     The string to parse.
  * @param caption
  *     addition text to add to start of error message
  * @returns
  *     the socket type
  */
int libexplain_parse_socket_type_or_die(const char *text, const char *caption);

#endif /* LIBEXPLAIN_BUFFER_SOCKET_TYPE_H */
