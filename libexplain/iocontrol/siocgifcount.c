/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
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
#include <libexplain/ac/linux/sockios.h>
#include <libexplain/ac/sys/sockio.h>

#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/siocgifcount.h>


#ifdef SIOCGIFCOUNT

const explain_iocontrol_t explain_iocontrol_siocgifcount =
{
    "SIOCGIFCOUNT", /* name */
    SIOCGIFCOUNT, /* value */
    0, /* disambiguate */
    0, /* print_name */
    explain_iocontrol_generic_print_data_ignored, /* print_data */
    0, /* print_explanation */
    0, /* print_data_returned */
    NOT_A_POINTER, /* data_size */
    __FILE__,
    __LINE__,
};

#else

const explain_iocontrol_t explain_iocontrol_siocgifcount =
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

#endif
