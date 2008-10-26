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

#include <libexplain/ac/sys/resource.h>

#include <libexplain/buffer/rlimit.h>
#include <libexplain/string_buffer.h>


void
libexplain_buffer_rlimit(libexplain_string_buffer_t *sb, const struct rlimit *p)
{
    libexplain_string_buffer_puts(sb, " (");
    if (p->rlim_cur == RLIM_INFINITY)
        libexplain_string_buffer_puts(sb, "infinity");
    else
        libexplain_string_buffer_printf(sb, "%lld", (long long)p->rlim_cur);
    if (p->rlim_cur != p->rlim_max)
    {
        libexplain_string_buffer_puts(sb, ", max ");
        if (p->rlim_max == RLIM_INFINITY)
            libexplain_string_buffer_puts(sb, "infinity");
        else
            libexplain_string_buffer_printf(sb, "%lld", (long long)p->rlim_max);
    }
    libexplain_string_buffer_putc(sb, ')');
}
