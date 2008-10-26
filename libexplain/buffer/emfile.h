/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
 * Written by Peter Miller <millerp@canb.auug.org.au>
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

#ifndef LIBEXPLAIN_BUFFER_EMFILE_H
#define LIBEXPLAIN_BUFFER_EMFILE_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_emfile function i sused to print an
  * explanation of the EMFILE error into the given buffer, including
  * "because".
  *
  * @param sb
  *     The string buffer to print into.
  */
void libexplain_buffer_emfile(libexplain_string_buffer_t *sb);

#endif /* LIBEXPLAIN_BUFFER_EMFILE_H */
