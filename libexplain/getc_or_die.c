/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010, 2012 Peter Miller
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

#include <libexplain/getc.h>
#include <libexplain/output.h>


#if __GNUC__ < 3
static
#endif

void
explain_getc_or_die_failed(FILE *fp)
{
    explain_output_error("%s", explain_getc(fp));
    explain_output_exit_failure();
}


#if __GNUC__ < 3

int
explain_getc_or_die(FILE *fp)
{
    int             c;

    c = getc(fp);
    if (c == EOF && ferror(fp))
        explain_getc_or_die_failed(fp);
    return c;
}

#endif


/* vim: set ts=8 sw=4 et : */
