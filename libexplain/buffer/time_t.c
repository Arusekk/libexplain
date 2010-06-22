/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/buffer/time_t.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/option.h>
#include <libexplain/path_is_efault.h>


void
explain_buffer_time_t(explain_string_buffer_t *sb, const time_t *data)
{
    if (explain_pointer_is_efault(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
        return;
    }

    explain_string_buffer_printf(sb, "{ %ld", (long)*data);
    if (explain_option_dialect_specific())
    {
        struct tm       *tmp;

        tmp = localtime(data);
        if (tmp)
        {
            char            buffer[200];

            strftime
            (
                buffer,
                sizeof(buffer),
                " \"%a, %Y-%b-%d %H:%M:%S %z\"",
                tmp
            );
            explain_string_buffer_puts(sb, buffer);
        }
    }
    explain_string_buffer_puts(sb, " }");
}
