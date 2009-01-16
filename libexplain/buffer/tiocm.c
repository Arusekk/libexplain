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

#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/tiocm.h>
#include <libexplain/parse_bits.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/sizeof.h>


static const libexplain_parse_bits_table_t table[] =
{
    { "TIOCM_LE", TIOCM_LE },
    { "TIOCM_DTR", TIOCM_DTR },
    { "TIOCM_RTS", TIOCM_RTS },
    { "TIOCM_ST", TIOCM_ST },
    { "TIOCM_SR", TIOCM_SR },
    { "TIOCM_CTS", TIOCM_CTS },
    { "TIOCM_CAR", TIOCM_CAR },
    { "TIOCM_CD", TIOCM_CD },
    { "TIOCM_RNG", TIOCM_RNG },
    { "TIOCM_RI", TIOCM_RI },
    { "TIOCM_DSR", TIOCM_DSR },
};


void
libexplain_buffer_tiocm(libexplain_string_buffer_t *sb, int value)
{
    libexplain_parse_bits_print(sb, value, table, SIZEOF(table));
}


void
libexplain_buffer_tiocm_star(libexplain_string_buffer_t *sb, const int *value)
{
    if (libexplain_pointer_is_efault(value, sizeof(*value)))
        libexplain_buffer_pointer(sb, value);
    else
    {
        libexplain_string_buffer_puts(sb, "{ ");
        libexplain_buffer_tiocm(sb, *value);
        libexplain_string_buffer_puts(sb, " }");
    }
}
