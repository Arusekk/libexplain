/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2011 Peter Miller
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

#include <libexplain/buffer/int64_t.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/is_efault.h>


void
explain_buffer_int64_t(explain_string_buffer_t *sb, int64_t data)
{
    explain_string_buffer_printf(sb, "%lld", (long long)data);
}


void
explain_buffer_int64_array(explain_string_buffer_t *sb, const int64_t *data,
    size_t data_size)
{
    size_t          j;

    if (explain_is_efault_pointer(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
        return;
    }

    explain_string_buffer_puts(sb, "{ ");
    for (j = 0; j < data_size; ++j)
    {
        if (j)
            explain_string_buffer_puts(sb, ", ");
        explain_buffer_int64_t(sb, data[j]);
    }
    explain_string_buffer_puts(sb, " }");
}


void
explain_buffer_uint64_t(explain_string_buffer_t *sb, uint64_t data)
{
    explain_string_buffer_printf(sb, "%llu", (unsigned long long)data);
}


void
explain_buffer_uint64_array(explain_string_buffer_t *sb, const uint64_t *data,
    size_t data_size)
{
    size_t          j;

    if (explain_is_efault_pointer(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
        return;
    }

    explain_string_buffer_puts(sb, "{ ");
    for (j = 0; j < data_size; ++j)
    {
        if (j)
            explain_string_buffer_puts(sb, ", ");
        explain_buffer_uint64_t(sb, data[j]);
    }
    explain_string_buffer_puts(sb, " }");
}
