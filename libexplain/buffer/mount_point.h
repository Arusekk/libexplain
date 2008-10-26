/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#ifndef LIBEXPLAIN_BUFFER_MOUNT_POINT_H
#define LIBEXPLAIN_BUFFER_MOUNT_POINT_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_mount_point function may be used to insert the
  * mount point of the file system the path is within.
  *
  * @param sb
  *     The string buffer to write to.
  * @param path
  *     The path of the file in the file system we care about.
  * @returns
  *     0 on success, -1 if no mount point inserted
  */
int libexplain_buffer_mount_point(libexplain_string_buffer_t *sb,
    const char *path);

/**
  * The libexplain_buffer_mount_point function may be used to insert the
  * mount point of the file system the dirname(path) is within.
  *
  * @param sb
  *     The string buffer to write to.
  * @param path
  *     The path of the file in the file system we care about.
  * @returns
  *     0 on success, -1 if no mount point inserted
  */
int libexplain_buffer_mount_point_dirname(libexplain_string_buffer_t *sb,
    const char *path);

/**
  * The libexplain_buffer_mount_point_fd function may be used to insert
  * the mount point of the file system the file descriptor is within.
  *
  * @param sb
  *     The string buffer to write to.
  * @param fildes
  *     The file descriptor of the file we care about.
  * @returns
  *     0 on success, -1 if no mount point inserted
  */
int libexplain_buffer_mount_point_fd(libexplain_string_buffer_t *sb,
    int fildes);

struct stat; /* forward */

/**
  * The libexplain_buffer_mount_point_stat function may be used to insert
  * the mount point of the file system, as described by the stat struct.
  *
  * @param sb
  *     The string buffer to write to.
  * @param st
  *     The stat struct describing the file we care about.
  * @returns
  *     0 on success, -1 if no mount point inserted
  */
int libexplain_buffer_mount_point_stat(libexplain_string_buffer_t *sb,
    const struct stat *st);

#endif /* LIBEXPLAIN_BUFFER_MOUNT_POINT_H */
