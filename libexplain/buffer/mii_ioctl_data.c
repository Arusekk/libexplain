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

#include <libexplain/ac/linux/mii.h>

#include <libexplain/buffer/mii_ioctl_data.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/parse_bits.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/sizeof.h>


static void
libexplain_buffer_mii_reg(libexplain_string_buffer_t *sb, int data)
{
    static const libexplain_parse_bits_table_t table[] =
    {
        { "MII_BMCR", MII_BMCR },
        { "MII_BMSR", MII_BMSR },
        { "MII_PHYSID1", MII_PHYSID1 },
        { "MII_PHYSID2", MII_PHYSID2 },
        { "MII_ADVERTISE", MII_ADVERTISE },
        { "MII_LPA", MII_LPA },
        { "MII_EXPANSION", MII_EXPANSION },
        { "MII_CTRL1000", MII_CTRL1000 },
        { "MII_STAT1000", MII_STAT1000 },
        { "MII_ESTATUS", MII_ESTATUS },
        { "MII_DCOUNTER", MII_DCOUNTER },
        { "MII_FCSCOUNTER", MII_FCSCOUNTER },
        { "MII_NWAYTEST", MII_NWAYTEST },
        { "MII_RERRCOUNTER", MII_RERRCOUNTER },
        { "MII_SREVISION", MII_SREVISION },
        { "MII_LBRERROR", MII_LBRERROR },
        { "MII_PHYADDR", MII_PHYADDR },
        { "MII_TPISTATUS", MII_TPISTATUS },
        { "MII_NCONFIG", MII_NCONFIG },
    };

    libexplain_parse_bits_print_single(sb, data, table, SIZEOF(table));
}


static void
libexplain_buffer_mii_bmcr(libexplain_string_buffer_t *sb, int data)
{
    static const libexplain_parse_bits_table_t table[] =
    {
        { "BMCR_RESV", BMCR_RESV },
        { "BMCR_SPEED1000", BMCR_SPEED1000 },
        { "BMCR_CTST", BMCR_CTST },
        { "BMCR_FULLDPLX", BMCR_FULLDPLX },
        { "BMCR_ANRESTART", BMCR_ANRESTART },
        { "BMCR_ISOLATE", BMCR_ISOLATE },
        { "BMCR_PDOWN", BMCR_PDOWN },
        { "BMCR_ANENABLE", BMCR_ANENABLE },
        { "BMCR_SPEED100", BMCR_SPEED100 },
        { "BMCR_LOOPBACK", BMCR_LOOPBACK },
        { "BMCR_RESET", BMCR_RESET },
    };

    libexplain_parse_bits_print(sb, data, table, SIZEOF(table));
}


static void
libexplain_buffer_mii_bmsr(libexplain_string_buffer_t *sb, int data)
{
    static const libexplain_parse_bits_table_t table[] =
    {
        { "BMSR_ERCAP", BMSR_ERCAP },
        { "BMSR_JCD", BMSR_JCD },
        { "BMSR_LSTATUS", BMSR_LSTATUS },
        { "BMSR_ANEGCAPABLE", BMSR_ANEGCAPABLE },
        { "BMSR_RFAULT", BMSR_RFAULT },
        { "BMSR_ANEGCOMPLETE", BMSR_ANEGCOMPLETE },
        { "BMSR_RESV", BMSR_RESV },
        { "BMSR_ESTATEN", BMSR_ESTATEN },
        { "BMSR_100HALF2", BMSR_100HALF2 },
        { "BMSR_100FULL2", BMSR_100FULL2 },
        { "BMSR_10HALF", BMSR_10HALF },
        { "BMSR_10FULL", BMSR_10FULL },
        { "BMSR_100HALF", BMSR_100HALF },
        { "BMSR_100FULL", BMSR_100FULL },
        { "BMSR_100BASE4", BMSR_100BASE4 },
    };

    libexplain_parse_bits_print(sb, data, table, SIZEOF(table));
}


static void
libexplain_buffer_mii_ad(libexplain_string_buffer_t *sb, int data)
{
    static const libexplain_parse_bits_table_t table[] =
    {
        { "ADVERTISE_SLCT", ADVERTISE_SLCT },
        { "ADVERTISE_CSMA", ADVERTISE_CSMA },
        { "ADVERTISE_10HALF", ADVERTISE_10HALF },
        { "ADVERTISE_1000XFULL", ADVERTISE_1000XFULL },
        { "ADVERTISE_10FULL", ADVERTISE_10FULL },
        { "ADVERTISE_1000XHALF", ADVERTISE_1000XHALF },
        { "ADVERTISE_100HALF", ADVERTISE_100HALF },
        { "ADVERTISE_1000XPAUSE", ADVERTISE_1000XPAUSE },
        { "ADVERTISE_100FULL", ADVERTISE_100FULL },
        { "ADVERTISE_1000XPSE_ASYM", ADVERTISE_1000XPSE_ASYM },
        { "ADVERTISE_100BASE4", ADVERTISE_100BASE4 },
        { "ADVERTISE_PAUSE_CAP", ADVERTISE_PAUSE_CAP },
        { "ADVERTISE_PAUSE_ASYM", ADVERTISE_PAUSE_ASYM },
        { "ADVERTISE_RESV", ADVERTISE_RESV },
        { "ADVERTISE_RFAULT", ADVERTISE_RFAULT },
        { "ADVERTISE_LPACK", ADVERTISE_LPACK },
        { "ADVERTISE_NPAGE", ADVERTISE_NPAGE },
    };

    libexplain_parse_bits_print(sb, data, table, SIZEOF(table));
}


