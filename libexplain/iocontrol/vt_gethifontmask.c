/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009-2011 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/ioctl.h>
#include <libexplain/ac/linux/vt.h>

#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/vt_gethifontmask.h>

#ifdef VT_GETHIFONTMASK


const explain_iocontrol_t explain_iocontrol_vt_gethifontmask =
{
    "VT_GETHIFONTMASK", /* name */
    VT_GETHIFONTMASK, /* value */
    0, /* disambiguate */
    0, /* print_name */
    explain_iocontrol_generic_print_data_pointer, /* print_data */
    0, /* print_explanation */
    explain_iocontrol_generic_print_data_short_star, /* print_data_returned */
    sizeof(short), /* data_size */
    "short *", /* data_type */
    IOCONTROL_FLAG_SIZE_DOES_NOT_AGREE, /* flags */
    __FILE__,
    __LINE__,
};

#else /* ndef VT_GETHIFONTMASK */

const explain_iocontrol_t explain_iocontrol_vt_gethifontmask =
{
    0, /* name */
    0, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
    0, /* print_data_returned */
    0, /* data_size */
    0, /* data_type */
    0, /* flags */
    __FILE__,
    __LINE__,
};

#endif /* VT_GETHIFONTMASK */
