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

#include <libexplain/buffer/errno/chown.h>
#include <libexplain/buffer/errno/fchown.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/gid.h>
#include <libexplain/buffer/uid.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_fchown_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes, int owner, int group)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "fchown(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_puts(sb, ", owner = ");
    libexplain_buffer_uid(sb, owner);
    libexplain_string_buffer_puts(sb, ", group = ");
    libexplain_buffer_gid(sb, group);
    libexplain_string_buffer_putc(sb, ')');
}


void
libexplain_buffer_errno_fchown(libexplain_string_buffer_t *sb, int errnum,
    int fildes, int owner, int group)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_fchown_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        owner,
        group
    );
    libexplain_buffer_errno_fchown_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        owner,
        group
    );
    libexplain_explanation_assemble(&exp, sb);
}
