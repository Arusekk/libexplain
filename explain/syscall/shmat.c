/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2011 Peter Miller
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

#include <libexplain/shmat.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/shmat.h>


void
explain_syscall_shmat(int errnum, int argc, char **argv)
{
    int             shmid;
    const void      *shmaddr;
    int             shmflg;

    if (argc != 3)
    {
        fprintf(stderr, "shmat: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    shmid = explain_string_to_int(argv[0]);
    shmaddr = explain_string_to_pointer(argv[1]);
    shmflg = explain_string_to_int(argv[2]);

    explain_wrap_and_print(stdout, explain_errno_shmat(errnum, shmid, shmaddr,
        shmflg));
}


/* vim: set ts=8 sw=4 et */
