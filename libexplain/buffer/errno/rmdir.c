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
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/errno/rmdir.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/path_to_pid.h>
#include <libexplain/buffer/success.h>
#include <libexplain/capability.h>
#include <libexplain/count_directory_entries.h>
#include <libexplain/option.h>


void
libexplain_buffer_errno_rmdir(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname)
{
    libexplain_string_buffer_puts(sb, "rmdir(pathname = ");
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
    libexplain_buffer_failed(sb, errnum);

    libexplain_buffer_errno_rmdir_because(sb, errnum, pathname);
}


static int
last_component_is_dotdot(const char *pathname)
{
    while (*pathname == '/')
        ++pathname;
    for (;;)
    {
        const char      *begin;
        const char      *end;

        begin = pathname;
        ++pathname;
        while (*pathname && *pathname != '/')
            ++pathname;
        end = pathname;
        while (*pathname == '/')
            ++pathname;
        if (!*pathname)
        {
            /* we are at end of pathname */
            return (end - begin == 2 && begin[0] == '.' && begin[1] == '.');
        }
    }
}


static int
last_component_is_dot(const char *pathname)
{
    while (*pathname == '/')
        ++pathname;
    for (;;)
    {
        const char      *begin;
        const char      *end;

        begin = pathname;
        ++pathname;
        while (*pathname && *pathname != '/')
            ++pathname;
        end = pathname;
        while (*pathname == '/')
            ++pathname;
        if (!*pathname)
        {
            /* we are at end of pathname */
            return (end - begin == 1 && begin[0] == '.');
        }
    }
}


void
libexplain_buffer_errno_rmdir_because(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname)
{
    libexplain_final_t final_component;

    libexplain_final_init(&final_component);
    final_component.want_to_unlink = 1;
    final_component.must_be_a_directory = 1;

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
                "write access to the directory containing pathname was "
                "not allowed; or, one of the directories in the path "
                "prefix of pathname did not allow search permission"
            );
        }
        break;

    case EBUSY:
        if (last_component_is_dot(pathname))
        {
            /* BSD */
            goto case_einval;
        }
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "pathname is currently in use by the system or some process "
            "that prevents its removal"
        );
        libexplain_buffer_path_to_pid(sb, pathname);
#ifdef __linux__
        if (libexplain_option_dialect_specific())
        {
            libexplain_string_buffer_puts
            (
                sb,
                "; this means pathname is currently used as a mount point "
                "or is the root directory of the calling process"
            );
        }
#endif
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "pathname");
        break;

    case EINVAL:
        case_einval:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "pathname has \".\" as last component"
        );
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
                "pathname, or a component used as a directory in "
                "pathname, is not, in fact, a directory"
            );
        }
        break;

    case EEXIST:
    case ENOTEMPTY:
        libexplain_buffer_because(sb);
        if (last_component_is_dotdot(pathname))
        {
            libexplain_string_buffer_puts
            (
                sb,
                "pathname has \"..\" as its final component"
            );
        }
        else
        {
            int             count;

            libexplain_string_buffer_puts
            (
                sb,
                "pathname is not an empty directory; that is, it "
                "contains entries other than \".\" and \"..\""
            );
            count = libexplain_count_directory_entries(pathname);
            if (count > 0)
                libexplain_string_buffer_printf(sb, " (%d)", count);
        }
        break;

    case EPERM:
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
                "the directory containing pathname has the sticky bit "
                "(S_ISVTX) set and the process's effective user ID is neither "
                "the user ID of the file to be deleted nor that of the "
                "directory containing it, and the process is not privileged"
            );
#ifdef HAVE_SYS_CAPABILITY_H
            if (libexplain_option_dialect_specific())
            {
                libexplain_string_buffer_puts
                (
                    sb,
                    " (does not have the CAP_FOWNER capability)"
                );
            }
#endif
            libexplain_string_buffer_puts
            (
                sb,
                "; or, the file system containing pathname does not "
                "support the removal of directories"
            );
        }
        break;

    case EROFS:
        libexplain_buffer_erofs(sb, pathname, "pathname");
        break;

    default:
        /* no explanations for other errors */
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
