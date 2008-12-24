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

#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/option.h>


void
libexplain_buffer_emfile(libexplain_string_buffer_t *sb)
{
    libexplain_buffer_gettext
    (
        sb,
        /*
         * xgettext: This message is used when a process fails to open a
         * file because that process already has the maximum number of file
         * descriptors open.
         */
        i18n("the process already has the maximum number of file "
            "descriptors open")
    );

    if (libexplain_option_dialect_specific())
    {
        long            open_max;

        open_max = sysconf(_SC_OPEN_MAX);
        if (open_max > 0)
        {
            libexplain_string_buffer_printf(sb, " (%ld)", open_max);
        }
    }
}
