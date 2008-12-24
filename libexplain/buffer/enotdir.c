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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/wrong_file_type.h>


void
libexplain_buffer_enotdir(libexplain_string_buffer_t *sb, const char *pathname,
    const char *caption, const libexplain_final_t *final_component)
{
    if
    (
        libexplain_buffer_errno_path_resolution
        (
            sb,
            ENOTDIR,
            pathname,
            caption,
            final_component
        )
    )
    {
        if (must_be_a_directory(final_component))
        {
            libexplain_string_buffer_printf
            (
                sb,
                "%s, or a directory component of %s, is not, in fact, a "
                    "directory",
                caption,
                caption
            );
        }
        else
        {
            libexplain_string_buffer_printf
            (
                sb,
                "a directory component of %s is not, in fact, a directory",
                caption
            );
        }
    }
}


void
libexplain_buffer_enotdir2(libexplain_string_buffer_t *sb,
    const char *oldpath, const char *oldpath_caption,
    const libexplain_final_t *oldpath_final_component,
    const char *newpath, const char *newpath_caption,
    const libexplain_final_t *newpath_final_component)
{
    if
    (
        libexplain_buffer_errno_path_resolution
        (
            sb,
            ENOTDIR,
            oldpath,
            oldpath_caption,
            oldpath_final_component
        )
    &&
        libexplain_buffer_errno_path_resolution
        (
            sb,
            ENOTDIR,
            newpath,
            newpath_caption,
            newpath_final_component
        )
    )
    {
        char            both[20];

        snprintf
        (
            both,
            sizeof(both),
            "%s or %s",
            oldpath_caption,
            newpath_caption
        );

        if
        (
            must_be_a_directory(oldpath_final_component)
        ||
            must_be_a_directory(newpath_final_component)
        )
        {
            libexplain_string_buffer_printf
            (
                sb,
                "%s, or a directory component of %s, is not, in fact, a "
                    "directory",
                both,
                both
            );
        }
        else
        {
            libexplain_string_buffer_printf
            (
                sb,
                "a directory component of %s is not, in fact, a directory",
                both
            );
        }
    }
}


void
libexplain_buffer_enotdir_fd(libexplain_string_buffer_t *sb, int fildes,
    const char *caption)
{
    libexplain_buffer_wrong_file_type(sb, fildes, caption, S_IFDIR);
}
