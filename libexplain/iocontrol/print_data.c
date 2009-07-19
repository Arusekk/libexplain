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

#include <libexplain/buffer/pointer.h>
#include <libexplain/iocontrol.h>
#include <libexplain/path_is_efault.h>
#include <libexplain/string_buffer.h>


void
explain_iocontrol_print_data(const explain_iocontrol_t *p,
    explain_string_buffer_t *sb, int errnum, int fildes, int request,
    const void *data)
{
    /*
     * It is tempting to check that data is a valid pointer, using
     * !explain_pointer_is_efault(data), at this point.  This is
     * undesiable in the general case because there are some ioctl
     * requests that actually pass an integer value as data, rather than
     * a pointer value.  Yes, it stinks; no, it can't be changed.
     */
    if (p && p->print_data)
        p->print_data(p, sb, errnum, fildes, request, data);
    else
        explain_buffer_pointer(sb, data);
}
