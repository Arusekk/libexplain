/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2011 Peter Miller
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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/ebusy.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/gettext.h>


void
explain_buffer_ebusy(explain_string_buffer_t *sb, int fildes,
    const char *syscall_name)
{
    struct stat     st;
    char            file_type[100];

    if (fstat(fildes, &st) >= 0)
    {
        explain_string_buffer_t buf;

        explain_string_buffer_init(&buf, file_type, sizeof(file_type));
        explain_buffer_file_type_st(&buf, &st);
    }
    else
        snprintf(file_type, sizeof(file_type), "file");

    /*
     * some nebulous default explanation
     */
    explain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This error message is issued when a system call
         * reports an EBUSY error.
         */
        i18n("the %s is in use by another process or by the system "
            "and this prevents the %s system call from operating"),
        file_type,
        syscall_name
    );
}
