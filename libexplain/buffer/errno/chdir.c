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

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/chdir.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/strerror.h>
#include <libexplain/buffer/success.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_errno_chdir(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname)
{
    libexplain_string_buffer_printf(sb, "chdir(pathname = ");
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
            libexplain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                O_RDONLY + O_DIRECTORY,
                "pathname"
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "search permission is denied for one of the components "
                "of pathname"
            );
        }
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "pathname");
        break;

    case EIO:
        libexplain_buffer_eio_path(sb, pathname);
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
                O_RDONLY + O_DIRECTORY,
                "pathname"
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "too many symbolic links were encountered in resolving "
                "pathname"
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
                O_RDONLY + O_DIRECTORY,
                "pathname"
            )
        )
        {
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
                O_RDONLY + O_DIRECTORY,
                "pathname"
            )
        )
        {
            libexplain_string_buffer_puts(sb, "the file does not exist");
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
                O_RDONLY + O_DIRECTORY,
                "pathname"
            )
        )
        {
            libexplain_string_buffer_puts
            (
                sb,
                "pathname, or a component of pathname, is not a directory"
            );
        }
        break;

    default:
        /* no additional info for other errno values */
        break;
    }
}
