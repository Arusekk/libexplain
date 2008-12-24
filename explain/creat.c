/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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

#include <libexplain/creat.h>
#include <libexplain/permission_mode.h>
#include <libexplain/wrap_and_print.h>

#include <explain/creat.h>


void
explain_creat(int errnum, int argc, char **argv)
{
    const char      *path;
    int             mode;

    path = 0;
    mode = 0666;
    switch (argc)
    {
    case 2:
        mode = libexplain_permission_mode_parse_or_die(argv[1], "create arg 2");
        /* fall through... */

    case 1:
        path = argv[0];
        break;

    default:
        fprintf(stderr, "creat: needs 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    libexplain_wrap_and_print
    (
        stdout,
        libexplain_errno_creat(errnum, path, mode)
    );
}
