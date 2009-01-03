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

#include <libexplain/buffer/eacces.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/efbig.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/errno/truncate.h>
#include <libexplain/buffer/etxtbsy.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>
#include <libexplain/option.h>


static void
libexplain_buffer_errno_truncate_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, long long length)
{
    libexplain_string_buffer_puts(sb, "truncate(pathname = ");
    if (errnum == EFAULT)
        libexplain_buffer_pointer(sb, pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_printf(sb, ", length = %lld)", length);
}


static void
libexplain_buffer_errno_truncate_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, long long length)
{
    libexplain_final_t final_component;

    libexplain_final_init(&final_component);
    final_component.want_to_write = 1;

    switch (errnum)
    {
    case EACCES:
        libexplain_buffer_eacces(sb, pathname, "pathname", &final_component);
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
        if (length < 0)
        {
            /* FIXME: i18n */
            libexplain_string_buffer_puts(sb, "length is negative");
            break;
        }

        {
            struct stat     st;

            /* FIXME: libexplain_buffer_wrong_file_type */
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
        libexplain_string_buffer_puts
        (
            sb,
            /*
             * xgettext:  This message is used to explain an EISDIR error
             * reported by the truncate(2) system call, in the case where the
             * named file is a directory.
             */
            i18n("the named file is a directory; directories may not be "
            "truncated, use rmdir(2) or remove(3) instead")
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
        libexplain_buffer_enotdir(sb, pathname, "pathname", &final_component);
        break;

    case EPERM:
        libexplain_string_buffer_puts
        (
            sb,
            /* FIXME: i18n */
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


void
libexplain_buffer_errno_truncate(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, long long length)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_truncate_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        length
    );
    libexplain_buffer_errno_truncate_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        length
    );
    libexplain_explanation_assemble(&exp, sb);
}
