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

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/errno/fstat.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/failed.h>
#include <libexplain/buffer/success.h>


void
libexplain_buffer_errno_fstat(libexplain_string_buffer_t *sb, int errnum,
    int fildes, const struct stat *buf)
{
    libexplain_string_buffer_printf(sb, "fstat(fildes = %d", fildes);
    libexplain_buffer_fildes_to_pathname(sb, fildes);
    libexplain_string_buffer_printf(sb, ", buf = %p)", buf);
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

    case ENOMEM:
        libexplain_buffer_enomem_kernel(sb);
        break;

    default:
        libexplain_buffer_errno_generic(sb, errnum);
        break;
    }
}
