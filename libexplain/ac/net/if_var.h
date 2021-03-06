/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2010, 2011, 2013 Peter Miller
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

#ifndef LIBEXPLAIN_AC_NET_IF_VAR_H
#define LIBEXPLAIN_AC_NET_IF_VAR_H

/**
  * @file
  * @brief Insulate <net/if_var.h> differences
  */

#include <libexplain/ac/linux/types.h> /* Ubuntu Hardy needs this first */
/* on FreeBSD these are pre-requisites */
#include <libexplain/ac/sys/types.h>
#include <libexplain/ac/sys/socket.h>
#include <libexplain/ac/net/if.h>
#include <libexplain/ac/arpa/inet.h>
#include <libexplain/ac/netinet/in.h>
#include <libexplain/ac/net/if_var.h>

#ifdef HAVE_NET_IF_VAR_H
#include <net/if_var.h>
#endif

#endif /* LIBEXPLAIN_AC_NET_IF_VAR_H */
/* vim: set ts=8 sw=4 et : */
