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

#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/mount_point.h>


void
libexplain_buffer_erofs(libexplain_string_buffer_t *sb, const char *pathname,
    const char *caption)
{
    libexplain_string_buffer_printf
    (
        sb,
        "write access was requested and %s refers to a file on a read-only "
            "file system",
        caption
    );
    if (libexplain_buffer_mount_point(sb, pathname) < 0)
        libexplain_buffer_mount_point_dirname(sb, pathname);
}


void
libexplain_buffer_erofs_fildes(libexplain_string_buffer_t *sb, int fildes,
    const char *caption)
{
    libexplain_string_buffer_printf
    (
        sb,
        "write access was requested and %s refers to a file on a read-only "
            "file system",
        caption
    );
    libexplain_buffer_mount_point_fd(sb, fildes);
}
