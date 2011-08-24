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

#include <libexplain/ac/sys/ioctl.h>
#include <libexplain/ac/sys/stat.h>

#include <libexplain/iocontrol.h>


int
explain_iocontrol_disambiguate_is_a_socket(int fildes, int request,
    const void *data)
{
    struct stat     st;

    (void)request;
    (void)data;
    if (fstat(fildes, &st) < 0 || !S_ISSOCK(st.st_mode))
        return DISAMBIGUATE_DO_NOT_USE;
    return DISAMBIGUATE_USE;
}
