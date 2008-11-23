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

#include <libexplain/fgets.h>
#include <libexplain/strtol_or_die.h>
#include <libexplain/wrap_and_print.h>

#include <explain/fgets.h>


void
explain_fgets(int errnum, int argc, char **argv)
{
    void            *data;
    int             data_size;
    FILE            *fp;

    fp = stdin;
    switch (argc)
    {
    case 3:
        fp = (FILE *)libexplain_strtol_or_die(argv[2]);
        break;

    case 2:
        break;

    default:
        fprintf(stderr, "fgets: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    data = (void *)libexplain_strtol_or_die(argv[0]);
    data_size = libexplain_strtol_or_die(argv[1]);

    libexplain_wrap_and_print
    (
        stdout,
        libexplain_errno_fgets(errnum, data, data_size, fp)
    );
}
