/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/param.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/capability.h>
#include <libexplain/group_in_groups.h>
#include <libexplain/have_permission.h>


static int
libexplain_have_permission(const struct stat *st, int wanted)
{
    /*
     * The first group of three is used when the effective user ID of
     * the calling process equals the owner ID of the file.
     */
    if (st->st_uid == geteuid())
    {
        return (0 != (st->st_mode & wanted & S_IRWXU));
    }

    /*
     * The second group of three is used when the group ID of the file
     * either equals the effective group ID of the calling process, or
     * is one of the supplementary group IDs of the calling process (as
     * set by setgroups(2)).
     */
    if (libexplain_group_in_groups(st->st_gid))
    {
        return (0 != (st->st_mode & wanted & S_IRWXG));
    }

    /*
     * When neither holds, the third group is used.
     */
    return (0 != (st->st_mode & wanted & S_IRWXO));
}


int
libexplain_have_read_permission(const struct stat *st)
{
    if (libexplain_capability_dac_read_search())
        return 1;
    return libexplain_have_permission(st, 0444);
}


int
libexplain_have_write_permission(const struct stat *st)
{
    if (libexplain_capability_dac_override())
        return 1;
    return libexplain_have_permission(st, 0222);
}


int
libexplain_have_execute_permission(const struct stat *st)
{
    if (!S_ISREG(st->st_mode))
        return 0;
    if
    (
        libexplain_capability_dac_override()
#ifdef __linux__
    &&
        (st->st_mode & 0111)
#endif
    )
        return 1;
    return libexplain_have_permission(st, 0111);
}


int
libexplain_have_search_permission(const struct stat *st)
{
    if (!S_ISDIR(st->st_mode))
        return 0;
    if (libexplain_capability_dac_read_search())
        return 1;
    return libexplain_have_permission(st, 0111);
}


int
libexplain_have_inode_permission(const struct stat *st)
{
    if (libexplain_capability_fowner())
        return 1;
    return (geteuid() == st->st_uid);
}
