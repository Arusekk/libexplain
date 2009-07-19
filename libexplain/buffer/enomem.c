/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/ac/sys/resource.h>

#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/rlimit.h>


void
explain_buffer_enomem_kernel(explain_string_buffer_t *sb)
{
    explain_buffer_gettext
    (
        sb,
        /*
         * xgettext:  This message is used when explaining an ENOMEM
         * error, when it is specific to kernel memory.
         */
        i18n("insufficient kernel memory was available")
    );
}


void
explain_buffer_enomem_user(explain_string_buffer_t *sb)
{
    struct rlimit   rlim;

    explain_buffer_gettext
    (
        sb,
        /*
         * xgettext:  This message is used when explaining an ENOMEM
         * error, when it is specific to user space memory.
         *
         * Note that this may be followed by the actual limit, if
         * available.
         */
        i18n("insufficient user-space memory was available")
    );

    /*
     * Try to provide more information about memory limits.
     */
    if (getrlimit(RLIMIT_DATA, &rlim) >= 0)
    {
        if (rlim.rlim_cur == RLIM_INFINITY)
        {
            explain_string_buffer_puts(sb, ", ");
            explain_buffer_gettext
            (
                sb,
                /*
                 * xgettext:  This message is used when supplementing
                 * an explation an ENOMEM error, when it is specific to
                 * user space memory, and the process has an infinite
                 * memory limit, meaning that a system limit on the
                 * total amout of user space memory available to all
                 * processes has been exhausted.
                 */
                i18n("probably by exhausting swap space")
            );
        }
        else
        {
            explain_string_buffer_putc(sb, ' ');
            explain_buffer_rlimit(sb, &rlim);
        }
    }
}


void
explain_buffer_enomem_kernel_or_user(explain_string_buffer_t *sb)
{
    explain_buffer_gettext
    (
        sb,
        /*
         * xgettext:  This message is used when explaining an ENOMEM
         * error, when it is not possible to distinguish whether it was
         * kernel memory of user space memory that was exhausted.
         */
        i18n("insufficient user-space or kernel memory was available")
    );
}
