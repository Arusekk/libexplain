/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/ac/stdio.h>

#include <libexplain/setsockopt.h>
#include <libexplain/option.h>
#include <libexplain/wrap_and_print.h>


int
explain_setsockopt_on_error(int fildes, int level, int name, void *data,
    socklen_t data_size)
{
    int             result;

    result = setsockopt(fildes, level, name, data, data_size);
    if (result < 0)
    {
        explain_program_name_assemble_internal(1);
        explain_wrap_and_print
        (
            stderr,
            explain_setsockopt(fildes, level, name, data, data_size)
        );
    }
    return result;
}

/* vim: set ts=8 sw=4 et */
