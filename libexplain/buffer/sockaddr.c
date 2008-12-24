/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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
#include <libexplain/ac/linux/atalk.h>
#include <libexplain/ac/linux/atm.h>
#include <libexplain/ac/linux/if_pppox.h>
#include <libexplain/ac/linux/irda.h>
#include <libexplain/ac/linux/netlink.h>
#include <libexplain/ac/linux/x25.h>
#include <libexplain/ac/netash/ash.h>
#include <libexplain/ac/netdb.h>
#include <libexplain/ac/neteconet/ec.h>
#include <libexplain/ac/netinet/in.h>
#include <libexplain/ac/netipx/ipx.h>
#include <libexplain/ac/netiucv/iucv.h>
#include <libexplain/ac/netpacket/packet.h>
#include <libexplain/ac/netrose/rose.h>
#include <libexplain/ac/sys/socket.h>
#include <libexplain/ac/sys/un.h>

#include <libexplain/buffer/hexdump.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/sockaddr.h>
#include <libexplain/buffer/address_family.h>
#include <libexplain/option.h>
#include <libexplain/path_is_efault.h>


/*
 * See unix(7) for more information.
 */
static void
libexplain_buffer_sockaddr_af_unix(libexplain_string_buffer_t *sb,
    const struct sockaddr_un *sa, size_t sa_len)
{
    if (sa_len > sizeof(sa->sun_family))
    {
        libexplain_string_buffer_puts(sb, ", sun_path = ");
        libexplain_string_buffer_puts_quoted(sb, sa->sun_path);
    }
}


/*
 * See ip(7) and inet(3) for more information.
 */
static void
libexplain_buffer_sockaddr_af_inet(libexplain_string_buffer_t *sb,
    const struct sockaddr_in *sa, size_t sa_len)
{
    unsigned short  port;

    /*
     * print the port number, and name if we can
     */
    (void)sa_len;
    port = ntohs(sa->sin_port);
    libexplain_string_buffer_printf(sb, ", sin_port = %hu", port);
    if (libexplain_option_dialect_specific())
    {
        struct servent  *sep;

        /*
         * We make this dialect specific, because different systems will
         * have different entries in their /etc/services file, and this
         * could cause false negatives for automated testing.
         */
        /* FIXME: use getservbyport_r if available */
        sep = getservbyport(port, "tcp");
        if (!sep)
            sep = getservbyport(port, "udp");
        if (sep)
        {
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts_quoted(sb, sep->s_name);
        }
    }

    /*
     * print the IP address, and name if we can
     */
    libexplain_string_buffer_puts(sb, ", sin_addr = ");
    libexplain_string_buffer_puts(sb, inet_ntoa(sa->sin_addr));
    if (libexplain_option_dialect_specific())
    {
        struct hostent  *hep;

        /*
         * We make this dialect specific, because different systems will
         * have different entries in their /etc/hosts file, or there
         * could be transient DNS failures, and these could cause false
         * negatives for automated testing.
         */
        /* FIXME: gethostbyaddr_r if available */
        hep = gethostbyaddr(&sa->sin_addr, sizeof(sa->sin_addr), AF_INET);
        if (hep)
        {
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts_quoted(sb, hep->h_name);
        }
    }
}


#ifdef AF_AX25

static void
libexplain_buffer_sockaddr_af_ax25(libexplain_string_buffer_t *sb,
    const struct sockaddr_ax25 *sa, size_t sa_len)
{
    /* amateur radio stuff */
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_ax25 */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
    /* file /usr/include/netax25/ax25.h */
}

#endif

#ifdef AF_IPX

static void
libexplain_buffer_sockaddr_af_ipx(libexplain_string_buffer_t *sb,
    const struct sockaddr_ipx *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_ipx */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
    /* file /usr/include/netipx/ipx.h */
}

#endif

#ifdef AF_APPLETALK

static void
libexplain_buffer_sockaddr_af_appletalk(libexplain_string_buffer_t *sb,
    const struct sockaddr_at *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_at */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
    /*
     * file /usr/include/linux/atalk.h
     * sat_port
     * sat_addr
     */
}

#endif

#ifdef AF_NETROM

static void
libexplain_buffer_sockaddr_af_netrom(libexplain_string_buffer_t *sb,
    const struct sockaddr *sa, size_t sa_len)
{
    /* amateur radio stuff */
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_netrom */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
    /* file /usr/include/netrom/netrom.h */
}

#endif

