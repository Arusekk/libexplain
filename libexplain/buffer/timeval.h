/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_TIMEVAL_H
#define LIBEXPLAIN_BUFFER_TIMEVAL_H

#include <libexplain/string_buffer.h>

struct timeval; /* forward */

/**
  * The explain_buffer_timeval function may be used to
  * print a representation of a struct timeval value.
  *
  * @param sb
  *    The string buffer to print into.
  * @param data
  *    Pointer to the timeval of interest.
  */
void explain_buffer_timeval(explain_string_buffer_t *sb,
    const struct timeval *data);

/**
  * The explain_buffer_timeval_array function may be used to
  * print a representation of a struct timeval array.
  *
  * @param sb
  *    The string buffer to print into.
  * @param data
  *    Pointer to the timeval of interest.
  * @param data_size
  *    The number of elements in the array
  */
void explain_buffer_timeval_array(explain_string_buffer_t *sb,
    const struct timeval *data, unsigned data_size);

#endif /* LIBEXPLAIN_BUFFER_TIMEVAL_H */
