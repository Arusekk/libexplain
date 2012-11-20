/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010, 2012 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/sys/socket.h>

#include <libexplain/output.h>
#include <libexplain/socketpair.h>


void
explain_socketpair_or_die(int domain, int type, int protocol, int *sv)
{
    if (explain_socketpair_on_error(domain, type, protocol, sv) < 0)
    {
        explain_output_exit_failure();
    }
}


int
explain_socketpair_on_error(int domain, int type, int protocol, int *sv)
{
    int             result;

    result = socketpair(domain, type, protocol, sv);
    if (result < 0)
    {
        int             hold_errno;

        hold_errno = errno;
        explain_output_error
        (
            "%s",
            explain_errno_socketpair(hold_errno, domain, type, protocol, sv)
        );
        errno = hold_errno;
    }
    return result;
}


/* vim: set ts=8 sw=4 et : */
