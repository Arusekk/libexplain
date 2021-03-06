/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/buffer/setvbuf_mode.h>
#include <libexplain/setvbuf.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/setvbuf.h>


void
explain_syscall_setvbuf(int errnum, int argc, char **argv)
{
    FILE            *fp;
    char            *data;
    int             mode;
    size_t          size;

    if (argc != 4)
    {
        fprintf(stderr, "setvbuf: requires 4 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    fp = explain_parse_pointer_or_die(argv[0]);
    data = explain_parse_pointer_or_die(argv[1]);
    mode = explain_parse_setvbuf_mode_or_die(argv[2], "arg 3");
    size = explain_parse_long_or_die(argv[3]);

    explain_wrap_and_print(stdout, explain_errno_setvbuf(errnum, fp, data, mode,
        size));
}


/* vim: set ts=8 sw=4 et : */
