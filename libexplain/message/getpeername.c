/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/sys/socket.h>

#include <libexplain/getpeername.h>


void
explain_message_getpeername(char *message, int message_size, int fildes,
    struct sockaddr *sock_addr, socklen_t *sock_addr_size)
{
    explain_message_errno_getpeername
    (
        message,
        message_size,
        errno,
        fildes,
        sock_addr,
        sock_addr_size
    );
}

/* vim: set ts=8 sw=4 et : */
