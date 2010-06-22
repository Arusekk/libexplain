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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/read.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/read.h>


void
explain_syscall_read(int errnum, int argc, char **argv)
{
    int fildes = -1;
    void *data = 0;
    size_t data_size = 0;
    switch (argc)
    {
    case 3:
        data_size = explain_string_to_long(argv[2]);
        /* fall through... */

    case 2:
        data = explain_string_to_pointer(argv[1]);
        /* fall through... */

    case 1:
        fildes = explain_string_to_long(argv[0]);
        break;

    default:
        fprintf(stderr, "read: needs 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_read(errnum, fildes, data, data_size)
    );
}
