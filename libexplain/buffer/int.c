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

#include <libexplain/buffer/int.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/path_is_efault.h>


void
libexplain_buffer_int(libexplain_string_buffer_t *sb, int data)
{
    libexplain_string_buffer_printf(sb, "%d", data);
}


void
libexplain_buffer_int_star(libexplain_string_buffer_t *sb, const int *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
        libexplain_buffer_pointer(sb, data);
    else
    {
        libexplain_string_buffer_puts(sb, "{ ");
        libexplain_buffer_int(sb, *data);
        libexplain_string_buffer_puts(sb, " }");
    }
}
