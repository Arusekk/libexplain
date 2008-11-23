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

#include <libexplain/ac/sys/wait.h>

#include <libexplain/buffer/waitpid_options.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>


static const libexplain_parse_bits_table_t table[] =
{
    { "WNOHANG", WNOHANG },
    { "WUNTRACED", WUNTRACED },
    { "WCONTINUED", WCONTINUED },
};


void
libexplain_buffer_waitpid_options(libexplain_string_buffer_t *sb, int options)
{
    int             first;
    int             other;
    const libexplain_parse_bits_table_t *tp;

    first = 1;
    other = 0;
    while (options)
    {
        int             bit;

        bit = options & -options;
        options -= bit;
        tp = libexplain_parse_bits_find_by_value(bit, table, SIZEOF(table));
        if (tp)
        {
            if (!first)
                libexplain_string_buffer_puts(sb, " | ");
            libexplain_string_buffer_puts(sb, tp->name);
            first = 0;
        }
        else
        {
            other |= bit;
        }
    }
    if (other)
    {
        if (!first)
            libexplain_string_buffer_puts(sb, " | ");
        libexplain_string_buffer_printf(sb, "%#x", other);
    }
    else
    {
        if (first)
            libexplain_string_buffer_putc(sb, '0');
    }
}


int
libexplain_parse_waitpid_options(const char *text)
{
    return libexplain_parse_bits(text, table, SIZEOF(table));
}
