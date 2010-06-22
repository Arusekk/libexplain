/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/does_not_have_inode_modify_permission.h>
#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eio.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/fchmod.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/permission_mode.h>
#include <libexplain/buffer/uid.h>
#include <libexplain/capability.h>
#include <libexplain/explanation.h>
#include <libexplain/option.h>


static void
explain_buffer_errno_fchmod_system_call(explain_string_buffer_t *sb,
    int errnum, int fildes, int mode)
{
    (void)errnum;
    explain_string_buffer_printf(sb, "fchmod(fildes = %d", fildes);
    explain_buffer_fildes_to_pathname(sb, fildes);
    explain_string_buffer_puts(sb, ", mode = ");
    explain_buffer_permission_mode(sb, mode);
    explain_string_buffer_putc(sb, ')');
}


static void
explain_buffer_errno_fchmod_explanation(explain_string_buffer_t *sb,
    int errnum, int fildes, int mode)
{
    (void)mode;
    switch (errnum)
    {
    case EBADF:
        explain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case EIO:
        explain_buffer_eio_fildes(sb, fildes);
        break;

    case EPERM:
        explain_buffer_does_not_have_inode_modify_permission_fd
        (
            sb,
            fildes,
            "fildes"
        );
        break;

    case EROFS:
        explain_buffer_erofs_fildes(sb, fildes, "fildes");
        break;

    default:
        explain_buffer_errno_generic(sb, errnum, "fchmod");
        break;
    }
}


void
explain_buffer_errno_fchmod(explain_string_buffer_t *sb, int errnum,
    int fildes, int mode)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_fchmod_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        mode
    );
    explain_buffer_errno_fchmod_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        mode
    );
    explain_explanation_assemble(&exp, sb);
}
