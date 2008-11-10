/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/fcntl.h>

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/errno/fclose.h>
#include <libexplain/buffer/errno/fflush.h>
#include <libexplain/buffer/errno/fopen.h>
#include <libexplain/buffer/errno/freopen.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/success.h>


void
libexplain_buffer_errno_freopen(struct libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, const char *flags, FILE *fp)
{
    libexplain_string_buffer_puts(sb, "freopen(pathname = ");
    libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_puts(sb, ", flags = ");
    libexplain_string_buffer_puts_quoted(sb, flags);
    libexplain_string_buffer_printf(sb, ", fp = %p)", fp);
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_buffer_failed(sb, errnum);

    switch (errnum)
    {
    case EFAULT:
    case EFBIG:
    case ENOSPC:
    case EPIPE:
        libexplain_buffer_errno_fflush_because(sb, errnum, fp);
        return;

    case EBADF:
        libexplain_buffer_ebadf(sb, "fp");
        break;

    case EINTR:
    case EIO:
        libexplain_buffer_errno_fclose_because(sb, errnum, fp);
        return;

    default:
        libexplain_buffer_errno_fopen_because(sb, errnum, pathname, flags);
        break;
    }

    libexplain_string_buffer_puts
    (
        sb,
        "; note that while the FILE stream is no longer valid, the "
        "underlying file descriptor may still be open"
    );
}
