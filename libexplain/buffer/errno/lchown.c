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

#include <libexplain/buffer/errno/chown.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/lchown.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/gid.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/uid.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_lchown_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, int owner, int group)
{
    libexplain_string_buffer_puts(sb, "lchown(pathname = ");
    if (errnum == EFAULT)
        libexplain_buffer_pointer(sb, pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_puts(sb, ", owner = ");
    libexplain_buffer_uid(sb, owner);
    libexplain_string_buffer_puts(sb, ", group = ");
    libexplain_buffer_gid(sb, group);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_lchown_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, int owner, int group)
{
    libexplain_final_t final_component;

    libexplain_final_init(&final_component);
    final_component.want_to_modify_inode = 1;
    final_component.follow_symlink = 0;

    libexplain_buffer_errno_chown_explanation_fc
    (
        sb,
        errnum,
        pathname,
        owner,
        group,
        &final_component
    );
}


void
libexplain_buffer_errno_lchown(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, int owner, int group)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_lchown_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        owner,
        group
    );
    libexplain_buffer_errno_lchown_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        owner,
        group
    );
    libexplain_explanation_assemble(&exp, sb);
}
