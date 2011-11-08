/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2011 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_ENOMEM_H
#define LIBEXPLAIN_BUFFER_ENOMEM_H

#include <libexplain/string_buffer.h>

/**
  * The explain_buffer_enomem function may be used to
  * explain a 'no kernel memory' error.
  *
  * @param sb
  *    The buffer to print the explanation to
  */
void explain_buffer_enomem_kernel(explain_string_buffer_t *sb);

/**
  * The explain_buffer_enomem function may be used to
  * explain a 'no user-space memory' error.
  *
  * @param sb
  *    The buffer to print the explanation to
  */
void explain_buffer_enomem_user(explain_string_buffer_t *sb);

/**
  * The explain_buffer_enomem_or_user function may be used to
  * explain a 'no user or kernel memory' error.
  *
  * @param sb
  *    The buffer to print the explanation to
  */
void explain_buffer_enomem_kernel_or_user(explain_string_buffer_t *sb);

/**
  * The explain_buffer_enomem_rlimit_exceeded function is used to test
  * whether or not the given data size would exceed the process's virtual
  * memory limit.
  *
  * If it would be exceeded, a message to that effect is printed, and
  * true (non-zero) is returned.  Otherwise nothing is printed, and
  * false (zero) is returned.
  *
  * @param sb
  *     String buffer to print into.
  * @param size
  *     The memory allocation size, in bytes.
  * @returns
  *     true if would exceed, false if not.
  */
int explain_buffer_enomem_rlimit_exceeded(explain_string_buffer_t *sb,
    size_t size);

#endif /* LIBEXPLAIN_BUFFER_ENOMEM_H */
