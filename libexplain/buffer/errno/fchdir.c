/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/errno/fchdir.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/explanation.h>
#include <libexplain/string_buffer.h>


static void
libexplain_buffer_errno_fchdir_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "fchdir(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_fchdir_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes)
{
    switch (errnum)
    {
    case EBADF:
        libexplain_buffer_ebadf(sb, "fildes");
        break;

    case ENOTDIR:
        libexplain_buffer_enotdir_fd(sb, fildes, "fildes");
        break;

    case EACCES:
        libexplain_string_buffer_puts
        (
            sb,
            "search permission was denied on the directory"
        );
        break;

    default:
        /* no additional info for other errno values */
        break;
    }
}


void
libexplain_buffer_errno_fchdir(libexplain_string_buffer_t *sb, int errnum,
    int fildes)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_fchdir_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes
    );
    libexplain_buffer_errno_fchdir_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes
    );
    libexplain_explanation_assemble(&exp, sb);
}
