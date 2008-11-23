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
#include <libexplain/buffer/errno/close.h>
#include <libexplain/buffer/errno/fclose.h>
#include <libexplain/buffer/errno/write.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/stream_to_pathname.h>
#include <libexplain/explanation.h>
#include <libexplain/stream_to_fildes.h>


static void
libexplain_buffer_errno_fclose_system_call(libexplain_string_buffer_t *sb,
    int errnum, FILE *fp)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "fclose(fp = ");
    libexplain_buffer_pointer(sb, fp);
    libexplain_buffer_stream_to_pathname(sb, fp);
    libexplain_string_buffer_putc(sb, ')');
}


void
libexplain_buffer_errno_fclose_explanation(libexplain_string_buffer_t *sb,
    int errnum, FILE *fp)
{
    int             fildes;

    if (!fp)
    {
        libexplain_buffer_gettext(sb, i18n("fp is the NULL pointer"));
        return;
    }

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
    fildes = libexplain_stream_to_fildes(fp);

    switch (errnum)
    {
    case EFAULT:
    case EFBIG:
    case EINVAL:
    case ENOSPC:
    case EPIPE:
        libexplain_buffer_errno_write_explanation(sb, errnum, fildes, NULL, 0);
        break;

    case EBADF:
        libexplain_buffer_ebadf(sb, "fp");
        break;

    case EINTR:
    case EIO:
    default:
        libexplain_buffer_errno_close_explanation(sb, errnum, fildes);
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


void
libexplain_buffer_errno_fclose(libexplain_string_buffer_t *sb, int errnum,
    FILE *fp)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_fclose_system_call(&exp.system_call_sb, errnum, fp);
    libexplain_buffer_errno_fclose_explanation(&exp.explanation_sb, errnum, fp);
    libexplain_explanation_assemble(&exp, sb);
}
