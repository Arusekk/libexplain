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

#ifndef LIBEXPLAIN_BUFFER_ENOTDIR_H
#define LIBEXPLAIN_BUFFER_ENOTDIR_H

#include <libexplain/string_buffer.h>

struct libexplain_final_t; /* forward */

/**
  * The libexplain_buffer_enotdir function may be used to
  *
  * @param sb
  *     The string buffer to print into
  * @param pathname
  *     The offending path
  * @param pathname_caption
  *     The name of the function argument of the offending path
  * @param pathname_final_component
  *     The required properties of the final component of the offending path
  */
void libexplain_buffer_enotdir(libexplain_string_buffer_t *sb,
    const char *pathname, const char *pathname_caption,
    const struct libexplain_final_t *pathname_final_component);

/**
  * The libexplain_buffer_enotdir function may be used to
  *
  * @param sb
  *     The string buffer to print into
  * @param oldpath
  *     The offending path
  * @param oldpath_caption
  *     The name of the function argument of the offending path
  * @param oldpath_final_component
  *     The required properties of the final component of the offending path
  * @param newpath
  *     The offending path
  * @param newpath_caption
  *     The name of the function argument of the offending path
  * @param newpath_final_component
  *     The required properties of the final component of the offending path
  */
void libexplain_buffer_enotdir2(libexplain_string_buffer_t *sb,
    const char *oldpath, const char *oldpath_caption,
    const struct libexplain_final_t *oldpath_final_component,
    const char *newpath, const char *newpath_caption,
    const struct libexplain_final_t *newpath_final_component);

/**
  * The libexplain_buffer_enotdir function may be used to
  *
  * @param sb
  *     The string buffer to print into
  * @param fildes
  *     The offending file descriptor
  * @param fildes_caption
  *     The name of the function argument of the offending file descriptor
  */
void libexplain_buffer_enotdir_fd(libexplain_string_buffer_t *sb,
    int fildes, const char *fildes_caption);

#endif /* LIBEXPLAIN_BUFFER_ENOTDIR_H */
