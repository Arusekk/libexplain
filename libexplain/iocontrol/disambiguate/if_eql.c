/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/linux/if_eql.h>
#include <libexplain/ac/net/if.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/iocontrol.h>


int
explain_iocontrol_disambiguate_is_if_eql(int fildes, int request,
    const void *data)
{
#ifdef HAVE_LINUX_IF_EQL_H
    struct ifreq    dummy;
    int             interface_index;

    (void)data;

    /*
     * Eliminate as much as possible without any system calls at all.
     */
    switch (request)
    {
    case EQL_EMANCIPATE:
    case EQL_ENSLAVE:
    case EQL_GETMASTRCFG:
    case EQL_GETSLAVECFG:
    case EQL_SETMASTRCFG:
    case EQL_SETSLAVECFG:
        break;

    default:
        return -1;
    }

    /*
     * We ask the file descriptor for its network interface index; if it says
     * no, this file descriptor isn't suitable.
     */
    memset(&dummy, 0, sizeof(dummy));
    if (ioctl(fildes, SIOCGIFINDEX, &dummy) < 0)
        return -1;
    interface_index = dummy.ifr_ifindex ;

    /*
     * We ask the file descriptor for its network interface name (based on the
     * interface index); if it says no, this file descriptor isn't suitable.
     */
    memset(&dummy, 0, sizeof(dummy));
    dummy.ifr_ifindex = interface_index;
    if (ioctl(fildes, SIOCGIFNAME, &dummy) < 0)
        return -1;

    /*
     * Finally, we test the interface name against "eql*".
     */
    return (0 == memcmp(dummy.ifr_name, "eql", 3) ? 0 : -1);
#else
    (void)fildes;
    (void)request;
    (void)data;
    return -1;
#endif
}
