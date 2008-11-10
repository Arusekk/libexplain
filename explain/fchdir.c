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

#include <libexplain/fchdir.h>
#include <libexplain/wrap_and_print.h>

#include <explain/fchdir.h>
#include <explain/strtol_or_die.h>


void
explain_fchdir(int errnum, int argc, char **argv)
{
    int fildes = -1;
    switch (argc)
    {
    case 0:
        fprintf(stderr, "fchdir: no fildes given");
        exit(1);

    case 1:
        fildes = strtol_or_die(argv[0]);
        break;

    default:
        fprintf(stderr, "fchdir: too many arguments given");
        exit(1);
    }

    libexplain_wrap_and_print
    (
        stdout,
        libexplain_errno_fchdir(errnum, fildes)
    );
}
