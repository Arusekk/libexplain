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

#include <libexplain/buffer/pointer.h>
#include <libexplain/iocontrol/siocgifvlan.h>
#include <libexplain/path_is_efault.h>


#ifndef SIOCGIFVLAN
#define SIOCGIFVLAN -1
#endif

const libexplain_iocontrol_t libexplain_iocontrol_siocgifvlan =
{
    "SIOCGIFVLAN", /* name */
    SIOCGIFVLAN, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
};
