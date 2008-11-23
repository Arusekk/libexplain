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

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/fstat.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_fstat_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes, const struct stat *buf)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "fstat(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_puts(sb, ", buf = ");
    libexplain_buffer_pointer(sb, buf);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_fstat_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes, const struct stat *buf)
{
    (void)fildes;
    (void)buf;
    switch (errnum)
    {
    case EBADF:
        libexplain_buffer_ebadf(sb, "fildes");
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_fstat(libexplain_string_buffer_t *sb, int errnum,
    int fildes, const struct stat *buf)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_fstat_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        buf
    );
    libexplain_buffer_errno_fstat_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        buf
    );
    libexplain_explanation_assemble(&exp, sb);
}
