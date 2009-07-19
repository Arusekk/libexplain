/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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

#include <libexplain/errno_info/table.h>


const explain_errno_info_t *
explain_errno_info_by_number(int errnum)
{
    const explain_errno_info_t *tp;
    const explain_errno_info_t *end;

    end = explain_errno_info + explain_errno_info_size;
    for (tp = explain_errno_info; tp < end; ++tp)
    {
        if (tp->error_number == errnum)
            return tp;
    }
    return 0;
}
