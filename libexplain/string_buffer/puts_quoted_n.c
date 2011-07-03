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

#include <libexplain/string_buffer.h>


void
explain_string_buffer_puts_quoted_n(explain_string_buffer_t *sb,
    const char *s, size_t n)
{
    if (!s)
    {
        explain_string_buffer_puts(sb, "NULL");
        return;
    }
    explain_string_buffer_putc(sb, '"');
    while (n > 0)
    {
        unsigned char   c;

        c = *s++;
        if (!c)
            break;
        --n;
        explain_string_buffer_putc_escaped(sb, c, '"');
    }
    explain_string_buffer_putc(sb, '"');
}


void
explain_string_buffer_putsu_quoted_n(explain_string_buffer_t *sb,
    const unsigned char *s, size_t n)
{
    explain_string_buffer_puts_quoted_n(sb, (const char *)s, n);
}
