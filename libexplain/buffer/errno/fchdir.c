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
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/because.h>
#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/errno/fchdir.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/file_type.h>
#include <libexplain/buffer/success.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_errno_fchdir(libexplain_string_buffer_t *sb, int errnum,
    int fildes)
{
    libexplain_string_buffer_printf(sb, "fchdir(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_putc(sb, ')');
    if (errnum == 0)
    {
        libexplain_buffer_success(sb);
        return;
    }
    libexplain_buffer_failed(sb, errnum);

    switch (errnum)
    {
    case EBADF:
        libexplain_buffer_ebadf(sb, "fildes");
        break;

    case ENOTDIR:
        {
            struct stat     st;

            if (fstat(fildes, &st) >= 0)
            {
                libexplain_buffer_because(sb);
                libexplain_string_buffer_puts(sb, "fildes refers to a ");
                libexplain_buffer_file_type(sb, st.st_mode);
                libexplain_string_buffer_puts(sb, ", not a directory");
            }
        }
        break;

    case EACCES:
        libexplain_buffer_because(sb);
        libexplain_string_buffer_puts
        (
            sb,
            "search permission was denied on the directory"
        );
        break;

    default:
        /* no additional info for other errno values */
        break;
    }
}
