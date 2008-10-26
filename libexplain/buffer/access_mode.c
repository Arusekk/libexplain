/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/access_mode.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>


static const libexplain_parse_bits_table_t table[] =
{
    { "F_OK", F_OK },
    { "R_OK", R_OK },
    { "W_OK", W_OK },
    { "X_OK", X_OK },
};


static void
mode_bit(libexplain_string_buffer_t *sb, int bit)
{
    const libexplain_parse_bits_table_t *tp;

    for (tp = table; tp < LIBEXPLAIN_ENDOF(table); ++tp)
    {
        if (tp->value == bit)
        {
            libexplain_string_buffer_puts(sb, tp->name);
            return;
        }
    }
    libexplain_string_buffer_printf(sb, "%#o", bit);
}


void
libexplain_buffer_access_mode(libexplain_string_buffer_t *sb, int mode)
{
    int             first;

    if (mode == 0)
    {
        libexplain_string_buffer_puts(sb, "F_OK");
        return;
    }
    first = 1;
    while (mode)
    {
        int             bit;

        bit = (mode & -mode);
        mode -= bit;
        if (!first)
            libexplain_string_buffer_puts(sb, " | ");
        mode_bit(sb, bit);
        first = 0;
    }
}


int
libexplain_access_mode_parse(const char *text)
{
    return libexplain_parse_bits(text, table, LIBEXPLAIN_SIZEOF(table));
}
