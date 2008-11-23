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

#include <libexplain/mkdir.h>
#include <libexplain/permission_mode.h>
#include <libexplain/wrap_and_print.h>

#include <explain/mkdir.h>


void
explain_mkdir(int errnum, int argc, char **argv)
{
    const char      *pathname;
    int             mode;

    mode = 0777;
    switch (argc)
    {
    default:
        fprintf(stderr, "mkdir: requires 1 argument, not %d\n", argc);
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
        pathname = argv[0];
        break;
    }

    libexplain_wrap_and_print
    (
        stdout,
        libexplain_errno_mkdir(errnum, pathname, mode)
    );
}
