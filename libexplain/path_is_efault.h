/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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

#ifndef LIBEXPLAIN_PATH_IS_EFAULT_H
#define LIBEXPLAIN_PATH_IS_EFAULT_H

/**
  * The libexplain_path_is_efault function may be used to determine whether
  * or not a path pointer is a valid pointer.  Typically this is used
  * when handling ambiguous EFAULT situations.
  *
  * @param path
  *    The path to be checked.
  * @returns
  *    int; 1 if the path is bad (points outside the process address
  *    space), or 0 if the path is OK.
  */
int libexplain_path_is_efault(const char *path);

/**
  * The libexplain_pointer_is_efault function may be used to determine
  * whether or not a pointer is valid.  Typically this is used when
  * handling ambiguous EFAULT situations.
  *
  * @param ptr
  *    The pointer to be checked.
  * @returns
  *    int; 1 if the pointer is bad (points outside the process address
  *    space), or 0 if the pointer is OK.
  */
int libexplain_pointer_is_efault(const void *ptr);

#endif /* LIBEXPLAIN_PATH_IS_EFAULT_H */
