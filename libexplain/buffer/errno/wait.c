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

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eintr.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/wait.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/note/sigchld.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_wait_system_call(libexplain_string_buffer_t *sb,
    int errnum, int *status)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "wait(status = ");
    libexplain_buffer_pointer(sb, status);
    libexplain_string_buffer_putc(sb, ')');
}


void
libexplain_buffer_errno_wait_explanation(libexplain_string_buffer_t *sb,
    int errnum, int *status)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/wait.html
     */
    (void)status;
    switch (errnum)
    {
    case ECHILD:
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
        libexplain_buffer_note_sigchld(sb);
        break;

    case EFAULT:
        libexplain_buffer_efault(sb, "status");
        break;

    case EINTR:
        libexplain_buffer_eintr(sb, "wait");
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_wait(libexplain_string_buffer_t *sb, int errnum,
    int *status)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_wait_system_call
    (
        &exp.system_call_sb,
        errnum,
        status
    );
    libexplain_buffer_errno_wait_explanation
    (
        &exp.explanation_sb,
        errnum,
        status
    );
    libexplain_explanation_assemble(&exp, sb);
}
