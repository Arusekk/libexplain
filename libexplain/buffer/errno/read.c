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
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/errno/read.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/strerror.h>
#include <libexplain/buffer/success.h>
#include <libexplain/is_same_inode.h>
#include <libexplain/open_flags.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_errno_read(libexplain_string_buffer_t *sb, int errnum,
    int fildes, const void *data, size_t data_size)
{
    libexplain_string_buffer_printf(sb, "read(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_printf
    (
        sb,
        ", data = %p, data_size = %lld)",
        data,
        (long long)data_size
    );
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }

    libexplain_string_buffer_puts(sb, " failed, ");
    libexplain_buffer_strerror(sb, errnum);

    switch (errnum)
    {
    case EAGAIN:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "non-blocking I/O has been selected using "
            "O_NONBLOCK and no data was immediately available for "
            "reading"
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
                    "the file descriptor is not open for reading ("
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

    case EINTR:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the call was interrupted by a signal before any "
            "data was read"
        );
        break;

    case EINVAL:
        {
            int             flags;

            libexplain_buffer_because(sb);
            flags = fcntl(fildes, F_GETFL);
            if (flags >= 0)
            {
                if ((flags & O_ACCMODE) == O_WRONLY)
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the file descriptor is attached to an object "
                        "which is unsuitable for reading ("
                    );
                    libexplain_buffer_open_flags(sb, flags);
                    libexplain_string_buffer_putc(sb, ')');
                }
                else if (flags & O_DIRECT)
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the file was opened with the O_DIRECT flag, "
                        "and either the address specified in data is "
                        "not suitably aligned, or the value specified "
                        "in data_size is not suitably aligned, or the "
                        "current file offset is not suitably aligned"
                    );
                }
                else
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the file descriptor was created via a call to "
                        "timerfd_create(2) and the wrong size buffer was "
                        "given"
                    );
                }
            }
            else
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "the file desriptor is attached to an object "
                    "which is unsuitable for reading; or, the file was "
                    "opened with the O_DIRECT flag, and either the "
                    "address specified in data, the value specified "
                    "in data_size, or the current file offset is not"
                    "suitably aligned; or, the file descriptor was "
                    "created via a call to timerfd_create(2) and the "
                    "wrong size buffer was given"
                );
            }
        }
        break;

    case EIO:
        {
            pid_t process_group = getpgid(0);
            int controlling_tty_fd = open("/dev/tty", O_RDWR);
            pid_t tty_process_group = tcgetpgrp(controlling_tty_fd);

            /* if reading controlling tty */
            if
            (
                process_group >= 0
            &&
                controlling_tty_fd >= 0
            &&
                process_group != tty_process_group
            )
            {
                struct stat st1;
                struct stat st2;
                if
                (
                    fstat(fildes, &st1) == 0
                &&
                    fstat(controlling_tty_fd, &st2) == 0
                &&
                    libexplain_is_same_inode(&st1, &st2)
                )
                {
                    libexplain_buffer_because(sb);
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the process is in a background "
                        "process group, and tried to read from its "
                        "controlling tty, and either it is ignoring or "
                        "blocking SIGTTIN"
                    );
                    close(controlling_tty_fd);
                    break;
                }
            }
            if (controlling_tty_fd < 0)
            {
                libexplain_buffer_because(sb);
                libexplain_string_buffer_puts
                (
                    sb,
                    "the process is in an orphaned process "
                    "group and tried to read from its controlling tty"
                );
                break;
            }
            close(controlling_tty_fd);

            libexplain_buffer_eio_fildes(sb, fildes);
        }
        break;

    case EISDIR:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the file descriptor refers to a directory, and "
            "you must use getdents(2) to read directories, preferbly "
            "via the higher-level interface provided by readdir(3)"
        );
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
