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

#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/no_outstanding_children.h>


void
libexplain_buffer_no_outstanding_children(libexplain_string_buffer_t *sb)
{
    libexplain_buffer_gettext
    (
        sb,
        /*
         * xgettext: This message is used when wait(2) is called and
         * the process does not have any unwaited-for child
         * processes.
         */
        i18n("the process does not have any unwaited-for child "
            "processes")
    );
}
