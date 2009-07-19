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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/fread.h>
#include <libexplain/option.h>
#include <libexplain/wrap_and_print.h>


size_t
explain_fread_or_die(void *ptr, size_t size, size_t nmemb, FILE *fp)
{
    size_t          result;

    result = explain_fread_on_error(ptr, size, nmemb, fp);
    if (result == 0 && ferror(fp))
    {
        exit(EXIT_FAILURE);
    }
    return result;
}
