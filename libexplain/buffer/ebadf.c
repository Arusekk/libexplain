/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#include <libexplain/buffer/check_fildes_range.h>
#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/software_error.h>
#include <libexplain/gettext.h>


void
libexplain_buffer_ebadf(libexplain_string_buffer_t *sb, int fildes,
    const char *caption)
{
    if (libexplain_buffer_check_fildes_range(sb, fildes, caption) >= 0)
        libexplain_string_buffer_puts(sb, ", ");
    libexplain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used when a file descriptor is not
         * valid and does not refer to an open file.
         *
         * %1$s => the name of the offending system call argument.
         */
        i18n("%s does not refer to an open file"),
        caption
    );
    libexplain_buffer_software_error(sb);
}
