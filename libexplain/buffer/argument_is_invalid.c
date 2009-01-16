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

#include <libexplain/buffer/argument_is_invalid.h>


void
libexplain_buffer_argument_is_invalid(libexplain_string_buffer_t *sb,
    const char *name, long value)
{
    libexplain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used to explain an EINVAL
         * error reported by the accept(2) system call (and others).
         *
         * %1$d => The name of the argument that has an invalid value
         * %2$ld => The actual size passed (because the
         *         recapitualtion fo the system call is going to
         *         have a pointer in it, not the actual value).
         */
        i18n("%s is invalid (%ld)"),
        name,
        value
    );
}
