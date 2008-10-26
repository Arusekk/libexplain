/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EXPLAIN_STRTOL_OR_DIE_H
#define EXPLAIN_STRTOL_OR_DIE_H

/**
  * The strtol_or_die function may be used to interpret a string as a
  * number.  If there is any error, a suitable message is printed to
  * stderr, and the process exits.
  *
  * @param text
  *    the string to be interpreted as a number
  * @returns
  *    the number
  */
long strtol_or_die(const char *text);

#endif /* EXPLAIN_STRTOL_OR_DIE_H */
