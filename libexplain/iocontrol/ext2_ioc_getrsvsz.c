/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/ioctl.h>
#include <libexplain/ac/linux/ext2_fs.h>

#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/ext2_ioc_getrsvsz.h>

#ifdef EXT2_IOC_GETRSVSZ


const explain_iocontrol_t explain_iocontrol_ext2_ioc_getrsvsz =
{
    "EXT2_IOC_GETRSVSZ", /* name */
    EXT2_IOC_GETRSVSZ, /* value */
    0, /* disambiguate */
    0, /* print_name */
    explain_iocontrol_generic_print_data_pointer, /* print_data */
    explain_iocontrol_generic_print_explanation, /* print_explanation */
    explain_iocontrol_generic_print_data_int_star, /* print_data_returned */
    sizeof(int), /* data_size */
    __FILE__,
    __LINE__,
};

#else /* ndef EXT2_IOC_GETRSVSZ */

const explain_iocontrol_t explain_iocontrol_ext2_ioc_getrsvsz =
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

#endif /* EXT2_IOC_GETRSVSZ */