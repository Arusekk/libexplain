/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/fildes_to_pathname.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_fildes_to_pathname(libexplain_string_buffer_t *sb, int fildes)
{
    char procpath[100];
    char symlink_data[2000];
    int n;

    snprintf(procpath, sizeof(procpath), "/proc/self/fd/%d", fildes);
    n = readlink(procpath, symlink_data, sizeof(symlink_data) - 1);
    if (n > 0)
    {
        symlink_data[n] = 0;
        libexplain_string_buffer_puts(sb, " /* ");
        libexplain_string_buffer_puts_quoted(sb, symlink_data);
        libexplain_string_buffer_puts(sb, " */");
    }
}
