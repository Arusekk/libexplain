/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/ac/stdlib.h>

#include <libexplain/fopen.h>
#include <libexplain/option.h>
#include <libexplain/wrap_and_print.h>


FILE *
explain_fopen_or_die(const char *pathname, const char *flags)
{
    FILE            *fp;

    fp = explain_fopen_on_error(pathname, flags);
    if (!fp)
    {
        exit(EXIT_FAILURE);
    }
    return fp;
}
