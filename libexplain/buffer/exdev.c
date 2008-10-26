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

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/exdev.h>
#include <libexplain/same_dev.h>


void
libexplain_buffer_exdev(libexplain_string_buffer_t *sb, const char *oldpath,
    const char *newpath, const char *sys_call_name)
{
    libexplain_buffer_because(sb);
    libexplain_string_buffer_puts
    (
        sb,
        "oldpath"
    );
    libexplain_buffer_mount_point(sb, oldpath);
    libexplain_string_buffer_puts
    (
        sb,
        " and newpath"
    );
    libexplain_buffer_mount_point_dirname(sb, newpath);
    libexplain_string_buffer_puts
    (
        sb,
        " are not on the same mounted file system"
    );
#ifdef __linux__
    if (libexplain_same_dev(oldpath, newpath))
    {
        libexplain_string_buffer_puts
        (
            sb,
            "; linux permits a file system to be mounted at "
            "multiple points, but the "
        );
        libexplain_string_buffer_puts(sb, sys_call_name);
        libexplain_string_buffer_puts
        (
            sb,
            " system call does not work across different mount points, "
            "even if the same file system is mounted on both"
        );
    }
#endif
}
