/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/errno/close.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/explanation.h>
#include <libexplain/string_buffer.h>


static void
libexplain_buffer_errno_close_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "close(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_putc(sb, ')');
}


void
libexplain_buffer_errno_close_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes)
{
    switch (errnum)
    {
    case EBADF:
        libexplain_buffer_ebadf(sb, "fildes");
        break;

    case EINTR:
        libexplain_buffer_eintr(sb, "close");
        libexplain_string_buffer_puts
        (
            sb,
            "; note that the file descriptor is still open"
        );
        break;

    case EIO:
        libexplain_buffer_eio_fildes(sb, fildes);
        break;

    case EWOULDBLOCK:
        libexplain_string_buffer_puts
        (
            sb,
            "the O_NONBLOCK flag was specified, and an "
            "operation has yet to complete"
        );
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }

    /*
     * See of the file is still open, and tell the user if it is.
     */
    if (fcntl(fildes, F_GETFL) >= 0)
    {
        libexplain_string_buffer_puts
        (
            sb,
            "; note the file descriptor is still open"
        );
    }
}


void
libexplain_buffer_errno_close(libexplain_string_buffer_t *sb, int errnum,
    int fildes)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_close_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes
    );
    libexplain_buffer_errno_close_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes
    );
    libexplain_explanation_assemble(&exp, sb);
}
