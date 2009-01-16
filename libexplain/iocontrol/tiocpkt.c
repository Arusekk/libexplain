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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/int.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/tiocpkt.h>


static void
print_data(const libexplain_iocontrol_t *p, libexplain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    libexplain_buffer_int_star(sb, data);
}


static void
print_explanation(const libexplain_iocontrol_t *p,
    libexplain_string_buffer_t *sb, int errnum, int fildes, int request,
    const void *data)
{
    switch (errnum)
    {
    case ENOTTY:
        libexplain_buffer_gettext
        (
            sb,
            /*
             * xgettext: This message is used when explaining an ENOTTY
             * error reported by an ioctl TIOCPKT system call.
             */
            i18n("this may only be applied to the master side of a "
            "pseudo-terminal")
        );
        break;

    default:
        libexplain_iocontrol_generic.print_explanation
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


const libexplain_iocontrol_t libexplain_iocontrol_tiocpkt =
{
    "TIOCPKT", /* name */
    TIOCPKT, /* value */
    0, /* disambiguate */
    0, /* print_name */
    print_data,
    print_explanation
};
