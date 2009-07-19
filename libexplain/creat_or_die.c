/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/creat.h>
#include <libexplain/option.h>
#include <libexplain/wrap_and_print.h>


int
explain_creat_or_die(const char *pathname, int mode)
{
    int             fildes;

    fildes = explain_creat_on_error(pathname, mode);
    if (fildes < 0)
    {
        exit(EXIT_FAILURE);
    }
    return fildes;
}
