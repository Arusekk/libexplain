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

#ifndef LIBEXPLAIN_BUFFER_ENOMEM_H
#define LIBEXPLAIN_BUFFER_ENOMEM_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_enomem function may be used to
  * explain a 'no kernel memory' error.
  *
  * @param sb
  *    The buffer to print the explanation to
  */
void libexplain_buffer_enomem_kernel(libexplain_string_buffer_t *sb);

/**
  * The libexplain_buffer_enomem function may be used to
  * explain a 'no user-space memory' error.
  *
  * @param sb
  *    The buffer to print the explanation to
  */
void libexplain_buffer_enomem_user(libexplain_string_buffer_t *sb);

/**
  * The libexplain_buffer_enomem_or_user function may be used to
  * explain a 'no user or kernel memory' error.
  *
  * @param sb
  *    The buffer to print the explanation to
  */
void libexplain_buffer_enomem_kernel_or_user(libexplain_string_buffer_t *sb);

#endif /* LIBEXPLAIN_BUFFER_ENOMEM_H */
