/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/ac/linux/ipv6.h>

#include <libexplain/buffer/in6_ifreq.h>
#include <libexplain/buffer/in6_addr.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>


void
libexplain_buffer_in6_ifreq(libexplain_string_buffer_t *sb,
    const struct in6_ifreq *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
        libexplain_buffer_pointer(sb, data);
    else
    {
        libexplain_string_buffer_puts(sb, "{ ifr6_addr = ");
        libexplain_buffer_in6_addr(sb, &data->ifr6_addr);
        libexplain_string_buffer_printf
        (
            sb,
            ", ifr6_prefixlen = %lu, ifr6_ifindex = %d }",
            (unsigned long)data->ifr6_prefixlen,
            data->ifr6_ifindex
        );
    }
}
