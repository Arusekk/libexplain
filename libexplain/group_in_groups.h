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

#ifndef LIBEXPLAIN_GROUP_IN_GROUPS_H
#define LIBEXPLAIN_GROUP_IN_GROUPS_H

/**
  * The libexplain_group_in_groups function may be used to determine
  * whether or not the given geoup matches the process' effective GID,
  * or is in the process' groups list.
  *
  * @param gid
  *    The group ID to test
  * @returns
  *    int; non-zero (true) if in groups, zero (false) if not
  */
int libexplain_group_in_groups(int gid);

#endif /* LIBEXPLAIN_GROUP_IN_GROUPS_H */
