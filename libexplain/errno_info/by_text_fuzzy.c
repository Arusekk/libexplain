/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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
#include <libexplain/fstrcmp.h>


const libexplain_errno_info_t *
libexplain_errno_info_by_text_fuzzy(const char *text)
{
    const libexplain_errno_info_t *tp;
    const libexplain_errno_info_t *end;
    const libexplain_errno_info_t *best_tp;
    double          best_weight;

    end = libexplain_errno_info + libexplain_errno_info_size;
    best_tp = 0;
    best_weight = 0.6;
    for (tp = libexplain_errno_info; tp < end; ++tp)
    {
        double          weight;

        weight =  libexplain_fstrcmp(strerror(tp->error_number), text);
        if (best_weight < weight)
        {
            best_weight = weight;
            best_tp = tp;
        }
    }
    return best_tp;
}
