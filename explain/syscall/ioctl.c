/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010 Peter Miller
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
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/mtio.h>

#include <libexplain/iocontrol.h>
#include <libexplain/ioctl.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/ioctl.h>


void
explain_syscall_ioctl(int errnum, int argc, char **argv)
{
    int             fildes;
    int             request;
    char            dummy_data[10000];
    void            *data;

    memset(dummy_data, 0, sizeof(dummy_data));
    data = dummy_data;
    switch (argc)
    {
    case 3:
        data = (void *)explain_strtol_or_die(argv[2], 0, 0);
        /* fall through... */

    case 2:
        request = explain_parse_ioctl_request_or_die(argv[1]);
        fildes = explain_strtol_or_die(argv[0], 0, 0);
        break;

    default:
        fprintf(stderr, "ioctl: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

#ifdef MTIOCTOP
    if (request == (int)MTIOCTOP && data == dummy_data)
    {
        struct mtop *p = data;
        p->mt_op = MTFSF;
    }
#endif
    explain_wrap_and_print
    (
        stdout,
        explain_errno_ioctl(errnum, fildes, request, data)
    );
}

/* vim:ts=8:sw=4:et */