#ifdef AF_BRIDGE

static void
libexplain_buffer_sockaddr_af_bridge(libexplain_string_buffer_t *sb,
    const struct sockaddr *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_bridge */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
}

#endif

#ifdef AF_ATMPVC

static void
libexplain_buffer_sockaddr_af_atmpvc(libexplain_string_buffer_t *sb,
    const struct sockaddr_atmpvc *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_atmpvc */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
    /* file /usr/include/linux/atm.h */
}

#endif

#ifdef AF_X25

/*
 * See x25(7) for more information.
 */
static void
libexplain_buffer_sockaddr_af_x25(libexplain_string_buffer_t *sb,
    const struct sockaddr_x25 *sa, size_t sa_len)
{
    (void)sa_len;
    libexplain_string_buffer_puts(sb, ", sx25_addr = ");
    libexplain_string_buffer_puts_quoted(sb, sa->sx25_addr.x25_addr);
}

#endif

#ifdef AF_INET6

/*
 * See man ipv6(7) for more information.
 */
static void
libexplain_buffer_sockaddr_af_inet6(libexplain_string_buffer_t *sb,
    const struct sockaddr_in6 *sa, size_t sa_len)
{
    unsigned short  port;
    char            straddr[200];

    /*
     * print the port number, and name if we can
     */
    (void)sa_len;
    port = ntohs(sa->sin6_port);
    libexplain_string_buffer_printf(sb, ", sin_port = %hu", port);
    if (libexplain_option_dialect_specific())
    {
        struct servent  *sep;

        /*
         * We make this dialect specific, because different systems will
         * have different entries in their /etc/services file, and this
         * could cause false negatives for automated testing.
         */
        /* FIXME: use getservbyport_r if available */
        sep = getservbyport(port, "tcp");
        if (!sep)
            sep = getservbyport(port, "udp");
        if (sep)
        {
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts_quoted(sb, sep->s_name);
        }
    }

    if (sa->sin6_flowinfo != 0)
    {
        libexplain_string_buffer_printf
        (
            sb,
            ", sin6_flowinfo = %ld",
            (long)sa->sin6_flowinfo
        );
    }

    /*
     * print the IP address, and name if we can
     */
    libexplain_string_buffer_puts(sb, ", sin6_addr = ");
    inet_ntop(AF_INET6, &sa->sin6_addr, straddr, sizeof(straddr));
    libexplain_string_buffer_puts(sb, straddr);
    if (libexplain_option_dialect_specific())
    {
        struct hostent  *hep;

        /*
         * We make this dialect specific, because different systems will
         * have different entries in their /etc/hosts file, or there
         * could be transient DNS failures, and these could cause false
         * negatives for automated testing.
         */
        /* FIXME: gethostbyaddr_r if available */
        hep = gethostbyaddr(&sa->sin6_addr, sizeof(sa->sin6_addr), AF_INET6);
        if (hep)
        {
            libexplain_string_buffer_putc(sb, ' ');
            libexplain_string_buffer_puts_quoted(sb, hep->h_name);
        }
    }

    if (sa->sin6_scope_id != 0)
    {
        /*
         * sin6_scope_id is an ID of depending of on the scope of the
         * address.  It is new in Linux 2.4.  Linux only supports it for
         * link scope addresses, in that case sin6_scope_id contains the
         * interface index (see netdevice(7))
         */
        libexplain_string_buffer_printf
        (
            sb,
            ", sin6_scope_id = %ld",
            (long)sa->sin6_scope_id
        );
    }
}

#endif

#ifdef AF_ROSE

static void
libexplain_buffer_sockaddr_af_rose(libexplain_string_buffer_t *sb,
    const struct sockaddr_rose *sa, size_t sa_len)
{
    /* amateur radio stuff */
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_rose */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
    /* file /usr/include/netrose/rose.h */
}

#endif

#ifdef AF_DECnet

static void
libexplain_buffer_sockaddr_af_decnet(libexplain_string_buffer_t *sb,
    const struct sockaddr *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_decnet */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
}

#endif

#ifdef AF_NETBEUI

static void
libexplain_buffer_sockaddr_af_netbeui(libexplain_string_buffer_t *sb,
    const struct sockaddr *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    /* FIXME: decode sockaddr_netbeui */
    libexplain_buffer_hexdump(sb, sa, sa_len);
}

#endif

#ifdef AF_SECURITY

