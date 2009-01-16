/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/linux/kd.h>

#include <libexplain/iocontrol/kdskbmeta.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>


#ifdef HAVE_LINUX_KD_H

static void
libexplain_buffer_kdskbmeta(libexplain_string_buffer_t *sb, int data)
{
    static const libexplain_parse_bits_table_t table[] =
    {
        { "K_METABIT", K_METABIT },
        { "K_ESCPREFIX", K_ESCPREFIX },
    };

    libexplain_parse_bits_print_single(sb, data, table, SIZEOF(table));
}


static void
print_data(const libexplain_iocontrol_t *p, libexplain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    libexplain_buffer_kdskbmeta(sb, (int)data);
}


const libexplain_iocontrol_t libexplain_iocontrol_kdskbmeta =
{
    "KDSKBMETA", /* name */
    KDSKBMETA, /* value */
    0, /* disambiguate */
    0, /* print_name */
    print_data,
    0, /* print_explanation */
};

#else

const libexplain_iocontrol_t libexplain_iocontrol_kdskbmeta =
{
    0, /* name */
    0, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
};

#endif /* HAVE_LINUX_KD_H */
