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

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/errno/readlink.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>
#include <libexplain/path_is_efault.h>


static void
libexplain_buffer_errno_readlink_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, char *data, int data_size)
{
    int             pathname_bad;

    pathname_bad = errnum == EFAULT && libexplain_path_is_efault(pathname);
    libexplain_string_buffer_puts(sb, "readlink(pathname = ");
    if (pathname_bad)
        libexplain_buffer_pointer(sb, pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_puts(sb, ", data = ");
    libexplain_buffer_pointer(sb, data);
    libexplain_string_buffer_printf(sb, ", data_size = %d)", data_size);
}


static void
libexplain_buffer_errno_readlink_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, char *data, int data_size)
{
    libexplain_final_t final_component;

    (void)data;
    libexplain_final_init(&final_component);
    final_component.want_to_read = 1;
    final_component.follow_symlink = 0;

    switch (errnum)
    {
    case EACCES:
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
                "search permission is denied for a component of pathname"
            );
        }
        break;

    case EFAULT:
        if (libexplain_path_is_efault(pathname))
            libexplain_buffer_efault(sb, "pathname");
        else
            libexplain_buffer_efault(sb, "data");
        break;

    case EINVAL:
        if (data_size <= 0)
            libexplain_string_buffer_puts(sb, "data_size is not positive");
        else
        {
            struct stat     st;

            if (lstat(pathname, &st) >= 0)
            {
                libexplain_string_buffer_puts(sb, "pathname is a ");
                libexplain_buffer_file_type(sb, st.st_mode);
                libexplain_string_buffer_puts(sb, ", not a ");
                libexplain_buffer_file_type(sb, S_IFLNK);
            }
            else
            {
                libexplain_string_buffer_puts(sb, "pathname is not a ");
                libexplain_buffer_file_type(sb, S_IFLNK);
            }
        }
        break;

    case EIO:
        libexplain_buffer_eio_path(sb, pathname);
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
        libexplain_buffer_enotdir(sb, pathname, "pathname", &final_component);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_readlink(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, char *data, int data_size)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_readlink_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        data,
        data_size
    );
    libexplain_buffer_errno_readlink_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        data,
        data_size
    );
    libexplain_explanation_assemble(&exp, sb);
}
