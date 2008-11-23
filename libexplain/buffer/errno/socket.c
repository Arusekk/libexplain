/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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
#include <libexplain/ac/netdb.h>

#include <libexplain/buffer/dac.h>
#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/enfile.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/socket.h>
#include <libexplain/buffer/socket_domain.h>
#include <libexplain/buffer/socket_protocol.h>
#include <libexplain/buffer/socket_type.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_socket_system_call(libexplain_string_buffer_t *sb,
    int errnum, int domain, int type, int protocol)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "socket(domain = ");
    libexplain_buffer_socket_domain(sb, domain);
    libexplain_string_buffer_puts(sb, ", type = ");
    libexplain_buffer_socket_type(sb, type);
    libexplain_string_buffer_puts(sb, ", protocol = ");
    libexplain_buffer_socket_protocol(sb, protocol);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_socket_explanation(libexplain_string_buffer_t *sb,
    int errnum, int domain, int type, int protocol)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/socket.html
     */
    (void)domain;
    (void)type;
    (void)protocol;
    switch (errnum)
    {
    case EACCES:
    case EPERM:
        libexplain_string_buffer_puts
        (
            sb,
            "the process does not have permission to create a socket of "
            "the specified type and/or protocol"
        );
        if (type == SOCK_RAW || type == SOCK_PACKET)
            libexplain_buffer_dac_net_raw(sb);
        break;

    case EAFNOSUPPORT:
        libexplain_string_buffer_puts
        (
            sb,
            "the operating system does not support the specified "
            "address family"
        );
        break;

    case EINVAL:
        libexplain_string_buffer_puts
        (
            sb,
            "unknown protocol, or protocol family not available"
        );
        break;

    case EMFILE:
        libexplain_buffer_emfile(sb);
        break;

    case ENFILE:
        libexplain_buffer_enfile(sb);
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    case ENOBUFS:
        libexplain_string_buffer_puts
        (
            sb,
            "the socket cannot be created until sufficient resources "
            "are available"
        );
        break;

    case EPROTONOSUPPORT:
        libexplain_string_buffer_puts
        (
            sb,
            "the protocol type or the specified protocol is not "
            "supported within this domain"
        );
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_socket(libexplain_string_buffer_t *sb, int errnum,
    int domain, int type, int protocol)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_socket_system_call
    (
        &exp.system_call_sb,
        errnum,
        domain,
        type,
        protocol
    );
    libexplain_buffer_errno_socket_explanation
    (
        &exp.explanation_sb,
        errnum,
        domain,
        type,
        protocol
    );
    libexplain_explanation_assemble(&exp, sb);
}
