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

#include <libexplain/iocontrol/kdsetmode.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>


#ifdef HAVE_LINUX_KD_H

static void
libexplain_buffer_kdsetmode(libexplain_string_buffer_t *sb, int led)
{
    static const libexplain_parse_bits_table_t table[] =
    {
        { "KD_TEXT", KD_TEXT },
        { "KD_GRAPHICS", KD_GRAPHICS },
        { "KD_TEXT0", KD_TEXT0 },
        { "KD_TEXT1", KD_TEXT1 },
    };

    libexplain_parse_bits_print_single(sb, led, table, SIZEOF(table));
}


static void
print_data(const libexplain_iocontrol_t *p, libexplain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    libexplain_buffer_kdsetmode(sb, (int)data);
}


const libexplain_iocontrol_t libexplain_iocontrol_kdsetmode =
{
    "KDSETMODE", /* name */
    KDSETMODE, /* value */
    0, /* disambiguate */
    0, /* print_name */
    print_data,
    0, /* print_explanation */
};

#else

const libexplain_iocontrol_t libexplain_iocontrol_kdsetmode =
{
    0, /* name */
    0, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
};

#endif /* HAVE_LINUX_KD_H */
