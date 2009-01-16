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

#include <libexplain/buffer/ifreq_ifmap.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>


static void
libexplain_buffer_ifmap(libexplain_string_buffer_t *sb,
    const struct ifmap *data)
{
    libexplain_string_buffer_printf
    (
        sb,
        "{ mem_start = %#lx, "
            "mem_end = %#lx, "
            "base_addr = %#x, "
            "irq = %u, "
            "dma = %u, "
            "port = %u }",
        data->mem_start,
        data->mem_end,
        data->base_addr,
        data->irq,
        data->dma,
        data->port
    );
}


void
libexplain_buffer_ifreq_ifmap(libexplain_string_buffer_t *sb,
    const struct ifreq *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
        libexplain_buffer_pointer(sb, data);
    else
    {
        const struct ifreq *ifr;

        /*
         * This is actually a huge big sucky union.  This specific case
         * is given the interface name and the ifr_map member.
         */
        ifr = data;
        libexplain_string_buffer_puts(sb, "{ ifr_name = ");
        libexplain_string_buffer_puts_quoted_n
        (
            sb,
            ifr->ifr_name,
            sizeof(ifr->ifr_name)
        );
        libexplain_string_buffer_puts(sb, ", ifr_map = ");
        libexplain_buffer_ifmap(sb, &ifr->ifr_map);
        libexplain_string_buffer_puts(sb, " }");
    }
}
