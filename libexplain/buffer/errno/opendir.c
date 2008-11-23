/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/opendir.h>
#include <libexplain/buffer/errno/open.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/explanation.h>


static void
libexplain_buffer_errno_opendir_system_call(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname)
{
    libexplain_string_buffer_printf(sb, "opendir(pathname = ");
    if (errnum == EFAULT)
        libexplain_buffer_pointer(sb, pathname);
    else
        libexplain_string_buffer_puts_quoted(sb, pathname);
    libexplain_string_buffer_putc(sb, ')');
}


static void
libexplain_buffer_errno_opendir_explanation(libexplain_string_buffer_t *sb,
    int errnum, const char *pathname)
{
    switch (errnum)
    {
    case 0:
        break;

    case ENOMEM:
        {
            int             fd;

            errno = 0;
            fd = open(pathname, O_RDONLY + O_DIRECTORY);
            if (fd < 0)
            {
                if (errno == ENOMEM)
                {
                    libexplain_buffer_enomem_kernel(sb);
                    return;
                }
                libexplain_buffer_enomem_kernel_or_user(sb);
                return;
            }
            close(fd);
            libexplain_buffer_enomem_user(sb);
        }
        break;

    default:
        libexplain_buffer_errno_open_explanation
        (
            sb,
            errnum,
            pathname,
            O_RDONLY + O_DIRECTORY,
            0
        );
        break;
    }
}


void
libexplain_buffer_errno_opendir(libexplain_string_buffer_t *sb, int errnum,
    const char *pathname)
{
    libexplain_explanation_t exp;

    libexplain_explanation_init(&exp, errnum);
    libexplain_buffer_errno_opendir_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname
    );
    libexplain_buffer_errno_opendir_explanation
    (
        &exp.explanation_sb,
        errnum,
        pathname
    );
    libexplain_explanation_assemble(&exp, sb);
}
