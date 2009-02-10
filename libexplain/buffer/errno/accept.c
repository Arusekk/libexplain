/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/enfile.h>
#include <libexplain/buffer/enotsock.h>
#include <libexplain/buffer/errno/accept.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/socket_type.h>
#include <libexplain/buffer/socklen.h>
#include <libexplain/explanation.h>
#include <libexplain/path_is_efault.h>


static void
libexplain_buffer_errno_accept_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes, struct sockaddr *sock_addr,
    socklen_t *sock_addr_size)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "accept(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_puts(sb, ", sock_addr = ");
    libexplain_buffer_pointer(sb, sock_addr);
    libexplain_string_buffer_printf(sb, ", sock_addr_size = ");
    libexplain_buffer_socklen_star(sb, sock_addr_size);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_accept_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes, struct sockaddr *sock_addr,
    socklen_t *sock_addr_size)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/accept.html
     */
    switch (errnum)
    {
    case EAGAIN:
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
    case EWOULDBLOCK:
#endif
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This message is used to explain an EAGAIN
             * error reprted by an accept(2) system call, in the case
             * where the socket is marked non-blocking (O_NONBLOCK) and
             * no connections are waiting to be accepted.
             */
            i18n("the socket is marked non-blocking and no connections "
            "are present to be accepted")
        );
        break;

    case EBADF:
        libexplain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case ECONNABORTED:
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This message is used to explan an ECONNABORTED
             * error reported by the accept(2) system call, in the case
             * where an incoming connection has been aborted by the
             * remote host.
             */
            i18n("an incoming connection has been aborted by the remote host")
        );
        break;

    case EINTR:
        libexplain_buffer_eintr(sb, "accept");
        break;

    case EINVAL:
        if
        (
            !libexplain_pointer_is_efault
            (
                sock_addr_size,
                sizeof(*sock_addr_size)
            )
        &&
            *sock_addr_size <= 0
        )
        {
            libexplain_buffer_einval_too_small
            (
                sb,
                "sock_addr_size",
                *sock_addr_size
            );
            break;
        }
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used to explain an EINVAL error
             * reported by the accept(2) system call, in the case where
             * the file descriptor is actually a socket, but is not in a
             * state that permits the use of the accept(2) system call.
             */
            i18n("the socket is not listening for connections")
        );
        break;

    case EMFILE:
        libexplain_buffer_emfile(sb);
        break;

    case ENFILE:
        libexplain_buffer_enfile(sb);
        break;

    case ENOTSOCK:
        libexplain_buffer_enotsock(sb, fildes, "fildes");
        break;

    case EOPNOTSUPP:
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used to explain an EOPNOTSUPP
             * error returned by a listen(2) system call.
             */
            i18n("the socket is not of a type that supports the "
            "accept(2) system call")
        );
        libexplain_buffer_socket_type_from_fildes(sb, fildes);
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
        if (libexplain_pointer_is_efault(sock_addr, *sock_addr_size))
        {
            libexplain_buffer_efault(sb, "sock_addr");
            break;
        }
        break;

    case EPERM:
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used to explain an EPERM error
             * reported by an accept(2) system call, in the case where
             * firewall rules forbid connection.
             */
            i18n("firewall rules forbid connection")
        );
        break;

    case EPROTO:
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used to explain and EPROTO
             * error reported by an accept(2) system call, in the case
             * where a protocol error has occurred.
             */
            i18n("a protocol error has occurred; for example, the "
            "STREAMS protocol stack has not been initialized")
        );
        break;

    /*
     * Linux accept(2) says
     *
     * "In addition, network errors for the new socket and as defined
     * for the protocol may be returned.  Various Linux kernels
     * can return other errors such as ENOSR, ESOCKTNOSUPPORT,
     * EPROTONOSUPPORT, ETIMEDOUT.  The value ERESTARTSYS may be seen
     * during a trace."
     */

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_accept(libexplain_string_buffer_t *sb, int errnum,
    int fildes, struct sockaddr *sock_addr, socklen_t *sock_addr_size)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_accept_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        sock_addr,
        sock_addr_size
    );
    libexplain_buffer_errno_accept_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        sock_addr,
        sock_addr_size
    );
    libexplain_explanation_assemble(&exp, sb);
}
