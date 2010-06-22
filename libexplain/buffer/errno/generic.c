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

#include <libexplain/ac/errno.h>

#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/enfile.h>
#include <libexplain/buffer/enobufs.h>
#include <libexplain/buffer/enomedium.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/enosys.h>
#include <libexplain/buffer/eoverflow.h>
#include <libexplain/buffer/erange.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/ewouldblock.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/option.h>


void
explain_buffer_errno_generic(explain_string_buffer_t *sb, int errnum,
    const char *syscall_name)
{
    switch (errnum)
    {
    case EAGAIN:
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
    case EWOULDBLOCK:
#endif
        explain_buffer_ewouldblock(sb, syscall_name);
        break;

    case EMFILE:
        explain_buffer_emfile(sb);
        break;

    case ENFILE:
        explain_buffer_enfile(sb);
        break;

    case EPERM:
        explain_string_buffer_printf_gettext
        (
            sb,
            /*
             * xgettext: This message is used as a generic explanation
             * of an EPERM error returned by any system call that does
             * not provide a more specific explanation.
             *
             * %1$s => the name of the offending system call
             */
            i18n("the process does not have the appropriate "
                "privileges to use the %s system call"),
            syscall_name
        );
        break;

    case EINTR:
        explain_buffer_eintr(sb, syscall_name);
        break;

#ifdef ENOMEDIUM
    case ENOMEDIUM:
        explain_buffer_enomedium_generic(sb);
        break;
#endif

    case ENOMEM:
        explain_buffer_enomem_kernel(sb);
        break;

    case ENOBUFS:
        explain_buffer_enobufs(sb);
        break;

    case ENOSYS:
    case ENOTTY:
#if defined(EOPNOTSUPP) && EOPNOTSUPP != ENOSYS
    case EOPNOTSUPP:
#endif
#if defined(ENOTSUP) && (ENOTSUP != EOPNOTSUPP)
    case ENOTSUP:
#endif
#ifdef ENOIOCTL
    case ENOIOCTL:
#endif
        explain_buffer_enosys_vague(sb, syscall_name);
        break;

    case ERANGE:
        explain_buffer_erange(sb);
        break;

    case EOVERFLOW:
        explain_buffer_eoverflow(sb);
        break;

    default:
        /*
         * no additional information for other errno values
         */
        if (explain_option_debug())
        {
            explain_string_buffer_printf
            (
                sb,
                "something weird happened, cause unknown (this error is "
                    "undocumented for the %s system call, you could improve "
                    "libexplain by contributing code to explain this error, "
                    "see the libexplain/buffer/errno/%s.c source file)",
                syscall_name,
                syscall_name
            );
        }
        break;
    }
}
