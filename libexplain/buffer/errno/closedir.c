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

#include <libexplain/buffer/dir_to_pathname.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/close.h>
#include <libexplain/buffer/errno/closedir.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/dir_to_fildes.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_closedir_system_call(libexplain_string_buffer_t *sb,
    int errnum, DIR *dir)
{
    (void)errnum;
    libexplain_string_buffer_puts(sb, "closedir(dir = ");
    libexplain_buffer_pointer(sb, dir);
    /* libc has probably nuked it, but we live in hope */
    libexplain_buffer_dir_to_pathname(sb, dir);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_closedir_explanation(libexplain_string_buffer_t *sb,
    int errnum, DIR *dir)
{
    int             fildes;

    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/closedir.html
     */
    if (dir == NULL)
    {
        libexplain_string_buffer_puts(sb, "dir is the NULL pointer");
        return;
    }
    if (errnum == EFAULT)
    {
        libexplain_buffer_efault(sb, "dir");
        return;
    }
    fildes = libexplain_dir_to_fildes(dir);
    libexplain_buffer_errno_close_explanation(sb, errno, fildes);
}


void
libexplain_buffer_errno_closedir(libexplain_string_buffer_t *sb, int errnum,
    DIR *dir)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_closedir_system_call
    (
        &exp.system_call_sb,
        errnum,
        dir
    );
    libexplain_buffer_errno_closedir_explanation
    (
        &exp.explanation_sb,
        errnum,
        dir
    );
    libexplain_explanation_assemble(&exp, sb);
}
