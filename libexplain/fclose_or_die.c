/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010 Peter Miller
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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/stdio.h>

#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/option.h>
#include <libexplain/output.h>


int
explain_fclose_on_error(FILE *fp)
{
    int             result;

    result = explain_fflush_on_error(fp);
    if (result < 0)
    {
        int             hold_errno;

        hold_errno = errno;
        explain_program_name_assemble_internal(1);
        explain_output_message(explain_errno_fflush(hold_errno, fp));
        fclose(fp);
        errno = hold_errno;
        return result;
    }

    result = fclose(fp);
    if (result < 0)
    {
        int             hold_errno;

        hold_errno = errno;
        explain_program_name_assemble_internal(1);
        explain_output_message(explain_errno_fclose(hold_errno, fp));
        errno = hold_errno;
    }
    return result;
}


void
explain_fclose_or_die(FILE *fp)
{
    if (explain_fclose_on_error(fp))
    {
        explain_output_exit_failure();
    }
}


/* vim: set ts=8 sw=4 et */
