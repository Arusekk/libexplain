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

#ifndef LIBEXPLAIN_BUFFER_CONSOLEFONTDESC_H
#define LIBEXPLAIN_BUFFER_CONSOLEFONTDESC_H

#include <libexplain/string_buffer.h>

struct consolefontdesc;

/**
  * The libexplain_buffer_consolefontdesc function may be used to
  * print a representation of a consolefontdesc structure value.
  *
  * @param sb
  *    The string buffer to print into.
  * @param value
  *    The consolefontdesc value to be printed.
  * @param extra
  *    Whether or not to print the chardata member.
  */
void libexplain_buffer_consolefontdesc(libexplain_string_buffer_t *sb,
    const struct consolefontdesc *value, int extra);

#endif /* LIBEXPLAIN_BUFFER_CONSOLEFONTDESC_H */
