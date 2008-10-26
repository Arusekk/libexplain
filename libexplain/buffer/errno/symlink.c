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
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/errno/symlink.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/strerror.h>
#include <libexplain/buffer/success.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_errno_symlink(libexplain_string_buffer_t *sb, int errnum,
    const char *oldpath, const char *newpath)
{
    int             oldpath_bad;
    int             newpath_bad;

    oldpath_bad = errnum == EFAULT && libexplain_path_is_efault(oldpath);
    newpath_bad = errnum == EFAULT && libexplain_path_is_efault(newpath);
    libexplain_string_buffer_printf(sb, "symlink(oldpath = ");
    if (oldpath_bad)
        libexplain_string_buffer_printf(sb, "%p", oldpath);
    else
        libexplain_string_buffer_puts_quoted(sb, oldpath);
    libexplain_string_buffer_puts(sb, ", newpath = ");
    if (newpath_bad)
        libexplain_string_buffer_printf(sb, "%p", newpath);
    else
        libexplain_string_buffer_puts_quoted(sb, newpath);
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
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                newpath,
                O_WRONLY + O_CREAT,
                "newpath"
            )
        )
        {
            /*
             * No specific cause was found, go with the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "write access to the directory containing newpath is "
                "denied, or one of the directories in the path prefix "
                "of newpath did not allow search permission"
            );
        }
        break;

    case EEXIST:
        libexplain_buffer_because(sb);
        {
            struct stat st;
            if (lstat(newpath, &st) == 0)
            {
                libexplain_string_buffer_putc(sb, ' ');
                libexplain_buffer_file_type(sb, st.st_mode);
            }
        }
        libexplain_string_buffer_puts(sb, " newpath already exists");
        break;

    case EFAULT:
        if (oldpath_bad)
            libexplain_buffer_efault(sb, "oldpath");
        else if (newpath_bad)
            libexplain_buffer_efault(sb, "newpath");
        else
            libexplain_buffer_efault(sb, "oldpath or newpath");
        break;

    case EIO:
        libexplain_buffer_eio_path_dirname(sb, newpath);
        break;

    case ELOOP:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                newpath,
                O_RDWR + O_CREAT,
                "newpath"
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "too many symbolic links were encountered in resolving "
                "newpath"
            );
        }
        break;

    case ENAMETOOLONG:
        libexplain_buffer_because(sb);
        {
            size_t          oldpath_len;
            long            path_max;

            oldpath_len = strlen(oldpath);
            path_max = pathconf(newpath, _PC_PATH_MAX);
            if (path_max <= 0)
                path_max = PATH_MAX;
            if (oldpath_len > (size_t)path_max)
            {
                libexplain_string_buffer_puts(sb, "oldpath is too long");
                libexplain_string_buffer_printf
                (
                    sb,
                    " (%ld > %ld)",
                    (long)oldpath_len,
                    path_max
                );
            }
            else if
            (
                libexplain_buffer_errno_path_resolution
                (
                    sb,
                    errnum,
                    newpath,
                    O_WRONLY | O_CREAT,
                    "newpath"
                )
            )
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    "newpath, or a directory component of newpath, was "
                    "too long"
                );
            }
        }
        break;

    case ENOENT:
        libexplain_buffer_because(sb);
        if (!*oldpath)
        {
            libexplain_string_buffer_puts
            (
                sb,
                "oldpath is the empty string; if you meant the current "
                "directory, use \".\" instead"
            );
        }
        else if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                newpath,
                O_WRONLY | O_CREAT,
                "newpath"
            )
        )
        {
            /*
             * No specific cause was found,
             * use the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "a directory component in newpath does not exist "
                "or is a dangling symbolic link"
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
            "the file system containing newpath"
        );
        libexplain_buffer_mount_point_dirname(sb, newpath);
        libexplain_string_buffer_puts
        (
            sb,
            " has no room for the new directory entry"
        );
        break;

    case ENOTDIR:
        libexplain_buffer_because(sb);
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                newpath,
                O_WRONLY | O_CREAT,
                "newpath"
            )
        )
        {
            /*
             * No specific cause was found, use the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "a component used as a directory in newpath is "
                "not, in fact, a directory"
            );
        }
        break;

    case EPERM:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the file system containing newpath"
        );
        libexplain_buffer_mount_point_dirname(sb, newpath);
        libexplain_string_buffer_puts
        (
            sb,
            " does not support the creation of symbolic links"
        );
        break;

    case EROFS:
        libexplain_buffer_because(sb);
        /* FIXME: which one? */
        libexplain_string_buffer_puts
        (
            sb,
            "newpath is on a read-only file system"
        );
        libexplain_buffer_mount_point_dirname(sb, newpath);
        break;

    default:
        /* no explanation for other errno values */
        break;
    }
}
