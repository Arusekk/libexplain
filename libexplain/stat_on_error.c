/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/stat.h>

#include <libexplain/option.h>
#include <libexplain/stat.h>
#include <libexplain/wrap_and_print.h>


int
explain_stat_on_error(const char *pathname, struct stat *data)
{
    int             result;

    result = stat(pathname, data);
    if (result < 0)
    {
        explain_program_name_assemble_internal(1);
        explain_wrap_and_print(stderr, explain_stat(pathname, data));
    }
    return result;
}
