/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/group_in_groups.h>


int
libexplain_group_in_groups(int gid)
{
    gid_t           groups[NGROUPS_MAX];
    int             n;
    int             j;

    if ((gid_t)gid == getegid())
        return 1;
    n = getgroups(NGROUPS_MAX, groups);
    for (j = 0; j < n; ++j)
        if (groups[j] == (gid_t)gid)
            return 1;
    return 0;
}
