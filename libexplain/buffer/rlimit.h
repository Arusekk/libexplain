/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_RLIMIT_H
#define LIBEXPLAIN_BUFFER_RLIMIT_H

struct libexplain_string_buffer_t; /* forward */
struct rlimit; /* forward */

/**
  * The libexplain_buffer_rlimit function may be used to insert a human
  * readable representation of an rlimit struct into the given buffer.
  *
  * @param sb
  *    The string buffer to write into
  * @param lim
  *    Pointer to the rlimit struct to be printed
  */
void libexplain_buffer_rlimit(struct libexplain_string_buffer_t *sb,
    const struct rlimit *lim);

#endif /* LIBEXPLAIN_BUFFER_RLIMIT_H */
