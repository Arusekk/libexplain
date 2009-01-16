/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/buffer/emfile.h>
#include <libexplain/buffer/enfile.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/popen.h>
#include <libexplain/buffer/pathname.h>
#include <libexplain/explanation.h>
#include <libexplain/string_flags.h>


static void
libexplain_buffer_errno_popen_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *command, const char *flags)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "popen(command = ");
    libexplain_buffer_pathname(sb, command);
    libexplain_string_buffer_puts(sb, ", flags = ");
    libexplain_buffer_pathname(sb, flags);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_popen_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *command, const char *flags)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/popen.html
     */
    (void)command;
    switch (errnum)
    {
    case EINVAL:
        {
            libexplain_string_flags_t sf;

            libexplain_string_flags_init(&sf, flags);
            libexplain_string_flags_einval(&sf, sb, "flags");
        }
        break;

    case EMFILE:
        libexplain_buffer_emfile(sb);
        break;

    case ENFILE:
        libexplain_buffer_emfile(sb);
        break;

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_popen(libexplain_string_buffer_t *sb, int errnum,
    const char *command, const char *flags)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_popen_system_call
    (
        &exp.system_call_sb,
        errnum,
        command,
        flags
    );
    libexplain_buffer_errno_popen_explanation
    (
        &exp.explanation_sb,
        errnum,
        command,
        flags
    );
    libexplain_explanation_assemble(&exp, sb);
}

/* vim:ts=8:sw=4:et */
