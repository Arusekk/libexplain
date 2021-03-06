/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2011, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>

#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/eperm.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/kill.h>
#include <libexplain/buffer/esrch.h>
#include <libexplain/buffer/pid_t_star.h>
#include <libexplain/buffer/signal.h>
#include <libexplain/explanation.h>


static void
explain_buffer_errno_kill_system_call(explain_string_buffer_t *sb, int errnum,
    pid_t pid, int sig)
{
    (void)errnum;
    explain_string_buffer_puts(sb, "kill(pid = ");
    explain_buffer_pid_t(sb, pid);
    explain_string_buffer_puts(sb, ", sig = ");
    explain_buffer_signal(sb, sig);
    explain_string_buffer_putc(sb, ')');
}


void
explain_buffer_errno_kill_explanation(explain_string_buffer_t *sb, int errnum,
    const char *syscall_name, pid_t pid, int sig)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/kill.html
     */
    (void)sig;
    switch (errnum)
    {
    case EINVAL:
        explain_buffer_einval_vague(sb, "sig");
        break;

    case EPERM:
        explain_buffer_eperm_kill(sb);
        break;

    case ESRCH:
        explain_buffer_esrch(sb, "pid", pid);
        break;

    default:
        explain_buffer_errno_generic(sb, errnum, syscall_name);
        break;
    }
}


void
explain_buffer_errno_kill(explain_string_buffer_t *sb, int errnum, pid_t pid,
    int sig)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_kill_system_call(&exp.system_call_sb, errnum, pid,
        sig);
    explain_buffer_errno_kill_explanation(&exp.explanation_sb, errnum, "kill",
        pid, sig);
    explain_explanation_assemble(&exp, sb);
}


/* vim: set ts=8 sw=4 et : */
