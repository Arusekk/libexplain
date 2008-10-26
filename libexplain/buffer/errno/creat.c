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
#include <libexplain/ac/fcntl.h>

#include <libexplain/buffer/errno/creat.h>
#include <libexplain/buffer/errno/open.h>
#include <libexplain/buffer/strerror.h>
#include <libexplain/buffer/success.h>
#include <libexplain/permission_mode.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_errno_creat(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, int mode)
{
    int             flags;

    libexplain_string_buffer_printf(sb, "creat(pathname = ");
    if (errnum == EFAULT)
        libexplain_string_buffer_printf(sb, "%p", pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_puts(sb, ", mode = ");
    libexplain_buffer_permission_mode(sb, mode);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_string_buffer_puts(sb, " failed, ");
    libexplain_buffer_strerror(sb, errnum);

    flags = O_WRONLY | O_CREAT | O_TRUNC;

    libexplain_buffer_errno_open_because(sb, errnum, pathname, flags, mode);
}
