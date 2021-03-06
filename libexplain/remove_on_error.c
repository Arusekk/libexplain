/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010, 2012, 2013 Peter Miller
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

#include <libexplain/remove.h>
#include <libexplain/output.h>


#ifndef HAVE_REMOVE

static int
remove(const char *pathname)
{
    int             result;

    result = unlink(pathname);
    if (result < 0 && errno == EISDIR)
        result = rmdir(pathname);
    return result;
}

#endif


int
explain_remove_on_error(const char *pathname)
{
    int             result;

    result = remove(pathname);
    if (result < 0)
    {
        int             hold_errno;

        hold_errno = errno;
        explain_output_error("%s", explain_errno_remove(hold_errno, pathname));
        errno = hold_errno;
    }
    return result;
}


/* vim: set ts=8 sw=4 et : */
