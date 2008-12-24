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

#include <libexplain/string_buffer.h>


void
libexplain_string_buffer_puts_quoted(libexplain_string_buffer_t *sb,
    const char *s)
{
    if (!s)
    {
        libexplain_string_buffer_puts(sb, "NULL");
        return;
    }
    libexplain_string_buffer_putc(sb, '"');
    for (;;)
    {
        unsigned char   c;

        c = *s++;
        switch (c)
        {
        case '\0':
            libexplain_string_buffer_putc(sb, '"');
            return;

        case '\\':
        case '"':
            libexplain_string_buffer_putc(sb, '\\');
            libexplain_string_buffer_putc(sb, c);
            break;

        case '\a':
            libexplain_string_buffer_puts(sb, "\\a");
            break;

        case '\b':
            libexplain_string_buffer_puts(sb, "\\b");
            break;

        case '\f':
            libexplain_string_buffer_puts(sb, "\\f");
            break;

        case '\n':
            libexplain_string_buffer_puts(sb, "\\n");
            break;

        case '\r':
            libexplain_string_buffer_puts(sb, "\\r");
            break;

        case '\t':
            libexplain_string_buffer_puts(sb, "\\t");
            break;

        case '\v':
            libexplain_string_buffer_puts(sb, "\\v");
            break;

        default:
            libexplain_string_buffer_putc(sb, c);
            break;
        }
    }
}
