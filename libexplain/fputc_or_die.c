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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/stdio.h>

#include <libexplain/fputc.h>
#include <libexplain/output.h>


int
explain_fputc_on_error(int c, FILE *fp)
{
    int             result;

    result = fputc(c, fp);
    if (result == EOF)
    {
        int             hold_errno;

        hold_errno = errno;
        explain_output_error("%s", explain_errno_fputc(hold_errno, c, fp));
        errno = hold_errno;
    }
    return result;
}


void
explain_fputc_or_die(int c, FILE *fp)
{
    if (explain_fputc_on_error(c, fp) < 0)
    {
        explain_output_exit_failure();
    }
}


/* vim: set ts=8 sw=4 et : */
