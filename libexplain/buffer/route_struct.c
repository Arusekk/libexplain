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

#include <libexplain/ac/arpa/inet.h>
#include <libexplain/ac/linux/x25.h>
#include <libexplain/ac/net/route.h>
#include <libexplain/ac/netax25/ax25.h>
#include <libexplain/ac/netrom/netrom.h>
#include <libexplain/ac/netrose/rose.h>

#include <libexplain/buffer/hexdump.h>
#include <libexplain/buffer/pathname.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/route_struct.h>
#include <libexplain/buffer/sockaddr.h>
#include <libexplain/fildes_to_address_family.h>
#include <libexplain/parse_bits.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/sizeof.h>


static void
libexplain_buffer_rtentry_flags(libexplain_string_buffer_t *sb, int flags)
{
    static const libexplain_parse_bits_table_t table[] =
    {
        { "RTF_UP", RTF_UP },
        { "RTF_GATEWAY", RTF_GATEWAY },
        { "RTF_HOST", RTF_HOST },
        { "RTF_REINSTATE", RTF_REINSTATE },
        { "RTF_DYNAMIC", RTF_DYNAMIC },
        { "RTF_MODIFIED", RTF_MODIFIED },
        { "RTF_MTU", RTF_MTU },
        { "RTF_WINDOW", RTF_WINDOW },
        { "RTF_IRTT", RTF_IRTT },
        { "RTF_REJECT", RTF_REJECT },
    };

    libexplain_parse_bits_print(sb, flags, table, SIZEOF(table));
}


static void
libexplain_buffer_rtentry(libexplain_string_buffer_t *sb,
    const struct rtentry *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
    {
        libexplain_buffer_pointer(sb, data);
        return;
    }

    libexplain_string_buffer_puts(sb, "{ rt_dst = ");
    libexplain_buffer_sockaddr(sb, &data->rt_dst, sizeof(data->rt_dst));
    libexplain_string_buffer_puts(sb, ", rt_gateway = ");
    libexplain_buffer_sockaddr(sb, &data->rt_gateway, sizeof(data->rt_gateway));
    libexplain_string_buffer_puts(sb, ", rt_genmask = ");
    libexplain_buffer_sockaddr(sb, &data->rt_genmask, sizeof(data->rt_genmask));
    libexplain_string_buffer_puts(sb, ", rt_flags = ");
    libexplain_buffer_rtentry_flags(sb, data->rt_flags);
    libexplain_string_buffer_printf(sb, ", rt_metric = %d, ", data->rt_metric);
    libexplain_string_buffer_puts(sb, "rt_dev = ");
    libexplain_buffer_pathname(sb, data->rt_dev);
    libexplain_string_buffer_printf(sb, ", rt_mtu = %ld, ", data->rt_mtu);
    libexplain_string_buffer_printf(sb, ", rt_mtu = %lu, ", data->rt_window);
    libexplain_string_buffer_printf(sb, ", rt_mtu = %u }", data->rt_irtt);
}


#ifdef HAVE_NETAX25_AX25_H

static void
libexplain_buffer_ax25_address(libexplain_string_buffer_t *sb,
    const ax25_address *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
    {
        libexplain_buffer_pointer(sb, data);
        return;
    }

    libexplain_string_buffer_puts(sb, "{ ax25_call =");
    libexplain_buffer_hexdump(sb, data->ax25_call, sizeof(data->ax25_call));
    libexplain_string_buffer_puts(sb, " }");

}


