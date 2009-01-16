/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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

#include <libexplain/dir_to_fildes.h>
#include <libexplain/path_is_efault.h>


int
libexplain_dir_to_fildes(DIR *dir)
{
    if (!dir)
        return -1;
    /*
     * DIR is an opaque type, so we don't really know how big
     * is actually is.  So guess.
     */
    if (libexplain_pointer_is_efault(dir, sizeof(int)))
        return -1;
    return dirfd(dir);
}
