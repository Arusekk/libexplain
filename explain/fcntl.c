/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/fcntl.h>
#include <libexplain/buffer/errno/fcntl.h>
#include <libexplain/open_flags.h>
#include <libexplain/wrap_and_print.h>

#include <explain/fcntl.h>
#include <explain/strtol_or_die.h>


void
explain_fcntl(int errnum, int argc, char **argv)
{
    int             fildes;
    int             command;

    if (argc < 2)
    {
        fprintf(stderr, "fcntl: at least 2 arguments required\n");
        exit(1);
    }
    fildes = strtol_or_die(argv[0]);
    command = libexplain_fcntl_command_parse(argv[1]);
    if (command < 0)
    {
        fprintf
        (
            stderr,
            "fcntl: unable to interpret \"%s\" as a command\n",
            argv[1]
        );
        exit(1);
    }
    switch (command)
    {
    default:
        if (argc > 2)
        {
            fprintf(stderr, "fcntl: need 2 arguments (not %d)\n", argc);
            exit(1);
        }
        libexplain_wrap_and_print
        (
            stdout,
            libexplain_errno_fcntl(errnum, fildes, command, 0)
        );
        break;

    case F_SETSIG: /* FIXME: parse signal */
    case F_NOTIFY: /* FIXME: parse notify bits */
    case F_SETLEASE: /* FIXME: parse lease bits */
    case F_DUPFD:
    case F_DUPFD_CLOEXEC:
    case F_SETFD:
    case F_SETOWN:
        {
            long            arg;

            if (argc != 3)
            {
                fprintf(stderr, "fcntl: need 3 arguments (not %d)\n", argc);
                exit(1);
            }
            arg = strtol_or_die(argv[2]);
            libexplain_wrap_and_print
            (
                stdout,
                libexplain_errno_fcntl(errnum, fildes, command, arg)
            );
        }
        break;

    case F_SETFL:
        {
            long            arg;

            if (argc != 3)
            {
                fprintf(stderr, "fcntl: need 3 arguments (not %d)\n", argc);
                exit(1);
            }
            arg = libexplain_open_flags_parse(argv[2]);
            if (arg < 0)
            {
                fprintf
                (
                    stderr,
                    "argument \"%s\" does not look like open flags\n",
                    argv[2]
                );
                exit(1);
            }
            libexplain_wrap_and_print
            (
                stdout,
                libexplain_errno_fcntl(errnum, fildes, command, arg)
            );
        }
        break;

    case F_GETLK:
    case F_SETLK:
    case F_SETLKW:
        {
            static struct flock f_zero;
            struct flock    f;

            if (argc > 2)
            {
                fprintf(stderr, "fcntl: need 2 arguments (not %d)\n", argc);
                exit(1);
            }
            f = f_zero;
            libexplain_wrap_and_print
            (
                stdout,
                libexplain_errno_fcntl(errnum, fildes, command, (long)&f)
            );
        }
        break;

#if defined(F_GETLK64) && F_GETLK != F_GETLK64
    case F_GETLK64:
    case F_SETLK64:
    case F_SETLKW64:
        {
            static struct flock64 f_zero;
            struct flock64  f;

            if (argc != 2)
            {
                fprintf(stderr, "fcntl: need 2 arguments (not %d)", argc);
                exit(1);
            }
            f = f_zero;
            libexplain_wrap_and_print
            (
                stdout,
                libexplain_errno_fcntl(errnum, fildes, command, (long)&f)
            );
        }
        break;
#endif
    }
}
