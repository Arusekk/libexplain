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
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/enfile.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/open.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/path_to_pid.h>
#include <libexplain/buffer/strerror.h>
#include <libexplain/buffer/uid.h>
#include <libexplain/buffer/success.h>
#include <libexplain/open_flags.h>
#include <libexplain/permission_mode.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_errno_open(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, int flags, int mode)
{
    libexplain_string_buffer_printf(sb, "open(pathname = ");
    if (errnum == EFAULT)
        libexplain_string_buffer_printf(sb, "%p", pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_puts(sb, ", flags = ");
    libexplain_buffer_open_flags(sb, flags);
    if (flags & O_CREAT)
    {
        libexplain_string_buffer_puts(sb, ", mode = ");
        libexplain_buffer_permission_mode(sb, mode);
    }
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_string_buffer_puts(sb, " failed, ");
    libexplain_buffer_strerror(sb, errnum);

    libexplain_buffer_errno_open_because(sb, errnum, pathname, flags, mode);
}


void
libexplain_buffer_errno_open_because(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, int flags, int mode)
{
    (void)mode;

    switch (errnum)
    {
    case EACCES:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                flags,
                "pathname"
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "the requested access to the file is not allowed, or "
                "search permission is denied for one of the directories "
                "in the path prefix of pathname, or the file did not "
                "exist yet and write access to the parent directory is "
                "not allowed"
            );
        }
        break;

    case EEXIST:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                flags,
                "pathname"
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "the file already exists when it should not (flags "
                "O_CREAT and O_EXCL were used)"
            );
        }
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "pathname");
        break;

    case EFBIG:
    case EOVERFLOW:
        {
            struct stat st;
            if (stat(pathname, &st) == 0 && S_ISREG(st.st_mode))
            {
                libexplain_buffer_because(sb);
                libexplain_string_buffer_printf
                (
                    sb,
                    "it is a regular file which is too large to be "
                    "opened (%lld bytes), the O_LARGEFILE flag is "
                    "necessary",
                    (long long)st.st_size
                );
            }
            else
            {
                libexplain_buffer_because(sb);
                libexplain_string_buffer_puts
                (
                    sb,
                    "it is a regular file which is too large to be "
                    "opened, the O_LARGEFILE flag is necessary "
                );
            }
        }
        break;

    case EISDIR:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "pathname refers to a directory and the access requested "
            "involved writing (that is, O_WRONLY or O_RDWR is set)"
        );
        break;

    case ELOOP:
        libexplain_buffer_because(sb);
        if (flags & O_NOFOLLOW)
        {
            struct stat st;
            if (lstat(pathname, &st) == 0 && S_ISLNK(st.st_mode))
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "O_NOFOLLOW was specified but pathname was a "
                    "symbolic link"
                );
                break;
            }
        }
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                flags,
                "pathname"
            )
        )
        {
            long            symloop_max;

            libexplain_string_buffer_puts
            (
                sb,
                "too many symbolic links were encountered in resolving "
                "pathname"
            );
            symloop_max = sysconf(_SC_SYMLOOP_MAX);
            if (symloop_max > 0)
                libexplain_string_buffer_printf(sb, " (%ld)", symloop_max);
        }
        break;

    case EMFILE:
        libexplain_buffer_emfile(sb);
        break;

    case ENAMETOOLONG:
        {
            if
            (
                libexplain_buffer_errno_path_resolution
                (
                    sb,
                    errnum,
                    pathname,
                    flags,
                    "pathname"
                )
            )
            {
                libexplain_buffer_because(sb);
                libexplain_string_buffer_printf
                (
                    sb,
                    "the pathname, or one of the pathname components, is "
                    "longer than the system maximum"
                );
            }
        }
        break;

    case ENFILE:
        libexplain_buffer_enfile(sb);
        break;

    case ENODEV:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "pathname refers to a device special file and no "
            "corresponding device exists.  (This is a Linux kernel bug; "
            "in this situation ENXIO must be returned.)"
        );
        break;

    case ENOENT:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                flags,
                "pathname"
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "O_CREAT is not set and the named file does not exist; "
                "or, a directory component of pathname does not exist; "
                "or, a component of pathname is a dangling symbolic link"
            );
        }
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    case ENOSPC:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the file system containing the pathname"
        );
        libexplain_buffer_mount_point_dirname(sb, pathname);
        libexplain_string_buffer_puts
        (
            sb,
            " has no room for the new file"
            /* has no blocks left or has no indoes left */
        );
        break;

    case ENOTDIR:
        libexplain_buffer_because(sb);
        if (flags & O_DIRECTORY)
        {
            struct stat st;
            if (lstat(pathname, &st) == 0 && !S_ISDIR(st.st_mode))
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "O_DIRECTORY was specified and pathname is a "
                );
                libexplain_buffer_file_type(sb, st.st_mode);
                libexplain_string_buffer_puts
                (
                    sb,
                    ", not a directory"
                );
                break;
            }
        }
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                flags,
                "pathname"
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "a component used as a directory in pathname is not, in "
                "fact, a directory"
            );
            if (flags & O_DIRECTORY)
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "; or, O_DIRECTORY was specified and pathname was "
                    "not a directory"
                );
            }
        }
        break;

    case ENXIO:
        {
            struct stat st;
            if (stat(pathname, &st) != 0)
            {
                enxio_generic:
                libexplain_buffer_because(sb);
                libexplain_string_buffer_puts
                (
                    sb,
                    "O_NONBLOCK | O_WRONLY is set, the named file is a "
                    "FIFO and no process has the file open for reading; "
                    "or, the file is a device special file and no "
                    "corresponding device exists"
                );
                break;
            }
            switch (st.st_mode & S_IFMT)
            {
            case S_IFIFO:
                libexplain_buffer_because(sb);
                libexplain_string_buffer_puts
                (
                    sb,
                    "the named file is a FIFO and no process has the "
                    "file open for reading"
                );
                break;

            case S_IFCHR:
            case S_IFBLK:
                libexplain_buffer_because(sb);
                libexplain_string_buffer_printf
                (
                    sb,
                    "the file is a device special file (%d, %d) and no "
                        "corresponding hardware device exists",
                    major(st.st_rdev),
                    minor(st.st_rdev)
                );
                break;

            default:
                goto enxio_generic;
            }
        }
        break;

    case EPERM:
        /*
         * The O_NOATIME flag was specified, but the effective user ID of
         * the caller did not match the owner of the file and the caller
         * was not privileged (CAP_FOWNER).
         */
        {
            struct stat st;
            if (stat(pathname, &st) != 0)
                break;
            libexplain_buffer_because(sb);
            libexplain_string_buffer_puts
            (
                sb,
                "the effective user ID of the caller ("
            );
            libexplain_buffer_uid(sb, geteuid());
            libexplain_string_buffer_puts
            (
                sb,
                ") did not match the owner of the file ("
            );
            libexplain_buffer_uid(sb, st.st_uid);
            libexplain_string_buffer_puts
            (
                sb,
                ") and the caller was not privileged"
#ifdef __linux__
                " (does not have the CAP_FOWNER capability)"
#endif
            );
        }
        break;

    case EROFS:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "write access was requested and pathname refers to a file "
            "on a read-only file system"
        );
        if (libexplain_buffer_mount_point(sb, pathname) < 0)
            libexplain_buffer_mount_point_dirname(sb, pathname);
        break;

    case ETXTBSY:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "write access was requested and pathname refers to an "
            "executable image which is currently being executed"
        );
        libexplain_buffer_path_to_pid(sb, pathname);
        break;

    case EWOULDBLOCK:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the O_NONBLOCK flag was specified, and an incompatible "
            "lease was held on the file"
        );
        libexplain_buffer_path_to_pid(sb, pathname);
        break;

    default:
        /* no additional info for other errno values */
        break;
    }
    if ((flags & O_EXCL) && !(flags & O_CREAT))
    {
        libexplain_string_buffer_puts
        (
            sb,
            "; note that the behavior of O_EXCL is undefined if O_CREAT "
            "is not specified"
        );
    }
}
