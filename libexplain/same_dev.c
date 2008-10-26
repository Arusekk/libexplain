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

#include <libexplain/dirname.h>
#include <libexplain/same_dev.h>


int
libexplain_same_dev(const char *oldpath, const char *newpath)
{
    struct stat     oldpath_st;
    struct stat     newdir_st;
    char            newdir[PATH_MAX + 1];

    if (stat(oldpath, &oldpath_st) < 0)
        return 0;
    libexplain_dirname(newdir, newpath, sizeof(newdir));
    if (stat(newdir, &newdir_st) < 0)
        return 0;
    return (oldpath_st.st_dev == newdir_st.st_dev);
}
