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
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/mkdir.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/permission_mode.h>
#include <libexplain/buffer/success.h>


void
libexplain_buffer_errno_mkdir(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, int mode)
{
    libexplain_final_t final_component;

    libexplain_string_buffer_puts(sb, "mkdir(pathname = ");
    if (errnum == EFAULT)
        libexplain_string_buffer_printf(sb, "%p", pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_puts(sb, ", mode = ");
    libexplain_buffer_permission_mode(sb, mode);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_buffer_failed(sb, errnum);

    libexplain_final_init(&final_component);
    final_component.must_exist = 0;
    final_component.must_not_exist = 1;
    final_component.want_to_create = 1;
    final_component.follow_symlink = 0;
    final_component.st_mode = S_IFDIR | mode;

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
                "pathname",
                &final_component
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "the parent directory does not allow write permission "
                "to the process; or, one of the directories in pathname "
                "did not allow search permission"
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
                "pathname",
                &final_component
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "pathname already exists (not necessarily as a "
                "directory); this includes the case where pathname is a "
                "symbolic link, dangling or not"
            );
        }
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "pathname");
        break;

    case ELOOP:
    case EMLINK: /* BSD */
        libexplain_buffer_eloop(sb, pathname, "pathname", &final_component);
        break;

    case ENAMETOOLONG:
        libexplain_buffer_enametoolong
        (
            sb,
            pathname,
            "pathname",
            &final_component
        );
        break;

    case ENOENT:
        libexplain_buffer_enoent(sb, pathname, "pathname", &final_component);
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    case ENOSPC:
        libexplain_buffer_because(sb);
        // FIXME: ENOSPC can be caused by quota system, too.
        libexplain_string_buffer_puts
        (
            sb,
            "the file system containing pathname has no room for the new "
            "directory"
        );
        libexplain_buffer_mount_point_dirname(sb, pathname);
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
                "pathname",
                &final_component
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "a directory component of pathname is not, in fact, a directory"
            );
        }
        break;

    case EPERM:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the file system containing pathname does not support the "
            "creation of directories"
        );
        libexplain_buffer_mount_point_dirname(sb, pathname);
        break;

    case EROFS:
        libexplain_buffer_erofs(sb, pathname, "pathname");
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}
