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

#ifndef LIBEXPLAIN_BUFFER_SOCKET_DOMAIN_H
#define LIBEXPLAIN_BUFFER_SOCKET_DOMAIN_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_socket_domain function may be used to print a
  * symbolic value of a socket domain value to the given string buffer.
  *
  * @param sb
  *     The string buffer to print into.
  * @param domain
  *     The socket domain value to interpret.
  */
void libexplain_buffer_socket_domain(libexplain_string_buffer_t *sb,
    int domain);

/**
  * The libexplain_parse_socket_domain function is used to parse a
  * string into a socket domain value.
  *
  * @param text
  *     The string to be parsed.
  * @returns
  *     the socket value, or -1 on error
  */
int libexplain_parse_socket_domain(const char *text);

#endif /* LIBEXPLAIN_BUFFER_SOCKET_DOMAIN_H */
