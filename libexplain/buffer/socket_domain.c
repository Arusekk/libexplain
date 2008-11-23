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
#include <libexplain/buffer/socket_domain.h>
#include <libexplain/sizeof.h>


static const libexplain_parse_bits_table_t table[] =
{
    { "PF_UNSPEC", PF_UNSPEC },
    { "PF_LOCAL", PF_LOCAL },
    { "PF_UNIX", PF_UNIX },
    { "PF_FILE", PF_FILE },
    { "PF_INET", PF_INET },
    { "PF_AX25", PF_AX25 },
    { "PF_IPX", PF_IPX },
    { "PF_APPLETALK", PF_APPLETALK },
    { "PF_NETROM", PF_NETROM },
    { "PF_BRIDGE", PF_BRIDGE },
    { "PF_ATMPVC", PF_ATMPVC },
    { "PF_X25", PF_X25 },
    { "PF_INET6", PF_INET6 },
    { "PF_ROSE", PF_ROSE },
    { "PF_DECnet", PF_DECnet },
    { "PF_NETBEUI", PF_NETBEUI },
    { "PF_SECURITY", PF_SECURITY },
    { "PF_KEY", PF_KEY },
    { "PF_NETLINK", PF_NETLINK },
    { "PF_ROUTE", PF_ROUTE },
    { "PF_PACKET", PF_PACKET },
    { "PF_ASH", PF_ASH },
    { "PF_ECONET", PF_ECONET },
    { "PF_ATMSVC", PF_ATMSVC },
    { "PF_SNA", PF_SNA },
    { "PF_IRDA", PF_IRDA },
    { "PF_PPPOX", PF_PPPOX },
    { "PF_WANPIPE", PF_WANPIPE },
    { "PF_BLUETOOTH", PF_BLUETOOTH },
    { "PF_IUCV", PF_IUCV },
    { "PF_RXRPC", PF_RXRPC },

    { "AF_UNSPEC", AF_UNSPEC },
    { "AF_LOCAL", AF_LOCAL },
    { "AF_UNIX", AF_UNIX },
    { "AF_FILE", AF_FILE },
    { "AF_INET", AF_INET },
    { "AF_AX25", AF_AX25 },
    { "AF_IPX", AF_IPX },
    { "AF_APPLETALK", AF_APPLETALK },
    { "AF_NETROM", AF_NETROM },
    { "AF_BRIDGE", AF_BRIDGE },
    { "AF_ATMPVC", AF_ATMPVC },
    { "AF_X25", AF_X25 },
    { "AF_INET6", AF_INET6 },
    { "AF_ROSE", AF_ROSE },
    { "AF_DECnet", AF_DECnet },
    { "AF_NETBEUI", AF_NETBEUI },
    { "AF_SECURITY", AF_SECURITY },
    { "AF_KEY", AF_KEY },
    { "AF_NETLINK", AF_NETLINK },
    { "AF_ROUTE", AF_ROUTE },
    { "AF_PACKET", AF_PACKET },
    { "AF_ASH", AF_ASH },
    { "AF_ECONET", AF_ECONET },
    { "AF_ATMSVC", AF_ATMSVC },
    { "AF_SNA", AF_SNA },
    { "AF_IRDA", AF_IRDA },
    { "AF_PPPOX", AF_PPPOX },
    { "AF_WANPIPE", AF_WANPIPE },
    { "AF_BLUETOOTH", AF_BLUETOOTH },
    { "AF_IUCV", AF_IUCV },
    { "AF_RXRPC", AF_RXRPC },
};


void
libexplain_buffer_socket_domain(libexplain_string_buffer_t *sb, int domain)
{
    const libexplain_parse_bits_table_t *tp;

    tp = libexplain_parse_bits_find_by_value(domain, table, SIZEOF(table));
    if (tp)
        libexplain_string_buffer_puts(sb, tp->name);
    else
        libexplain_string_buffer_printf(sb, "%d", domain);
}


int
libexplain_parse_socket_domain(const char *text)
{
    return libexplain_parse_bits(text, table, SIZEOF(table));
}
