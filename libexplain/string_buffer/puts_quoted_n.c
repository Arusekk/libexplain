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

#include <libexplain/string_buffer.h>


void
libexplain_string_buffer_puts_quoted_n(libexplain_string_buffer_t *sb,
    const char *s, size_t n)
{
    if (!s)
    {
        libexplain_string_buffer_puts(sb, "NULL");
        return;
    }
    libexplain_string_buffer_putc(sb, '"');
    while (n > 0)
    {
        unsigned char   c;

        c = *s++;
        if (!c)
            break;
        --n;
        libexplain_string_buffer_putc_escaped(sb, c, '"');
    }
    libexplain_string_buffer_putc(sb, '"');
}
