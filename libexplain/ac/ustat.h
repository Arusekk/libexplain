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

#ifndef LIBEXPLAIN_AC_USTAT_H
#define LIBEXPLAIN_AC_USTAT_H

/**
  * @file
  * @brief Insulate <ustat.h> differences
  */

#include <libexplain/ac/sys/types.h>

#ifdef HAVE_USTAT_H
#include <ustat.h>
#else

/* bogus definition, just so things compile */
struct ustat
{
    long f_tfree;
    long f_tinode;
    char f_fname[6];
    char f_fpack[6];
};

#endif

#if !HAVE_DECL_USTAT
int ustat(dev_t dev, struct ustat *data);
#endif

#endif /* LIBEXPLAIN_AC_USTAT_H */
