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
#include <libexplain/ac/signal.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/errno/fflush.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/strerror.h>
#include <libexplain/buffer/success.h>
#include <libexplain/open_flags.h>


void
libexplain_buffer_errno_fflush(libexplain_string_buffer_t *sb, int errnum,
    FILE *fp)
{
    int             fildes;

    fildes = -1;
    if (fp)
        fildes = fileno(fp);

    libexplain_string_buffer_printf(sb, "fflush(fp = %p", fp);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_string_buffer_puts(sb, " failed, ");
    libexplain_buffer_strerror(sb, errnum);

    if (!fp)
    {
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts(sb, "fp is the NULL pointer");
        return;
    }
    switch (errnum)
    {
    case EAGAIN:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the file descriptor has been marked non-blocking "
            "(O_NONBLOCK) and the write would block"
        );
        break;

    case EBADF:
        {
            int             flags;

            libexplain_buffer_because(sb);
            flags = fcntl(fildes, F_GETFL);
            if (flags >= 0)
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "the file descriptor not open for writing ("
                );
                libexplain_buffer_open_flags(sb, flags);
                libexplain_string_buffer_putc(sb, ')');
            }
            else
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "the file descriptor is not valid"
                );
            }
        }
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "data");
        break;

    case EFBIG:
        /*
         * Not enough information to be as informative as we are for the
         * write(2) explanation.
         */
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "an attempt was made to write a file that exceeds "
            "the implementation-defined maximum file size or "
            "the implementation-defined maximum file size or "
            "the process's file size limit, or to write at a "
            "position past the maximum allowed offset"
        );
        break;

    case EINTR:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the call was interrupted by a signal before any "
            "data was written"
        );
        break;

    case EINVAL:
        {
            int             flags;

            flags = fcntl(fildes, F_GETFL);
            libexplain_buffer_because(sb);
            if (flags >= 0)
            {
                if ((flags & O_ACCMODE) == O_RDONLY)
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the file descriptor is attached to an object "
                        "which is unsuitable for writing ("
                    );
                    libexplain_buffer_open_flags(sb, flags);
                    libexplain_string_buffer_putc(sb, ')');
                }
                else
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the file was opened with the O_DIRECT flag, "
                        "and either the address specified in buf is not "
                        "suitably aligned, the value specified in count "
                        "is not suitably aligned, or the current file "
                        "offset is not suitably aligned"
                    );
                }
            }
            else
            {
                libexplain_string_buffer_puts
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
        libexplain_buffer_eio_fildes(sb, fildes);
        break;

    case ENOSPC:
        {
            struct stat st;
            if (fstat(fildes, &st) == 0)
            {
                if (S_ISBLK(st.st_mode) || S_ISCHR(st.st_mode))
                {
                    libexplain_buffer_because(sb);
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the device has no room for the data"
                    );
                }
                else
                {
                    libexplain_buffer_because(sb);
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the file system containing the file"
                    );
                    libexplain_buffer_mount_point_stat(sb, &st);
                    libexplain_string_buffer_puts
                    (
                        sb,
                        " has no room for the data"
                    );
                }
            }
            else
            {
                libexplain_buffer_because(sb);
                libexplain_string_buffer_puts
                (
                    sb,
                    "the device containing the file referred to by the "
                    "file descriptor has no room for the data; or, the "
                    "file system containing the file has no room for "
                    "the data"
                );
            }
        }
        break;

    case EPIPE:
        {
            sigset_t mask;
            struct sigaction sa;

            libexplain_buffer_because(sb);
            libexplain_string_buffer_puts
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
                libexplain_string_buffer_puts
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
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "; this process is ignoring the SIGPIPE signal"
                    );
                    break;
                }
                if (sa.sa_handler != SIG_DFL)
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "; this process is catching the SIGPIPE signal"
                    );
                    break;
                }
            }
            libexplain_string_buffer_puts
            (
                sb,
                "; this process catches, blocks or ignores the SIGPIPE signal"
            );
        }
        break;

    case ENOENT:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the file is on a file system"
        );
        libexplain_buffer_mount_point_fd(sb, fildes);
        libexplain_string_buffer_puts
        (
            sb,
            " which does not support Unix open file semantics, and the "
            "file has been deleted from underneath you"
        );
        break;

    default:
        /* no explanation for other errno values */
        break;
    }
}
