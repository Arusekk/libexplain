/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/mman.h>

#include <libexplain/munmap.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/munmap.h>


void
explain_syscall_munmap(int errnum, int argc, char **argv)
{
    void            *data;
    size_t          data_size;

    if (argc != 2)
    {
        fprintf(stderr, "munmap: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    data = explain_string_to_pointer(argv[0]);
    data_size = explain_string_to_size_t(argv[1]);

    explain_wrap_and_print(stdout, explain_errno_munmap(errnum, data,
        data_size));
}


/* vim: set ts=8 sw=4 et */