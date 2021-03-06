/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2013 Peter Miller
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
#include <libexplain/ac/sys/types.h>

#include <libexplain/buffer/lseek_whence.h>
#include <libexplain/fseeko.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/fseeko.h>


void
explain_syscall_fseeko(int errnum, int argc, char **argv)
{
    FILE            *fp;
    off_t           offset;
    int             whence;

    fp = 0;
    offset = 0;
    whence = SEEK_SET;
    switch (argc)
    {
    default:
        explain_output_error_and_die("fseeko: requires 3 arguments, not %d\n",
            argc);

    case 3:
        whence = explain_lseek_whence_parse_or_die(argv[2], "arg 3");
        /* fall through... */

    case 2:
        offset = explain_parse_off_t_or_die(argv[1]);
        /* fall through... */

    case 1:
        fp = explain_parse_stream_or_die(argv[0], "r");
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_fseeko(errnum, fp, offset,
        whence));
}


/* vim: set ts=8 sw=4 et : */
