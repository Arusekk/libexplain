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

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/errno/unlink.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/path_to_pid.h>
#include <libexplain/buffer/sticky_permissions.h>
#include <libexplain/buffer/strerror.h>
#include <libexplain/buffer/success.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_errno_unlink(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname)
{
    libexplain_string_buffer_printf(sb, "unlink(pathname = ");
    if (errnum == EFAULT)
        libexplain_string_buffer_printf(sb, "%p", pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_string_buffer_puts(sb, " failed, ");
    libexplain_buffer_strerror(sb, errnum);

    switch (errnum)
    {
    case EACCES:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_sticky_permissions(sb, pathname, "pathname")
        &&
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                O_RDONLY,
                "pathname"
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "write access to the directory containing pathname "
                "is not allowed for the process's effective UID, or one "
                "of the directories in pathname did not allow search "
                "permission"
            );
            libexplain_string_buffer_puts
            (
                sb,
                "; or, the directory containing pathname has the "
                "sticky bit (S_ISVTX) set and the process's effective UID "
                "is neither the UID of the file to be deleted nor that "
                "of the directory containing it, and the process is not "
                "privileged"
#ifdef __linux__
                " (does not have the CAP_FOWNER capability)"
#endif
            );
        }
        break;

    case EBUSY:
        libexplain_buffer_because(sb);
        /* not on Linux */
        libexplain_string_buffer_puts
        (
            sb,
            "the file pathname "
            "is being used by the system or another process and the "
            "implementation considers this an error"
        );
        libexplain_buffer_path_to_pid(sb, pathname);
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "pathname");
        break;

    case EIO:
        libexplain_buffer_eio_path(sb, pathname);
        break;

    case EISDIR:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "pathname refers to a directory"
        );
        break;

    case ELOOP:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                O_RDONLY,
                "pathname"
            )
        )
        {
            /*
             * No specific cause was found,
             * issue the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "too many symbolic links were encountered in "
                "translating pathname"
            );
        }
        break;

    case ENAMETOOLONG:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                O_RDONLY,
                "pathname"
            )
        )
        {
            /*
             * No specific cause was found,
             * issue the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "pathname, or a component of pathname, is too long"
            );
        }
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
                O_RDONLY,
                "pathname"
            )
        )
        {
            /*
             * No specific cause was found,
             * issue the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "pathname, or a component of pathname, does not exist "
                "or is a dangling symbolic link, or pathname is empty"
            );
        }
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    case ENOTDIR:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                O_RDONLY,
                "pathname"
            )
        )
        {
            /*
             * No specific cause was found,
             * issue the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "a component used as a directory in pathname is "
                "not, in fact, a directory"
            );
        }
        break;

    case EPERM:
        libexplain_buffer_because(sb);
#ifndef __linux__
        /* FIXME: which one? */
        libexplain_string_buffer_puts
        (
            sb,
            "the system does not allow unlinking of "
            "directories, or unlinking of directories requires "
            "privileges that the calling process does not have "
        );
#else
        /* FIXME: which one? */
        libexplain_string_buffer_puts
        (
            sb,
            "the file system does not allow unlinking of files "
        );
#endif
        /* FIXME: which one? */
        libexplain_string_buffer_puts
        (
            sb,
            "; or, the directory containing pathname has the "
            "sticky bit (S_ISVTX) set and the process's effective UID "
            "is neither the UID of the file to be deleted nor that "
            "of the directory containing it, and the process is not "
            "privileged"
#ifdef __linux__
            " (does not have the CAP_FOWNER capability)"
#endif
        );
        break;

    case EROFS:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "pathname refers to a file on a read-only "
            "file system"
        );
        libexplain_buffer_mount_point(sb, pathname);
        break;

    default:
        /* no additional info for other errno values */
        break;
    }

    {
        struct stat     st;

        if (lstat(pathname, &st) == 0)
        {
            libexplain_string_buffer_puts
            (
                sb,
                "; note that pathname still exists"
            );
        }
    }
}
