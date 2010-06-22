/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010 Peter Miller
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

#include <libexplain/output/stderr.h>
#include <libexplain/wrap_and_print.h>


static void
message(explain_output_t *op, const char *text)
{
    (void)op;
    explain_wrap_and_print(stderr, text);
}


static const explain_output_vtable_t vtable =
{
    0, /* destructor */
    message,
    0, /* exit */
    sizeof(explain_output_t)
};


explain_output_t explain_output_static_stderr = { &vtable };


explain_output_t *
explain_output_stderr_new(void)
{
    return explain_output_new(&vtable);
}
