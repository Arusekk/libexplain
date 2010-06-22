/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010 Peter Miller
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

#include <libexplain/ac/fcntl.h>

#include <libexplain/open_flags.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>

#ifndef O_DIRECT
#define O_DIRECT 0
#endif
#ifndef O_DIRECTORY
#define O_DIRECTORY 0
#endif
#ifndef O_NOFOLLOW
#define O_NOFOLLOW 0
#endif
#ifndef O_NOATIME
#define O_NOATIME 0
#endif
#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif
#ifndef O_LARGEFILE
#define O_LARGEFILE 0
#endif

static const explain_parse_bits_table_t table[] =
{
    { "O_RDONLY", O_RDONLY },
    { "O_WRONLY", O_WRONLY },
    { "O_RDWR", O_RDWR },
    { "O_CREAT", O_CREAT },
    { "O_EXCL", O_EXCL },
#ifdef O_NOCTTY
    { "O_NOCTTY", O_NOCTTY },
#endif
    { "O_TRUNC", O_TRUNC },
    { "O_APPEND", O_APPEND },
#ifdef FAPPEND
    { "FAPPEND", FAPPEND }, /* BSD */
#endif
#ifdef O_NONBLOCK
    { "O_NONBLOCK", O_NONBLOCK },
#endif
#ifdef FNONBLOCK
    { "FNONBLOCK", FNONBLOCK }, /* BSD */
#endif
#ifdef O_NDELAY
    { "O_NDELAY", O_NDELAY },
#endif
#ifdef FNDELAY
    { "FNDELAY", FNDELAY }, /* BSD */
#endif
#ifdef O_SYNC
    { "O_SYNC", O_SYNC },
#endif

    /*
     * No, this isn't a mistake.  On debian sparc there is a
     *
     *     #define FFSYNC O_FSYNC
     *
     * but O_FSYNC isn't actually defined, leading to a build failure.
     * Thus, the O_FSYNC bracketing goes around both of them.
     */
#ifdef O_FSYNC
    { "O_FSYNC", O_FSYNC },
#ifdef FFSYNC
    { "FFSYNC", FFSYNC }, /* BSD */
#endif
#endif

#ifdef O_DSYNC
    { "O_DSYNC", O_DSYNC },
#endif
#ifdef O_RSYNC
    { "O_RSYNC", O_RSYNC },
#endif
#ifdef O_ASYNC
    { "O_ASYNC", O_ASYNC },
#endif
#ifdef FASYNC
    { "FASYNC", FASYNC }, /* BSD */
#endif
#ifdef O_DIRECT
    { "O_DIRECT", O_DIRECT },
#endif
#ifdef O_DIRECTORY
    { "O_DIRECTORY", O_DIRECTORY },
#endif
#ifdef O_NOFOLLOW
    { "O_NOFOLLOW", O_NOFOLLOW },
#endif
#ifdef O_NOATIME
    { "O_NOATIME", O_NOATIME },
#endif
#ifdef O_CLOEXEC
    { "O_CLOEXEC", O_CLOEXEC },
#endif
#ifdef O_LARGEFILE_HIDDEN
    { "O_LARGEFILE", O_LARGEFILE_HIDDEN },
#else
    { "O_LARGEFILE", O_LARGEFILE },
#endif
#ifdef O_BINARY
    { "O_BINARY", O_BINARY },
#endif
#ifdef O_TEXT
    { "O_TEXT", O_TEXT },
#endif
};


void
explain_buffer_open_flags(explain_string_buffer_t *sb, int flags)
{
    int             low_bits;
    int             other;

    low_bits = flags & O_ACCMODE;
    flags &= ~O_ACCMODE;
    switch (low_bits)
    {
    case O_RDONLY:
        explain_string_buffer_puts(sb, "O_RDONLY");
        break;

    case O_RDWR:
        explain_string_buffer_puts(sb, "O_RDWR");
        break;

    case O_WRONLY:
        explain_string_buffer_puts(sb, "O_WRONLY");
        break;

    default:
        explain_string_buffer_printf(sb, "%d", low_bits);
        break;
    }
    other = 0;
    while (flags != 0)
    {
        int             bit;
        const explain_parse_bits_table_t *tp;

        bit = (flags & -flags);
        flags &= ~bit;
        tp = explain_parse_bits_find_by_value(bit, table, SIZEOF(table));
        if (tp)
        {
            explain_string_buffer_puts(sb, " | ");
            explain_string_buffer_puts(sb, tp->name);
        }
        else
            other |= bit;
    }
    if (other != 0)
        explain_string_buffer_printf(sb, " | %#o", other);
}


int
explain_open_flags_parse_or_die(const char *text, const char *caption)
{
    return explain_parse_bits_or_die(text, table, SIZEOF(table), caption);
}
