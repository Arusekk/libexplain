/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/buffer/tcflow_action.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/tcflow.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/tcflow.h>


void
explain_syscall_tcflow(int errnum, int argc, char **argv)
{
    int             fildes;
    int             action;

    if (argc != 2)
    {
        fprintf(stderr, "tcflow: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    fildes = explain_string_to_int(argv[0]);
    action = explain_parse_tcflow_action_or_die(argv[1], "argv[1]");

    explain_wrap_and_print(stdout, explain_errno_tcflow(errnum, fildes,
        action));
}


/* vim: set ts=8 sw=4 et */
