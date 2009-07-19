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

#include <libexplain/ac/net/route.h>

#include <libexplain/buffer/pathname.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/rtentry.h>
#include <libexplain/buffer/sockaddr.h>
#include <libexplain/parse_bits.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/sizeof.h>


static void
explain_buffer_rtentry_flags(explain_string_buffer_t *sb, int data)
{
    static const explain_parse_bits_table_t table[] =
    {
        { "RTF_UP", RTF_UP },
        { "RTF_GATEWAY", RTF_GATEWAY },
        { "RTF_HOST", RTF_HOST },
        { "RTF_REINSTATE", RTF_REINSTATE },
        { "RTF_DYNAMIC", RTF_DYNAMIC },
        { "RTF_MODIFIED", RTF_MODIFIED },
        { "RTF_MTU", RTF_MTU },
        { "RTF_MSS", RTF_MSS },
        { "RTF_WINDOW", RTF_WINDOW },
        { "RTF_IRTT", RTF_IRTT },
        { "RTF_REJECT", RTF_REJECT },
        { "RTF_STATIC", RTF_STATIC },
        { "RTF_XRESOLVE", RTF_XRESOLVE },
        { "RTF_NOFORWARD", RTF_NOFORWARD },
        { "RTF_THROW", RTF_THROW },
        { "RTF_NOPMTUDISC", RTF_NOPMTUDISC },
    };

    explain_parse_bits_print(sb, data, table, SIZEOF(table));
}


void
explain_buffer_rtentry(explain_string_buffer_t *sb,
    const struct rtentry *data)
{
    if (explain_pointer_is_efault(data, sizeof(*data)))
        explain_buffer_pointer(sb, data);
    else
    {
        explain_string_buffer_puts(sb, "{ rt_dst = ");
        explain_buffer_sockaddr(sb, &data->rt_dst, sizeof(data->rt_dst));
        if (data->rt_flags & RTF_GATEWAY)
        {
            explain_string_buffer_puts(sb, ", rt_gateway = ");
            explain_buffer_sockaddr
            (
                sb,
                &data->rt_gateway,
                sizeof(data->rt_gateway)
            );
        }
        explain_string_buffer_puts(sb, ", rt_genmask = ");
        explain_buffer_sockaddr
        (
            sb,
            &data->rt_genmask,
            sizeof(data->rt_genmask)
        );
        explain_string_buffer_puts(sb, ", rt_flags = ");
        explain_buffer_rtentry_flags(sb, data->rt_flags);
        explain_string_buffer_printf(sb, ", rt_tos = %d, ", data->rt_tos);
        explain_string_buffer_printf(sb, "rt_class = %d, ", data->rt_class);
        explain_string_buffer_printf(sb, "rt_class = %d, ", data->rt_metric);
        explain_string_buffer_puts(sb, "rt_dev = ");
        explain_buffer_pathname(sb, data->rt_dev);
        explain_string_buffer_printf(sb, ", rt_mtu = %lu, ", data->rt_mtu);
        explain_string_buffer_printf
        (
            sb,
            "rt_window = %lu, ",
            data->rt_window
        );
        explain_string_buffer_printf(sb, "rt_irtt = %u }", data->rt_irtt);
    }
}
