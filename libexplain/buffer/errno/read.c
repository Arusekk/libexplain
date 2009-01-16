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
#include <libexplain/ac/sys/mtio.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/read.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>
#include <libexplain/is_same_inode.h>
#include <libexplain/open_flags.h>
#include <libexplain/string_buffer.h>


static void
libexplain_buffer_errno_read_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes, const void *data, size_t data_size)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "read(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_puts(sb, ", data = ");
    libexplain_buffer_pointer(sb, data);
    libexplain_string_buffer_printf
    (
        sb,
        ", data_size = %lld)",
        (long long)data_size
    );
}


static int
is_a_tape(int fildes)
{
    struct mtop     args;

    args.mt_op = MTNOP;
    args.mt_count = 0;
    return (ioctl(fildes, MTIOCTOP, &args) >= 0);
}


void
libexplain_buffer_errno_read_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes, const void *data, size_t data_size)
{
    (void)data;
    (void)data_size;
    switch (errnum)
    {
    case EAGAIN:
        libexplain_string_buffer_puts
        (
            sb,
            /* FIXME: i18n */
            "non-blocking I/O has been selected using "
            "O_NONBLOCK and no data was immediately available for "
            "reading"
        );
        break;

    case EBADF:
        {
            int             flags;

            flags = fcntl(fildes, F_GETFL);
            if (flags >= 0)
            {
                libexplain_buffer_gettext
                (
                    sb,
                    /*
                     * xgettext: This message is used when an attempt is
                     * made to read from a file descriptor that was not
                     * opened for reading.  The actual open mode will be
                     * printed separately.
                     */
                    i18n("the file descriptor is not open for reading")
                );
                libexplain_string_buffer_puts(sb, " (");
                libexplain_buffer_open_flags(sb, flags);
                libexplain_string_buffer_putc(sb, ')');
            }
            else
            {
                libexplain_buffer_ebadf(sb, fildes, "fildes");
            }
        }
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "data");
        break;

    case EINTR:
        libexplain_buffer_eintr(sb, "read");
        break;

    case EINVAL:
        {
            int             flags;

            flags = fcntl(fildes, F_GETFL);
            if (flags >= 0)
            {
                if ((flags & O_ACCMODE) == O_WRONLY)
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        /* FIXME: i18n */
                        "the file descriptor is attached to an object "
                        "which is unsuitable for reading ("
                    );
                    libexplain_buffer_open_flags(sb, flags);
                    libexplain_string_buffer_putc(sb, ')');
                }
#ifdef O_DIRECT
                else if (flags & O_DIRECT)
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        /* FIXME: i18n */
                        "the file was opened with the O_DIRECT flag, "
                        "and either the address specified in data is "
                        "not suitably aligned, or the value specified "
                        "in data_size is not suitably aligned, or the "
                        "current file offset is not suitably aligned"
                    );
                }
#endif
                else
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        /* FIXME: i18n */
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
                    /* FIXME: i18n */
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
                struct stat     st1;
                struct stat     st2;

                if
                (
                    fstat(fildes, &st1) == 0
                &&
                    fstat(controlling_tty_fd, &st2) == 0
                &&
                    libexplain_is_same_inode(&st1, &st2)
                )
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        /* FIXME: i18n */
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
                libexplain_string_buffer_puts
                (
                    sb,
                    /* FIXME: i18n */
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
        libexplain_string_buffer_puts
        (
            sb,
            /* FIXME: i18n */
            "fildes refers to a directory, and you must use getdents(2) to "
            "read directories, preferably via the higher-level interface "
            "provided by readdir(3)"
        );
        break;

    case ENOENT:
        libexplain_string_buffer_puts
        (
            sb,
            /* FIXME: i18n */
            "the file is on a file system"
        );
        libexplain_buffer_mount_point_fd(sb, fildes);
        libexplain_string_buffer_puts
        (
            sb,
            " that does not support Unix open file semantics, and the "
            "file has been deleted from underneath you"
        );
        break;

    case EOVERFLOW:
        if (data_size > ((size_t)1 << 16) && is_a_tape(fildes))
        {
            libexplain_string_buffer_printf
            (
                sb,
                /* FIXME: i18n */
                "the tape read operation was supplied with a %ld byte "
                    "buffer, however the kernal has been compiled with a limit "
                    "smaller than this; you need to reconfigure your system, "
                    "or recompile your tape device driver, to have a fixed "
                    "limit of at least 64KiB",
                (long)data_size
            );
        }
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_read(libexplain_string_buffer_t *sb, int errnum,
    int fildes, const void *data, size_t data_size)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_read_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        data,
        data_size
    );
    libexplain_buffer_errno_read_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        data,
        data_size
    );
    libexplain_explanation_assemble(&exp, sb);
}