static void
libexplain_buffer_ax25_routes_struct(libexplain_string_buffer_t *sb,
    const struct ax25_routes_struct *data)
{
    unsigned        j;

    if (libexplain_pointer_is_efault(data, sizeof(*data)))
    {
        libexplain_buffer_pointer(sb, data);
        return;
    }

    libexplain_string_buffer_puts(sb, "{ port_addr = ");
    libexplain_buffer_ax25_address(sb, &data->port_addr);
    libexplain_string_buffer_puts(sb, ", dest_addr = ");
    libexplain_buffer_ax25_address(sb, &data->dest_addr);
    libexplain_string_buffer_printf
    (
        sb,
        ", digi_count = %u, digi_addr = {",
        data->digi_count
    );
    for (j = 0; j < data->digi_count && j < AX25_MAX_DIGIS; ++j)
    {
        libexplain_string_buffer_puts(sb, ", ");
        libexplain_buffer_ax25_address(sb, &data->digi_addr[j]);
    }
    libexplain_string_buffer_puts(sb, " } }");
}

#endif

#ifdef HAVE_NETROM_NETROM_H

static void
libexplain_buffer_netrom_route_type(libexplain_string_buffer_t *sb, int data)
{
    static const libexplain_parse_bits_table_t table[] =
    {
        { "NETROM_NEIGH", NETROM_NEIGH },
        { "NETROM_NODE", NETROM_NODE },
    };

    libexplain_parse_bits_print_single(sb, data, table, SIZEOF(table));
}

static void
libexplain_buffer_nr_route_struct(libexplain_string_buffer_t *sb,
    const struct nr_route_struct *data)
{
    unsigned        j;

    if (libexplain_pointer_is_efault(data, sizeof(*data)))
    {
        libexplain_buffer_pointer(sb, data);
        return;
    }

    libexplain_string_buffer_puts(sb, "{ type = ");
    libexplain_buffer_netrom_route_type(sb, data->type);
    libexplain_string_buffer_puts(sb, ", callsign = ");
    libexplain_buffer_ax25_address(sb, &data->callsign);
    libexplain_string_buffer_puts(sb, ", device = ");
    libexplain_string_buffer_puts_quoted(sb, data->device);
    libexplain_string_buffer_printf(sb, ", quality = %u, ", data->quality);
    libexplain_string_buffer_puts(sb, "mnemonic = ");
    libexplain_string_buffer_puts_quoted_n
    (
        sb,
        data->mnemonic,
        sizeof(data->mnemonic)
    );
    libexplain_string_buffer_puts(sb, ", neighbour = ");
    libexplain_buffer_ax25_address(sb, &data->neighbour);
    libexplain_string_buffer_printf(sb, ", obs_count = %u, ", data->obs_count);
    libexplain_string_buffer_printf(sb, "ndigis = %u, ", data->ndigis);
    libexplain_string_buffer_puts(sb, ", digipeaters = {");
    for (j = 0; j < data->ndigis && j < AX25_MAX_DIGIS; ++j)
    {
        if (j)
            libexplain_string_buffer_putc(sb, ',');
        libexplain_string_buffer_putc(sb, ' ');
        libexplain_buffer_ax25_address(sb, &data->digipeaters[j]);
    }
    libexplain_string_buffer_puts(sb, " } }");
}

#endif


static void
libexplain_buffer_in6_addr(libexplain_string_buffer_t *sb,
    const struct in6_addr *data)
{
    char            straddr[200];

    inet_ntop(AF_INET6, data, straddr, sizeof(straddr));
    libexplain_string_buffer_puts(sb, straddr);
}


static void
libexplain_buffer_rtmsg_type(libexplain_string_buffer_t *sb,
    unsigned long data)
{
    static const libexplain_parse_bits_table_t table[] =
    {
#if 0
        /*
         * These are defined, but in terms of symbols are are NOT
         * defined.  Sheesh.
         */
        { "RTMSG_ACK", RTMSG_ACK },
        { "RTMSG_OVERRUN", RTMSG_OVERRUN },
#endif
        { "RTMSG_NEWDEVICE", RTMSG_NEWDEVICE },
        { "RTMSG_DELDEVICE", RTMSG_DELDEVICE },
        { "RTMSG_NEWROUTE", RTMSG_NEWROUTE },
        { "RTMSG_DELROUTE", RTMSG_DELROUTE },
        { "RTMSG_NEWRULE", RTMSG_NEWRULE },
        { "RTMSG_DELRULE", RTMSG_DELRULE },
        { "RTMSG_CONTROL", RTMSG_CONTROL },
        { "RTMSG_AR_FAILED", RTMSG_AR_FAILED },
    };

    libexplain_parse_bits_print(sb, data, table, SIZEOF(table));
}


