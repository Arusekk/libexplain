/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_KBDIACRS_H
#define LIBEXPLAIN_BUFFER_KBDIACRS_H

#include <libexplain/string_buffer.h>

struct kbdiacrs; /* forward */

/**
  * The libexplain_buffer_kbdiacrs function may be used to
  * print a representation of a kbdiacrs structure value.
  *
  * @param sb
  *     The string buffer to print into.
  * @param value
  *     The kbdiacrs structure value to be printed.
  * @param extra
  *     if are to print entries as well
  */
void libexplain_buffer_kbdiacrs(libexplain_string_buffer_t *sb,
    const struct kbdiacrs *value, int extra);

#endif /* LIBEXPLAIN_BUFFER_KBDIACRS_H */
