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

#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/ifreq_qlen.h>
#include <libexplain/iocontrol/siocsiftxqlen.h>


#ifdef SIOCSIFTXQLEN

static void
print_data(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    explain_buffer_ifreq_qlen(sb, data);
}


const explain_iocontrol_t explain_iocontrol_siocsiftxqlen =
{
    "SIOCSIFTXQLEN", /* name */
    SIOCSIFTXQLEN, /* value */
    0, /* disambiguate */
    0, /* print_name */
    print_data,
    0, /* print_explanation */
};

#else /* ndef SIOCSIFTXQLEN */

const explain_iocontrol_t explain_iocontrol_siocsiftxqlen =
{
    0, /* name */
    0, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
};

#endif /* SIOCSIFTXQLEN */
