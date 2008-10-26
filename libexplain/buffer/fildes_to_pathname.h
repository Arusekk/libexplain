/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#ifndef LIBEXPLAIN_BUFFER_FILDES_TO_PATHNAME_H
#define LIBEXPLAIN_BUFFER_FILDES_TO_PATHNAME_H

struct libexplain_string_buffer_t; /* forward */

/**
  * The libexplain_buffer_fildes_to_pathname function may be used to
  * turn a file descriptor into a pathname, for the improvement of error
  * messages.  If no translation is possible, no text is emitted.
  *
  * @param sb
  *    The string buffer to place the pathname.
  * @param fildes
  *    The file descriptor to decipher
  */
void libexplain_buffer_fildes_to_pathname(struct libexplain_string_buffer_t *sb,
    int fildes);

#endif /* LIBEXPLAIN_BUFFER_FILDES_TO_PATHNAME_H */
