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

#include <libexplain/buffer/errno/fclose.h>
#include <libexplain/buffer/errno/fflush.h>
#include <libexplain/buffer/errno/fopen.h>
#include <libexplain/buffer/errno/freopen.h>
#include <libexplain/buffer/strerror.h>
#include <libexplain/buffer/success.h>


static void
libexplain_buffer_errno_freopen_inner(struct libexplain_string_buffer_t *sb,
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
    libexplain_string_buffer_puts(sb, " failed, ");
    libexplain_buffer_strerror(sb, errnum);

    libexplain_string_buffer_puts
    (
        sb,
        "; note that while the FILE stream is no longer valid, the "
        "underlying file descriptor may still be open"
    );
}


void
libexplain_buffer_errno_freopen(struct libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, const char *flags, FILE *fp)
{
    /*
     * The freopen function can be thought of as an fclose followed by
     * an fopen, done with some libc-internal sleight of hand.
     *
     * The Linux fclose(3) man page says
     *
     *     "RETURN VALUE:  Upon successful completion 0 is returned.
     *     Otherwise, EOF is returned and the global variable errno is
     *     set to indicate the error.  In either case any further access
     *     (including another call to fclose()) to the stream results in
     *     undefined behavior."
     *
     * which is interesting because if close(2) fails, the file
     * descriptor is usually still open.  Thus, we make an attempt
     * to recover the file descriptor, to see if we can produce some
     * additional information.
     *
     * If you are using glibc you are plain out of luck, because
     * it very carefully assigns -1 to the file descriptor member.
     * Other implementations may not be so careful, indeed other
     * implementations may keep the FILE pointer valid if the underlying
     * file descriptor is still valid.
     */
    int fildes = -1;
    if (fp)
        fildes = fileno(fp);
    if (fildes >= 0 && fcntl(fildes, F_GETFL) < 0)
        fildes = -1;

    /*
     * If possible, try to figure out what went wrong based on the error
     * number.  It could be any of the errors from fflush, fclose or
     * fopen.
     */
    if (fildes >= 0)
    {
        switch (errnum)
        {
        case EFAULT:
        case EFBIG:
        case ENOSPC:
        case EPIPE:
            libexplain_buffer_errno_fflush(sb, errnum, fp);
            return;

        case EINTR:
        case EIO:
        case EBADF:
            libexplain_buffer_errno_fclose(sb, errnum, fp);
            return;

        case EINVAL:
        case ENOMEM:
            libexplain_buffer_errno_fopen(sb, errnum, pathname, flags);
            break;

        default:
            break;
        }
    }

    libexplain_buffer_errno_freopen_inner(sb, errnum, pathname, flags, fp);
}
