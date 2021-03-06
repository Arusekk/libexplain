/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2013 Peter Miller
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

#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/errno/path_resolution.h>


static void
report_error(explain_string_buffer_t *sb, const char *caption)
{
    explain_string_buffer_printf
    (
        sb,
        "%s, or a directory component of %s, is too long",
        caption,
        caption
    );
}


void
explain_buffer_enametoolong(explain_string_buffer_t *sb,
    const char *pathname, const char *pathname_caption,
    const explain_final_t *final_component)
{
    if
    (
        explain_buffer_errno_path_resolution
        (
            sb,
            ENAMETOOLONG,
            pathname,
            pathname_caption,
            final_component
        )
    )
    {
        /*
         * Unable to find a specific cause,
         * emit the generic explanation.
         */
        report_error(sb, pathname_caption);
    }
}


void
explain_buffer_enametoolong2(explain_string_buffer_t *sb,
    const char *oldpath, const char *oldpath_caption,
    const explain_final_t *oldpath_final_component,
    const char *newpath, const char *newpath_caption,
    const explain_final_t *newpath_final_component)
{
    if
    (
        explain_buffer_errno_path_resolution
        (
            sb,
            ENAMETOOLONG,
            oldpath,
            oldpath_caption,
            oldpath_final_component
        )
    &&
        explain_buffer_errno_path_resolution
        (
            sb,
            ENAMETOOLONG,
            newpath,
            newpath_caption,
            newpath_final_component
        )
    )
    {
        char            buffer[40];

        /*
         * Unable to pin point a specific cause,
         * use the generic explanatiuon.
         */
        snprintf
        (
            buffer,
            sizeof(buffer),
            "%s or %s",
            oldpath_caption,
            newpath_caption
        );
        report_error(sb, buffer);
    }
}


/* vim: set ts=8 sw=4 et : */
