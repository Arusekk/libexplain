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
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/errno/dup.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_dup_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "dup(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_dup_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes)
{
    (void)fildes;
    switch (errnum)
    {
    case EBADF:
        libexplain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case EBUSY:
        libexplain_string_buffer_puts(sb, "a race condition was detected");
        break;

    case EINTR:
        libexplain_buffer_eintr(sb, "dup");
        break;

    case EMFILE:
        libexplain_buffer_emfile(sb);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_dup(libexplain_string_buffer_t *sb, int errnum,
    int fildes)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_dup_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes
    );
    libexplain_buffer_errno_dup_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes
    );
    libexplain_explanation_assemble(&exp, sb);
}
