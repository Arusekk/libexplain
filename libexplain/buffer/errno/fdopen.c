/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/fcntl.h>

#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/fdopen.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/explanation.h>
#include <libexplain/open_flags.h>
#include <libexplain/string_flags.h>


static void
libexplain_buffer_errno_fdopen_system_call(libexplain_string_buffer_t *sb,
    int errnum, int fildes, const char *flags)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "fdopen(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_puts(sb, ", flags = ");
    libexplain_string_buffer_puts_quoted(sb, flags);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_fdopen_explanation(libexplain_string_buffer_t *sb,
    int errnum, int fildes, const char *flags)
{
    libexplain_string_flags_t sf;
    int             xflags;

    libexplain_string_flags_init(&sf, flags);
    sf.flags &= ~(O_CREAT | O_EXCL | O_NOCTTY | O_TRUNC);

    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/fdopen.html
     */
    switch (errnum)
    {
    case EINVAL:
        libexplain_string_flags_einval(&sf, sb, "flags");
        xflags = fcntl(fildes, F_GETFL);
        if (xflags >= 0 && xflags != sf.flags)
        {
            libexplain_string_buffer_puts(sb, ", the file descriptor flags (");
            libexplain_buffer_open_flags(sb, xflags);
            libexplain_string_buffer_puts
            (
                sb,
                ") do not match the flags specified ("
            );
            libexplain_buffer_open_flags(sb, sf.flags);
            libexplain_string_buffer_putc(sb, ')');
        }
        break;

    case ENOMEM:
        libexplain_buffer_enomem_user(sb);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_fdopen(libexplain_string_buffer_t *sb, int errnum,
    int fildes, const char *flags)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_fdopen_system_call
    (
        &exp.system_call_sb,
        errnum,
        fildes,
        flags
    );
    libexplain_buffer_errno_fdopen_explanation
    (
        &exp.explanation_sb,
        errnum,
        fildes,
        flags
    );
    libexplain_explanation_assemble(&exp, sb);
}
