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

#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/exdev.h>
#include <libexplain/option.h>
#include <libexplain/same_dev.h>


void
explain_buffer_exdev(explain_string_buffer_t *sb, const char *oldpath,
    const char *newpath, const char *sys_call_name)
{
    explain_string_buffer_puts
    (
        sb,
        "oldpath"
    );
    explain_buffer_mount_point(sb, oldpath);
    explain_string_buffer_puts
    (
        sb,
        " and newpath"
    );
    explain_buffer_mount_point_dirname(sb, newpath);
    explain_string_buffer_puts
    (
        sb,
        " are not on the same mounted file system"
    );
#ifdef __linux__
    if (explain_option_dialect_specific())
    {
        if (explain_same_dev(oldpath, newpath))
        {
            explain_string_buffer_puts
            (
                sb,
                "; Linux permits a file system to be mounted at "
                "multiple points, but the "
            );
            explain_string_buffer_puts(sb, sys_call_name);
            explain_string_buffer_puts
            (
                sb,
                " system call does not work across different mount points, "
                "even if the same file system is mounted on both"
            );
        }
    }
#endif
}
