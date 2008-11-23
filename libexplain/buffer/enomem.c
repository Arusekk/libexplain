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

#include <libexplain/ac/sys/resource.h>

#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/rlimit.h>


void
libexplain_buffer_enomem_kernel(libexplain_string_buffer_t *sb)
{
    libexplain_string_buffer_puts
    (
        sb,
        "insufficient kernel memory was available"
    );
}


void
libexplain_buffer_enomem_user(libexplain_string_buffer_t *sb)
{
    struct rlimit   rlim;

    libexplain_string_buffer_puts
    (
        sb,
        "insufficient user-space memory was available"
    );

    /*
     * Try to provide more information about memory limits.
     */
    if (getrlimit(RLIMIT_DATA, &rlim) >= 0)
    {
        if (rlim.rlim_cur == RLIM_INFINITY)
        {
            libexplain_string_buffer_puts
            (
                sb,
                ", probably by exhausting swap space"
            );
        }
        else
        {
            libexplain_buffer_rlimit(sb, &rlim);
        }
    }
}


void
libexplain_buffer_enomem_kernel_or_user(libexplain_string_buffer_t *sb)
{
    libexplain_string_buffer_puts
    (
        sb,
        "insufficient user-space or kernel memory was available"
    );
}
