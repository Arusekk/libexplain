/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#include <libexplain/ac/sys/socket.h>

#include <libexplain/fildes_to_address_family.h>


int
libexplain_fildes_to_address_family(int fildes)
{
    struct sockaddr_storage ss;
    struct sockaddr *sa;
    socklen_t       sa_size;

    sa = (struct sockaddr *)&ss;
    sa_size = sizeof(ss);
    if (getsockname(fildes, sa, &sa_size) < 0)
        return -1;
    return sa->sa_family;
}
