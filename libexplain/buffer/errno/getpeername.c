/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/enobufs.h>
#include <libexplain/buffer/enotconn.h>
#include <libexplain/buffer/enotsock.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/getpeername.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/socklen.h>
#include <libexplain/explanation.h>
#include <libexplain/path_is_efault.h>


static void
libexplain_buffer_errno_getpeername_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes, struct sockaddr *sock_addr,
    socklen_t *sock_addr_size)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "getpeername(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_puts(sb, ", sock_addr = ");
    libexplain_buffer_pointer(sb, sock_addr);
    libexplain_string_buffer_puts(sb, ", sock_addr_size = ");
    libexplain_buffer_socklen_star(sb, sock_addr_size);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_getpeername_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes, struct sockaddr *sock_addr,
    socklen_t *sock_addr_size)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/getpeername.html
     */
    switch (errnum)
    {
    case EBADF:
        libexplain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case EFAULT:
        if
        (
            libexplain_pointer_is_efault
            (
                sock_addr_size,
                sizeof(*sock_addr_size)
            )
        )
        {
            libexplain_buffer_efault(sb, "sock_addr_size");
            break;
        }
        if
        (
            *sock_addr_size > 0
        &&
            libexplain_pointer_is_efault(sock_addr, *sock_addr_size)
        )
        {
            libexplain_buffer_efault(sb, "sock_addr");
            break;
        }
        goto dunno;

    case EINVAL:
        if
        (
            libexplain_pointer_is_efault
            (
                sock_addr_size,
                sizeof(*sock_addr_size)
            )
        )
            goto dunno;
        libexplain_buffer_einval_too_small
        (
            sb,
            "sock_addr_size",
            *sock_addr_size
        );
        break;

    case ENOBUFS:
        libexplain_buffer_enobufs(sb);
        break;

    case ENOTCONN:
        libexplain_buffer_enotconn(sb, "fildes");
        break;

    case ENOTSOCK:
        libexplain_buffer_enotsock(sb, fildes, "fildes");
        break;

    default:
        dunno:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_getpeername(libexplain_string_buffer_t *sb, int errnum,
    int fildes, struct sockaddr *sock_addr, socklen_t *sock_addr_size)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_getpeername_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        sock_addr,
        sock_addr_size
    );
    libexplain_buffer_errno_getpeername_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        sock_addr,
        sock_addr_size
    );
    libexplain_explanation_assemble(&exp, sb);
}

/* vim:ts=8:sw=4:et */
