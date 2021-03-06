/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/ioctl.h>
#include <libexplain/output.h>


int
explain_ioctl_or_die(int fildes, int request, void *data)
{
    int             result;

    errno = 0;
    result = explain_ioctl_on_error(fildes, request, data);
    if (result == -1 && errno != 0)
    {
        explain_output_exit_failure();
    }
    return result;
}

/* vim: set ts=8 sw=4 et : */
