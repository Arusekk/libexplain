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

#include <libexplain/buffer/errno/ferror.h>
#include <libexplain/buffer/errno/read.h>
#include <libexplain/buffer/errno/write.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/stream_to_pathname.h>
#include <libexplain/explanation.h>
#include <libexplain/stream_to_fildes.h>


static void
libexplain_buffer_errno_ferror_system_call(libexplain_string_buffer_t *sb,
    int errnum, FILE *fp)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "ferror(fp = ");
    libexplain_buffer_pointer(sb, fp);
    libexplain_buffer_stream_to_pathname(sb, fp);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_ferror_explanation(libexplain_string_buffer_t *sb,
    int errnum, FILE *fp)
{
    int             fildes;
    int             flags;

    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/ferror.html
     */
    if (fp == NULL)
    {
        libexplain_string_buffer_puts(sb, "fp is the NULL pointer");
        return;
    }

    /*
     * probably had a problem reading or writing.
     * See if we can get a clue from the file flags.
     */
    fildes = libexplain_stream_to_fildes(fp);
    if (fildes < 0)
        goto ambiguous;
    flags = fcntl(fildes, F_GETFL);
    if (flags < 0)
        goto ambiguous;
    switch (flags & O_ACCMODE)
    {
    case O_RDONLY:
        read_error:
        libexplain_buffer_errno_read_explanation(sb, errnum, fildes, NULL, 0);
        break;

    case O_WRONLY:
        write_error:
        libexplain_buffer_errno_write_explanation(sb, errnum, fildes, NULL, 0);
        break;

    default:
        ambiguous:
        switch (errnum)
        {
        default:
            /*
             * If we still can't tell, assume they were writing
             * as this is more likely to have problems than reading.
             */
            goto write_error;

        case EAGAIN:
        case EBADF:
        case EFAULT:
        case EFBIG:
        case EINTR:
        case EINVAL:
        case EIO:
        case ENOSPC:
        case EPIPE:
            goto write_error;

#if 0
        case EAGAIN:
        case EBADF:
        case EFAULT:
        case EINTR:
        case EINVAL:
        case EIO:
            /*
             * For ambiguous errno values, assume write error, see above
             * comment.
             */
            goto read_error;
#endif

        case EISDIR:
            goto read_error;
        }
        break;
    }
}


void
libexplain_buffer_errno_ferror(libexplain_string_buffer_t *sb, int errnum,
    FILE *fp)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_ferror_system_call
    (
        &exp.system_call_sb,
        errnum,
        fp
    );
    libexplain_buffer_errno_ferror_explanation
    (
        &exp.explanation_sb,
        errnum,
        fp
    );
    libexplain_explanation_assemble(&exp, sb);
}
