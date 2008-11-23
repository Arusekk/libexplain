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
#include <libexplain/ac/netdb.h>
#include <libexplain/ac/netinet/in.h>

#include <libexplain/parse_bits.h>
#include <libexplain/buffer/socket_protocol.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>


static const libexplain_parse_bits_table_t table[] =
{
    { "IPPROTO_IP", IPPROTO_IP },
    { "IPPROTO_ICMP", IPPROTO_ICMP },
    { "IPPROTO_IGMP", IPPROTO_IGMP },
    { "IPPROTO_IPIP", IPPROTO_IPIP },
    { "IPPROTO_TCP", IPPROTO_TCP },
    { "IPPROTO_EGP", IPPROTO_EGP },
    { "IPPROTO_PUP", IPPROTO_PUP },
    { "IPPROTO_UDP", IPPROTO_UDP },
    { "IPPROTO_IDP", IPPROTO_IDP },
    { "IPPROTO_TP", IPPROTO_TP },
    { "IPPROTO_IPV6", IPPROTO_IPV6 },
    { "IPPROTO_ROUTING", IPPROTO_ROUTING },
    { "IPPROTO_FRAGMENT", IPPROTO_FRAGMENT },
    { "IPPROTO_RSVP", IPPROTO_RSVP },
    { "IPPROTO_GRE", IPPROTO_GRE },
    { "IPPROTO_ESP", IPPROTO_ESP },
    { "IPPROTO_AH", IPPROTO_AH },
    { "IPPROTO_ICMPV6", IPPROTO_ICMPV6 },
    { "IPPROTO_NONE", IPPROTO_NONE },
    { "IPPROTO_MTP", IPPROTO_MTP },
    { "IPPROTO_ENCAP", IPPROTO_ENCAP },
    { "IPPROTO_PIM", IPPROTO_PIM },
    { "IPPROTO_COMP", IPPROTO_COMP },
    { "IPPROTO_SCTP", IPPROTO_SCTP },
    { "IPPROTO_RAW", IPPROTO_RAW },
};


void
libexplain_buffer_socket_protocol(libexplain_string_buffer_t *sb, int protocol)
{
    const libexplain_parse_bits_table_t *tp;
    struct protoent *pep;

    tp = libexplain_parse_bits_find_by_value(protocol, table, SIZEOF(table));
    if (tp)
    {
        libexplain_string_buffer_puts(sb, tp->name);
        return;
    }

    libexplain_string_buffer_printf(sb, "%d", protocol);

    pep = getprotobynumber(protocol);
    if (pep)
    {
        libexplain_string_buffer_putc(sb, ' ');
        libexplain_string_buffer_puts_quoted(sb, pep->p_name);
    }
}


int
libexplain_parse_socket_protocol(const char *text)
{
    struct protoent *pep;
    int             n;

    n = libexplain_parse_bits(text, table, SIZEOF(table));
    if (n >= 0)
        return n;
    pep = getprotobyname(text);
    if (pep)
        return pep->p_proto;
    return -1;
}
