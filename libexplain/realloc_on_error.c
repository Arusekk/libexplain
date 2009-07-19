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

#include <libexplain/realloc.h>
#include <libexplain/option.h>
#include <libexplain/wrap_and_print.h>


void *
explain_realloc_on_error(void *ptr, size_t size)
{
    size_t          ok_size;
    void            *result;

    /*
     * Common mis-implementations of the posix standard:
     * Some realloc implementations can't cope with a size of zero.
     * Some realloc implementations can't cope with a ptr of NULL.
     */
    ok_size = size ? size : 1;
    result = ptr ? realloc(ptr, ok_size) : malloc(ok_size);
    if (!result)
    {
        explain_program_name_assemble_internal(1);
        explain_wrap_and_print(stderr, explain_realloc(ptr, size));
    }
    return result;
}


/* vim:ts=8:sw=4:et */
