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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/linux/fs.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/dac.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/iocontrol/blkflsbuf.h>
#include <libexplain/iocontrol/generic.h>


#ifdef BLKFLSBUF

static void
print_explanation(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    switch (errnum)
    {
    case EACCES:
        explain_buffer_gettext
        (
            sb,
            /*
             * xgettext:  This error message is issued to explain an
             * EACCES error reported by the BLKFLSBUF ioctl, in the case
             * where the process does not have permission to flush the
             * buffers.
             */
            i18n("the process does not have permission to flush the buffers")
        );
        explain_buffer_dac_sys_admin(sb);
        break;

    default:
        explain_iocontrol_generic.print_explanation(p, sb, errnum, fildes,
            request, data);
        break;
    }
}


const explain_iocontrol_t explain_iocontrol_blkflsbuf =
{
    "BLKFLSBUF", /* name */
    BLKFLSBUF, /* value */
    0, /* disambiguate */
    0, /* print_name */
    explain_iocontrol_generic_print_data_int, /* print_data */
    print_explanation,
    0, /* print_data_returned */
    NOT_A_POINTER, /* data_size */
    __FILE__,
    __LINE__,
};

#else /* ndef BLKFLSBUF */

const explain_iocontrol_t explain_iocontrol_blkflsbuf =
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

#endif /* BLKFLSBUF */