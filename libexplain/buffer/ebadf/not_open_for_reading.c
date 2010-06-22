/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/open_flags.h>


void
explain_buffer_ebadf_not_open_for_reading(explain_string_buffer_t *sb,
    int flags)
{
    explain_buffer_gettext
    (
        sb,
        /*
         * xgettext: This message is used when an attempt is made to read from
         * a file descriptor that was not opened for reading.  The actual open
         * mode will be printed separately.
         */
        i18n("the file descriptor is not open for reading")
    );
    explain_string_buffer_puts(sb, " (");
    explain_buffer_open_flags(sb, flags);
    explain_string_buffer_putc(sb, ')');
}
