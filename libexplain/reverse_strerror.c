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

#include <libexplain/errno_info.h>
#include <libexplain/reverse_strerror.h>


int
libexplain_reverse_strerror(const char *text)
{
    const libexplain_errno_info_t *tp;

    /*
     * Look for an exact match.
     */
    tp = libexplain_errno_info_by_text(text);
    if (tp)
        return tp->error_number;

    /*
     * No exact match, look for a fuzzy match.
     */
    tp = libexplain_errno_info_by_text_fuzzy(text);
    if (tp)
        return tp->error_number;

    /*
     * No idea what they are smoking.
     */
    return -1;
}
