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

#include <libexplain/ac/net/if.h>

#include <libexplain/buffer/ifreq_flags.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/parse_bits.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/sizeof.h>

static void
libexplain_buffer_ifflags(libexplain_string_buffer_t *sb, int flags)
{
    static const libexplain_parse_bits_table_t table[] =
    {
        { "IFF_UP", IFF_UP },
        { "IFF_BROADCAST", IFF_BROADCAST },
        { "IFF_DEBUG", IFF_DEBUG },
        { "IFF_LOOPBACK", IFF_LOOPBACK },
        { "IFF_POINTOPOINT", IFF_POINTOPOINT },
        { "IFF_NOTRAILERS", IFF_NOTRAILERS },
        { "IFF_RUNNING", IFF_RUNNING },
        { "IFF_NOARP", IFF_NOARP },
        { "IFF_PROMISC", IFF_PROMISC },
        { "IFF_ALLMULTI", IFF_ALLMULTI },
        { "IFF_MASTER", IFF_MASTER },
        { "IFF_SLAVE", IFF_SLAVE },
        { "IFF_MULTICAST", IFF_MULTICAST },
        { "IFF_PORTSEL", IFF_PORTSEL },
        { "IFF_AUTOMEDIA", IFF_AUTOMEDIA },
        { "IFF_DYNAMIC", IFF_DYNAMIC },
    };

    libexplain_parse_bits_print(sb, flags, table, SIZEOF(table));
}


void
libexplain_buffer_ifreq_flags(libexplain_string_buffer_t *sb,
    const struct ifreq *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
        libexplain_buffer_pointer(sb, data);
    else
    {
        const struct ifreq *ifr;

        /*
         * This is actually a huge big sucky union.  This specific
         * case is given the interface name and the interface flags.
         */
        ifr = data;
        libexplain_string_buffer_puts(sb, "{ ifr_name = ");
        libexplain_string_buffer_puts_quoted_n
        (
            sb,
            ifr->ifr_name,
            sizeof(ifr->ifr_name)
        );
        libexplain_string_buffer_puts(sb, ", ifr_flags = ");
        libexplain_buffer_ifflags(sb, ifr->ifr_flags);
        libexplain_string_buffer_puts(sb, " }");
    }
}
