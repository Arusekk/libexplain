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

#include <libexplain/ac/net/if.h>
#include <libexplain/ac/linux/ethtool.h>

#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/siocethtool.h>
#include <libexplain/parse_bits.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/sizeof.h>


void
libexplain_buffer_siocethtool(libexplain_string_buffer_t *sb,
    const struct ifreq *ifr)
{
    if (libexplain_pointer_is_efault(ifr, sizeof(*ifr)))
        libexplain_buffer_pointer(sb, ifr);
    else
    {
        libexplain_string_buffer_puts(sb, "{ ifr_name = ");
        libexplain_string_buffer_puts_quoted_n
        (
            sb,
            ifr->ifr_name,
            sizeof(ifr->ifr_name)
        );
        libexplain_string_buffer_puts(sb, ", ifr_data = ");
        if
        (
            libexplain_pointer_is_efault
            (
                ifr->ifr_data,
                sizeof(struct ethtool_cmd)
            )
        )
        {
            libexplain_buffer_pointer(sb, ifr->ifr_data);
        }
        else
        {
            static const libexplain_parse_bits_table_t table[] =
            {
                { "ETHTOOL_GDRVINFO", ETHTOOL_GDRVINFO },
                { "ETHTOOL_GMSGLVL", ETHTOOL_GMSGLVL },
                { "ETHTOOL_GCOALESCE", ETHTOOL_GCOALESCE },
                { "ETHTOOL_GRINGPARAM", ETHTOOL_GRINGPARAM },
                { "ETHTOOL_GPAUSEPARAM", ETHTOOL_GPAUSEPARAM },
                { "ETHTOOL_GRXCSUM", ETHTOOL_GRXCSUM },
                { "ETHTOOL_GTXCSUM", ETHTOOL_GTXCSUM },
                { "ETHTOOL_GSG", ETHTOOL_GSG },
                { "ETHTOOL_GSTRINGS", ETHTOOL_GSTRINGS },
                { "ETHTOOL_GTSO", ETHTOOL_GTSO },
                { "ETHTOOL_GPERMADDR", ETHTOOL_GPERMADDR },
                { "ETHTOOL_GUFO", ETHTOOL_GUFO },
                { "ETHTOOL_GGSO", ETHTOOL_GGSO },
                { "ETHTOOL_GFLAGS", ETHTOOL_GFLAGS },
                { "ETHTOOL_GPFLAGS", ETHTOOL_GPFLAGS },
                { "ETHTOOL_GRXFH", ETHTOOL_GRXFH },
            };
            const struct ethtool_cmd *etp;

            etp = (const struct ethtool_cmd *)ifr->ifr_data;
            libexplain_string_buffer_puts(sb, "{ cmd = ");
            libexplain_parse_bits_print_single
            (
                sb,
                etp->cmd,
                table,
                SIZEOF(table)
            );
            libexplain_string_buffer_puts(sb, " }");
        }
        libexplain_string_buffer_puts(sb, " }");
    }
}
