/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2010 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_GCC_ATTRIBUTES_H
#define LIBEXPLAIN_GCC_ATTRIBUTES_H

#ifdef __GNUC__
#define LIBEXPLAIN_NORETURN __attribute__((noreturn))
#define LIBEXPLAIN_LINKAGE_HIDDEN __attribute__((visibility("hidden")))
#define LIBEXPLAIN_FORMAT_PRINTF(x, y) __attribute__((format(printf, x, y)))
#define LIBEXPLAIN_FORMAT_VPRINTF(x) __attribute__((format(printf, x, 0)))
#else
#define LIBEXPLAIN_NORETURN
#define LIBEXPLAIN_LINKAGE_HIDDEN
#define LIBEXPLAIN_FORMAT_PRINTF(x, y)
#define LIBEXPLAIN_FORMAT_VPRINTF(x)
#endif

#endif /* LIBEXPLAIN_GCC_ATTRIBUTES_H */
