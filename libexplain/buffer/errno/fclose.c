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

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/errno/close.h>
#include <libexplain/buffer/errno/fclose.h>
#include <libexplain/buffer/errno/write.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/success.h>


void
libexplain_buffer_errno_fclose(libexplain_string_buffer_t *sb, int errnum,
    FILE *fp)
{
    libexplain_string_buffer_printf(sb, "fclose(fp = %p)", fp);
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_buffer_failed(sb, errnum);

    libexplain_buffer_errno_fclose_because(sb, errnum, fp);
}



void
libexplain_buffer_errno_fclose_because(libexplain_string_buffer_t *sb,
    int errnum, FILE *fp)
{
    /*
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

    switch (errnum)
    {
    case EFAULT:
    case EFBIG:
    case EINVAL:
    case ENOSPC:
    case EPIPE:
        libexplain_buffer_errno_write_because(sb, errnum, fildes, NULL, 0);
        break;

    case EBADF:
        libexplain_buffer_ebadf(sb, "fildes");
        break;

    case EINTR:
    case EIO:
    default:
        libexplain_buffer_errno_close_because(sb, errnum, fildes);
        break;
    }
    if (errnum != EBADF)
    {
        libexplain_string_buffer_puts
        (
            sb,
            "; note that while the FILE stream is no longer valid, the "
            "underlying file descriptor may still be open"
        );
    }
}
