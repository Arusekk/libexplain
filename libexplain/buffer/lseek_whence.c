/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/lseek_whence.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>

static const libexplain_parse_bits_table_t table[] =
{
    { "SEEK_SET", SEEK_SET },
    { "SEEK_CUR", SEEK_CUR },
    { "SEEK_END", SEEK_END },
#ifdef SEEK_MAX
    { "SEEK_MAX", SEEK_MAX },
#endif
    { "L_SET",    SEEK_SET },
    { "L_INCR",   SEEK_CUR },
    { "L_XTND",   SEEK_END },
};


void
libexplain_buffer_lseek_whence(libexplain_string_buffer_t *sb, int whence)
{
    const libexplain_parse_bits_table_t *tp;

    tp = libexplain_parse_bits_find_by_value(whence, table, SIZEOF(table));
    if (tp)
        libexplain_string_buffer_puts(sb, tp->name);
    else
        libexplain_string_buffer_printf(sb, "%d", whence);
}


int
libexplain_lseek_whence_parse_or_die(const char *text, const char *caption)
{
    return libexplain_parse_bits_or_die(text, table, SIZEOF(table), caption);
}
