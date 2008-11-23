/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/fchmod.h>
#include <libexplain/permission_mode.h>
#include <libexplain/strtol_or_die.h>
#include <libexplain/wrap_and_print.h>

#include <explain/fchmod.h>


void
explain_fchmod(int errnum, int argc, char **argv)
{
    int             fildes;
    int             mode;

    fildes = -1;
    mode = 0644;
    switch (argc)
    {
    case 0:
        fprintf(stderr, "fchmod: no pathname given\n");
        exit(EXIT_FAILURE);

    case 2:
        mode = libexplain_permission_mode_parse(argv[1]);
        if (mode < 0)
        {
            fprintf
            (
                stderr,
                "argument \"%s\" does not look like a permission mode\n",
                argv[1]
            );
            exit(EXIT_FAILURE);
        }
        /* fall through... */

    case 1:
        fildes = libexplain_strtol_or_die(argv[0]);
        break;

    default:
        fprintf(stderr, "fchmod: too many arguments given\n");
        exit(EXIT_FAILURE);
    }

    libexplain_wrap_and_print
    (
        stdout,
        libexplain_errno_fchmod(errnum, fildes, mode)
    );
}
