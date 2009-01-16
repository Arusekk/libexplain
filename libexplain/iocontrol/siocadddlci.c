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
#include <libexplain/ac/linux/if_frad.h>

#include <libexplain/buffer/dlci_add.h>
#include <libexplain/iocontrol/siocadddlci.h>
#include <libexplain/path_is_efault.h>


#ifdef SIOCADDDLCI

static void
print_data(const libexplain_iocontrol_t *p, libexplain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    libexplain_buffer_dlci_add(sb, data);
}


const libexplain_iocontrol_t libexplain_iocontrol_siocadddlci =
{
    "SIOCADDDLCI", /* name */
    SIOCADDDLCI, /* value */
    0, /* disambiguate */
    0, /* print_name */
    print_data,
    0, /* print_explanation */
};

#else /* ndef SIOCADDDLCI */

const libexplain_iocontrol_t libexplain_iocontrol_siocadddlci =
{
    0, /* name */
    0, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
};

#endif /* SIOCADDDLCI */
