/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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

#include <libexplain/buffer/eacces.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/errno/chdir.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>
#include <libexplain/string_buffer.h>


static void
libexplain_buffer_errno_chdir_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname)
{
    libexplain_string_buffer_printf(sb, "chdir(pathname = ");
    if (errnum == EFAULT)
        libexplain_buffer_pointer(sb, pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_chdir_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname)
{
    libexplain_final_t final_component;

    libexplain_final_init(&final_component);
    final_component.want_to_search = 1;
    final_component.must_be_a_st_mode = 1;
    final_component.st_mode = S_IFDIR;

    switch (errnum)
    {
    case EACCES:
        libexplain_buffer_eacces(sb, pathname, "pathname", &final_component);
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "pathname");
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
libexplain_buffer_errno_chdir(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_chdir_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname
    );
    libexplain_buffer_errno_chdir_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname
    );
    libexplain_explanation_assemble(&exp, sb);
}
