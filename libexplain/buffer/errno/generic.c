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

#include <libexplain/ac/errno.h>

#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/enfile.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/option.h>


void
libexplain_buffer_errno_generic(libexplain_string_buffer_t *sb, int errnum)
{
    switch (errnum)
    {
    case EMFILE:
        libexplain_buffer_emfile(sb);
        break;

    case ENFILE:
        libexplain_buffer_enfile(sb);
        break;

    case EPERM:
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used as a generic explanation
             * of an EPERM error returned by any system call that does
             * not provide a more specific explanation.
             */
            i18n("the process does not have the appropriate "
            "privileges to use this system call")
        );
        break;

    case EINTR:
        libexplain_buffer_eintr(sb, "operation");
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    case ENOBUFS:
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used as a generic explanation
             * of an ENOBUFS error returned by any system call that does
             * not provide a more specific explanation.
             */
            i18n("insufficient kernel resources are available in the "
            "system to complete the system call")
        );
        break;

    default:
        /*
         * no additional information for other errno values
         */
        if (libexplain_option_debug())
        {
            libexplain_string_buffer_puts
            (
                sb,
                "(this error is unknown for this system call, you could "
                "improve libexplain by contributing code to explain "
                "this error)"
            );
        }
        break;
    }
}
