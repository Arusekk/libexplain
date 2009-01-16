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
libexplain_buffer_errno_utime_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, const struct utimbuf *times)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "utime(pathname = ");
    libexplain_buffer_pathname(sb, pathname);
    libexplain_string_buffer_puts(sb, ", times = ");
    libexplain_buffer_utimbuf(sb, times);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_utime_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname, const struct utimbuf *times)
{
    libexplain_final_t final_component;

    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/utime.html
     */
    libexplain_final_init(&final_component);
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
        libexplain_buffer_eacces(sb, pathname, "pathname", &final_component);
        break;

    case EPERM:
        /*
         * if times is not NULL, change as given,
         * but need inode modify permission
         * */
        libexplain_buffer_eacces(sb, pathname, "pathname", &final_component);
        break;

    case EFAULT:
        if (libexplain_path_is_efault(pathname))
        {
            libexplain_buffer_efault(sb, "pathname");
            break;
        }
        if (libexplain_pointer_is_efault(times, sizeof(*times)))
        {
            libexplain_buffer_efault(sb, "times");
            break;
        }
        break;

    case ENOENT:
        libexplain_buffer_enoent(sb, pathname, "pathname", &final_component);
        break;

    case EROFS:
        libexplain_buffer_erofs(sb, pathname, "pathname");
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}


void
libexplain_buffer_errno_utime(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname, const struct utimbuf *times)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_utime_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        times
    );
    libexplain_buffer_errno_utime_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname,
        times
    );
    libexplain_explanation_assemble(&exp, sb);
}
