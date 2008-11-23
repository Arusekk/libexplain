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
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/errno/symlink.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/string_buffer.h>


static void
libexplain_buffer_errno_symlink_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *oldpath, const char *newpath)
{
    int             oldpath_bad;
    int             newpath_bad;

    oldpath_bad = errnum == EFAULT && libexplain_path_is_efault(oldpath);
    newpath_bad = errnum == EFAULT && libexplain_path_is_efault(newpath);
    libexplain_string_buffer_printf(sb, "symlink(oldpath = ");
    if (oldpath_bad)
        libexplain_buffer_pointer(sb, oldpath);
    else
        libexplain_string_buffer_puts_quoted(sb, oldpath);
    libexplain_string_buffer_puts(sb, ", newpath = ");
    if (newpath_bad)
        libexplain_buffer_pointer(sb, newpath);
    else
        libexplain_string_buffer_puts_quoted(sb, newpath);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_symlink_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *oldpath, const char *newpath)
{
    libexplain_final_t final_component;

    libexplain_final_init(&final_component);
    final_component.must_exist = 0;
    final_component.must_not_exist = 1;
    final_component.want_to_create = 1;
    final_component.st_mode = S_IFLNK;

    switch (errnum)
    {
    case EACCES:
        if
        (
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                newpath,
                "newpath",
                &final_component
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
        if (libexplain_path_is_efault(oldpath))
            libexplain_buffer_efault(sb, "oldpath");
        else if (libexplain_path_is_efault(newpath))
            libexplain_buffer_efault(sb, "newpath");
        else
            libexplain_buffer_efault(sb, "oldpath or newpath");
        break;

    case EIO:
        libexplain_buffer_eio_path_dirname(sb, newpath);
        break;

    case ELOOP:
    case EMLINK: /* BSD */
        libexplain_buffer_eloop(sb, newpath, "newpath", &final_component);
        break;

    case ENAMETOOLONG:
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
            else
            {
                libexplain_buffer_enametoolong
                (
                    sb,
                    newpath,
                    "newpath",
                    &final_component
                );
            }
        }
        break;

    case ENOENT:
        if (!*oldpath)
        {
            libexplain_string_buffer_puts
            (
                sb,
                "oldpath is the empty string; if you meant the current "
                "directory, use \".\" instead"
            );
            break;
        }
        libexplain_buffer_enoent(sb, newpath, "newpath", &final_component);
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    case ENOSPC:
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
        libexplain_buffer_enotdir(sb, newpath, "newpath", &final_component);
        break;

    case EPERM:
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
        libexplain_buffer_erofs(sb, newpath, "newpath");
        break;

    default:
        /* no explanation for other errno values */
        break;
    }
}


void
libexplain_buffer_errno_symlink(libexplain_string_buffer_t *sb, int errnum,
    const char *oldpath, const char *newpath)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_symlink_system_call
    (
        &exp.system_call_sb,
        errnum,
        oldpath,
        newpath
    );
    libexplain_buffer_errno_symlink_explanation
    (
        &exp.explanation_sb,
        errnum,
        oldpath,
        newpath
    );
    libexplain_explanation_assemble(&exp, sb);
}
