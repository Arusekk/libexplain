/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/socket.h>

#include <libexplain/parse_bits.h>
#include <libexplain/buffer/socket_type.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>


static const libexplain_parse_bits_table_t table[] =
{
    { "SOCK_STREAM", SOCK_STREAM },
    { "SOCK_DGRAM", SOCK_DGRAM },
    { "SOCK_RAW", SOCK_RAW },
    { "SOCK_RDM", SOCK_RDM },
    { "SOCK_SEQPACKET", SOCK_SEQPACKET },
    { "SOCK_PACKET", SOCK_PACKET },
};


void
libexplain_buffer_socket_type(libexplain_string_buffer_t *sb, int type)
{
    const libexplain_parse_bits_table_t *tp;

    tp = libexplain_parse_bits_find_by_value(type, table, SIZEOF(table));
    if (tp)
        libexplain_string_buffer_puts(sb, tp->name);
    else
        libexplain_string_buffer_printf(sb, "%d", type);
}


int
libexplain_parse_socket_type(const char *text)
{
    return libexplain_parse_bits(text, table, SIZEOF(table));
}
