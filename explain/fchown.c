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

#include <libexplain/fchown.h>
#include <libexplain/strtol_or_die.h>
#include <libexplain/wrap_and_print.h>

#include <explain/fchown.h>


void
explain_fchown(int errnum, int argc, char **argv)
{
    int             fildes;
    int             owner;
    int             group;

    owner = -1;
    group = -1;
    switch (argc)
    {
    case 3:
        group = libexplain_strtol_or_die(argv[2]);
        /* fall through... */

    case 2:
        owner = libexplain_strtol_or_die(argv[1]);
        /* fall through... */

    case 1:
        fildes = libexplain_strtol_or_die(argv[0]);
        break;

    default:
        fprintf(stderr, "fchown: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    libexplain_wrap_and_print
    (
        stdout,
        libexplain_errno_fchown(errnum, fildes, owner, group)
    );
}
