/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#include <libexplain/buffer/einval.h>


void
libexplain_buffer_einval_bits(libexplain_string_buffer_t *sb,
    const char *caption)
{
    libexplain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used when explaining an EINVAL
         * error returned by a system call that is complaining about
         * undefined bits in a bitfield argument, e.g. access(2).
         *
         * %1$s => the name of the offending system call argument
         */
        i18n("%s was incorrectly specified, it contains undefined bits"),
        caption
    );
}
