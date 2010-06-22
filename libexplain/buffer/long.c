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

#include <libexplain/buffer/long.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>


void
explain_buffer_long(explain_string_buffer_t *sb, long data)
{
    explain_string_buffer_printf(sb, "%ld", data);
}


void
explain_buffer_ulong(explain_string_buffer_t *sb, unsigned long data)
{
    explain_string_buffer_printf(sb, "%lu", data);
}


void
explain_buffer_long_star(explain_string_buffer_t *sb, const long *data)
{
    if (explain_pointer_is_efault(data, sizeof(*data)))
        explain_buffer_pointer(sb, data);
    else
    {
        explain_string_buffer_puts(sb, "{ ");
        explain_buffer_long(sb, *data);
        explain_string_buffer_puts(sb, " }");
    }
}


void
explain_buffer_ulong_star(explain_string_buffer_t *sb,
    const unsigned long *data)
{
    if (explain_pointer_is_efault(data, sizeof(*data)))
        explain_buffer_pointer(sb, data);
    else
    {
        explain_string_buffer_puts(sb, "{ ");
        explain_buffer_ulong(sb, *data);
        explain_string_buffer_puts(sb, " }");
    }
}
