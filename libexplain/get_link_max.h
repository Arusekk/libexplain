/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010 Peter Miller
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

#ifndef LIBEXPLAIN_GET_LINK_MAX_H
#define LIBEXPLAIN_GET_LINK_MAX_H

/**
  * The explain_get_link_max function may be used to obtain the LINK_MAX
  * value for a given directory tree.
  *
  * @param pathname
  *    The directory tree of interest.
  * @returns
  *    a positive number (it may be too small if pathconf fails).
  */
long explain_get_link_max(const char *pathname);

#endif /* LIBEXPLAIN_GET_LINK_MAX_H */