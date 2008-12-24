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


const libexplain_parse_bits_table_t *
libexplain_parse_bits_find_by_value(int value,
    const libexplain_parse_bits_table_t *table, size_t table_size)
{
    const libexplain_parse_bits_table_t *tp;
    const libexplain_parse_bits_table_t *end;

    end = table + table_size;
    for (tp = table; tp < end; ++tp)
    {
        if (tp->value == value)
        {
            return tp;
        }
    }
    return 0;
}
