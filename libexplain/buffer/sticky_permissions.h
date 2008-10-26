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

#ifndef LIBEXPLAIN_BUFFER_STICKY_PERMISSIONS_H
#define LIBEXPLAIN_BUFFER_STICKY_PERMISSIONS_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_sticky_permissions function may be used to
  * check for sticky bit permissions screw-ups, and then prints the
  * problem onto the given string buffer.
  *
  * @param sb
  *    The string buffer to print into.
  * @param pathname
  *    The path of the file being removed.
  * @param caption
  *    The name of the augment passed as pathname
  * @returns
  *    int; -1 if it printed nothing, 0 if it printed something.
  *
  * @note
  *    FIXME: this function has the wrong name,
  *    it should be libexplain_buffer_unlink_permissions
  */
int libexplain_buffer_sticky_permissions(libexplain_string_buffer_t *sb,
    const char *pathname, const char *caption);

#endif /* LIBEXPLAIN_BUFFER_STICKY_PERMISSIONS_H */
