/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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
#include <libexplain/ac/stdlib.h>

#include <libexplain/getcwd.h>
#include <libexplain/wrap_and_print.h>


char *
libexplain_getcwd_or_die(char *data, size_t data_size)
{
    char            *result;

    result = getcwd(data, data_size);
    if (!result)
    {
        libexplain_wrap_and_print(stderr, libexplain_getcwd(data, data_size));
        exit(EXIT_FAILURE);
    }
    return result;
}
