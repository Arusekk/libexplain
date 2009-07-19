/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/buffer/dac.h>
#include <libexplain/buffer/eacces.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/errno/utime.h>
#include <libexplain/buffer/pathname.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/utimbuf.h>
#include <libexplain/explanation.h>
#include <libexplain/path_is_efault.h>


static void
explain_buffer_errno_utime_system_call(explain_string_buffer_t *sb,
    int errnum, const char *pathname, const struct utimbuf *times)
{
    (void)errnum;
    explain_string_buffer_puts(sb, "utime(pathname = ");
    explain_buffer_pathname(sb, pathname);
    explain_string_buffer_puts(sb, ", times = ");
    explain_buffer_utimbuf(sb, times);
    explain_string_buffer_putc(sb, ')');
}


static void
explain_buffer_errno_utime_explanation(explain_string_buffer_t *sb,
    int errnum, const char *pathname, const struct utimbuf *times)
{
    explain_final_t final_component;

    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/utime.html
     */
    explain_final_init(&final_component);
    if (times)
        final_component.want_to_modify_inode = 1;
    else
        final_component.want_to_write = 1;
    switch (errnum)
    {
    case EACCES:
        /*
         * if time is NULL, change to "now",
         * but need write permission
         */
        explain_buffer_eacces(sb, pathname, "pathname", &final_component);
        break;

    case EPERM:
        /*
         * if times is not NULL, change as given,
         * but need inode modify permission
         * */
        explain_buffer_eacces(sb, pathname, "pathname", &final_component);
        break;

    case EFAULT:
        if (explain_path_is_efault(pathname))
        {
            explain_buffer_efault(sb, "pathname");
            break;
        }
        if (explain_pointer_is_efault(times, sizeof(*times)))
        {
            explain_buffer_efault(sb, "times");
            break;
        }
        break;

    case ENOENT:
        explain_buffer_enoent(sb, pathname, "pathname", &final_component);
        break;

    case EROFS:
        explain_buffer_erofs(sb, pathname, "pathname");
        break;

    default:
        explain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
explain_buffer_errno_utime(explain_string_buffer_t *sb, int errnum,
    const char *pathname, const struct utimbuf *times)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_utime_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        times
    );
    explain_buffer_errno_utime_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        times
    );
    explain_explanation_assemble(&exp, sb);
}