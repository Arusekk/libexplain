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

#include <libexplain/ac/string.h>

#include <libexplain/buffer/strerror.h>
#include <libexplain/errno_info.h>
#include <libexplain/option.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_strerror(libexplain_string_buffer_t *sb, int errnum)
{
    const libexplain_errno_info_t *eip;
    int             first;

    libexplain_string_buffer_puts(sb, strerror(errnum));
    first = 1;
    if (libexplain_option_numeric_errno())
    {
        libexplain_string_buffer_printf(sb, " (%d", errnum);
        first = 0;
    }
    eip = libexplain_errno_info_by_number(errnum);
    if (eip)
    {
        if (first)
            libexplain_string_buffer_puts(sb, " (");
        else
            libexplain_string_buffer_puts(sb, ", ");
        libexplain_string_buffer_puts(sb, eip->name);
        first = 0;
    }
    if (!first)
        libexplain_string_buffer_putc(sb, ')');
}
