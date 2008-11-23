/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/gettimeofday.h>
#include <libexplain/strtol_or_die.h>
#include <libexplain/wrap_and_print.h>

#include <explain/gettimeofday.h>


void
explain_gettimeofday(int errnum, int argc, char **argv)
{
    struct timeval  *tv;
    struct timezone *tz;

    tv = NULL;
    tz = NULL;
    switch (argc)
    {
    case 2:
        tz = (struct timezone *)libexplain_strtol_or_die(argv[1]);
        /* fall through... */

    case 1:
        tv = (struct timeval *)libexplain_strtol_or_die(argv[0]);
        /* fall through... */

    case 0:
        break;

    default:
        fprintf(stderr, "gettimeofday: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    libexplain_wrap_and_print
    (
        stdout,
        libexplain_errno_gettimeofday(errnum, tv, tz)
    );
}
