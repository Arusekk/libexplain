/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2013 Peter Miller
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

#ifndef LIBEXPLAIN_AC_NETROSE_ROSE_H
#define LIBEXPLAIN_AC_NETROSE_ROSE_H

/**
  * @file
  * @brief Insulate <netrose/rose.h> differences
  */

#include <libexplain/ac/netax25/ax25.h>

#ifdef HAVE_NETROSE_ROSE_H
#include <netrose/rose.h>
#endif

#endif /* LIBEXPLAIN_AC_NETROSE_ROSE_H */
/* vim: set ts=8 sw=4 et : */
