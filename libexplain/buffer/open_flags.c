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

#include <libexplain/ac/fcntl.h>

#include <libexplain/open_flags.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>


static const libexplain_parse_bits_table_t table[] =
{
    { "O_RDONLY", O_RDONLY },
    { "O_WRONLY", O_WRONLY },
    { "O_RDWR", O_RDWR },
    { "O_CREAT", O_CREAT },
    { "O_EXCL", O_EXCL },
    { "O_NOCTTY", O_NOCTTY },
    { "O_TRUNC", O_TRUNC },
    { "O_APPEND", FAPPEND },
    { "FAPPEND", O_APPEND }, /* BSD */
    { "O_NONBLOCK", O_NONBLOCK },
    { "FNONBLOCK", FNONBLOCK }, /* BSD */
    { "O_NDELAY", O_NDELAY },
    { "FNDELAY", FNDELAY }, /* BSD */
    { "O_SYNC", O_SYNC },
    { "O_FSYNC", O_FSYNC },
    { "FFSYNC", FFSYNC }, /* BSD */
    { "O_DSYNC", O_DSYNC },
    { "O_RSYNC", O_RSYNC },
    { "O_ASYNC", O_ASYNC },
    { "FASYNC", FASYNC }, /* BSD */
    { "O_DIRECT", O_DIRECT },
    { "O_DIRECTORY", O_DIRECTORY },
    { "O_NOFOLLOW", O_NOFOLLOW },
    { "O_NOATIME", O_NOATIME },
    { "O_CLOEXEC", O_CLOEXEC },
    { "O_LARGEFILE", O_LARGEFILE },
    { "O_BINARY", O_BINARY },
    { "O_TEXT", O_TEXT },
};


static void
flag_bit(libexplain_string_buffer_t *sb, int bit)
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
libexplain_buffer_open_flags(libexplain_string_buffer_t *sb, int flags)
{
    int             low_bits;

    low_bits = flags & O_ACCMODE;
    flags &= ~O_ACCMODE;
    switch (low_bits)
    {
    case O_RDONLY:
        libexplain_string_buffer_puts(sb, "O_RDONLY");
        break;

    case O_RDWR:
        libexplain_string_buffer_puts(sb, "O_RDWR");
        break;

    case O_WRONLY:
        libexplain_string_buffer_puts(sb, "O_WRONLY");
        break;

    default:
        libexplain_string_buffer_printf(sb, "%d", low_bits);
        break;
    }
    while (flags)
    {
        int             bit;

        bit = (flags & -flags);
        flags -= bit;
        libexplain_string_buffer_puts(sb, " | ");
        flag_bit(sb, bit);
    }
}


int
libexplain_open_flags_parse(const char *text)
{
    return libexplain_parse_bits(text, table, LIBEXPLAIN_SIZEOF(table));
}
