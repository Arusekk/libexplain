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

#ifndef LIBEXPLAIN_BUFFER_GID_H
#define LIBEXPLAIN_BUFFER_GID_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_gid function may be used to
  * insert a group ID into the given buffer.
  *
  * @param sb
  *    The string buffer to print into
  * @param gid
  *    The goup ID to print.
  */
void libexplain_buffer_gid(libexplain_string_buffer_t *sb, int gid);

/**
  * The libexplain_buffer_gid_supplementary function is used to print a
  * list of the processes supplementary groups.
  *
  * @param sb
  *     The string buffer to print into.
  */
void libexplain_buffer_gid_supplementary(struct libexplain_string_buffer_t *sb);

#endif /* LIBEXPLAIN_BUFFER_GID_H */
