/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>

#include <libexplain/buffer/accept4_flags.h>
#include <libexplain/buffer/eagain.h>
#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/econnaborted.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/ehostdown.h>
#include <libexplain/buffer/ehostunreach.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/enetdown.h>
#include <libexplain/buffer/enetunreach.h>
#include <libexplain/buffer/enfile.h>
#include <libexplain/buffer/enobufs.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/enonet.h>
#include <libexplain/buffer/enoprotoopt.h>
#include <libexplain/buffer/enosr.h>
#include <libexplain/buffer/enosys.h>
#include <libexplain/buffer/enotsock.h>
#include <libexplain/buffer/eperm.h>
#include <libexplain/buffer/eproto.h>
#include <libexplain/buffer/eprotonosupport.h>
#include <libexplain/buffer/erestart.h>
#include <libexplain/buffer/errno/accept4.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/esocktnosupport.h>
#include <libexplain/buffer/etimedout.h>
#include <libexplain/buffer/fildes.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/socklen.h>
#include <libexplain/explanation.h>
#include <libexplain/path_is_efault.h>


static void
explain_buffer_errno_accept4_system_call(explain_string_buffer_t *sb, int
    errnum, int fildes, struct sockaddr *sock_addr, socklen_t *sock_addr_size,
    int flags)
{
    (void)errnum;
    explain_string_buffer_puts(sb, "accept4(fildes = ");
    explain_buffer_fildes(sb, fildes);
    explain_string_buffer_puts(sb, ", sock_addr = ");
    explain_buffer_pointer(sb, sock_addr);
    explain_string_buffer_puts(sb, ", sock_addr_size = ");
    explain_buffer_socklen_star(sb, sock_addr_size);
    explain_string_buffer_puts(sb, ", flags = ");
    explain_buffer_accept4_flags(sb, flags);
    explain_string_buffer_putc(sb, ')');
}


static void
explain_buffer_errno_accept4_explanation(explain_string_buffer_t *sb,
    int errnum, int fildes, struct sockaddr *sock_addr,
    socklen_t *sock_addr_size, int flags)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/accept4.html
     */
    (void)sock_addr;
    (void)flags;
    switch (errnum)
    {
    case EAGAIN:
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
    case EWOULDBLOCK:
#endif
        explain_buffer_eagain_accept(sb);
        break;

    case EBADF:
        explain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case ECONNABORTED:
        explain_buffer_econnaborted(sb);
        break;

    case EFAULT:
        if (explain_pointer_is_efault(sock_addr_size, sizeof(*sock_addr_size)))
            explain_buffer_efault(sb, "sock_addr_size");
        else
            explain_buffer_efault(sb, "sock_addr");
        break;

    case EHOSTDOWN:
        explain_buffer_ehostdown(sb);
        break;

    case EHOSTUNREACH:
        explain_buffer_ehostunreach(sb);
        break;

    case EINTR:
        explain_buffer_eintr(sb, "accept4");
        break;

    case EINVAL:
        if
        (
            !explain_pointer_is_efault
            (
                sock_addr_size,
                sizeof(*sock_addr_size)
            )
        &&
            *sock_addr_size <= 0
        )
        {
            explain_buffer_einval_too_small
            (
                sb,
                "sock_addr_size",
                *sock_addr_size
            );
            break;
        }
        explain_buffer_einval_not_listening(sb);
        break;

    case EMFILE:
        explain_buffer_emfile(sb);
        break;

    case ENETDOWN:
        explain_buffer_enetdown(sb);
        break;

    case ENETUNREACH:
        explain_buffer_enetunreach(sb);
        break;

    case ENFILE:
        explain_buffer_enfile(sb);
        break;

    case ENOBUFS:
        explain_buffer_enobufs(sb);
        break;

    case ENOMEM:
        explain_buffer_enomem_kernel(sb);
        break;

    case ENONET:
        explain_buffer_enonet(sb);
        break;

    case ENOPROTOOPT:
        explain_buffer_enoprotoopt(sb, "flags");
        break;

    case ENOTSOCK:
        explain_buffer_enotsock(sb, fildes, "fildes");
        break;

    case ENOSYS:
#if defined(EOPNOTSUPP) && EOPNOTSUPP != ENOSYS
    case EOPNOTSUPP:
#endif
        explain_buffer_enosys_socket(sb, "accept4", fildes);
        break;

    case ENOSR:
        explain_buffer_enosr(sb);
        break;

    case EPERM:
        explain_buffer_eperm_accept(sb);
        break;

    case EPROTO:
        explain_buffer_eproto_accept(sb, fildes);
        break;

    case EPROTONOSUPPORT:
        explain_buffer_eprotonosupport(sb);
        break;

    case ERESTART:
        explain_buffer_erestart(sb, "accept4");
        break;

    case ESOCKTNOSUPPORT:
        explain_buffer_esocktnosupport(sb, "accept4", fildes);
        break;

    case ETIMEDOUT:
        explain_buffer_etimedout(sb, "accept4");
        break;

    default:
        explain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
explain_buffer_errno_accept4(explain_string_buffer_t *sb, int errnum, int
    fildes, struct sockaddr *sock_addr, socklen_t *sock_addr_size, int flags)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_accept4_system_call(&exp.system_call_sb, errnum,
        fildes, sock_addr, sock_addr_size, flags);
    explain_buffer_errno_accept4_explanation(&exp.explanation_sb, errnum,
        fildes, sock_addr, sock_addr_size, flags);
    explain_explanation_assemble(&exp, sb);
}


/* vim: set ts=8 sw=4 et */
