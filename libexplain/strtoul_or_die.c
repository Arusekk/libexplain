/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/strtoul.h>


unsigned long
explain_strtoul_or_die(const char *nptr, char **endptr, int base)
{
    int             err;
    unsigned long   result;

    err = errno;
    errno = 0;
    result = explain_strtoul_on_error(nptr, endptr, base);
    if (errno)
        exit(EXIT_FAILURE);
    errno = err;
    return result;
}
