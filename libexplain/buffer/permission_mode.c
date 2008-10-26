/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#include <libexplain/ac/sys/stat.h>

#include <libexplain/parse_bits.h>
#include <libexplain/permission_mode.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>


static const libexplain_parse_bits_table_t table[] =
{
    { "S_ISUID", S_ISUID },
    { "S_ISGID", S_ISGID },
    { "S_ISVTX", S_ISVTX },
    { "S_IRWXU", S_IRWXU },
    { "S_IRUSR", S_IRUSR },
    { "S_IWUSR", S_IWUSR },
    { "S_IXUSR", S_IXUSR },
    { "S_IRWXG", S_IRWXG },
    { "S_IRGRP", S_IRGRP },
    { "S_IWGRP", S_IWGRP },
    { "S_IXGRP", S_IXGRP },
    { "S_IRWXO", S_IRWXO },
    { "S_IROTH", S_IROTH },
    { "S_IWOTH", S_IWOTH },
    { "S_IXOTH", S_IXOTH },
};


void
libexplain_buffer_permission_mode(libexplain_string_buffer_t *sb, int mode)
{
    const libexplain_parse_bits_table_t *tp;
    int             first;

    if (mode == 0)
    {
        libexplain_string_buffer_putc(sb, '0');
        return;
    }
    first = 1;
    for (tp = table; tp < LIBEXPLAIN_ENDOF(table); ++tp)
    {
        if (tp->value != 0 && (mode & tp->value) == tp->value)
        {
            if (!first)
                libexplain_string_buffer_puts(sb, " | ");
            libexplain_string_buffer_puts(sb, tp->name);
            first = 0;
            mode -= tp->value;
        }
    }
    if (mode != 0)
    {
        if (!first)
            libexplain_string_buffer_puts(sb, " | ");
        libexplain_string_buffer_printf(sb, "%#o", mode);
    }
}


int
libexplain_permission_mode_parse(const char *text)
{
    return libexplain_parse_bits(text, table, LIBEXPLAIN_SIZEOF(table));
}
