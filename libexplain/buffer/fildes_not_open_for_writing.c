/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/fcntl.h>

#include <libexplain/buffer/fildes_not_open_for_writing.h>
#include <libexplain/open_flags.h>


int
explain_buffer_fildes_not_open_for_writing(explain_string_buffer_t *sb,
    int fildes, const char *fildes_caption)
{
    int             flags;

    flags = fcntl(fildes, F_GETFL);
    if (flags < 0)
        return -1;
    switch (flags & O_ACCMODE)
    {
    case O_WRONLY:
    case O_RDWR:
        return -1;

    default:
        break;
    }

    explain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used when an attempt is made to write to
         * a file descriptor that was not opened for writing.  The actual open
         * mode will be printed separately.
         *
         * %1$s => The name of the offending system call argument
         */
        i18n("%s is not open for writing"),
        fildes_caption
    );
    explain_string_buffer_puts(sb, " (");
    explain_buffer_open_flags(sb, flags);
    explain_string_buffer_putc(sb, ')');
    return 0;
}
