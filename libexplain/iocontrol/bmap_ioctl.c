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

#include <libexplain/ac/linux/fs.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/iocontrol/bmap_ioctl.h>
#include <libexplain/iocontrol/generic.h>


#ifdef BMAP_IOCTL

static int
disambiguate(int fildes, int request, const void *data)
{
    (void)fildes;
    (void)request;
    (void)data;
    /* success => 0, failure => -1 */
#ifdef FIBMAP
    /*
     * check if synonym of FIBMAP
     *
     * On Linux, on the alpha architecture, we can't use an #if because
     * the defines contain code that cannot be evaulated by the
     * pre-processor.
     */
    return (BMAP_IOCTL == FIBMAP ? -1 : 0);
#else
    return 0;
#endif
}


const explain_iocontrol_t explain_iocontrol_bmap_ioctl =
{
    "BMAP_IOCTL", /* name */
    BMAP_IOCTL, /* value */
    disambiguate,
    0, /* print_name */
    explain_iocontrol_generic_print_data_pointer, /* print_data */
    0, /* print_explanation */
    explain_iocontrol_generic_print_data_int_star, /* print_data_returned */
    sizeof(int), /* data_size */
    __FILE__,
    __LINE__,
};

#else /* ndef BMAP_IOCTL */

const explain_iocontrol_t explain_iocontrol_bmap_ioctl =
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

#endif /* BMAP_IOCTL */
