/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009-2011 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/filio.h>
#include <libexplain/ac/sys/ioctl.h>
#include <libexplain/ac/sys/socket.h>

#include <libexplain/iocontrol/fiosetown.h>
#include <libexplain/iocontrol/generic.h>


const explain_iocontrol_t explain_iocontrol_fiosetown =
{
    "FIOSETOWN", /* name */
    FIOSETOWN, /* value */
    0, /* disambiguate */
    0, /* print_name */
    explain_iocontrol_generic_print_data_int_star, /* print_data */
    0, /* print_explanation */
    0, /* print_data_returned */
    sizeof(int), /* data_size */
    "int *", /* data_type */
    IOCONTROL_FLAG_SIZE_DOES_NOT_AGREE, /* flags */
    __FILE__,
    __LINE__,
};
