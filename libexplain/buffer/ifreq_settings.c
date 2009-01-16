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

#include <libexplain/ac/sys/socket.h>
#include <libexplain/ac/linux/if.h>

#include <libexplain/buffer/ifreq_settings.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>


static void
libexplain_buffer_if_settings(libexplain_string_buffer_t *sb,
    const struct if_settings *data)
{
    libexplain_string_buffer_printf
    (
        sb,
        "{ type = %u, size = %u }",
        data->type,
        data->size
    );
}


void
libexplain_buffer_ifreq_settings(libexplain_string_buffer_t *sb,
    const struct ifreq *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
        libexplain_buffer_pointer(sb, data);
    else
    {
        const struct ifreq *ifr;

        /*
         * This is actually a huge big sucky union.  This specific case
         * is given the interface name and the settings.
         */
        ifr = data;
        libexplain_string_buffer_puts(sb, "{ ifr_name = ");
        libexplain_string_buffer_puts_quoted_n
        (
            sb,
            ifr->ifr_name,
            sizeof(ifr->ifr_name)
        );
        libexplain_string_buffer_puts(sb, ", ifr_settings = ");
        libexplain_buffer_if_settings(sb, &ifr->ifr_settings);
        libexplain_string_buffer_puts(sb, " }");
    }
}
