/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2012, 2013 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_EAGAIN_H
#define LIBEXPLAIN_BUFFER_EAGAIN_H

#include <libexplain/buffer/ewouldblock.h>

/**
  * The explain_buffer_eagain function may be used to print an explanation
  * for an EAGAIN error issued by one of the set*uid functions.
  *
  * @param sb
  *     The string buffer to print into.
  * @param caption
  *     The name of the offending system call argument.
  */
void explain_buffer_eagain_setuid(explain_string_buffer_t *sb,
    const char *caption);

#endif /* LIBEXPLAIN_BUFFER_EAGAIN_H */
/* vim: set ts=8 sw=4 et : */
