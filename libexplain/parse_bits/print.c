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

#include <libexplain/parse_bits.h>
#include <libexplain/string_buffer.h>


void
libexplain_parse_bits_print(libexplain_string_buffer_t *sb, int value,
    const libexplain_parse_bits_table_t *table, int table_size)
{
    int             first;
    int             other;

    if (value == 0)
    {
        libexplain_string_buffer_puts(sb, "0");
        return;
    }
    first = 1;
    other = 0;
    for (;;)
    {
        int             bit;
        const libexplain_parse_bits_table_t *tp;

        bit = value & -value;
        value -= bit;
        tp = libexplain_parse_bits_find_by_value(bit, table, table_size);
        if (tp)
        {
            if (!first)
                libexplain_string_buffer_puts(sb, " | ");
            libexplain_string_buffer_puts(sb, tp->name);
            first = 0;
        }
        else
            other |= bit;
        if (!value)
            break;
    }
    if (other)
    {
        if (!first)
            libexplain_string_buffer_puts(sb, " | ");
        libexplain_string_buffer_printf(sb, "0x%X", other);
    }
}
