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
#include <libexplain/ac/fcntl.h>

#include <libexplain/buffer/does_not_have_inode_modify_permission.h>
#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/futimes.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/fildes_not_open_for_writing.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/timeval.h>
#include <libexplain/explanation.h>
#include <libexplain/path_is_efault.h>


static void
libexplain_buffer_errno_futimes_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes, const struct timeval *tv)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "futimes(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_puts(sb, ", tv = ");
    if (libexplain_pointer_is_efault(tv, sizeof(*tv)))
    {
        libexplain_buffer_pointer(sb, tv);
    }
    else
    {
        libexplain_string_buffer_putc(sb, '{');
        libexplain_buffer_timeval(sb, &tv[0]);
        libexplain_string_buffer_puts(sb, ", ");
        libexplain_buffer_timeval(sb, &tv[1]);
        libexplain_string_buffer_putc(sb, '}');
    }
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_futimes_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes, const struct timeval *tv)
{
    (void)tv;
    switch (errnum)
    {
    case EBADF:
        if (libexplain_buffer_fildes_not_open_for_writing(sb, fildes, "fildes"))
            libexplain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "tv");
        break;

    case EACCES:
    case EPERM:
        /*
         * If tv is not NULL, times are changed as given,
         * but you need inode modify permission
         */
        libexplain_buffer_does_not_have_inode_modify_permission_fd
        (
            sb,
            fildes,
            "fildes"
        );
        break;

    case EROFS:
        libexplain_buffer_erofs_fildes(sb, fildes, "fildes");
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_futimes(libexplain_string_buffer_t *sb, int errnum,
    int fildes, const struct timeval *tv)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_futimes_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        tv
    );
    libexplain_buffer_errno_futimes_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        tv
    );
    libexplain_explanation_assemble(&exp, sb);
}

/* vim:ts=8:sw=4:et */
