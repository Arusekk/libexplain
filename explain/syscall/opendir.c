/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010 Peter Miller
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

#include <libexplain/ac/dirent.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/opendir.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/opendir.h>


void
explain_syscall_opendir(int errnum, int argc, char **argv)
{
    const char      *pathname;

    pathname = 0;
    switch (argc)
    {
    case 0:
        fprintf(stderr, "opendir: no path given\n");
        exit(EXIT_FAILURE);

    case 1:
        pathname = argv[0];
        break;

    default:
        fprintf(stderr, "opendir: too many arguments given\n");
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_opendir(errnum, pathname)
    );
}
