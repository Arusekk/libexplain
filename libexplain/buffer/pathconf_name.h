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

#ifndef LIBEXPLAIN_BUFFER_PATHCONF_NAME_H
#define LIBEXPLAIN_BUFFER_PATHCONF_NAME_H

#include <libexplain/string_buffer.h>

/**
  * The libexplain_buffer_pathconf_name function may be used to
  * translate a numeric pathconf parameter name into a synbolic name.
  *
  * @param sb
  *    The string buffer to print into.
  * @param name
  *    The name to be translated
  */
void libexplain_buffer_pathconf_name(libexplain_string_buffer_t *sb, int name);

/**
  * The libexplain_parse_pathconf_name_or_die function is used to parse a text
  * representation of a pathconf name into a numeric value.
  *
  * @param text
  *     The text to be parsed
  * @returns
  *     on success, the numeric value.
  *     On faulure it prints a diagnostic and exits.
  */
int libexplain_parse_pathconf_name_or_die(const char *text);

/**
  * The libexplain_valid_pathconf_name function is used determine
  * whether or not a pathconf name is valid.
  *
  * @param name
  *     The name to be verified.
  * @returns
  *     int (bool); non-zero (true) if name is valid, zero (false) if
  *     name is not valid.
  */
int libexplain_valid_pathconf_name(int name);

#endif /* LIBEXPLAIN_BUFFER_PATHCONF_NAME_H */
