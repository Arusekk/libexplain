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
#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/emlink.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/link.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/exdev.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/success.h>
#include <libexplain/dirname.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/string_buffer.h>


static int
get_mode(const char *pathname)
{
    struct stat     st;

    if (stat(pathname, &st) < 0)
        return S_IFREG;
    return st.st_mode;
}


void
libexplain_buffer_errno_link(libexplain_string_buffer_t *sb, int errnum,
    const char *oldpath, const char *newpath)
{
    int             oldpath_bad;
    libexplain_final_t oldpath_final_component;
    int             newpath_bad;
    libexplain_final_t newpath_final_component;

    oldpath_bad = errnum == EFAULT && libexplain_path_is_efault(oldpath);
    newpath_bad = errnum == EFAULT && libexplain_path_is_efault(newpath);
    libexplain_string_buffer_printf(sb, "link(oldpath = ");
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
    libexplain_buffer_failed(sb, errnum);

    libexplain_final_init(&oldpath_final_component);
    libexplain_final_init(&newpath_final_component);
    newpath_final_component.must_exist = 0;
    newpath_final_component.want_to_create = 1;
    newpath_final_component.st_mode = get_mode(oldpath);

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
                oldpath,
                "oldpath",
                &oldpath_final_component
            )
        &&
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                newpath,
                "newpath",
                &newpath_final_component
            )
        )
        {
            /*
             * Unable to pin point an exact cause, go with the generic
             * explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "write access to the directory containing newpath is "
                "denied, or search permission is denied for one of the "
                "directories in the path prefix of oldpath or newpath "
            );
        }
        break;

    case EEXIST:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "newpath already exists"
        );
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
        libexplain_buffer_eio_path(sb, oldpath);
        break;

    case ELOOP:
        libexplain_buffer_eloop2
        (
            sb,
            oldpath,
            "oldpath",
            &oldpath_final_component,
            newpath,
            "newpath",
            &newpath_final_component
        );
        break;

    case EMLINK:
        libexplain_buffer_emlink(sb, oldpath, newpath);
        break;

    case ENAMETOOLONG:
        libexplain_buffer_enametoolong2
        (
            sb,
            oldpath,
            "oldpath",
            &oldpath_final_component,
            newpath,
            "newpath",
            &newpath_final_component
        );
        break;

    case ENOENT:
        libexplain_buffer_enoent2
        (
            sb,
            oldpath,
            "oldpath",
            &oldpath_final_component,
            newpath,
            "newpath",
            &newpath_final_component
        );
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
                oldpath,
                "oldpath",
                &oldpath_final_component
            )
        &&
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                newpath,
                "newpath",
                &newpath_final_component
            )
        )
        {
            /*
             * Unable to pin point an exact cause,
             * so we will use the generic explanation.
             */
            libexplain_string_buffer_puts
            (
                sb,
                "a component used as a directory in oldpath or "
                "newpath is not, in fact, a directory"
            );
        }
        break;

    case EPERM:
        {
            struct stat     oldpath_st;

            libexplain_buffer_because(sb);
            if (stat(oldpath, &oldpath_st) >= 0)
            {
                if (S_ISDIR(oldpath_st.st_mode))
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "oldpath is a directory and it is not possible "
                        "to make hard links to directories; have you "
                        "considered using a symbolic link?"
                    );
                }
                else
                {
                    libexplain_string_buffer_puts
                    (
                        sb,
                        "the file system containing oldpath and newpath "
                        "does not support the creation of hard links"
                    );
                }
            }
            else
            {
                /*
                 * Unable to pin point a specific cause,
                 * issue the generic explanation.
                 */
                libexplain_string_buffer_puts
                (
                    sb,
                    "oldpath is a directory; or, the file system "
                    "containing oldpath and newpath does not support "
                    "the creation of hard links"
                );
            }
        }
        break;

    case EROFS:
        libexplain_buffer_erofs(sb, oldpath, "oldpath");
        break;

    case EXDEV:
        libexplain_buffer_exdev(sb, oldpath, newpath, "link");
        break;

    default:
        /* no explanation for other errno values */
        break;
    }

    /*
     * Let the user know where things stand after the failure.
     */
    {
        struct stat st;
        if (lstat(newpath, &st) == 0)
        {
            libexplain_string_buffer_puts
            (
                sb,
                "; note that newpath exists"
            );
        }
    }

}
