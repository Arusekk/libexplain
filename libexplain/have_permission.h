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

#ifndef LIBEXPLAIN_HAVE_PERMISSION_H
#define LIBEXPLAIN_HAVE_PERMISSION_H

struct stat; /* forward */

/**
  * The libexplain_have_read_permission function may be used to test
  * whether or not the current process has read permissions on an inode.
  *
  * @param st
  *    stat structure containing information about the file.
  * @returns
  *    int; nonzero(true) if have permission, zero (false) if not.
  */
int libexplain_have_read_permission(const struct stat *st);

/**
  * The libexplain_have_write_permission function may be used to test
  * whether or not the current process has write permissions on an
  * inode.
  *
  * @param st
  *    stat structure containing information about the file.
  * @returns
  *    int; nonzero(true) if have permission, zero (false) if not.
  */
int libexplain_have_write_permission(const struct stat *st);

/**
  * The libexplain_have_execute_permission function may be used to test
  * whether or not the current process has execute permissions on an
  * inode.
  *
  * @param st
  *    stat structure containing information about the file.
  * @returns
  *    int; nonzero(true) if have permission, zero (false) if not.
  */
int libexplain_have_execute_permission(const struct stat *st);

/**
  * The libexplain_have_search_permission function may be used to test
  * whether or not the current process has search permissions on an
  * inode.
  *
  * @param st
  *    stat structure containing information about the file.
  * @returns
  *    int; nonzero(true) if have permission, zero (false) if not.
  */
int libexplain_have_search_permission(const struct stat *st);

#endif /* LIBEXPLAIN_HAVE_PERMISSION_H */
