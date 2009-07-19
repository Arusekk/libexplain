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

#include <libexplain/buffer/ifreq_mii.h>
#include <libexplain/buffer/mii_ioctl_data.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>


void
explain_buffer_ifreq_mii(explain_string_buffer_t *sb,
    const struct ifreq *data)
{
    if (explain_pointer_is_efault(data, sizeof(*data)))
        explain_buffer_pointer(sb, data);
    else
    {
        const struct ifreq *ifr;

        /*
         * This is actually a huge big sucky union.  This specific case
         * is given the interface name and mii data.
         */
        ifr = data;
        explain_string_buffer_puts(sb, "{ ifr_name = ");
        explain_string_buffer_puts_quoted_n
        (
            sb,
            ifr->ifr_name,
            sizeof(ifr->ifr_name)
        );
        explain_string_buffer_puts(sb, ", mii = ");
        explain_buffer_mii_ioctl_data
        (
            sb,
            /*
             * This cast is disgusting, but that's how it's done within
             * the kernel itself.
             */
            (const struct mii_ioctl_data *)&ifr->ifr_ifru
        );
        explain_string_buffer_puts(sb, " }");
    }
}