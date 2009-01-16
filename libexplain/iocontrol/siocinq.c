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

#include <libexplain/ac/linux/sockios.h>
#include <libexplain/ac/sys/ioctl.h>
#include <libexplain/ac/sys/stat.h>

#include <libexplain/iocontrol/siocinq.h>


#ifndef SIOCINQ
#define SIOCINQ -1
#endif


static int
disambiguate(int fildes, int request, const void *data)
{
    struct stat     st;

    (void)request;
    (void)data;
    return (fstat(fildes, &st) >= 0 && S_ISSOCK(st.st_mode));
}


const libexplain_iocontrol_t libexplain_iocontrol_siocinq =
{
    "SIOCINQ", /* name */
    SIOCINQ, /* value */
    disambiguate,
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
};