static void
libexplain_buffer_mii_lpa(libexplain_string_buffer_t *sb, int data)
{
    static const libexplain_parse_bits_table_t table[] =
    {
        { "LPA_SLCT", LPA_SLCT },
        { "LPA_10HALF", LPA_10HALF },
        { "LPA_1000XFULL", LPA_1000XFULL },
        { "LPA_10FULL", LPA_10FULL },
        { "LPA_1000XHALF", LPA_1000XHALF },
        { "LPA_100HALF", LPA_100HALF },
        { "LPA_1000XPAUSE", LPA_1000XPAUSE },
        { "LPA_100FULL", LPA_100FULL },
        { "LPA_1000XPAUSE_ASYM", LPA_1000XPAUSE_ASYM },
        { "LPA_100BASE4", LPA_100BASE4 },
        { "LPA_PAUSE_CAP", LPA_PAUSE_CAP },
        { "LPA_PAUSE_ASYM", LPA_PAUSE_ASYM },
        { "LPA_RESV", LPA_RESV },
        { "LPA_RFAULT", LPA_RFAULT },
        { "LPA_LPACK", LPA_LPACK },
        { "LPA_NPAGE", LPA_NPAGE },
    };

    libexplain_parse_bits_print(sb, data, table, SIZEOF(table));
}


static void
libexplain_buffer_mii_expansion(libexplain_string_buffer_t *sb, int data)
{
    static const libexplain_parse_bits_table_t table[] =
    {
        { "EXPANSION_NWAY", EXPANSION_NWAY },
        { "EXPANSION_LCWP", EXPANSION_LCWP },
        { "EXPANSION_ENABLENPAGE", EXPANSION_ENABLENPAGE },
        { "EXPANSION_NPCAPABLE", EXPANSION_NPCAPABLE },
        { "EXPANSION_MFAULTS", EXPANSION_MFAULTS },
    };

    libexplain_parse_bits_print(sb, data, table, SIZEOF(table));
}


void
libexplain_buffer_mii_ioctl_data(libexplain_string_buffer_t *sb,
    const struct mii_ioctl_data *data)
{
    if (libexplain_pointer_is_efault(data, sizeof(*data)))
        libexplain_buffer_pointer(sb, data);
    else
    {
        libexplain_string_buffer_printf(sb, "{ phy_id = %d, ", data->phy_id);
        libexplain_string_buffer_puts(sb, "reg_num = %d, ");
        libexplain_buffer_mii_reg(sb, data->reg_num);
        switch (data->reg_num)
        {
        case MII_BMCR:
            libexplain_string_buffer_puts(sb, ", val_in = ");
            libexplain_buffer_mii_bmcr(sb, data->val_in);
            libexplain_string_buffer_puts(sb, ", val_out = ");
            libexplain_buffer_mii_bmcr(sb, data->val_out);
            break;

        case MII_BMSR:
            libexplain_string_buffer_puts(sb, ", val_in = ");
            libexplain_buffer_mii_bmsr(sb, data->val_in);
            libexplain_string_buffer_puts(sb, ", val_out = ");
            libexplain_buffer_mii_bmsr(sb, data->val_out);
            break;

        case MII_ADVERTISE:
            libexplain_string_buffer_puts(sb, ", val_in = ");
            libexplain_buffer_mii_ad(sb, data->val_in);
            libexplain_string_buffer_puts(sb, ", val_out = ");
            libexplain_buffer_mii_ad(sb, data->val_out);
            break;

        case MII_LPA:
            libexplain_string_buffer_puts(sb, ", val_in = ");
            libexplain_buffer_mii_lpa(sb, data->val_in);
            libexplain_string_buffer_puts(sb, ", val_out = ");
            libexplain_buffer_mii_lpa(sb, data->val_out);
            break;

        case MII_EXPANSION:
            libexplain_string_buffer_puts(sb, ", val_in = ");
            libexplain_buffer_mii_expansion(sb, data->val_in);
            libexplain_string_buffer_puts(sb, ", val_out = ");
            libexplain_buffer_mii_expansion(sb, data->val_out);
            break;

        default:
            libexplain_string_buffer_printf
            (
                sb,
                ", val_in = %d, val_out = %d",
                data->val_in,
                data->val_out
            );
            break;
        }
        libexplain_string_buffer_puts(sb, " }");
    }
}
