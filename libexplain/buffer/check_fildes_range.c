/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2013 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/check_fildes_range.h>
#include <libexplain/option.h>


static void
whinge(explain_string_buffer_t *sb, const char *caption)
{
    explain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used when a file descriptor
         * is detected that is negative, or larger than
         * sysconf(_SC_OPEN_MAX).
         *
         * The range will be printed separately, if available, so do not
         * mention sysconf(_SC_OPEN_MAX) in the translation.
         */
        i18n("%s is outside the allowed range for file descriptors"),
        caption
    );
}


int
explain_buffer_check_fildes_range(explain_string_buffer_t *sb,
    int fildes, const char *caption)
{
    long            open_max;

    open_max = sysconf(_SC_OPEN_MAX);
    if (open_max < 0)
    {
        if (fildes < 0)
        {
            whinge(sb, caption);
            return 0;
        }
    }
    else
    {
        if (fildes < 0 || fildes >= open_max)
        {
            whinge(sb, caption);
            if (explain_option_dialect_specific())
            {
                explain_string_buffer_printf(sb, " (0..%ld)", open_max - 1);
            }
            return 0;
        }
    }

    /*
     * No error, yay!
     * Except that we wanted to find one.
     */
    return -1;
}


/* vim: set ts=8 sw=4 et : */
