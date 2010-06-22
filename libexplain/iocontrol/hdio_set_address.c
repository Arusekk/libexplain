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
#include <libexplain/ac/errno.h>
#include <libexplain/ac/linux/hdreg.h>
#include <libexplain/ac/stdint.h>

#include <libexplain/buffer/dac.h>
#include <libexplain/capability.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/hdio_set_address.h>

#ifdef HDIO_SET_ADDRESS

static void
print_explanation(const explain_iocontrol_t *p, explain_string_buffer_t *sb, int
    errnum, int fildes, int request, const void *data)
{
    switch (errnum)
    {
    case EACCES:
        if (!explain_capability_sys_admin())
        {
            explain_buffer_dac_sys_admin(sb);
            break;
        }
        /* fall through... */

    default:
        explain_iocontrol_generic_print_explanation
        (
            p,
            sb,
            errnum,
            fildes,
            request,
            data
        );
        break;
    }
}


const explain_iocontrol_t explain_iocontrol_hdio_set_address =
{
    "HDIO_SET_ADDRESS", /* name */
    HDIO_SET_ADDRESS, /* value */
    0, /* disambiguate */
    0, /* print_name */
    explain_iocontrol_generic_print_data_long, /* print_data */
    print_explanation,
    0, /* print_data_returned */
    NOT_A_POINTER, /* data_size */
    __FILE__,
    __LINE__,
};

#else /* ndef HDIO_SET_ADDRESS */

const explain_iocontrol_t explain_iocontrol_hdio_set_address =
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

#endif /* HDIO_SET_ADDRESS */
