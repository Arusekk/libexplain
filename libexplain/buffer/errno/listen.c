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
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/socket.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/enotsock.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/listen.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/sockaddr.h>
#include <libexplain/buffer/socket_type.h>
#include <libexplain/explanation.h>
#include <libexplain/option.h>
#include <libexplain/sizeof.h>


static void
libexplain_buffer_errno_listen_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes, int backlog)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "listen(");
    libexplain_string_buffer_printf(sb, "fildes = %d", fildes);
    libexplain_string_buffer_puts(sb, ", ");
    libexplain_string_buffer_printf(sb, "backlog = %d", backlog);
    libexplain_string_buffer_putc(sb, ')');
}


static void
append_getsockname(libexplain_string_buffer_t *sb, int fildes)
{
    struct sockaddr_storage sas;
    struct sockaddr *sa;
    socklen_t sa_len;

    sa = (struct sockaddr *)&sas;
    sa_len = sizeof(sas);
    if (getsockname(fildes, sa, &sa_len) >= 0)
    {
        libexplain_string_buffer_putc(sb, ' ');
        libexplain_buffer_sockaddr(sb, sa, sa_len);
    }
}


#ifdef __linux__
#include <sys/syscall.h>
#include <linux/sysctl.h>
#endif


static int
get_somaxconn(void)
{
#ifdef __linux__
    struct __sysctl_args args;
    long somaxconn;
    size_t somaxconn_size = sizeof(somaxconn);
    int name[] = { CTL_NET, NET_CORE_SOMAXCONN };

    memset(&args, 0, sizeof(struct __sysctl_args));
    args.name = name;
    args.nlen = SIZEOF(name);
    args.oldval = &somaxconn;
    args.oldlenp = &somaxconn_size;

    if (syscall(SYS__sysctl, &args) >= 0)
        return somaxconn;
#endif
#ifdef SOMAXCONN
    return SOMAXCONN;
#else
    return -1;
#endif
}


static void
libexplain_buffer_errno_listen_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes, int backlog)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/listen.html
     */
    switch (errnum)
    {
    case EADDRINUSE:
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used to explain an EADDRINUSE error.
             */
            i18n("another socket is already listening on the same port")
        );
        append_getsockname(sb, fildes);
        break;

    case EBADF:
        libexplain_buffer_ebadf(sb, fildes, "fildes");
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
            "listen(2) system call")
        );
        libexplain_buffer_socket_type_from_fildes(sb, fildes);
        break;

    case EDESTADDRREQ:
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used to explain an EDESTADDRREQ
             * error returned by a listen(2) system call.
             */
            i18n("the socket is not bound to a local address, and the "
            "protocol does not support listening on an unbound socket")
        );
        append_getsockname(sb, fildes);
        libexplain_buffer_socket_type_from_fildes(sb, fildes);
        break;

    case EINVAL:
        /* FIXME: which is it? */
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used to explain an EINVAL
             * error returned by a listen(2) system call.
             */
            i18n("the socket is already connected, or the socket has "
            "been shut down")
        );
        append_getsockname(sb, fildes);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
    if (libexplain_option_dialect_specific())
    {
        int             somaxconn;

        somaxconn = get_somaxconn();
        if (somaxconn > 0 && backlog > somaxconn)
        {
            libexplain_string_buffer_puts(sb, "; ");
            libexplain_string_buffer_printf_gettext
            (
                sb,
                /*
                 * xgettext: This message is used to inform users of the
                 * listen(2) system call when they specify a backlog
                 * valie in excess of SOMAXCONN.
                 *
                 * %1$d => The maximum queue length for completely
                 *         established sockets waiting to be accepted.
                 */
                i18n("large backlog values are silently truncated to the "
                    "system maximum (SOMAXCONN, %d)"),
                somaxconn
            );
        }
    }
}


void
libexplain_buffer_errno_listen(libexplain_string_buffer_t *sb, int errnum,
    int fildes, int backlog)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_listen_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        backlog
    );
    libexplain_buffer_errno_listen_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        backlog
    );
    libexplain_explanation_assemble(&exp, sb);
}
