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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/common_message_buffer.h>
#include <libexplain/program_name.h>
#include <libexplain/string_buffer.h>
#include <libexplain/strtod_or_die.h>
#include <libexplain/wrap_and_print.h>


double
libexplain_strtod_or_die(const char *cp)
{
    char            *ep;
    double          n;

    ep = 0;
    n = strtod(cp, &ep);
    if (cp == ep || *ep)
    {
        const char      *prog;

        libexplain_string_buffer_t sb;
        libexplain_string_buffer_init
        (
            &sb,
            libexplain_common_message_buffer,
            libexplain_common_message_buffer_size
        );
        prog = libexplain_program_name_get();
        if (prog && *prog)
        {
            libexplain_string_buffer_puts(&sb, prog);
            libexplain_string_buffer_puts(&sb, ": ");
        }
        libexplain_string_buffer_puts(&sb, "the string ");
        libexplain_string_buffer_puts_quoted(&sb, cp);
        libexplain_string_buffer_puts(&sb, " doesn't look like a number");
        libexplain_wrap_and_print(stderr, libexplain_common_message_buffer);
        exit(EXIT_FAILURE);
    }
    return n;
}
