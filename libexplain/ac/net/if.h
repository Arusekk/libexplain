/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_AC_NET_IF_H
#define LIBEXPLAIN_AC_NET_IF_H

#include <libexplain/ac/sys/types.h>
#include <libexplain/ac/sys/socket.h>

#ifdef HAVE_NET_IF_H
# ifdef __linux__
   /* these two conflict, because they have a common ancestor */
#  define _NET_IF_H 1
#  include <linux/if.h>
# else
#  include <net/if.h>
# endif
#endif

#endif /* LIBEXPLAIN_AC_NET_IF_H */
