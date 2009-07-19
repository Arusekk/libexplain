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

#include <libexplain/ac/assert.h>
#include <libexplain/ac/errno.h>
#include <libexplain/ac/netdb.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/getaddrinfo.h>
#include <libexplain/option.h>
#include <libexplain/wrap_and_print.h>


int
explain_getaddrinfo_on_error(const char *node, const char *service,
    const struct addrinfo *hints, struct addrinfo **res)
{
    int             result;

    result = getaddrinfo(node, service, hints, res);
    assert(EAI_SYSTEM < 0);
    if (result == EAI_SYSTEM)
        result = errno;
    if (result)
    {
        explain_program_name_assemble_internal(1);
        explain_wrap_and_print
        (
            stderr,
            explain_errcode_getaddrinfo(result, node, service, hints, res)
        );
    }
    return result;
}
