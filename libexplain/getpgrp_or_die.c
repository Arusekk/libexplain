/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2011, 2012 Peter Miller
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
#include <libexplain/ac/unistd.h>

#include <libexplain/getpgrp.h>
#include <libexplain/output.h>


pid_t
explain_getpgrp_or_die(pid_t pid)
{
    pid_t           result;

    result = explain_getpgrp_on_error(pid);
    if (result < 0)
    {
        explain_output_exit_failure();
    }
    return result;
}


pid_t
explain_getpgrp_on_error(pid_t pid)
{
    pid_t           result;

    result =
        getpgrp
        (
#if GETPGRP_NARGS > 0
            pid
#endif
        );
    if (result < 0)
    {
        int             hold_errno;

        hold_errno = errno;
        explain_output_error("%s", explain_errno_getpgrp(hold_errno, pid));
        errno = hold_errno;
    }
    return result;
}


/* vim: set ts=8 sw=4 et : */
