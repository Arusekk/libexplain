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
#include <libexplain/ac/sys/ptrace.h>

#include <libexplain/buffer/signal.h>
#include <libexplain/buffer/ptrace_request.h>
#include <libexplain/buffer/ptrace_options.h>
#include <libexplain/ptrace.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/ptrace.h>


void
explain_syscall_ptrace(int errnum, int argc, char **argv)
{
    int             request;
    pid_t           pid;
    void            *addr;
    void            *data;

    if (argc < 2)
    {
        oops:
        exit(EXIT_FAILURE);
    }
    request = explain_parse_ptrace_request_or_die(argv[0], "arg 1");
    pid = 0;
    addr = 0;
    data = 0;
    switch (argc)
    {
    case 4:
        switch (request)
        {
        case PTRACE_CONT:
        case PTRACE_SYSCALL:
        case PTRACE_SINGLESTEP:
#ifdef PTRACE_SYSEMU
        case PTRACE_SYSEMU:
#endif
#ifdef PTRACE_SYSEMU_SINGLESTEP
        case PTRACE_SYSEMU_SINGLESTEP:
#endif
        case PTRACE_DETACH:
            data = (void *)(long)explain_signal_parse_or_die(argv[3], "arg 4");
            break;

        case PTRACE_SETOPTIONS:
#ifdef PTRACE_OLDSETOPTIONS
        case PTRACE_OLDSETOPTIONS:
#endif
            data =
                (void *)explain_parse_ptrace_options_or_die(argv[3], "arg 4");
            break;

        case PTRACE_POKETEXT:
        case PTRACE_POKEDATA:
        case PTRACE_POKEUSER:
            data = (void *)explain_strtol_or_die(argv[3], 0, 0);
            break;

        default:
            data = explain_string_to_pointer(argv[3]);
            break;
        }
        /* Fall through... */

    case 3:
        addr = explain_string_to_pointer(argv[2]);
        /* Fall through... */

    case 2:
        pid = explain_strtol_or_die(argv[1], 0, 0);
        /* Fall through... */

    case 1:
        break;

    default:
        goto oops;
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_ptrace(errnum, request, pid, addr, data)
    );
}


/* vim: set ts=8 sw=4 et */
