/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_EBADF_H
#define LIBEXPLAIN_BUFFER_EBADF_H

#include <libexplain/string_buffer.h>

/**
  * The explain_buffer_ebadf function may be used to print a generic
  * EBADF explanation into the string buffer.
  *
  * @param sb
  *    The string buffer to print into
  * @param fildes
  *    The value of the manky file descriptor
  * @param caption
  *    the argument name to whinge about
  */
void explain_buffer_ebadf(explain_string_buffer_t *sb, int fildes,
    const char *caption);

/**
  * The explain_buffer_ebadf_stream function may be used to print a
  * generic EBADF explanation into the string buffer.
  *
  * @param sb
  *    The string buffer to print into
  * @param caption
  *    the argument name to whinge about
  */
void explain_buffer_ebadf_stream(explain_string_buffer_t *sb,
    const char *caption);

/**
  * The explain_buffer_ebadf_dir function may be used to print a generic
  * EBADF or EINVAL explanation into the string buffer, where a DIR*
  * argument does not refer to a valid directory stream.
  *
  * @param sb
  *    The string buffer to print into
  * @param caption
  *    the argument name to whinge about
  */
void explain_buffer_ebadf_dir(explain_string_buffer_t *sb,
    const char *caption);

#endif /* LIBEXPLAIN_BUFFER_EBADF_H */
