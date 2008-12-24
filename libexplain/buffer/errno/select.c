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
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/select.h>
#include <libexplain/buffer/fd_set.h>
#include <libexplain/buffer/timeval.h>
#include <libexplain/explanation.h>
#include <libexplain/option.h>
#include <libexplain/path_is_efault.h>


static void
libexplain_buffer_errno_select_system_call(libexplain_string_buffer_t *sb,
    int errnum, int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
    struct timeval *timeout)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "select(nfds = %d", nfds);
    libexplain_string_buffer_puts(sb, ", readfds = ");
    libexplain_buffer_fd_set(sb, nfds, readfds);
    libexplain_string_buffer_puts(sb, ", writefds = ");
    libexplain_buffer_fd_set(sb, nfds, writefds);
    libexplain_string_buffer_puts(sb, ", exceptfds = ");
    libexplain_buffer_fd_set(sb, nfds, exceptfds);
    libexplain_string_buffer_puts(sb, ", timeout = ");
    libexplain_buffer_timeval(sb, timeout);
    libexplain_string_buffer_putc(sb, ')');
}


static int
file_descriptor_is_open(int fildes)
{
    int             err;

    err = fcntl(fildes, F_GETFL);
    return (err >= 0);
}


static void
libexplain_buffer_errno_select_explanation(libexplain_string_buffer_t *sb,
    int errnum, int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
    struct timeval *timeout)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/select.html
     */
    switch (errnum)
    {
    case EBADF:
        {
            int             fildes;

            for (fildes = 0; fildes < nfds; ++fildes)
            {
                if
                (
                    (
                        (readfds && FD_ISSET(fildes, readfds))
                    ||
                        (writefds && FD_ISSET(fildes, writefds))
                    ||
                        (exceptfds && FD_ISSET(fildes, exceptfds))
                    )
                &&
                    !file_descriptor_is_open(fildes)
                )
                    break;
            }
            if (fildes < nfds)
            {
                char            caption[40];

                snprintf(caption, sizeof(caption), "fildes %d", fildes);
                libexplain_buffer_ebadf(sb, fildes, caption);
            }
            else
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "an invalid file descriptor was given in one of the sets; "
                    "perhaps a file descriptor that was already closed, or one "
                    "on which an error has occurred"
                );
            }
        }
        break;

    case EFAULT:
        if (readfds && libexplain_pointer_is_efault(readfds))
        {
            libexplain_buffer_efault(sb, "readfds");
            break;
        }
        if (writefds && libexplain_pointer_is_efault(writefds))
        {
            libexplain_buffer_efault(sb, "writefds");
            break;
        }
        if (exceptfds && libexplain_pointer_is_efault(exceptfds))
        {
            libexplain_buffer_efault(sb, "exceptfds");
            break;
        }
        if (timeout && libexplain_pointer_is_efault(timeout))
        {
            libexplain_buffer_efault(sb, "timeout");
            break;
        }
        break;

    case EINTR:
        libexplain_buffer_eintr(sb, "select");
        break;

    case EINVAL:
        if (nfds < 0)
        {
            libexplain_string_buffer_puts(sb, "nfds is negative");
            break;
        }
        if (nfds > FD_SETSIZE)
        {
            libexplain_string_buffer_puts
            (
                sb,
                "nfds is greater than FD_SETSIZE"
            );
            if (libexplain_option_dialect_specific())
                libexplain_string_buffer_printf(sb, " (%d)", FD_SETSIZE);
            break;
        }
        if (timeout && (timeout->tv_sec < 0 || timeout->tv_usec < 0))
        {
            libexplain_string_buffer_puts(sb, "timeout is invalid");
            break;
        }
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_select(libexplain_string_buffer_t *sb, int errnum,
    int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
    struct timeval *timeout)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_select_system_call
    (
        &exp.system_call_sb,
        errnum,
        nfds,
        readfds,
        writefds,
        exceptfds,
        timeout
    );
    libexplain_buffer_errno_select_explanation
    (
        &exp.explanation_sb,
        errnum,
        nfds,
        readfds,
        writefds,
        exceptfds,
        timeout
    );
    libexplain_explanation_assemble(&exp, sb);
}
