/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/ac/fcntl.h>

#include <libexplain/string_buffer.h>
#include <libexplain/string_flags.h>


void
explain_string_flags_init(explain_string_flags_t *sf, const char *flags)
{
    int             acc_mode;
    int             options;
    const char      *cp;
    unsigned char   c;
    explain_string_buffer_t yuck_buf;

    /*
     * Parse the flags string.
     *
     * (It turns out glibc is more generous than this, when it comes to
     * validity, but we only complain for EINVAL.  Different systems
     * will see validity differently.)
     */
    sf->rwa_seen = 0;
    acc_mode = O_RDONLY;
    options = 0;
    sf->flags_string_valid = 1;
    cp = flags;
    c = *cp++;
    switch (c)
    {
    case 'b':
        options |= O_BINARY;
        c = *cp++;
        break;

    case 't':
        options |= O_TEXT;
        c = *cp++;
        break;

    default:
        break;
    }
    switch (c)
    {
    case 'r':
        acc_mode = O_RDONLY;
        sf->rwa_seen = 1;
        break;

    case 'w':
        acc_mode = O_WRONLY;
        options |= O_CREAT | O_TRUNC;
        sf->rwa_seen = 1;
        break;

    case 'a':
        acc_mode = O_WRONLY;
        options |= O_CREAT | O_APPEND;
        sf->rwa_seen = 1;
        break;

    case '\0':
        --cp;
        /* fall through... */

    default:
        sf->flags_string_valid = 0;
        break;
    }
    explain_string_buffer_init(&yuck_buf, sf->invalid, sizeof(sf->invalid));
    for (;;)
    {
        c = *cp++;
        switch (c)
        {
        case '+':
            acc_mode = O_RDWR;
            continue;

        case 'b':
            options |= O_BINARY;
            continue;

        case 'c':
            /* no cancel */
            continue;

        case 'e':
#ifdef O_CLOEXEC
            options |= O_CLOEXEC;
#endif
            continue;

        case 'm':
            /* mmap */
            continue;

        case 't':
            options |= O_TEXT;
            continue;

        case 'x':
            options |= O_EXCL;
            continue;

        default:
            sf->flags_string_valid = 0;
            explain_string_buffer_putc(&yuck_buf, c);
            continue;

        case '\0':
            --cp;
            break;
        }
        break;
    }
    sf->flags = acc_mode | options;
#ifdef O_LARGEFILE
    sf->flags |= O_LARGEFILE;
#endif
}
