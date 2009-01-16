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

#include <libexplain/buffer/char_data.h>
#include <libexplain/buffer/hexdump.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/option.h>
#include <libexplain/path_is_efault.h>


#define A_FEW_BYTES 8


void
libexplain_buffer_char_data(libexplain_string_buffer_t *sb, const void *data,
    size_t data_size)
{
    if (libexplain_pointer_is_efault(data, data_size))
        libexplain_buffer_pointer(sb, data);
    else
    {
        libexplain_string_buffer_putc(sb, '{');
        if (data_size <= A_FEW_BYTES)
            libexplain_buffer_hexdump(sb, data, data_size);
        else if (libexplain_option_debug())
            libexplain_buffer_hexdump(sb, data, data_size);
        else
        {
            libexplain_buffer_hexdump(sb, data, A_FEW_BYTES);
            libexplain_string_buffer_puts(sb, ", ...");
        }
        libexplain_string_buffer_puts(sb, " }");
    }
}
