/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_EACCES_H
#define LIBEXPLAIN_BUFFER_EACCES_H

#include <libexplain/string_buffer.h>

struct explain_final_t; /* forward */

/**
  * The explain_buffer_eacces function may be used to
  *
  * @param sb
  *    The string buffer to print into.
  * @param pathname
  *    The problematic pathname
  * @param caption
  *    The name of the system call argument containing the problematic pathname.
  * @param final_component
  *    The desired properties of the final component
  */
void explain_buffer_eacces(explain_string_buffer_t *sb,
    const char *pathname, const char *caption,
    const struct explain_final_t *final_component);

#endif /* LIBEXPLAIN_BUFFER_EACCES_H */
