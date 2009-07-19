/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/option.h>
#include <libexplain/wrap_and_print.h>


int
explain_fclose_on_error(FILE *fp)
{
    int             result;

    result = explain_fflush_on_error(fp);
    if (result < 0)
    {
        int             err;

        err = errno;
        fclose(fp);
        errno = err;
        return result;
    }

    result = fclose(fp);
    if (result < 0)
    {
        explain_program_name_assemble_internal(1);
        explain_wrap_and_print(stderr, explain_fclose(fp));
    }
    return result;
}

/* vim: set ts=8 sw=4 et */
