/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/buffer/fd_set.h>
#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>


void
libexplain_buffer_fd_set(libexplain_string_buffer_t *sb, int nfds,
    const fd_set *fds)
{
    int             fildes;
    int             first;

    if (libexplain_pointer_is_efault(fds))
    {
        libexplain_buffer_pointer(sb, fds);
        return;
    }
    first = 1;
    for (fildes = 0; fildes < nfds; ++fildes)
    {
        if (FD_ISSET(fildes, fds))
        {
            libexplain_string_buffer_putc(sb, (first ? '{' : ','));
            libexplain_string_buffer_printf(sb, " %d", fildes);
            libexplain_buffer_fildes_to_pathname(sb, fildes);
            first = 0;
        }
    }
    libexplain_string_buffer_puts(sb, (first ? "{}" : " }"));
}
