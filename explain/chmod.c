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

#include <libexplain/chmod.h>
#include <libexplain/permission_mode.h>
#include <libexplain/wrap_and_print.h>

#include <explain/chmod.h>


void
explain_chmod(int errnum, int argc, char **argv)
{
    const char      *pathname;
    int             mode;

    pathname = 0;
    mode = 0644;
    switch (argc)
    {
    case 0:
        fprintf(stderr, "chmod: no pathname given\n");
        exit(1);

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
            exit(1);
        }
        /* fall through... */

    case 1:
        pathname = argv[0];
        break;

    default:
        fprintf(stderr, "chmod: too many arguments given\n");
        exit(1);
    }

    libexplain_wrap_and_print
    (
        stdout,
        libexplain_errno_chmod(errnum, pathname, mode)
    );
}
