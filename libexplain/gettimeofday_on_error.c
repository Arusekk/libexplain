/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/sys/time.h>

#include <libexplain/gettimeofday.h>
#include <libexplain/option.h>
#include <libexplain/output.h>


int
explain_gettimeofday_on_error(struct timeval *tv, struct timezone *tz)
{
    int             result;

    result = gettimeofday(tv, tz);
    if (result < 0)
    {
        int             hold_errno;

        hold_errno = errno;
        explain_program_name_assemble_internal(1);
        explain_output_message(explain_errno_gettimeofday(hold_errno,
            tv, tz));
        errno = hold_errno;
    }
    return result;
}


/* vim: set ts=8 sw=4 et */
