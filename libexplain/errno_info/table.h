/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#ifndef LIBEXPLAIN_ERRNO_INFO_TABLE_H
#define LIBEXPLAIN_ERRNO_INFO_TABLE_H

#include <libexplain/ac/stddef.h>

#include <libexplain/errno_info.h>

/**
  * The explain_errno_info global variable is used to record error
  * numbers and their names.
  */
extern const explain_errno_info_t explain_errno_info[];

/**
  * The explain_errno_info_size global variable is used to record
  * the size, in elements, of the explain_errno_info array.
  */
extern const size_t explain_errno_info_size;

#endif /* LIBEXPLAIN_ERRNO_INFO_TABLE_H */