static void
libexplain_buffer_rtmsg_flags(libexplain_string_buffer_t *sb,
    unsigned long data)
{
    static const libexplain_parse_bits_table_t table[] =
    {
        { "RTF_DEFAULT", RTF_DEFAULT },
        { "RTF_ALLONLINK", RTF_ALLONLINK },
        { "RTF_ADDRCONF", RTF_ADDRCONF },
        { "RTF_LINKRT", RTF_LINKRT },
        { "RTF_NONEXTHOP", RTF_NONEXTHOP },
        { "RTF_CACHE", RTF_CACHE },
        { "RTF_FLOW", RTF_FLOW },
        { "RTF_POLICY", RTF_POLICY },
        { "RTCF_VALVE", RTCF_VALVE },
        { "RTCF_MASQ", RTCF_MASQ },
        { "RTCF_NAT", RTCF_NAT },
        { "RTCF_DOREDIRECT", RTCF_DOREDIRECT },
        { "RTCF_LOG", RTCF_LOG },
        { "RTCF_DIRECTSRC", RTCF_DIRECTSRC },
        { "RTF_LOCAL", RTF_LOCAL },
        { "RTF_INTERFACE", RTF_INTERFACE },
        { "RTF_MULTICAST", RTF_MULTICAST },
        { "RTF_BROADCAST", RTF_BROADCAST },
        { "RTF_NAT", RTF_NAT },
    };

    libexplain_parse_bits_print(sb, data, table, SIZEOF(table));
}


static void
libexplain_buffer_in6_rtmsg(libexplain_string_buffer_t *sb,
    const struct in6_rtmsg *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
    {
        libexplain_buffer_pointer(sb, data);
        return;
    }

    libexplain_string_buffer_puts(sb, "{ rtmsg_dst = ");
    libexplain_buffer_in6_addr(sb, &data->rtmsg_dst);
    libexplain_string_buffer_puts(sb, ", rtmsg_src = ");
    libexplain_buffer_in6_addr(sb, &data->rtmsg_src);
    libexplain_string_buffer_puts(sb, ", rtmsg_gateway = ");
    libexplain_buffer_in6_addr(sb, &data->rtmsg_gateway);
    libexplain_string_buffer_puts(sb, ", rtmsg_type = ");
    libexplain_buffer_rtmsg_type(sb, data->rtmsg_type);
    libexplain_string_buffer_printf
    (
        sb,
        ", rtmsg_dst_len = %u, ",
        data->rtmsg_dst_len
    );
    libexplain_string_buffer_printf
    (
        sb,
        "rtmsg_src_len = %u, ",
        data->rtmsg_src_len
    );
    libexplain_string_buffer_printf
    (
        sb,
        "rtmsg_metric = %lu, ",
        (unsigned long)data->rtmsg_metric
    );
    libexplain_string_buffer_printf
    (
        sb,
        "rtmsg_info = %lu, ",
        data->rtmsg_info
    );
    libexplain_string_buffer_puts(sb, "rtmsg_flags = ");
    libexplain_buffer_rtmsg_flags(sb, data->rtmsg_flags);
    libexplain_string_buffer_printf
    (
        sb,
        ", rtmsg_ifindex = %d }",
        data->rtmsg_ifindex
    );
}

#ifdef HAVE_NETROSE_ROSE_H

static void
libexplain_buffer_rose_address(libexplain_string_buffer_t *sb,
    const rose_address *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
    {
        libexplain_buffer_pointer(sb, data);
        return;
    }

    libexplain_string_buffer_puts(sb, "{ rose_addr = ");
    libexplain_string_buffer_puts_quoted_n
    (
        sb,
        data->rose_addr,
        sizeof(data->rose_addr)
    );
    libexplain_string_buffer_puts(sb, " }");
}