static void
libexplain_buffer_sockaddr_af_security(libexplain_string_buffer_t *sb,
    const struct sockaddr *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_security */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
}

#endif

#ifdef AF_KEY

static void
libexplain_buffer_sockaddr_af_key(libexplain_string_buffer_t *sb,
    const struct sockaddr *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_key */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
}

#endif

#ifdef AF_NETLINK

static void
libexplain_buffer_sockaddr_af_netlink(libexplain_string_buffer_t *sb,
    const struct sockaddr_nl *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_nl */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
    /* file /usr/include/linux/netlink.h */
}

#endif

#ifdef AF_PACKET

static void
libexplain_buffer_sockaddr_af_packet(libexplain_string_buffer_t *sb,
    const struct sockaddr_ll *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_ll */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
}

#endif

#ifdef AF_ASH

static void
libexplain_buffer_sockaddr_af_ash(libexplain_string_buffer_t *sb,
    const struct sockaddr_ash *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_ash */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
    /* file /usr/include/netash/ash.h */
}

#endif

#ifdef AF_ECONET

static void
libexplain_buffer_sockaddr_af_econet(libexplain_string_buffer_t *sb,
    const struct sockaddr_ec *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_ec */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
    /* file /usr/include/neteconet/ec.h */
}

#endif

#ifdef AF_ATMSVC

static void
libexplain_buffer_sockaddr_af_atmsvc(libexplain_string_buffer_t *sb,
    const struct sockaddr_atmsvc *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_atmsvc */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
}

#endif

#ifdef AF_SNA

static void
libexplain_buffer_sockaddr_af_sna(libexplain_string_buffer_t *sb,
    const struct sockaddr *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_sna */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
}

#endif

#ifdef AF_IRDA

static void
libexplain_buffer_sockaddr_af_irda(libexplain_string_buffer_t *sb,
    const struct sockaddr_irda *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_irda */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
}

#endif

#ifdef AF_PPPOX

static void
libexplain_buffer_sockaddr_af_pppox(libexplain_string_buffer_t *sb,
    const struct sockaddr_pppox *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_pppox */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
    /* file /usr/include/linux/if_pppox.h */
}

#endif

#ifdef AF_WANPIPE

static void
libexplain_buffer_sockaddr_af_wanpipe(libexplain_string_buffer_t *sb,
    const struct sockaddr *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_wanpipe */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
}

#endif

#ifdef AF_BLUETOOTH

static void
libexplain_buffer_sockaddr_af_bluetooth(libexplain_string_buffer_t *sb,
    const struct sockaddr *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_(something) */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);

    /*
     * file /usr/include/bluetooth/hci.h:    struct sockaddr_hci
     * file /usr/include/bluetooth/l2cap.h:  struct sockaddr_l2
     * file /usr/include/bluetooth/rfcomm.h: struct sockaddr_rc
     * file /usr/include/bluetooth/sco.h:    struct sockaddr_sco
     */
}

#endif

#ifdef AF_IUCV

static void
libexplain_buffer_sockaddr_af_iucv(libexplain_string_buffer_t *sb,
    const struct sockaddr_iucv *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_iucv */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
}

#endif

#ifdef AF_RXRPC

static void
libexplain_buffer_sockaddr_af_rxrpc(libexplain_string_buffer_t *sb,
    const struct sockaddr *sa, size_t sa_len)
{
    libexplain_string_buffer_putc(sb, ',');
    /* FIXME: decode sockaddr_rxrpc */
    if (sa_len > sizeof(*sa))
        sa_len = sizeof(*sa);
    libexplain_buffer_hexdump(sb, sa, sa_len);
}

#endif

