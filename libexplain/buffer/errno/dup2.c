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

#include <libexplain/buffer/check_fildes_range.h>
#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/dup2.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_dup2_system_call(libexplain_string_buffer_t *sb,
    int errnum, int oldfd, int newfd)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "dup2(oldfd = %d", oldfd);
    libexplain_buffer_fildes_to_pathname(sb, oldfd);
    libexplain_string_buffer_printf(sb, ", newfd = %d", newfd);
    libexplain_buffer_fildes_to_pathname(sb, newfd);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_dup2_explanation(libexplain_string_buffer_t *sb,
    int errnum, int oldfd, int newfd)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/dup2.html
     */
    switch (errnum)
    {
    case EBADF:
        if (fcntl(oldfd, F_GETFL) < 0)
        {
            libexplain_buffer_ebadf(sb, oldfd, "oldfd");
        }
        else if (libexplain_buffer_check_fildes_range(sb, newfd, "newfd"))
        {
            libexplain_string_buffer_puts
            (
                sb,
                "oldfd isn't an open file descriptor; or, newfd is "
                "outside the allowed range for file descriptors"
            );
        }
        break;

    case EINVAL:
       /* not on Linux */
       libexplain_buffer_check_fildes_range(sb, newfd, "newfd");
       break;

    case EBUSY:
        libexplain_string_buffer_puts
        (
            sb,
            "a race condition was detected between open(2) and dup(2)"
        );
        break;

    case EINTR:
        libexplain_buffer_eintr(sb, "dup2");
        break;

    case EMFILE:
        libexplain_buffer_emfile(sb);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
    if (fcntl(newfd, F_GETFL) >= 0)
    {
        libexplain_string_buffer_puts
        (
            sb,
            "; any errors that would have been reported by close(newfd) "
            "are lost, a careful programmer will not use dup2() without "
            "closing newfd first"
        );
    }
}


void
libexplain_buffer_errno_dup2(libexplain_string_buffer_t *sb, int errnum,
    int oldfd, int newfd)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_dup2_system_call
    (
        &exp.system_call_sb,
        errnum,
        oldfd,
        newfd
    );
    libexplain_buffer_errno_dup2_explanation
    (
        &exp.explanation_sb,
        errnum,
        oldfd,
        newfd
    );
    libexplain_explanation_assemble(&exp, sb);
}
