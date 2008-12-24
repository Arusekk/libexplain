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

#ifndef LIBEXPLAIN_SYMLOOPMAX_H
#define LIBEXPLAIN_SYMLOOPMAX_H

/**
  * The libexplain_symloopmax function may be used to
  *
  * @returns
  *    The systm's idea of the maximum of symbolic links that may be
  *    traversed in resolving a path.  Always positive.
  */
int libexplain_symloopmax(void);

#endif /* LIBEXPLAIN_SYMLOOPMAX_H */
