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

#include <libexplain/buffer/errno/fork.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/system.h>
#include <libexplain/buffer/errno/wait.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_system_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *command)
{
    libexplain_string_buffer_puts(sb, "system(command = ");
    if (!command)
        libexplain_string_buffer_puts(sb, "NULL");
    else if (errnum == EFAULT)
        libexplain_buffer_pointer(sb, command);
    else
        libexplain_string_buffer_puts_quoted(sb, command);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_system_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *command)
{
    int             junk;

    /*
     * Here is the relevant page of The Single Unix Standard:
     * http://www.opengroup.org/onlinepubs/009695399/functions/system.html
     */
    (void)command;
    switch (errnum)
    {
    case EAGAIN:
    case ENOMEM:
        libexplain_buffer_errno_fork_explanation(sb, errnum);
        break;

    case ECHILD:
    case EINTR:
    case EINVAL:
        libexplain_buffer_errno_wait_explanation(sb, errnum, &junk);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_system(libexplain_string_buffer_t *sb, int errnum,
    const char *command)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_system_system_call
    (
        &exp.system_call_sb,
        errnum,
        command
    );
    libexplain_buffer_errno_system_explanation
    (
        &exp.explanation_sb,
        errnum,
        command
    );
    libexplain_explanation_assemble(&exp, sb);
}
