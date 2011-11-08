/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011 Peter Miller
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

#include <libexplain/ac/sys/resource.h>

#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/size_t.h>


int
explain_buffer_enomem_rlimit_exceeded(explain_string_buffer_t *sb, size_t size)
{
    struct rlimit   rlim;

    if (getrlimit(RLIMIT_AS, &rlim) >= 0)
    {
        if (rlim.rlim_cur + size > rlim.rlim_max)
        {
            explain_buffer_gettext
            (
                sb,
                /**
                  * xgettext:  This message is used to explain an ENOMEM error
                  * reported by a system call (e.g. mmap or shmat), in the case
                  * where the virtual memory size of the process would have been
                  * exceeded.  The relevant getrlimit values will be printed
                  * separately.
                  */
                i18n("the virtual memory size limit of the process "
                    "would have been exceeded")
            );

            /*
             * Now print the limits, while being wary about exactly how
             * big a size_t actually is.
             */
            explain_string_buffer_puts(sb, " (RLIMIT_AS: ");
            explain_buffer_size_t(sb, rlim.rlim_cur);
            explain_string_buffer_puts(sb, " + ");
            explain_buffer_size_t(sb, size);
            explain_string_buffer_puts(sb, " > ");
            explain_buffer_size_t(sb, rlim.rlim_max);
            explain_string_buffer_putc(sb, ')');
            return 1;
        }
    }
    return 0;
}


/* vim: set ts=8 sw=4 et : */
