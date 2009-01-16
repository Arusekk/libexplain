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

#include <libexplain/iocontrol/generic.h>
#include <libexplain/string_buffer.h>


void
libexplain_iocontrol_print_explanation(const libexplain_iocontrol_t *p,
    libexplain_string_buffer_t *sb, int errnum, int fildes, int request,
    const void *data)
{
    void (*func)(const libexplain_iocontrol_t *p,
        libexplain_string_buffer_t *sb, int errnum, int fildes, int request,
        const void *data);

    func = libexplain_iocontrol_generic.print_explanation;
    if (p && p->print_explanation)
        func = p->print_explanation;
    if (func)
        func(p, sb, errnum, fildes, request, data);
}
