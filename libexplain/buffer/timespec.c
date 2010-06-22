/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
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

#include <libexplain/ac/time.h>

#include <libexplain/buffer/timespec.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>


static void
print(explain_string_buffer_t *sb,
    const struct timespec *data)
{
    if (data->tv_nsec < 0)
    {
        explain_string_buffer_printf
        (
            sb,
            "{ tv_sec = %ld, tv_nsec = %ld }",
            (long)data->tv_sec,
            (long)data->tv_nsec
        );
    }
    else
    {
        explain_string_buffer_printf
        (
            sb,
            "{ %.11g seconds }",
            data->tv_sec + 1e-9 * data->tv_nsec
        );
    }
}


void
explain_buffer_timespec(explain_string_buffer_t *sb,
    const struct timespec *data)
{
    if (explain_pointer_is_efault(data, sizeof(*data)))
        explain_buffer_pointer(sb, data);
    else
        print(sb, data);
}


void
explain_buffer_timespec_array(explain_string_buffer_t *sb,
    const struct timespec *data, unsigned data_size)
{
    unsigned        j;

    if (explain_pointer_is_efault(data, sizeof(*data) * data_size))
    {
        explain_buffer_pointer(sb, data);
        return;
    }
    explain_string_buffer_putc(sb, '{');
    for (j = 0; j < data_size; ++j)
    {
        if (j)
            explain_string_buffer_puts(sb, ", ");
        print(sb, data + j);
    }
    explain_string_buffer_putc(sb, '}');
}