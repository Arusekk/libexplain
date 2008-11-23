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
#include <libexplain/ac/signal.h>
#include <libexplain/ac/sys/wait.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/waitpid.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/waitpid_options.h>
#include <libexplain/buffer/note/sigchld.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_waitpid_system_call(libexplain_string_buffer_t *sb,
    int errnum, int pid, int *status, int options)
{
    (void)errnum;
    libexplain_string_buffer_printf(sb, "waitpid(pid = %d", pid);
    if (pid == 0)
        libexplain_string_buffer_printf(sb, " = process group %d", getpgrp());
    else if (pid < -1)
        libexplain_string_buffer_printf(sb, " = process group %d", -pid);
    libexplain_string_buffer_puts(sb, ", status = ");
    libexplain_buffer_pointer(sb, status);
    libexplain_string_buffer_puts(sb, ", options = ");
    libexplain_buffer_waitpid_options(sb, options);
    libexplain_string_buffer_putc(sb, ')');
}


static int
process_exists(int pid)
{
    return (kill(pid, 0) >= 0 || errno != ESRCH);
}


void
libexplain_buffer_errno_waitpid_explanation(libexplain_string_buffer_t *sb,
    int errnum, int pid, int *status, int options)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/waitpid.html
     */
    (void)status;
    switch (errnum)
    {
    case ECHILD:
        if (pid > 0)
        {
            if (process_exists(pid))
            {
                libexplain_string_buffer_printf_gettext
                (
                    sb,
                    /*
                     * xgettext:  This message is use when a wait*()
                     * function is asked to wait for a process that is
                     * not a child of the calling process.
                     *
                     * The %s string is the name of the offending
                     * argument.
                     */
                    i18n("the process specified by %s is not a child of "
                        "the calling process"),
                    "pid"
                );
            }
            else
            {
                libexplain_string_buffer_printf_gettext
                (
                    sb,
                    /*
                     * xgettext:  This message is use when a wait*()
                     * function is asked to wait for a process that does
                     * not exist.
                     *
                     * The %s string is the name of the offending
                     * argument.
                     */
                    i18n("the process specified by %s does not exist"),
                    "pid"
                );
            }
        }
        else if (pid == -1)
        {
            libexplain_buffer_gettext
            (
                sb,
                /*
                 * xgettext: This message is used when wait(2) is called and
                 * the calling process does not have any unwaited-for child
                 * processes.
                 */
                i18n("the calling process does not have any unwaited-for "
                    "child processes")
            );
        }
        else
        {
            int             pgid;

            pgid = pid ? -pid : getpgrp();
            if (process_exists(pid))
            {
                libexplain_string_buffer_printf_gettext
                (
                    sb,
                    /*
                     * xgettext: This message is used when a wait*()
                     * function was called to wait for a process group
                     * that does not have any member process that is a
                     * child of the calling proces.
                     *
                     * The %d value is the process group number.
                     */
                    i18n("process group %d does not have any member process "
                        "that is a child of the calling process"),
                    pgid
                );
            }
            else
            {
                libexplain_string_buffer_printf_gettext
                (
                    sb,
                    /*
                     * xgettext: This message is used when a wait*()
                     * function was called to wait for a process group
                     * that does not exist.
                     *
                     * The %d value is the process group number.
                     */
                    i18n("process group %d does not exist"),
                    pgid
                );
            }
        }
        libexplain_buffer_note_sigchld(sb);
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "status");
        break;

    case EINTR:
        /*
         * WNOHANG was not set and an unblocked signal or a SIGCHLD was
         * caught.
         */
        libexplain_buffer_eintr(sb, "waitpid");

    case EINVAL:
        libexplain_string_buffer_printf_gettext
        (
            sb,
            /*
             * xgettext: This message is used when an argment of a
             * system call is invalid.
             *
             * The %s string is the name of the offending argument.
             */
            i18n("the %s argument was invalid"),
            "options"
        );
        libexplain_string_buffer_printf
        (
            sb,
            " (%#x)",
            options & ~(WCONTINUED | WNOHANG | WUNTRACED)
        );
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_waitpid(libexplain_string_buffer_t *sb, int errnum,
    int pid, int *status, int options)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_waitpid_system_call
    (
        &exp.system_call_sb,
        errnum,
        pid,
        status,
        options
    );
    libexplain_buffer_errno_waitpid_explanation
    (
        &exp.explanation_sb,
        errnum,
        pid,
        status,
        options
    );
    libexplain_explanation_assemble(&exp, sb);
}
