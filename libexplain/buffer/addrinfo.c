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

#include <libexplain/ac/netdb.h>

#include <libexplain/buffer/address_family.h>
#include <libexplain/buffer/addrinfo.h>
#include <libexplain/buffer/addrinfo_flags.h>
#include <libexplain/buffer/pathname.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/sockaddr.h>
#include <libexplain/buffer/socket_protocol.h>
#include <libexplain/buffer/socket_type.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>


void
libexplain_buffer_addrinfo(libexplain_string_buffer_t *sb,
    const struct addrinfo *ai)
{
    if (libexplain_pointer_is_efault(ai))
    {
        libexplain_buffer_pointer(sb, ai);
        return;
    }
    libexplain_string_buffer_puts(sb, "{ ai_flags = ");
    libexplain_buffer_addrinfo_flags(sb, ai->ai_flags);
    libexplain_string_buffer_puts(sb, ", ai_family = ");
    libexplain_buffer_address_family(sb, ai->ai_family);
    libexplain_string_buffer_puts(sb, ", ai_socktype = ");
    libexplain_buffer_socket_type(sb, ai->ai_socktype);
    libexplain_string_buffer_puts(sb, ", ai_protocol = ");
    libexplain_buffer_socket_protocol(sb, ai->ai_protocol);
    libexplain_string_buffer_puts(sb, ", ai_addr = ");
    libexplain_buffer_sockaddr(sb, ai->ai_addr, ai->ai_addrlen);
    if (ai->ai_flags & AI_CANONNAME)
    {
        libexplain_string_buffer_puts(sb, "ai_canonname = ");
        libexplain_buffer_pathname(sb, ai->ai_canonname);
    }
    libexplain_string_buffer_puts(sb, " }");
}
