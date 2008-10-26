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

#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/emlink.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/dirname.h>


void
libexplain_buffer_emlink(libexplain_string_buffer_t *sb, const char *oldpath,
    const char *newpath)
{
    struct stat     oldpath_st;

    libexplain_buffer_because(sb);
    if (stat(oldpath, &oldpath_st) >= 0)
    {
        long            link_max;

        /*
         * By definition, oldpath and newpath are on the same file
         * system to get this error, so we don't need to call
         * pathconf twice.
         */
        link_max = pathconf(oldpath, _PC_LINK_MAX);

        if (S_ISDIR(oldpath_st.st_mode))
        {
            char            npdir[PATH_MAX + 1];

            libexplain_dirname(npdir, newpath, sizeof(npdir));
            libexplain_string_buffer_puts
            (
                sb,
                "oldpath is a directory and the "
            );
            libexplain_string_buffer_puts_quoted(sb, npdir);
            libexplain_string_buffer_puts
            (
                sb,
                " directory already has the maximum number of links"
            );
        }
        else
        {
            libexplain_string_buffer_puts
            (
                sb,
                "oldpath is a "
            );
            libexplain_buffer_file_type(sb, oldpath_st.st_mode);
            libexplain_string_buffer_puts
            (
                sb,
                " and already has the maximum number of links"
            );
        }
        if (link_max > 0)
            libexplain_string_buffer_printf(sb, " (%ld)", link_max);
    }
    else
    {
        /*
         * Unable to locate a specific cause, use the generic explanation.
         *
         * (If can't stat, then will not be able to pconf either, so
         * can't supplement explanation with the value of the maximum.)
         */
        libexplain_string_buffer_puts
        (
            sb,
            "oldpath already has the maximum number of links to it, "
            "or oldpath is a directory and the directory containing "
            "newpath has the maximum number of links"
        );
    }
}
