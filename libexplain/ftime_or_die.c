/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
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
#include <libexplain/ac/time.h>
#include <libexplain/ac/sys/timeb.h>

#include <libexplain/ftime.h>
#include <libexplain/option.h>
#include <libexplain/output.h>


void
explain_ftime_or_die(struct timeb *tp)
{
    if (explain_ftime_on_error(tp) < 0)
    {
        explain_output_exit_failure();
    }
}


int
explain_ftime_on_error(struct timeb *tp)
{
    int             result;

#ifdef HAVE_FTIME
    result = ftime(tp);
#else
    errno = ENOSYS;
    result = -1;
#endif
    if (result < 0)
    {
        int             hold_errno;

        hold_errno = errno;
        explain_program_name_assemble_internal(1);
        explain_output_message(explain_errno_ftime(hold_errno, tp));
        errno = hold_errno;
    }
    return result;
}


/* vim: set ts=8 sw=4 et */
