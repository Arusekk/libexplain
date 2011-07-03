/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010, 2011 Peter Miller
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

#include <libexplain/buffer/ifreq_data/string.h>
#include <libexplain/buffer/pathname.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/is_efault.h>


void
explain_buffer_ifreq_data_string(explain_string_buffer_t *sb,
    const struct ifreq *data, int follow)
{
    if (explain_is_efault_pointer(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
    }
    else
    {
        explain_string_buffer_puts(sb, "{ ifr_data = ");
        if (follow)
            explain_buffer_pathname(sb, data->ifr_data);
        else
            explain_buffer_pointer(sb, data->ifr_data);
        explain_string_buffer_puts(sb, " }");
    }
}
