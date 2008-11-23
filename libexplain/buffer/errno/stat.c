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
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/errno/stat.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/string_buffer.h>


static void
libexplain_buffer_errno_stat_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, const struct stat *buf)
{
    libexplain_string_buffer_printf(sb, "stat(pathname = ");
    if (errnum == EFAULT && libexplain_path_is_efault(pathname))
        libexplain_buffer_pointer(sb, pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_printf(sb, ", buf = %p)", buf);
}


static void
libexplain_buffer_errno_stat_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, const struct stat *buf)
{
    libexplain_final_t final_component;

    (void)errnum;
    (void)buf;
    libexplain_final_init(&final_component);

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
                "search permission is denied for one of the directories "
                "in the path prefix of pathname"
            );
        }
        break;

    case EFAULT:
        if (libexplain_path_is_efault(pathname))
            libexplain_buffer_efault(sb, "pathname");
        else
            libexplain_buffer_efault(sb, "buf");
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
        /* no additional info for other errno values */
        break;
    }
}


void
libexplain_buffer_errno_stat(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, const struct stat *buf)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_stat_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        buf
    );
    libexplain_buffer_errno_stat_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        buf
    );
    libexplain_explanation_assemble(&exp, sb);
}
