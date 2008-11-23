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
#include <libexplain/ac/sys/wait.h>

#include <libexplain/buffer/waitpid_options.h>
#include <libexplain/strtol_or_die.h>
#include <libexplain/wait4.h>
#include <libexplain/wrap_and_print.h>

#include <explain/wait4.h>


void
explain_wait4(int errnum, int argc, char **argv)
{
    int             pid;
    int             status_dummy;
    int             *status;
    int             options;
    struct rusage   rusage_dummy;
    struct rusage   *rusage;

    pid = -1;
    status_dummy = 0;
    status = &status_dummy;
    options = 0;
    rusage = &rusage_dummy;
    switch (argc)
    {
    case 4:
        rusage = (struct rusage *)libexplain_strtol_or_die(argv[3]);
        /* fall through... */

    case 3:
        options = libexplain_parse_waitpid_options(argv[2]);
        if (options < 0)
        {
            fprintf
            (
                stderr,
                "argument \"%s\" does not look like a waitpid options value",
                argv[2]
            );
            exit(EXIT_FAILURE);
        }
        /* fall through... */

    case 2:
        status = (int *)libexplain_strtol_or_die(argv[1]);
        /* fall through... */

    case 1:
        pid = libexplain_strtol_or_die(argv[0]);
        break;

    default:
        fprintf(stderr, "wait4: requires 4 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    libexplain_wrap_and_print
    (
        stdout,
        libexplain_errno_wait4(errnum, pid, status, options, rusage)
    );
}
