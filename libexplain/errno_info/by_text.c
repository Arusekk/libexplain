/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#include <libexplain/ac/string.h>

#include <libexplain/errno_info/table.h>


const libexplain_errno_info_t *
libexplain_errno_info_by_text(const char *text)
{
    const libexplain_errno_info_t *tp;
    const libexplain_errno_info_t *end;

    end = libexplain_errno_info + libexplain_errno_info_size;
    for (tp = libexplain_errno_info; tp < end; ++tp)
    {
        if (0 == strcasecmp(strerror(tp->error_number), text))
            return tp;
    }
    return 0;
}