static void
libexplain_buffer_rose_route_struct(libexplain_string_buffer_t *sb,
    const struct rose_route_struct *data)
{
    unsigned        j;

    if (libexplain_pointer_is_efault(data, sizeof(*data)))
    {
        libexplain_buffer_pointer(sb, data);
        return;
    }

    libexplain_string_buffer_puts(sb, "{ address = ");
    libexplain_buffer_rose_address(sb, &data->address);
    libexplain_string_buffer_printf(sb, ", mask = %u, ", data->mask);
    libexplain_string_buffer_puts(sb, "neighbour = ");
    libexplain_buffer_ax25_address(sb, &data->neighbour);
    libexplain_string_buffer_puts(sb, ", device = ");
    libexplain_string_buffer_puts_quoted_n
    (
        sb,
        data->device,
        sizeof(data->device)
    );
    libexplain_string_buffer_printf(sb, ", ndigis = %u, ", data->ndigis);
    libexplain_string_buffer_puts(sb, "digipeaters = {");
    for (j = 0; j < data->ndigis && j < AX25_MAX_DIGIS; ++j)
    {
        if (j)
            libexplain_string_buffer_putc(sb, ',');
        libexplain_string_buffer_putc(sb, ' ');
        libexplain_buffer_ax25_address(sb, &data->digipeaters[j]);
    }
    libexplain_string_buffer_puts(sb, " } }");
}

#endif
#ifdef HAVE_LINUX_X25_H

static void
libexplain_buffer_x25_address(libexplain_string_buffer_t *sb,
    const struct x25_address *data)
{
    libexplain_string_buffer_puts(sb, "{ x25_addr = ");
    libexplain_string_buffer_puts_quoted_n
    (
        sb,
        data->x25_addr,
        sizeof(data->x25_addr)
    );
    libexplain_string_buffer_puts(sb, " }");
}

static void
libexplain_buffer_x25_route_struct(libexplain_string_buffer_t *sb,
    const struct x25_route_struct *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
    {
        libexplain_buffer_pointer(sb, data);
        return;
    }

    libexplain_string_buffer_puts(sb, "{ address = ");
    libexplain_buffer_x25_address(sb, &data->address);
    libexplain_string_buffer_printf(sb, ", sigdigits = %u, ", data->sigdigits);
    libexplain_string_buffer_puts(sb, "device = ");
    libexplain_string_buffer_puts_quoted_n
    (
        sb,
        data->device,
        sizeof(data->device)
    );
    libexplain_string_buffer_puts(sb, " }");
}

#endif


void
libexplain_buffer_route_struct(libexplain_string_buffer_t *sb, int fildes,
    const void *data)
{
    if (!data)
    {
        print_pointer:
        libexplain_buffer_pointer(sb, data);
        return;
    }
    switch (libexplain_fildes_to_address_family(fildes))
    {
    case -1:
        goto print_pointer;

    default:
        libexplain_buffer_rtentry(sb, data);
        break;

#ifdef HAVE_NETAX25_AX25_H
    case AF_AX25:
        libexplain_buffer_ax25_routes_struct(sb, data);
        break;
#endif

    case AF_INET6:
        libexplain_buffer_in6_rtmsg(sb, data);
        break;

#ifdef HAVE_NETROM_NETROM_H
    case AF_NETROM:
        libexplain_buffer_nr_route_struct(sb, data);
        break;
#endif

#ifdef HAVE_NETROSE_ROSE_H
    case AF_ROSE:
        libexplain_buffer_rose_route_struct(sb, data);
        break;
#endif

#ifdef HAVE_LINUX_X25_H
    case AF_X25:
        libexplain_buffer_x25_route_struct(sb, data);
        break;
#endif
    }
}
