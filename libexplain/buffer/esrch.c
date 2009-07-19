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

#include <libexplain/buffer/esrch.h>
#include <libexplain/buffer/gettext.h>


void
explain_buffer_esrch(explain_string_buffer_t *sb, pid_t pid)
{
    if (pid >= 0)
    {
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This error message is issued when a system call
             * reports an ESRCH error, in the case where the pid was
             * positive.
             */
            i18n("the process does not exist")
        );
    }
    else
    {
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This error message is issued when a system call
             * reports an ESRCH error, in the case where the pid was
             * negative.
             */
            i18n("the process group does not exist")
        );
    }
}
