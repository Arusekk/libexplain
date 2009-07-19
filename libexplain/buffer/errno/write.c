/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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
#include <libexplain/ac/signal.h>
#include <libexplain/ac/sys/resource.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/efbig.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/write.h>
#include <libexplain/buffer/fildes_not_open_for_writing.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/pretty_size.h>
#include <libexplain/buffer/rlimit.h>
#include <libexplain/explanation.h>
#include <libexplain/open_flags.h>
#include <libexplain/option.h>
#include <libexplain/string_buffer.h>


static void
explain_buffer_errno_write_system_call(explain_string_buffer_t *sb,
    int errnum, int fildes, const void *data, size_t data_size)
{
    (void)errnum;
    explain_string_buffer_printf(sb, "write(fildes = %d", fildes);
    explain_buffer_fildes_to_pathname(sb, fildes);
    explain_string_buffer_puts(sb, ", data = ");
    explain_buffer_pointer(sb, data);
    explain_string_buffer_printf
    (
        sb,
        ", data_size = %lld)",
        (long long)data_size
    );
}


void
explain_buffer_errno_write_explanation(explain_string_buffer_t *sb,
    int errnum, int fildes, const void *data, size_t data_size)
{
    switch (errnum)
    {
    case EAGAIN:
        explain_string_buffer_puts
        (
            sb,
            "the file descriptor has been marked non-blocking "
            "(O_NONBLOCK) and the write would block"
        );
        break;

    case EBADF:
        if (explain_buffer_fildes_not_open_for_writing(sb, fildes, "fildes"))
            explain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case EFAULT:
        explain_buffer_efault(sb, "data");
        break;

    case EFBIG:
        {
            off_t         max_file_size;
            off_t         pos;

            pos = lseek(fildes, 0, SEEK_CUR);
            if (pos == (off_t)-1)
                pos = 0;
            max_file_size = explain_get_max_file_size_by_fildes(fildes);
            if (pos >= 0 && pos + data_size > max_file_size)
            {
                explain_string_buffer_puts
                (
                    sb,
                    "an attempt was made to write a file that "
                    "exceeds the process's file size limit"
                );
            }
            else
            {
                explain_string_buffer_puts
                (
                    sb,
                    "an attempt was made to write at a position past the "
                    "process's file size limit"
                );
            }
            if (explain_option_dialect_specific())
            {
                explain_string_buffer_puts(sb, " (");
                explain_buffer_pretty_size(sb, max_file_size);
                explain_string_buffer_putc(sb, ')');
            }
        }
        break;

    case EINTR:
        explain_buffer_eintr(sb, "write");
        break;

    case EINVAL:
        {
            int flags = fcntl(fildes, F_GETFL);
            if (flags >= 0)
            {
                if ((flags & O_ACCMODE) == O_RDONLY)
                {
                    explain_string_buffer_puts
                    (
                        sb,
                        "the file descriptor is attached to an object "
                        "which is unsuitable for writing ("
                    );
                    explain_buffer_open_flags(sb, flags);
                    explain_string_buffer_putc(sb, ')');
                }
                else
                {
                    long            alignment;
                    off_t           pos;

                    explain_string_buffer_puts
                    (
                        sb,
                        "the file was opened with the O_DIRECT flag, "
                        "and either the address specified in buf is not "
                        "suitably aligned, the value specified in count "
                        "is not suitably aligned, or the current file "
                        "offset is not suitably aligned"
                    );

                    alignment = (long)data | (long)data_size;
                    pos = lseek(fildes, 0, SEEK_CUR);
                    if (pos != (off_t)-1)
                        alignment |= (long)pos;
                    alignment &= -alignment;
                    if (alignment != 0)
                    {
                        explain_string_buffer_printf
                        (
                            sb,
                            " (0x%lX)",
                            alignment
                        );
                    }
                }
            }
            else
            {
                explain_string_buffer_puts
                (
                    sb,
                    "the file descriptor is attached to an object which "
                    "is unsuitable for writing; or, the file was opened "
                    "with the O_DIRECT flag, and either the address "
                    "specified in buf is not suitably aligned, the "
                    "value specified in count is not suitably aligned, "
                    "or the current file offset is not suitably aligned"
                );
            }
        }
        break;

    case EIO:
        explain_buffer_eio_fildes(sb, fildes);
        break;

    case ENOSPC:
        {
            struct stat st;
            if (fstat(fildes, &st) == 0)
            {
                if (S_ISBLK(st.st_mode) || S_ISCHR(st.st_mode))
                {
                    explain_string_buffer_puts
                    (
                        sb,
                        "the device has no room for the data"
                    );
                }
                else
                {
                    explain_string_buffer_puts
                    (
                        sb,
                        "the file system containing the file"
                    );
                    explain_buffer_mount_point_stat(sb, &st);
                    explain_string_buffer_puts
                    (
                        sb,
                        " has no room for the data"
                    );
                }
            }
            else
            {
                explain_string_buffer_puts
                (
                    sb,
                    "the device containing the file referred "
                    "to by the file descriptor has no room for the "
                    "data; or, the file system containing the file has "
                    "no room for the data"
                );
            }
        }
        break;

    case EPIPE:
        {
            sigset_t mask;
            struct sigaction sa;

            explain_string_buffer_puts
            (
                sb,
                "the file descriptor is connected to a pipe or "
                "socket whose reading end is closed, when this happens "
                "the writing process will also receive a SIGPIPE signal"
            );
            if
            (
                sigprocmask(SIG_BLOCK, 0, &mask) == 0
            &&
                sigismember(&mask, SIGPIPE)
            )
            {
                /*
                 * FIXME: The use of sigprocmask() is undefined in a
                 * multithreaded process; see pthread_sigmask(3).
                 */
                explain_string_buffer_puts
                (
                    sb,
                    "; this process is blocking the SIGPIPE signal"
                );
                break;
            }
            if
            (
                sigaction(SIGPIPE, 0, &sa) == 0
            &&
                (sa.sa_flags & SA_SIGINFO) != 0
            )
            {
                if (sa.sa_handler == SIG_IGN)
                {
                    explain_string_buffer_puts
                    (
                        sb,
                        "; this process is ignoring the SIGPIPE signal"
                    );
                    break;
                }
                if (sa.sa_handler != SIG_DFL)
                {
                    explain_string_buffer_puts
                    (
                        sb,
                        "; this process is catching the SIGPIPE signal"
                    );
                    break;
                }
            }
            explain_string_buffer_puts
            (
                sb,
                "; this process catches, blocks or ignores the SIGPIPE signal"
            );
        }
        break;

    case ENOENT:
        explain_string_buffer_puts
        (
            sb,
            "the file is on a file system"
        );
        explain_buffer_mount_point_fd(sb, fildes);
        explain_string_buffer_puts
        (
            sb,
            " which does not support Unix open file semantics, and the "
            "file has been deleted from underneath you"
        );
        break;

    default:
        explain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
explain_buffer_errno_write(explain_string_buffer_t *sb, int errnum,
    int fildes, const void *data, size_t data_size)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_write_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        data,
        data_size
    );
    explain_buffer_errno_write_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        data,
        data_size
    );
    explain_explanation_assemble(&exp, sb);
}
