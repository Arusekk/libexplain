/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/string_to_thing.h>
#include <libexplain/strtoull.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/strtoull.h>


void
explain_syscall_strtoull(int errnum, int argc, char **argv)
{
    const char      *nptr;
    char            *ep;
    char            **endptr;
    int             base;

    nptr = 0;
    ep = 0;
    endptr = &ep;
    base = 0;
    switch (argc)
    {
    case 1:
        nptr = argv[0];
        break;

    case 2:
        nptr = argv[0];
        base = explain_string_to_int(argv[1]);
        break;

    case 3:
        nptr = argv[0];
        endptr = explain_string_to_pointer(argv[0]);
        base = explain_string_to_int(argv[2]);
        break;

    default:
        fprintf(stderr, "strtoull: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    explain_wrap_and_print(stdout, explain_errno_strtoull(errnum, nptr, endptr,
        base));
}


/* vim: set ts=8 sw=4 et */