/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2010, 2013 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_VA_LIST_H
#define LIBEXPLAIN_BUFFER_VA_LIST_H

#include <libexplain/ac/stdarg.h>

#include <libexplain/string_buffer.h>

/**
  * The explain_buffer_va_list function may be used
  * to print a representation of a va_list value.
  *
  * @param sb
  *     The string buffer to print into.
  * @param data
  *     The va_list value to be printed.
  */
void explain_buffer_va_list(explain_string_buffer_t *sb, va_list data);

#endif /* LIBEXPLAIN_BUFFER_VA_LIST_H */
/* vim: set ts=8 sw=4 et : */
