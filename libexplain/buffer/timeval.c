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

#include <libexplain/ac/time.h>

#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/timeval.h>
#include <libexplain/path_is_efault.h>


void
libexplain_buffer_timeval(libexplain_string_buffer_t *sb,
    const struct timeval *tvp)
{
    if (libexplain_pointer_is_efault(tvp))
    {
        libexplain_buffer_pointer(sb, tvp);
        return;
    }
    if (tvp->tv_usec < 0)
    {
        libexplain_string_buffer_printf
        (
            sb,
            "{ tv_sec = %ld, tv_usec = %ld }",
            tvp->tv_sec,
            tvp->tv_usec
        );
    }
    else
    {
        libexplain_string_buffer_printf
        (
            sb,
            "{ %.8g seconds }",
            tvp->tv_sec + 1e-6 * tvp->tv_usec
        );
    }
}