void
libexplain_buffer_sockaddr(libexplain_string_buffer_t *sb,
    const struct sockaddr *sa, int sa_len)
{
    if (libexplain_pointer_is_efault(sa))
    {
        libexplain_buffer_pointer(sb, sa);
        return;
    }
    libexplain_string_buffer_puts(sb, "{ sa_family = ");
    libexplain_buffer_address_family(sb, sa->sa_family);
    switch (sa->sa_family)
    {
    case AF_UNSPEC:
        break;

    case AF_UNIX:
        libexplain_buffer_sockaddr_af_unix
        (
            sb,
            (const struct sockaddr_un *)sa,
            sa_len
        );
        break;

    case AF_INET:
        libexplain_buffer_sockaddr_af_inet
        (
            sb,
            (const struct sockaddr_in *)sa,
            sa_len
        );
        break;

#ifdef AF_AX25
    case AF_AX25:
        libexplain_buffer_sockaddr_af_ax25
        (
            sb,
            (const struct sockaddr_ax25 *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_IPX
    case AF_IPX:
        libexplain_buffer_sockaddr_af_ipx
        (
            sb,
            (const struct sockaddr_ipx *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_APPLETALK
    case AF_APPLETALK:
        libexplain_buffer_sockaddr_af_appletalk
        (
            sb,
            (const struct sockaddr_at *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_NETROM
    case AF_NETROM:
        libexplain_buffer_sockaddr_af_netrom(sb, sa, sa_len);
        break;
#endif

#ifdef AF_BRIDGE
    case AF_BRIDGE:
        libexplain_buffer_sockaddr_af_bridge(sb, sa, sa_len);
        break;
#endif

#ifdef AF_ATMPVC
    case AF_ATMPVC:
        libexplain_buffer_sockaddr_af_atmpvc
        (
            sb,
            (const struct sockaddr_atmpvc *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_X25
    case AF_X25:
        libexplain_buffer_sockaddr_af_x25
        (
            sb,
            (const struct sockaddr_x25 *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_INET6
    case AF_INET6:
        libexplain_buffer_sockaddr_af_inet6
        (
            sb,
            (const struct sockaddr_in6 *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_ROSE
    case AF_ROSE:
        libexplain_buffer_sockaddr_af_rose
        (
            sb,
            (const struct sockaddr_rose *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_DECnet
    case AF_DECnet:
        libexplain_buffer_sockaddr_af_decnet(sb, sa, sa_len);
        break;
#endif

#ifdef AF_NETBEUI
    case AF_NETBEUI:
        libexplain_buffer_sockaddr_af_netbeui(sb, sa, sa_len);
        break;
#endif

#ifdef AF_SECURITY
    case AF_SECURITY:
        libexplain_buffer_sockaddr_af_security(sb, sa, sa_len);
        break;
#endif

#ifdef AF_KEY
    case AF_KEY:
        libexplain_buffer_sockaddr_af_key(sb, sa, sa_len);
        break;
#endif

#ifdef AF_NETLINK
    case AF_NETLINK:
 /* case AF_ROUTE: */
        libexplain_buffer_sockaddr_af_netlink
        (
            sb,
            (const struct sockaddr_nl *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_PACKET
    case AF_PACKET:
        libexplain_buffer_sockaddr_af_packet
        (
            sb,
            (const struct sockaddr_ll *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_ASH
    case AF_ASH:
        libexplain_buffer_sockaddr_af_ash
        (
            sb,
            (const struct sockaddr_ash *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_ECONET
    case AF_ECONET:
        libexplain_buffer_sockaddr_af_econet
        (
            sb,
            (const struct sockaddr_ec *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_ATMSVC
    case AF_ATMSVC:
        libexplain_buffer_sockaddr_af_atmsvc
        (
            sb,
            (const struct sockaddr_atmsvc *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_SNA
    case AF_SNA:
        libexplain_buffer_sockaddr_af_sna(sb, sa, sa_len);
        break;
#endif

#ifdef AF_IRDA
    case AF_IRDA:
        libexplain_buffer_sockaddr_af_irda
        (
            sb,
            (const struct sockaddr_irda *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_PPPOX
    case AF_PPPOX:
        libexplain_buffer_sockaddr_af_pppox
        (
            sb,
            (const struct sockaddr_pppox *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_WANPIPE
    case AF_WANPIPE:
        libexplain_buffer_sockaddr_af_wanpipe(sb, sa, sa_len);
        break;
#endif

#ifdef AF_BLUETOOTH
    case AF_BLUETOOTH:
        libexplain_buffer_sockaddr_af_bluetooth(sb, sa, sa_len);
        break;
#endif

#ifdef AF_IUCV
    case AF_IUCV:
        libexplain_buffer_sockaddr_af_iucv
        (
            sb,
            (const struct sockaddr_iucv *)sa,
            sa_len
        );
        break;
#endif

#ifdef AF_RXRPC
    case AF_RXRPC:
        libexplain_buffer_sockaddr_af_rxrpc(sb, sa, sa_len);
        break;
#endif

    default:
        /* no idea */
        libexplain_string_buffer_putc(sb, ',');
        libexplain_buffer_hexdump(sb, sa, sa_len);
        break;
    }
    libexplain_string_buffer_puts(sb, " }");
}
