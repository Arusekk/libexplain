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
#include <libexplain/ac/unistd.h>
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/efbig.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/errno/truncate.h>
#include <libexplain/buffer/etxtbsy.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/success.h>
#include <libexplain/option.h>


void
libexplain_buffer_errno_truncate(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, long long length)
{
    libexplain_final_t final_component;

    libexplain_string_buffer_puts(sb, "truncate(pathname = ");
    if (errnum == EFAULT)
        libexplain_string_buffer_printf(sb, "%p", pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_printf(sb, ", length = %lld)", length);
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_buffer_failed(sb, errnum);

    libexplain_final_init(&final_component);
    final_component.want_to_write = 1;

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
            libexplain_buffer_because(sb);
            libexplain_string_buffer_puts
            (
                sb,
                "search permission is denied for a dorectory component of "
                "pathname; or, pathname is not writable by the user "
            );
        }
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "pathname");
        break;

    case EFBIG:
        libexplain_buffer_efbig(sb, pathname, length, "length");
        break;

    case EINTR:
        libexplain_buffer_eintr(sb, "truncate");
        break;

    case EINVAL:
        libexplain_buffer_because(sb);
        if (length < 0)
        {
            libexplain_string_buffer_puts(sb, "length is negative");
            break;
        }

        {
            struct stat     st;

            if (stat(pathname, &st) >= 0 && !S_ISREG(st.st_mode))
            {
                libexplain_string_buffer_puts(sb, "pathname is a ");
                libexplain_buffer_file_type(sb, st.st_mode);
                libexplain_string_buffer_puts(sb, ", not a ");
                libexplain_buffer_file_type(sb, S_IFREG);
                break;
            }
        }

        libexplain_buffer_efbig(sb, pathname, length, "length");
        break;

    case EIO:
        libexplain_buffer_eio_path(sb, pathname);
        break;

    case EISDIR:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "the named file is a directory; "
            "directories may not be truncated, use unlink and rmdir instead"
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
            "the underlying file system does not support extending a "
            "file beyond its current size"
        );
        libexplain_buffer_mount_point(sb, pathname);
        break;

    case EROFS:
        libexplain_buffer_erofs(sb, pathname, "pathname");
        break;

    case ETXTBSY:
        libexplain_buffer_etxtbsy(sb, pathname);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}
