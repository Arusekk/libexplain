/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_EINVAL_H
#define LIBEXPLAIN_BUFFER_EINVAL_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_einval_bits function may be used to
  * explain a bit-field containing undefined bits.
  *
  * @param sb
  *    The string buffer to print into.
  * @param caption
  *    The name of the offending system call argument.
  */
void libexplain_buffer_einval_bits(libexplain_string_buffer_t *sb,
    const char *caption);

#endif /* LIBEXPLAIN_BUFFER_EINVAL_H */
