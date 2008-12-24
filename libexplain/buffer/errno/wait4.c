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
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/errno/wait4.h>
#include <libexplain/buffer/errno/waitpid.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/waitpid_options.h>
#include <libexplain/explanation.h>
#include <libexplain/path_is_efault.h>


static void
libexplain_buffer_errno_wait4_system_call(libexplain_string_buffer_t *sb,
    int errnum, int pid, int *status, int options, struct rusage *rusage)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "wait4(pid = %d", pid);
    if (pid == 0)
        libexplain_string_buffer_printf(sb, " = process group %d", getpgrp());
    else if (pid < -1)
        libexplain_string_buffer_printf(sb, " = process group %d", -pid);
    libexplain_string_buffer_puts(sb, ", status = ");
    libexplain_buffer_pointer(sb, status);
    libexplain_string_buffer_puts(sb, ", options = ");
    libexplain_buffer_waitpid_options(sb, options);
    libexplain_string_buffer_puts(sb, ", rusage = ");
    libexplain_buffer_pointer(sb, rusage);
    libexplain_string_buffer_putc(sb, ')');
}


void
libexplain_buffer_errno_wait4_explanation(libexplain_string_buffer_t *sb,
    int errnum, int pid, int *status, int options, struct rusage *rusage)
{
    switch (errnum)
    {
    case EFAULT:
        if (rusage && libexplain_pointer_is_efault(rusage))
        {
            libexplain_buffer_efault(sb, "rusage");
            break;
        }
        if (libexplain_pointer_is_efault(status))
        {
            libexplain_buffer_efault(sb, "status");
            break;
        }
        break;

    default:
        libexplain_buffer_errno_waitpid_explanation
        (
            sb,
            errnum,
            pid,
            status,
            options
        );
        break;
    }
}


void
libexplain_buffer_errno_wait4(libexplain_string_buffer_t *sb, int errnum,
    int pid, int *status, int options, struct rusage *rusage)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_wait4_system_call
    (
        &exp.system_call_sb,
        errnum,
        pid,
        status,
        options,
        rusage
    );
    libexplain_buffer_errno_wait4_explanation
    (
        &exp.explanation_sb,
        errnum,
        pid,
        status,
        options,
        rusage
    );
    libexplain_explanation_assemble(&exp, sb);
}
