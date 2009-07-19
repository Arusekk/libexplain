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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/strtold.h>
#include <libexplain/option.h>
#include <libexplain/wrap_and_print.h>


long double
explain_strtold_on_error(const char *nptr, char **endptr)
{
    int             err;
    char            *dummy;
    long double     result;

    err = errno;
    dummy = 0;
    errno = 0;
    result = strtold(nptr, endptr ? endptr : &dummy);
    if (errno == 0 && (endptr ? *endptr : dummy) == nptr)
    {
        /*
         * Some implementations do this natively.
         * Fake it on those that don't.
         */
        errno = EINVAL;
    }
    if (errno != 0)
    {
        explain_program_name_assemble_internal(1);
        explain_wrap_and_print(stderr, explain_strtold(nptr, endptr));
    }
    else
        errno = err;
    return result;
}


/* vim: set ts=8 sw=4 et */