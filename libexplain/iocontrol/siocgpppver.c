/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010 Peter Miller
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

#include <libexplain/ac/net/if_ppp.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/ifreq_data/string.h>
#include <libexplain/iocontrol/disambiguate/if_ppp.h>
#include <libexplain/iocontrol/siocgpppver.h>

#ifdef SIOCGPPPVER


static void
print_data(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    explain_buffer_ifreq_data_string(sb, data, 0);
}


static void
print_data_returned(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    explain_buffer_ifreq_data_string(sb, data, 1);
}


const explain_iocontrol_t explain_iocontrol_siocgpppver =
{
    "SIOCGPPPVER", /* name */
    SIOCGPPPVER, /* value */
    explain_iocontrol_disambiguate_is_if_ppp, /* disambiguate */
    0, /* print_name */
    print_data,
    0, /* print_explanation */
    print_data_returned,
    sizeof(struct ifreq), /* data_size */
    __FILE__,
    __LINE__,
};

#else /* ndef SIOCGPPPVER */

const explain_iocontrol_t explain_iocontrol_siocgpppver =
{
    0, /* name */
    0, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
    0, /* print_data_returned */
    0, /* data_size */
    __FILE__,
    __LINE__,
};

#endif /* SIOCGPPPVER */
